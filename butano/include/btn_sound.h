/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

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

    void play_with_priority(int priority, sound_item item);

    void play_with_priority(int priority, sound_item item, fixed volume);

    void play_with_priority(int priority, sound_item item, fixed volume, fixed speed, fixed panning);

    void stop_all();
}
}

#endif
