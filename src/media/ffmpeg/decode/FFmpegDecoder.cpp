#include "../../../pch.h" // IWYU pragma: keep

#include "../FFmpegException.h"
#include "FFmpegDecoder.h"

namespace slimenano::media {

FFmpegDecoder::FFmpegDecoder(unsigned int streamIndex, AVStream* stream) :
    m_streamIndex(streamIndex) {

    auto pCodecParam = stream->codecpar;
    m_pCodec = avcodec_find_decoder(pCodecParam->codec_id);
    if (!m_pCodec) {
        throw FFmpegException(-1, "Unable found a decoder!");
    }

    m_pFrame = av_frame_alloc();
    if (!m_pFrame) {
        throw FFmpegException(-1, "Allocate Error!");
    }

    m_pCodecContext = avcodec_alloc_context3(m_pCodec);
    if (!m_pCodecContext) {
        throw FFmpegException(-1, "Allocate Error!");
    }

    int errNum = avcodec_parameters_to_context(m_pCodecContext, pCodecParam);
    if (errNum != 0) {
        throw FFmpegException(errNum);
    }

    errNum = avcodec_open2(m_pCodecContext, m_pCodec, nullptr);
    if (errNum != 0) {
        throw FFmpegException(errNum);
    }
}

FFmpegDecoder::~FFmpegDecoder() {

    if (m_pCodecContext) {
        avcodec_free_context(&m_pCodecContext);
    }

    if (m_pFrame) {
        av_frame_free(&m_pFrame);
    }

    m_pCodec = nullptr;
    m_streamIndex = 0xFFFFFFFF;
}

} // namespace slimenano::media
