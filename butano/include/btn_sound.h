#ifndef BTN_SOUND_H
#define BTN_SOUND_H

#include "btn_fixed_fwd.h"

namespace btn
{

class sound_item;

namespace sound
{
    void play(sound_item item);

    void play(sound_item item, fixed volume);

    void play(sound_item item, fixed volume, fixed speed, fixed panning);

    void stop_all();
}
}

#endif
