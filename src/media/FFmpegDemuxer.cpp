#include "../pch.h" // IWYU pragma: keep

#include "FFmpegDemuxer.h"

namespace slimenano::media {

FFmpegDemuxer::FFmpegDemuxer(const std::string url) :
    m_url(url) {
}

FFmpegDemuxer::~FFmpegDemuxer() {
    m_streams.clear();

    if (m_pPacket) {
        av_packet_free(&m_pPacket);
    }

    if (m_pFormatContext) {
        avformat_close_input(&m_pFormatContext);
    }
}

int FFmpegDemuxer::Open() {

    int errNum = avformat_open_input(&m_pFormatContext, m_url.c_str(), nullptr, nullptr);
    if (errNum != 0) {
        return errNum;
    }

    errNum = avformat_find_stream_info(m_pFormatContext, nullptr);
    if (errNum != 0) {
        return errNum;
    }

    for (unsigned int streamIndex = 0; streamIndex < m_pFormatContext->nb_streams; ++streamIndex) {
        auto stream = FFmpegStream(streamIndex, m_pFormatContext->streams[streamIndex]);
        m_streams.push_back(stream);
    }

    m_pPacket = av_packet_alloc();
    if (!m_pPacket) {
        return -1;
    }

    return 0;
}

} // namespace slimenano::media
