#include "../../../pch.h" // IWYU pragma: keep

#include "../FFmpegException.h"
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

    for (unsigned int streamIndex = 0; streamIndex < m_pFormatContext->nb_streams; ++streamIndex) {
        auto decoder = FFmpegDecoder(streamIndex, m_pFormatContext->streams[streamIndex]);
        m_decoders.push_back(decoder);
    }

    m_pPacket = AVPacketPtr(av_packet_alloc());
    if (!m_pPacket) {
        throw FFmpegException(-1, "Allocate Error!");
    }

    m_demuxerThread = std::thread(&FFmpegDemuxer::DemuxerLoop, this);
}

void FFmpegDemuxer::DemuxerLoop() {

    while (!m_isStopped) {

        std::unique_lock<std::mutex> lock(m_pauseMutex);
        m_pauseCond.wait(lock, [this] {
            return !this->m_isPaused || this->m_isStopped;
        });

        if (m_isStopped) {
            break;
        }
    }
}

FFmpegDemuxer::~FFmpegDemuxer() {

    m_isStopped = true;

    m_pauseCond.notify_all();

    if (m_demuxerThread.joinable()) {
        m_demuxerThread.join();
    }

    if (m_pFormatContext) {
        avformat_close_input(&m_pFormatContext);
    }
}

} // namespace slimenano::media
