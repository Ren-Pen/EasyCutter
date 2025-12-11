#ifndef EASY_CUTTER_SRC_MEDIA_FFMPEG_DEMUXER_H
#define EASY_CUTTER_SRC_MEDIA_FFMPEG_DEMUXER_H

#include <string>
#include <memory>
#include <unordered_map>
#include <thread>
#include <atomic>

#include "FFmpegDecoder.h"
#include "../utils/FFmpegAVFrame.h"

extern "C" {
struct AVFormatContext;
}

namespace slimenano::media {

class FFmpegDemuxer {
public:
    FFmpegDemuxer(const std::string url);
    ~FFmpegDemuxer();

    FFmpegDecoder::Ref GetActivityVideoDecoder();
    FFmpegDecoder::Ref GetActivityAudioDecoder();
    FFmpegDecoder::Ref GetActivitySubtitleDecoder();
    void Play();

protected:
    void DemuxerLoop();

private:
    bool IsActivatedDecoder(unsigned int streamIndex);

private:
    const std::string m_url;

    std::thread m_demuxerThread;
    std::unordered_map<unsigned int, std::unique_ptr<FFmpegDecoder>> m_decoders;

    unsigned int m_activityVideoDecoder = 0xFFFFFFFF;
    unsigned int m_activityAudioDecoder = 0xFFFFFFFF;
    unsigned int m_activitySubtitleDecoder = 0xFFFFFFFF;

    std::mutex m_decoderMutex;

    std::atomic<bool> m_isStopped = false;

    AVFormatContext* m_pFormatContext = nullptr;
};

} // namespace slimenano::media

#endif
