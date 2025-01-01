/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_backdrop.h"

#include "bn_palettes_bank.h"
#include "bn_display_manager.h"
#include "bn_palettes_manager.h"

namespace bn::backdrop
{

const optional<bn::color>& color()
{
    return palettes_manager::bg_palettes_bank().transparent_color();
}

void set_color(bn::color color)
{
    palettes_manager::bg_palettes_bank().set_transparent_color(color);
}

void set_color(const optional<bn::color>& color)
{
    palettes_manager::bg_palettes_bank().set_transparent_color(color);
}

void remove_color()
{
    palettes_manager::bg_palettes_bank().set_transparent_color(nullopt);
}

bool blending_bottom_enabled()
{
    return display_manager::blending_bottom_backdrop_enabled();
}

void set_blending_bottom_enabled(bool blending_bottom_enabled)
{
    display_manager::set_blending_bottom_backdrop_enabled(blending_bottom_enabled);
}

}
