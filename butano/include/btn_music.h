#ifndef BTN_MUSIC_H
#define BTN_MUSIC_H

#include "btn_fixed_fwd.h"

namespace btn
{
    class music_item;
}

namespace btn::music
{
    [[nodiscard]] bool playing();

    void play(music_item item);

    void play(music_item item, fixed volume);

    void play(music_item item, fixed volume, bool loop);

    void stop();

    [[nodiscard]] bool paused();

    void pause();

    void resume();

    [[nodiscard]] int position();

    void set_position(int position);

    [[nodiscard]] fixed volume();

    void set_volume(fixed volume);
}

#endif
