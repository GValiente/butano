/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_music_item.h"

#include "bn_music.h"
#include "bn_jingle.h"

namespace bn
{

void music_item::play() const
{
    music::play(*this);
}

void music_item::play(fixed volume) const
{
    music::play(*this, volume);
}

void music_item::play(fixed volume, bool loop) const
{
    music::play(*this, volume, loop);
}

void music_item::play_jingle() const
{
    jingle::play(*this);
}

void music_item::play_jingle(fixed volume) const
{
    jingle::play(*this, volume);
}

}
