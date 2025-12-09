#include "../../../pch.h" // IWYU pragma: keep

#include "FFmpegAVPacket.h"

namespace slimenano::media {

void AVPacketDeleter::operator()(AVPacket* ptr) const {
    if (ptr) {
        av_packet_free(&ptr);
    }
}

} // namespace slimenano::media
