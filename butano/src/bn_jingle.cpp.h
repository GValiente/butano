/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_jingle.h"

#include "bn_music_item.h"
#include "bn_audio_manager.h"

namespace bn::jingle
{

bool playing()
{
    return audio_manager::jingle_playing();
}

optional<music_item> playing_item()
{
    return audio_manager::playing_jingle_item();
}

void play(music_item item)
{
    audio_manager::play_jingle(item, 1);
}

void play(music_item item, fixed volume)
{
    BN_BASIC_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0..1]: ", volume);

    audio_manager::play_jingle(item, volume);
}

fixed volume()
{
    return audio_manager::jingle_volume();
}

void set_volume(fixed volume)
{
    BN_BASIC_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0..1]: ", volume);

    audio_manager::set_jingle_volume(volume);
}

}
