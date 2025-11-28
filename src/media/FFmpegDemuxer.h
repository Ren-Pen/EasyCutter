#ifndef EASY_CUTTER_SRC_MEDIA_FFMPEG_DEMUXER_H
#define EASY_CUTTER_SRC_MEDIA_FFMPEG_DEMUXER_H

namespace slimenano::media {

class FFmpegDemuxer {

public:
    FFmpegDemuxer(const FFmpegDemuxer&) = delete;
    FFmpegDemuxer(FFmpegDemuxer&&) = delete;
    FFmpegDemuxer& operator=(const FFmpegDemuxer&) = delete;
    FFmpegDemuxer& operator=(FFmpegDemuxer&&) = delete;

    FFmpegDemuxer(const char* file);
    ~FFmpegDemuxer();
};

} // namespace slimenano::media

#endif
