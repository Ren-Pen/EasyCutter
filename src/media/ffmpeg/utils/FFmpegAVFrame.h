#ifndef EASY_CUTTER_SRC_MEDIA_FFMPEG_AV_FRAME_H
#define EASY_CUTTER_SRC_MEDIA_FFMPEG_AV_FRAME_H

#include <functional>
#include <memory>

extern "C" {
struct AVFrame;
enum AVMediaType;
}

namespace slimenano::media {
class AVFrameDeleter {
public:
    void operator()(AVFrame* ptr) const;
};

using AVFramePtr = std::unique_ptr<AVFrame, AVFrameDeleter>;
using AVFrameCallback = std::function<void(class FFmpegDecoder&, AVFramePtr)>;

} // namespace slimenano::media

#endif
