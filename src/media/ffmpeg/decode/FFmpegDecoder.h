
#ifndef EASY_CUTTER_SRC_MEDIA_FFMPEG_DECODER_H
#define EASY_CUTTER_SRC_MEDIA_FFMPEG_DECODER_H

extern "C" {
struct AVCodecContext;
struct AVFrame;
struct AVStream;
struct AVCodec;
}

namespace slimenano::media {

class FFmpegDecoder {

public:
    FFmpegDecoder(unsigned int streamIndex, AVStream* stream);
    ~FFmpegDecoder();

private:
    unsigned int m_streamIndex = 0xFFFFFFFF;
    AVCodecContext* m_pCodecContext = nullptr;
    const AVCodec* m_pCodec = nullptr;
    AVFrame* m_pFrame = nullptr;
};

} // namespace slimenano::media

#endif
