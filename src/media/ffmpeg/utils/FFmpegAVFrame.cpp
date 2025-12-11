#include "../../../pch.h" // IWYU pragma: keep

#include "FFmpegAVFrame.h"

namespace slimenano::media {

void AVFrameDeleter::operator()(AVFrame* ptr) const {
    if (ptr) {
        av_frame_free(&ptr);
    }
}

} // namespace slimenano::media
