#ifndef EASY_CUTTER_SRC_MEDIA_FFMPEG_AV_CODEC_CONTEXT_H
#define EASY_CUTTER_SRC_MEDIA_FFMPEG_AV_CODEC_CONTEXT_H

#include <memory>

extern "C" {
struct AVCodecContext;
}

namespace slimenano::media {
class AVCodecContextDeleter {
public:
    void operator()(AVCodecContext* ptr) const;
};

using AVCodecContextPtr = std::unique_ptr<AVCodecContext, AVCodecContextDeleter>;

} // namespace slimenano::media

#endif
