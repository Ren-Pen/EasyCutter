#ifndef EASY_CUTTER_SRC_MEDIA_PLAYER_H
#define EASY_CUTTER_SRC_MEDIA_PLAYER_H

namespace slimenano::media {

class Player {

public:
    Player();
    ~Player();

    void Play();
    void Pause();
    void Resume();
    void Stop();
};

} // namespace slimenano::media

#endif
