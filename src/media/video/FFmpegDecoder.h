#ifndef EASY_CUTTER_SRC_MEDIA_VIDEO_FFMPEG_DECODER_H
#define EASY_CUTTER_SRC_MEDIA_VIDEO_FFMPEG_DECODER_H

#include <string>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

#include "VideoFrame.h"

namespace slimenano::media {

class FFmpegDecoder {
public:
    FFmpegDecoder();
    ~FFmpegDecoder();

    bool Open(const std::string& path);
    bool ReadFrame(VideoFrame& outFrame);
    void Close();

private:
    AVFormatContext* m_pFormatCtx = nullptr;
    const AVCodec* m_pVideoCodec = nullptr;
    AVCodecContext* m_pVideoCodecContext = nullptr;

    int m_videoStreamIndex = -1;
    VideoInfo* m_videoInfo = nullptr;
};

} // namespace slimenano::media

#endif
