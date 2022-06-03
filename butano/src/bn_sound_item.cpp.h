/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sound_item.h"

#include "bn_sound.h"

namespace bn
{

void sound_item::play() const
{
    sound::play(*this);
}

void sound_item::play(fixed volume) const
{
    sound::play(*this, volume);
}

void sound_item::play(fixed volume, fixed speed, fixed panning) const
{
    sound::play(*this, volume, speed, panning);
}

void sound_item::play_with_priority(int priority) const
{
    sound::play_with_priority(priority, *this);
}

void sound_item::play_with_priority(int priority, fixed volume) const
{
    sound::play_with_priority(priority, *this, volume);
}

void sound_item::play_with_priority(int priority, fixed volume, fixed speed, fixed panning) const
{
    sound::play_with_priority(priority, *this, volume, speed, panning);
}

}
