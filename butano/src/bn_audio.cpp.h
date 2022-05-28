/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_audio.h"

#include "bn_audio_manager.h"

namespace bn::audio
{

bool update_on_vblank()
{
    return audio_manager::update_on_vblank();
}

void set_update_on_vblank(bool update_on_vblank)
{
    audio_manager::set_update_on_vblank(update_on_vblank);
}

bool dmg_sync_enabled()
{
    return audio_manager::dmg_sync_enabled();
}

void set_dmg_sync_enabled(bool dmg_sync_enabled)
{
    return audio_manager::set_dmg_sync_enabled(dmg_sync_enabled);
}

}
