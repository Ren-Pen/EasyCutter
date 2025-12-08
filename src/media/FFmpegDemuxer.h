#ifndef EASY_CUTTER_SRC_MEDIA_FFMPEG_DEMUXER_H
#define EASY_CUTTER_SRC_MEDIA_FFMPEG_DEMUXER_H

#include <string>
#include <vector>

#include "FFmpegDecoder.h"

extern "C" {
struct AVFormatContext;
struct AVPacket;
}

namespace slimenano::media {

class FFmpegDemuxer {

public:
    FFmpegDemuxer(const std::string url);
    ~FFmpegDemuxer();

    int Open();

private:
    const std::string m_url;
    AVFormatContext* m_pFormatContext = nullptr;
    AVPacket* m_pPacket = nullptr;

    std::vector<FFmpegStream> m_streams = {};
};

} // namespace slimenano::media

#endif
