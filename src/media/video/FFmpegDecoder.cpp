#include "FFmpegDecoder.h"

namespace slimenano::media {

FFmpegDecoder::FFmpegDecoder() {
}

FFmpegDecoder::~FFmpegDecoder() {
    Close();
}

void FFmpegDecoder::Close() {
    m_videoStreamIndex = -1;
    m_pVideoCodec = nullptr;

    if (m_videoInfo) {
        delete m_videoInfo;
        m_videoInfo = nullptr;
    }

    if (m_pVideoCodecContext != nullptr) {
        avcodec_free_context(&m_pVideoCodecContext);
    }

    if (m_pFormatCtx) {
        avformat_close_input(&m_pFormatCtx);
    }
}

bool FFmpegDecoder::Open(const std::string& path) {

    Close();
    if (avformat_open_input(&m_pFormatCtx, path.c_str(), nullptr, nullptr) != 0) {
        return false;
    }

    if (avformat_find_stream_info(m_pFormatCtx, nullptr) < 0) {
        return false;
    }

    m_videoStreamIndex = -1;
    for (unsigned int i = 0; i < m_pFormatCtx->nb_streams; i++) {
        if (m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            m_videoStreamIndex = i;
            auto pVideoCodecPar = m_pFormatCtx->streams[i]->codecpar;
            m_pVideoCodec = avcodec_find_decoder(pVideoCodecPar->codec_id);
            if (m_pVideoCodec) {
                m_pVideoCodecContext = avcodec_alloc_context3(m_pVideoCodec);
                if (avcodec_parameters_to_context(m_pVideoCodecContext, pVideoCodecPar) != 0) {
                    return false;
                }
                if (avcodec_open2(m_pVideoCodecContext, m_pVideoCodec, nullptr) != 0) {
                    return false;
                }

                auto fps = av_q2d(m_pFormatCtx->streams[i]->r_frame_rate);
                auto timeUnit = av_q2d(m_pFormatCtx->streams[i]->time_base);

                auto duration = static_cast<unsigned long long>(m_pFormatCtx->streams[i]->duration * (fps * timeUnit));

                m_videoInfo = new VideoInfo{
                    .width = m_pVideoCodecContext->coded_width,
                    .height = m_pVideoCodecContext->coded_height,
                    .fps = fps,
                    .timeUnit = timeUnit,
                    .duration = duration
                };
                break;
            }
        }
    }

    return true;
}

bool FFmpegDecoder::ReadFrame([[maybe_unused]] VideoFrame& outFrame) {

    if (!m_pFormatCtx) {
        return false;
    }
    if (!m_pVideoCodecContext) {
        return false;
    }

    return true;
}

} // namespace slimenano::media
