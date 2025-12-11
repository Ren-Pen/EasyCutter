#include "../../../pch.h" // IWYU pragma: keep

#include "../FFmpegException.h"
#include "FFmpegDecoder.h"

namespace slimenano::media {

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
                    std::this_thread::sleep_for(std::chrono::duration<double>(0.0333));
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
