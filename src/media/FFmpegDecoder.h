
#ifndef EASY_CUTTER_SRC_MEDIA_FFMPEG_STREAM_H
#define EASY_CUTTER_SRC_MEDIA_FFMPEG_STREAM_H

extern "C" {
struct AVCodecContext;
struct AVFrame;
struct AVStream;
}

namespace slimenano::media {

class FFmpegStream {

public:
    FFmpegStream(unsigned int streamIndex, AVStream* stream);
    ~FFmpegStream();

private:
    AVCodecContext* m_pCodecContext = nullptr;
    unsigned int m_streamIndex = 0xFFFFFFFF;
};

} // namespace slimenano::media

#endif
