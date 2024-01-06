/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprites.h"

#include "bn_display_manager.h"
#include "bn_sprites_manager.h"

namespace bn::sprites
{

int used_items_count()
{
    return sprites_manager::used_items_count();
}

int available_items_count()
{
    return sprites_manager::available_items_count();
}

bool visible()
{
    return display_manager::sprites_visible();
}

void set_visible(bool visible)
{
    display_manager::set_sprites_visible(visible);
}

bool blending_bottom_enabled()
{
    return display_manager::blending_bottom_sprites_enabled();
}

void set_blending_bottom_enabled(bool blending_bottom_enabled)
{
    display_manager::set_blending_bottom_sprites_enabled(blending_bottom_enabled);
}

int reserved_handles_count()
{
    return sprites_manager::reserved_handles_count();
}

void set_reserved_handles_count(int reserved_handles_count)
{
    return sprites_manager::set_reserved_handles_count(reserved_handles_count);
}

void reload()
{
    sprites_manager::reload_all();
}

}
