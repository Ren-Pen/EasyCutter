#ifndef EASY_CUTTER_SRC_MEDIA_FFMPEG_AV_PACKET_H
#define EASY_CUTTER_SRC_MEDIA_FFMPEG_AV_PACKET_H

#include <memory>

extern "C" {
struct AVPacket;
}

namespace slimenano::media {
class AVPacketDeleter {
public:
    void operator()(AVPacket* ptr) const;
};

using AVPacketPtr = std::unique_ptr<AVPacket, AVPacketDeleter>;

} // namespace slimenano::media

#endif
