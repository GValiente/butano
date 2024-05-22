/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_dmg_music_item.h"

#include "bn_dmg_music.h"

namespace bn
{

void dmg_music_item::play() const
{
    dmg_music::play(*this);
}

void dmg_music_item::play(int speed) const
{
    dmg_music::play(*this, speed);
}

void dmg_music_item::play(int speed, bool loop) const
{
    dmg_music::play(*this, speed, loop);
}

}
