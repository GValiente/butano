/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sound_item.h"

#include "bn_fixed.h"
#include "bn_audio_manager.h"

namespace bn
{

void sound_item::play() const
{
    audio_manager::play_sound(0, *this);
}

void sound_item::play(fixed volume) const
{
    audio_manager::play_sound(0, *this, volume, 1, 0);
}

void sound_item::play(fixed volume, fixed speed, fixed panning) const
{
    audio_manager::play_sound(0, *this, volume, speed, panning);
}

void sound_item::play_with_priority(int priority) const
{
    audio_manager::play_sound(priority, *this);
}

void sound_item::play_with_priority(int priority, fixed volume) const
{
    audio_manager::play_sound(priority, *this, volume, 1, 0);
}

void sound_item::play_with_priority(int priority, fixed volume, fixed speed, fixed panning) const
{
    audio_manager::play_sound(priority, *this, volume, speed, panning);
}

}
