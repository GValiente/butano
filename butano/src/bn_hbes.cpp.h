/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_hbes.h"

#include "bn_hblank_effects_manager.h"

namespace bn::hbes
{

int used_count()
{
    return hblank_effects_manager::used_count();
}

int available_count()
{
    return hblank_effects_manager::available_count();
}

}
