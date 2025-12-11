#include "../../../pch.h" // IWYU pragma: keep

#include "FFmpegAVPacketQueue.h"

namespace slimenano::media {

FFmpegAVPacketQueue::FFmpegAVPacketQueue(const std::size_t MAX_QUEUE_SIZE, const std::size_t MAX_PACKET_SIZE) :
    m_MAX_QUEUE_SIZE(MAX_QUEUE_SIZE), m_MAX_PACKET_SIZE(MAX_PACKET_SIZE) {
}

bool FFmpegAVPacketQueue::IsPacketQueueFull() {
    return m_packetSize > m_MAX_PACKET_SIZE || m_packetQueue.size() > m_MAX_QUEUE_SIZE;
}

bool FFmpegAVPacketQueue::Push(AVPacketPtr packet, std::function<bool()> abortFunc) {

    std::unique_lock<std::mutex> locker(m_mutex);

    while (IsPacketQueueFull()) {
        if (abortFunc()) {
            return false;
        }
        m_condVar.wait(locker);
    }

    m_packetSize += packet->size;
    m_packetQueue.push_back(std::move(packet));
    m_condVar.notify_all();

    return true;
}

bool FFmpegAVPacketQueue::Pop(AVPacketPtr& packet, std::function<bool()> abortFunc) {
    std::unique_lock<std::mutex> locker(m_mutex);

    while (m_packetQueue.empty()) {
        if (abortFunc()) {
            return false;
        }
        m_condVar.wait(locker);
    }

    packet = std::move(m_packetQueue.front());
    m_packetQueue.pop_front();
    m_packetSize -= packet->size;

    m_condVar.notify_all();
    return true;
}

void FFmpegAVPacketQueue::Notify() {
    m_condVar.notify_all();
}

void FFmpegAVPacketQueue::Clear() {
    std::unique_lock<std::mutex> locker(m_mutex);
    m_packetQueue.clear();
    m_packetSize = 0;
}

} // namespace slimenano::media
