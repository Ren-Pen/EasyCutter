#ifndef EASY_CUTTER_SRC_MEDIA_TRACK_H
#define EASY_CUTTER_SRC_MEDIA_TRACK_H

#include "Clock.h"

namespace slimenano::media {

enum class TrackType {
    VIDEO = 0,
    AUDIO,
    SUBTITLE
};

class Track {

public:
    Track(TrackType type);
    virtual ~Track();

    TrackType Type();

private:
    TrackType m_trackType;
};

} // namespace slimenano::media

#endif
