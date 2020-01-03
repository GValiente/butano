#ifndef BTN_MUSIC_H
#define BTN_MUSIC_H

#include "btn_fixed_fwd.h"

namespace btn
{

class music_item;

namespace music
{
    [[nodiscard]] bool playing();

    void play(music_item item);

    void play(music_item item, bool loop, fixed volume);

    void stop();

    void pause();

    void resume();

    void set_volume(fixed volume);
}
}

#endif
