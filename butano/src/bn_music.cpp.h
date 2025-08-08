/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_music.h"

#include "bn_music_item.h"
#include "bn_audio_manager.h"

namespace bn::music
{

bool playing()
{
    return audio_manager::music_playing();
}

optional<music_item> playing_item()
{
    return audio_manager::playing_music_item();
}

void play(music_item item)
{
    audio_manager::play_music(item, 1, true);
}

void play(music_item item, fixed volume)
{
    BN_BASIC_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0..1]: ", volume);

    audio_manager::play_music(item, volume, true);
}

void play(music_item item, fixed volume, bool loop)
{
    BN_BASIC_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0..1]: ", volume);

    audio_manager::play_music(item, volume, loop);
}

void stop()
{
    audio_manager::stop_music();
}

bool paused()
{
    return audio_manager::music_paused();
}

void pause()
{
    audio_manager::pause_music();
}

void resume()
{
    audio_manager::resume_music();
}

int position()
{
    return audio_manager::music_position();
}

void set_position(int position)
{
    BN_ASSERT(position >= 0, "Invalid position: ", position);

    audio_manager::set_music_position(position);
}

fixed volume()
{
    return audio_manager::music_volume();
}

void set_volume(fixed volume)
{
    BN_BASIC_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0..1]: ", volume);

    audio_manager::set_music_volume(volume);
}

fixed tempo()
{
    return audio_manager::music_tempo();
}

void set_tempo(fixed tempo)
{
    BN_BASIC_ASSERT(tempo >= 0.5 && tempo <= 2, "Tempo range is [0.5..2]: ", tempo);

    audio_manager::set_music_tempo(tempo);
}

fixed pitch()
{
    return audio_manager::music_pitch();
}

void set_pitch(fixed pitch)
{
    BN_BASIC_ASSERT(pitch >= 0.5 && pitch <= 2, "Pitch range is [0.5..2]: ", pitch);

    audio_manager::set_music_pitch(pitch);
}

}
