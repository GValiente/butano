/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sound.h"

#include "bn_assert.h"
#include "bn_sound_item.h"
#include "bn_audio_manager.h"

namespace bn::sound
{

void play(sound_item item)
{
    audio_manager::play_sound(0, item);
}

void play(sound_item item, fixed volume)
{
    BN_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0..1]: ", volume);

    audio_manager::play_sound(0, item, volume, 1, 0);
}

void play(sound_item item, fixed volume, fixed speed, fixed panning)
{
    BN_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0..1]: ", volume);
    BN_ASSERT(speed >= 0 && speed <= 64, "Speed range is [0..64]: ", speed);
    BN_ASSERT(panning >= -1 && panning <= 1, "Panning range is [-1..1]: ", panning);

    audio_manager::play_sound(0, item, volume, speed, panning);
}

void play_with_priority(int priority, sound_item item)
{
    BN_ASSERT(priority >= -32767 && priority <= 32767, "Priority range is [-32767..32767]: ", priority);

    audio_manager::play_sound(priority, item);
}

void play_with_priority(int priority, sound_item item, fixed volume)
{
    BN_ASSERT(priority >= -32767 && priority <= 32767, "Priority range is [-32767..32767]: ", priority);
    BN_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0..1]: ", volume);

    audio_manager::play_sound(priority, item, volume, 1, 0);
}

void play_with_priority(int priority, sound_item item, fixed volume, fixed speed, fixed panning)
{
    BN_ASSERT(priority >= -32767 && priority <= 32767, "Priority range is [-32767..32767]: ", priority);
    BN_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0..1]: ", volume);
    BN_ASSERT(speed >= 0 && speed <= 64, "Speed range is [0..64]: ", speed);
    BN_ASSERT(panning >= -1 && panning <= 1, "Panning range is [-1..1]: ", panning);

    audio_manager::play_sound(priority, item, volume, speed, panning);
}

void stop_all()
{
    audio_manager::stop_all_sounds();
}

}
