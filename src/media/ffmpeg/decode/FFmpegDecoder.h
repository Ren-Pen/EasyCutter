
#ifndef EASY_CUTTER_SRC_MEDIA_FFMPEG_DECODER_H
#define EASY_CUTTER_SRC_MEDIA_FFMPEG_DECODER_H

#include <thread>
#include "../utils/FFmpegAVCodecContext.h"
#include "../utils/FFmpegAVPacketQueue.h"
#include "../utils/FFmpegAVFrame.h"

extern "C" {
struct AVStream;
struct AVCodec;
struct AVRational;
}

namespace slimenano::media {

[[maybe_unused]] constexpr size_t MAX_VIDEO_PACKET_SIZE = 100 * 1024 * 1024;
[[maybe_unused]] constexpr size_t MAX_AUDIO_PACKET_SIZE = 15 * 1024 * 1024;
[[maybe_unused]] constexpr size_t MAX_DEFAULT_PACKET_SIZE = 20 * 1024 * 1024;
[[maybe_unused]] constexpr size_t MAX_VIDEO_QUEUE_SIZE = 500;
[[maybe_unused]] constexpr size_t MAX_AUDIO_QUEUE_SIZE = 500;
[[maybe_unused]] constexpr size_t MAX_DEFAULT_QUEUE_SIZE = 500;

class FFmpegDecoder {
    friend class FFmpegDemuxer;

public:
    using Ref = std::optional<std::reference_wrapper<FFmpegDecoder>>;

    FFmpegDecoder(unsigned int streamIndex, AVStream* stream);
    ~FFmpegDecoder();

    AVCodecContext& GetCodecContext();

    void SetFrameCallback(AVFrameCallback m_frameCallback);

protected:
    void DecodeLoop();

private:
    bool DispatchPacket(AVPacketPtr packet);

public:
    const unsigned int stream_index = 0xFFFFFFFF;
    const AVCodec* const codec;
    const AVMediaType codec_type;
    const AVRational time_base;
    const AVRational avg_frame_rate;

private:
    AVCodecContextPtr m_pCodecContext = nullptr;
    std::thread m_decodeThread;

    std::atomic<bool> m_isStopped = false;

    FFmpegAVPacketQueue m_packetQueue;
    AVFrameCallback m_frameCallback;
};

} // namespace slimenano::media

#endif
