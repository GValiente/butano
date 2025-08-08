/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_audio.h"

#include "bn_audio_manager.h"

namespace bn::audio
{

span<const audio_mixing_rate> available_mixing_rates()
{
    return audio_manager::available_mixing_rates();
}

audio_mixing_rate mixing_rate()
{
    return audio_manager::mixing_rate();
}

void set_mixing_rate(audio_mixing_rate mixing_rate)
{
    audio_manager::set_mixing_rate(mixing_rate);
}

bool update_on_vblank()
{
    return audio_manager::update_on_vblank();
}

void set_update_on_vblank(bool update_on_vblank)
{
    audio_manager::set_update_on_vblank(update_on_vblank);
}

}
