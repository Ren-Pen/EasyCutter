#ifndef EASY_CUTTER_SRC_MEDIA_VIDEO_VIDEO_FRAME_H
#define EASY_CUTTER_SRC_MEDIA_VIDEO_VIDEO_FRAME_H

#include <vector>
#include <cstdint>
namespace slimenano::media {

struct VideoInfo {
    int width = 0;
    int height = 0;
    double fps = 0.0;
    double timeUnit = 0.0;
    unsigned long long duration = 0;
};

struct VideoFrame {
    int width = 0;
    int height = 0;
    std::vector<uint8_t> frameData{};
};

} // namespace slimenano::media

#endif
