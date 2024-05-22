/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprite_affine_mats.h"

#include "bn_sprite_affine_mats_manager.h"

namespace bn::sprite_affine_mats
{

int used_count()
{
    return sprite_affine_mats_manager::used_count();
}

int available_count()
{
    return sprite_affine_mats_manager::available_count();
}

}
