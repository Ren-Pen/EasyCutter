#include "../../../pch.h" // IWYU pragma: keep

#include "FFmpegAVCodecContext.h"

namespace slimenano::media {

void AVCodecContextDeleter::operator()(AVCodecContext* ptr) const {
    if (ptr) {
        avcodec_free_context(&ptr);
    }
}

} // namespace slimenano::media
