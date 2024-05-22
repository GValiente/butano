/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sound_handle.h"

#include "bn_sound_item.h"
#include "bn_audio_manager.h"

namespace bn
{

bool sound_handle::active() const
{
    return audio_manager::sound_data(_id);
}

sound_item sound_handle::item() const
{
    return audio_manager::sound_item(_id);
}

void sound_handle::stop()
{
    audio_manager::stop_sound(_id);
}

void sound_handle::release()
{
    audio_manager::release_sound(_id);
}

fixed sound_handle::speed() const
{
    return audio_manager::sound_speed(_id);
}

void sound_handle::set_speed(fixed speed)
{
    BN_BASIC_ASSERT(speed >= 0 && speed <= 64, "Speed range is [0..64]: ", speed);

    audio_manager::set_sound_speed(_id, speed);
}

fixed sound_handle::panning() const
{
    return audio_manager::sound_panning(_id);
}

void sound_handle::set_panning(fixed panning)
{
    BN_BASIC_ASSERT(panning >= -1 && panning <= 1, "Panning range is [-1..1]: ", panning);

    audio_manager::set_sound_panning(_id, panning);
}

}
