
#ifndef EASY_CUTTER_SRC_MEDIA_Clock_H
#define EASY_CUTTER_SRC_MEDIA_Clock_H

#include "cstdlib"

namespace slimenano::media {
using TimeUnit = unsigned long long;

class Clock {
public:
    TimeUnit Current();

    void Seek(TimeUnit time, int duration);

private:
    TimeUnit m_systemTime;
    TimeUnit m_baseTime;
};

} // namespace slimenano::media

#endif
