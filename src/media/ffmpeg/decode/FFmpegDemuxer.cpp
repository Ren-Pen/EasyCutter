#include "../../../pch.h" // IWYU pragma: keep

#include "../FFmpegException.h"
#include "../utils/FFmpegAVPacket.h"
#include "FFmpegDemuxer.h"

namespace slimenano::media {

FFmpegDemuxer::FFmpegDemuxer(const std::string url) :
    m_url(url) {

    int errNum = avformat_open_input(&m_pFormatContext, m_url.c_str(), nullptr, nullptr);
    if (errNum != 0) {
        throw FFmpegException(errNum);
    }

    errNum = avformat_find_stream_info(m_pFormatContext, nullptr);
    if (errNum != 0) {
        throw FFmpegException(errNum);
    }

    std::unique_lock<std::mutex> locker(m_decoderMutex);
    for (unsigned int streamIndex = 0; streamIndex < m_pFormatContext->nb_streams; ++streamIndex) {
        auto decoder = std::make_unique<FFmpegDecoder>(streamIndex, m_pFormatContext->streams[streamIndex]);
        if (decoder->codec_type == AVMEDIA_TYPE_VIDEO && m_activityVideoDecoder == 0xFFFFFFFF) {
            m_activityVideoDecoder = streamIndex;
        }
        if (decoder->codec_type == AVMEDIA_TYPE_AUDIO && m_activityAudioDecoder == 0xFFFFFFFF) {
            m_activityAudioDecoder = streamIndex;
        }
        if (decoder->codec_type == AVMEDIA_TYPE_SUBTITLE && m_activitySubtitleDecoder == 0xFFFFFFFF) {
            m_activitySubtitleDecoder = streamIndex;
        }
        m_decoders.insert(std::make_pair(streamIndex, std::move(decoder)));
    }
}

bool FFmpegDemuxer::IsActivatedDecoder(unsigned int streamIndex) {
    return m_activityAudioDecoder == streamIndex || m_activityVideoDecoder == streamIndex || m_activitySubtitleDecoder == streamIndex;
}

void FFmpegDemuxer::Play() {
    m_demuxerThread = std::thread(&FFmpegDemuxer::DemuxerLoop, this);
}

void FFmpegDemuxer::DemuxerLoop() {

    while (!m_isStopped) {

        AVPacketPtr packet = AVPacketPtr(av_packet_alloc());
        int ret = av_read_frame(m_pFormatContext, packet.get());
        if (ret == 0) {
            std::unique_lock<std::mutex> locker(m_decoderMutex);
            if (IsActivatedDecoder(packet->stream_index)) {
                auto iter = m_decoders.find(packet->stream_index);
                if (iter != m_decoders.end()) {
                    auto& decoder = iter->second;
                    decoder->DispatchPacket(std::move(packet));
                }
            }
            continue; // Drop unused packet
        }
    }
}

FFmpegDemuxer::~FFmpegDemuxer() {

    m_isStopped = true;

    if (m_demuxerThread.joinable()) {
        m_demuxerThread.join();
    }

    if (m_pFormatContext) {
        avformat_close_input(&m_pFormatContext);
    }
}

FFmpegDecoder::Ref FFmpegDemuxer::GetActivityVideoDecoder() {
    std::unique_lock<std::mutex> locker(m_decoderMutex);
    auto iter = m_decoders.find(m_activityVideoDecoder);
    if (iter != m_decoders.end()) {
        return std::ref(*(iter->second));
    }
    return std::nullopt;
}

FFmpegDecoder::Ref FFmpegDemuxer::GetActivityAudioDecoder() {
    std::unique_lock<std::mutex> locker(m_decoderMutex);
    auto iter = m_decoders.find(m_activityAudioDecoder);
    if (iter != m_decoders.end()) {
        return std::ref(*(iter->second));
    }
    return std::nullopt;
}

FFmpegDecoder::Ref FFmpegDemuxer::GetActivitySubtitleDecoder() {
    std::unique_lock<std::mutex> locker(m_decoderMutex);
    auto iter = m_decoders.find(m_activitySubtitleDecoder);
    if (iter != m_decoders.end()) {
        return std::ref(*(iter->second));
    }
    return std::nullopt;
}

} // namespace slimenano::media
