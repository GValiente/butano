/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_music_item.h"

#include "bn_fixed.h"
#include "bn_audio_manager.h"

namespace bn
{

void music_item::play() const
{
    audio_manager::play_music(*this, 1, true);
}

void music_item::play(fixed volume) const
{
    audio_manager::play_music(*this, volume, true);
}

void music_item::play(fixed volume, bool loop) const
{
    audio_manager::play_music(*this, volume, loop);
}

}
