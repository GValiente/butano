/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sound_item.h"

#include "bn_sound.h"

namespace bn
{

sound_handle sound_item::play() const
{
    return sound::play(*this);
}

sound_handle sound_item::play(fixed volume) const
{
    return sound::play(*this, volume);
}

sound_handle sound_item::play(fixed volume, fixed speed, fixed panning) const
{
    return sound::play(*this, volume, speed, panning);
}

sound_handle sound_item::play_with_priority(int priority) const
{
    return sound::play_with_priority(priority, *this);
}

sound_handle sound_item::play_with_priority(int priority, fixed volume) const
{
    return sound::play_with_priority(priority, *this, volume);
}

sound_handle sound_item::play_with_priority(int priority, fixed volume, fixed speed, fixed panning) const
{
    return sound::play_with_priority(priority, *this, volume, speed, panning);
}

}
