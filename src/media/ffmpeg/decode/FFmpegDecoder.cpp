#include "../../../pch.h" // IWYU pragma: keep

#include "../FFmpegException.h"
#include "FFmpegDecoder.h"

namespace slimenano::media {

static const std::vector<AVHWDeviceType> PriorityList = {
#if defined(_WIN32)
    AV_HWDEVICE_TYPE_D3D11VA,
    AV_HWDEVICE_TYPE_DXVA2,
    AV_HWDEVICE_TYPE_CUDA,
#elif defined(__APPLE__)
    AV_HWDEVICE_TYPE_VIDEOTOOLBOX,
#else // Linux
    AV_HWDEVICE_TYPE_CUDA,
    AV_HWDEVICE_TYPE_VAAPI,
#endif
};

std::vector<AVHWDeviceType> getSupportedHW(const AVCodec* codec) {
    std::vector<AVHWDeviceType> list;

    for (int i = 0;; i++) {
        const AVCodecHWConfig* config = avcodec_get_hw_config(codec, i);
        if (!config)
            break;

        if (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX) {
            list.push_back(config->device_type);
        }
    }
    return list;
}

AVHWDeviceType selectBestHWDevice(const AVCodec* codec) {
    auto supported = getSupportedHW(codec);

    for (auto prefer : PriorityList) {
        if (std::find(supported.begin(), supported.end(), prefer) != supported.end()) {
            return prefer;
        }
    }

    return AV_HWDEVICE_TYPE_NONE;
}

size_t getSuggestQueueSize(const AVMediaType& mediaType) {
    if (mediaType == AVMEDIA_TYPE_VIDEO) {
        return MAX_VIDEO_QUEUE_SIZE;
    } else if (mediaType == AVMEDIA_TYPE_AUDIO) {
        return MAX_AUDIO_QUEUE_SIZE;
    } else {
        return MAX_DEFAULT_QUEUE_SIZE;
    }
}

size_t getSuggestPacketSize(const AVMediaType& mediaType) {
    if (mediaType == AVMEDIA_TYPE_VIDEO) {
        return MAX_VIDEO_PACKET_SIZE;
    } else if (mediaType == AVMEDIA_TYPE_AUDIO) {
        return MAX_AUDIO_PACKET_SIZE;
    } else {
        return MAX_DEFAULT_PACKET_SIZE;
    }
}

FFmpegDecoder::FFmpegDecoder(unsigned int streamIndex, AVStream* stream) :
    stream_index(streamIndex),
    codec(avcodec_find_decoder(stream->codecpar->codec_id)),
    codec_type(stream->codecpar->codec_type),
    time_base(stream->time_base),
    avg_frame_rate(stream->avg_frame_rate),
    m_packetQueue(getSuggestQueueSize(stream->codecpar->codec_type), getSuggestPacketSize(stream->codecpar->codec_type)) {

    if (!codec) {
        throw FFmpegException(-1, "Unable found a decoder!");
    }

    m_pCodecContext = AVCodecContextPtr(avcodec_alloc_context3(codec));
    if (!m_pCodecContext) {
        throw FFmpegException(-1, "Allocate Error!");
    }

    int errNum = avcodec_parameters_to_context(m_pCodecContext.get(), stream->codecpar);
    if (errNum != 0) {
        throw FFmpegException(errNum);
    }

    auto deviceType = selectBestHWDevice(codec);
    AVBufferRef* hwDeviceCtx = nullptr;

    errNum = av_hwdevice_ctx_create(&hwDeviceCtx, deviceType, nullptr, nullptr, 0);
    if (errNum != 0) {
        hwDeviceCtx = nullptr;
    }

    if (hwDeviceCtx) {
        m_pCodecContext->hw_device_ctx = av_buffer_ref(hwDeviceCtx);
        av_buffer_unref(&hwDeviceCtx);
    }

    errNum = avcodec_open2(m_pCodecContext.get(), codec, nullptr);
    if (errNum != 0) {
        throw FFmpegException(errNum);
    }

    m_decodeThread = std::thread(&FFmpegDecoder::DecodeLoop, this);
}

FFmpegDecoder::~FFmpegDecoder() {

    m_isStopped = true;

    m_packetQueue.Notify();

    if (m_decodeThread.joinable()) {
        m_decodeThread.join();
    }
}

bool FFmpegDecoder::DispatchPacket(AVPacketPtr packet) {
    return m_packetQueue.Push(std::move(packet), [this]() {
        return this->m_isStopped.load();
    });
}

void FFmpegDecoder::DecodeLoop() {

    while (!m_isStopped) {
        AVPacketPtr packet = nullptr;
        m_packetQueue.Pop(packet, [this]() {
            return this->m_isStopped.load();
        });

        auto ret = avcodec_send_packet(m_pCodecContext.get(), packet.get());
        if (ret == 0) {
            AVFramePtr frame = AVFramePtr(av_frame_alloc());
            auto recvRet = avcodec_receive_frame(m_pCodecContext.get(), frame.get());
            if (recvRet == 0) {
                if (m_frameCallback) {
                    m_frameCallback(*this, std::move(frame));
                    // std::this_thread::sleep_for(std::chrono::duration<double>(0.0001));
                }
            }
        }
    }
}

AVCodecContext& FFmpegDecoder::GetCodecContext() {
    return *m_pCodecContext;
}

void FFmpegDecoder::SetFrameCallback(AVFrameCallback frameCallback) {

    m_frameCallback = frameCallback;
}

} // namespace slimenano::media
