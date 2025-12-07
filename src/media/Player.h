#ifndef EASY_CUTTER_SRC_MEDIA_PLAYER_H
#define EASY_CUTTER_SRC_MEDIA_PLAYER_H

#include "Clock.h"
#include "Track.h"

namespace slimenano::media {

class Player {

public:
    Player();
    ~Player();

    void Play();
    void Pause();
    void Resume();
    void Stop();

    int AttachTrack(Track* track);
    int DetachTrack(Track* track);
};

} // namespace slimenano::media

#endif
