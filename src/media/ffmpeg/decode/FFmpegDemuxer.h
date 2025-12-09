#ifndef EASY_CUTTER_SRC_MEDIA_FFMPEG_DEMUXER_H
#define EASY_CUTTER_SRC_MEDIA_FFMPEG_DEMUXER_H

#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include "../utils/FFmpegAVPacket.h"
#include "FFmpegDecoder.h"

extern "C" {
struct AVFormatContext;
}

namespace slimenano::media {

class FFmpegDemuxer {

public:
    FFmpegDemuxer(const std::string url);
    ~FFmpegDemuxer();

protected:
    void DemuxerLoop();

private:
    const std::string m_url;

    std::thread m_demuxerThread;
    std::vector<FFmpegDecoder> m_decoders;
    std::queue<AVPacketPtr> m_packetQueue;

    std::atomic<bool> m_isStopped = false;
    std::atomic<bool> m_isPaused = false;
    std::mutex m_pauseMutex;
    std::condition_variable m_pauseCond;

    AVFormatContext* m_pFormatContext = nullptr;
    AVPacketPtr m_pPacket = nullptr;
};

} // namespace slimenano::media

#endif
