/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_dmg_music.h"

#include "bn_optional.h"
#include "bn_dmg_music_item.h"
#include "bn_audio_manager.h"

namespace bn::dmg_music
{

bool playing()
{
    return audio_manager::dmg_music_playing();
}

optional<dmg_music_item> playing_item()
{
    return audio_manager::playing_dmg_music_item();
}

void play(dmg_music_item item)
{
    audio_manager::play_dmg_music(item, 1, true);
}

void play(dmg_music_item item, int speed)
{
    BN_ASSERT(speed >= 1 && speed <= 256, "Speed range is [1..256]: ", speed);

    audio_manager::play_dmg_music(item, speed, true);
}

void play(dmg_music_item item, int speed, bool loop)
{
    BN_ASSERT(speed >= 1 && speed <= 256, "Speed range is [1..256]: ", speed);

    audio_manager::play_dmg_music(item, speed, loop);
}

void stop()
{
    audio_manager::stop_dmg_music();
}

bool paused()
{
    return audio_manager::dmg_music_paused();
}

void pause()
{
    audio_manager::pause_dmg_music();
}

void resume()
{
    audio_manager::resume_dmg_music();
}

}
