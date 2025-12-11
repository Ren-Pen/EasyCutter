#ifndef EASY_CUTTER_SRC_MEDIA_FFMPEG_AV_PACKET_QUEUE_H
#define EASY_CUTTER_SRC_MEDIA_FFMPEG_AV_PACKET_QUEUE_H

#include <cstdint>
#include <cstddef>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <cassert>
#include <deque>
#include <atomic>

#include "FFmpegAVPacket.h"

namespace slimenano::media {

class FFmpegAVPacketQueue {

public:
    FFmpegAVPacketQueue(const std::size_t MAX_QUEUE_SIZE, const std::size_t MAX_PACKET_SIZE);
    FFmpegAVPacketQueue(const FFmpegAVPacketQueue&) = delete;
    FFmpegAVPacketQueue& operator=(const FFmpegAVPacketQueue&) = delete;

    bool Push(AVPacketPtr packet, std::function<bool()> abortFunc);

    bool Pop(AVPacketPtr& packet, std::function<bool()> abortFunc);

    void Notify();

    void Clear();

private:
    bool IsPacketQueueFull();
    const size_t m_MAX_QUEUE_SIZE;
    const size_t m_MAX_PACKET_SIZE;
    std::atomic<uint64_t> m_packetSize;
    std::deque<AVPacketPtr> m_packetQueue;
    std::mutex m_mutex;
    std::condition_variable m_condVar;
};

} // namespace slimenano::media

#endif
