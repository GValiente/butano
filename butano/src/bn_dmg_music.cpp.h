/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_dmg_music.h"

#include "bn_dmg_music_item.h"
#include "bn_dmg_music_position.h"
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

void play(const dmg_music_item& item)
{
    audio_manager::play_dmg_music(item, 1, true);
}

void play(const dmg_music_item& item, int speed)
{
    BN_BASIC_ASSERT(speed >= 1 && speed <= 256, "Speed range is [1..256]: ", speed);

    audio_manager::play_dmg_music(item, speed, true);
}

void play(const dmg_music_item& item, int speed, bool loop)
{
    BN_BASIC_ASSERT(speed >= 1 && speed <= 256, "Speed range is [1..256]: ", speed);

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

const dmg_music_position& position()
{
    return audio_manager::dmg_music_position();
}

void set_position(int pattern, int row)
{
    audio_manager::set_dmg_music_position(dmg_music_position(pattern, row));
}

void set_position(const dmg_music_position& position)
{
    audio_manager::set_dmg_music_position(position);
}

fixed left_volume()
{
    return audio_manager::dmg_music_left_volume();
}

fixed right_volume()
{
    return audio_manager::dmg_music_right_volume();
}

void set_left_volume(fixed left_volume)
{
    BN_BASIC_ASSERT(left_volume >= 0 && left_volume <= 1, "Left volume range is [0..1]: ", left_volume);

    audio_manager::set_dmg_music_left_volume(left_volume);
}

void set_right_volume(fixed right_volume)
{
    BN_BASIC_ASSERT(right_volume >= 0 && right_volume <= 1, "Right volume range is [0..1]: ", right_volume);

    audio_manager::set_dmg_music_right_volume(right_volume);
}

void set_volume(fixed volume)
{
    BN_BASIC_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0..1]: ", volume);

    audio_manager::set_dmg_music_volume(volume, volume);
}

void set_volume(fixed left_volume, fixed right_volume)
{
    BN_BASIC_ASSERT(left_volume >= 0 && left_volume <= 1, "Left volume range is [0..1]: ", left_volume);
    BN_BASIC_ASSERT(right_volume >= 0 && right_volume <= 1, "Right volume range is [0..1]: ", right_volume);

    audio_manager::set_dmg_music_volume(left_volume, right_volume);
}

dmg_music_master_volume master_volume()
{
    return audio_manager::dmg_music_master_volume();
}

void set_master_volume(dmg_music_master_volume volume)
{
    audio_manager::set_dmg_music_master_volume(volume);
}

}
