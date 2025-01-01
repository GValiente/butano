/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_bg_palettes.h"

#include "bn_palettes_bank.h"
#include "bn_palettes_manager.h"

namespace bn::bg_palettes
{

int used_colors_count()
{
    return palettes_manager::bg_palettes_bank().used_colors_count();
}

int available_colors_count()
{
    return palettes_manager::bg_palettes_bank().available_colors_count();
}

const optional<color>& transparent_color()
{
    return palettes_manager::bg_palettes_bank().transparent_color();
}

void set_transparent_color(color transparent_color)
{
    palettes_manager::bg_palettes_bank().set_transparent_color(transparent_color);
}

void set_transparent_color(const optional<color>& transparent_color)
{
    palettes_manager::bg_palettes_bank().set_transparent_color(transparent_color);
}

void remove_transparent_color()
{
    palettes_manager::bg_palettes_bank().set_transparent_color(nullopt);
}

fixed brightness()
{
    return palettes_manager::bg_palettes_bank().brightness();
}

void set_brightness(fixed brightness)
{
    palettes_manager::bg_palettes_bank().set_brightness(brightness);
}

fixed contrast()
{
    return palettes_manager::bg_palettes_bank().contrast();
}

void set_contrast(fixed contrast)
{
    palettes_manager::bg_palettes_bank().set_contrast(contrast);
}

fixed intensity()
{
    return palettes_manager::bg_palettes_bank().intensity();
}

void set_intensity(fixed intensity)
{
    palettes_manager::bg_palettes_bank().set_intensity(intensity);
}

bool inverted()
{
    return palettes_manager::bg_palettes_bank().inverted();
}

void set_inverted(bool inverted)
{
    palettes_manager::bg_palettes_bank().set_inverted(inverted);
}

fixed grayscale_intensity()
{
    return palettes_manager::bg_palettes_bank().grayscale_intensity();
}

void set_grayscale_intensity(fixed intensity)
{
    palettes_manager::bg_palettes_bank().set_grayscale_intensity(intensity);
}

fixed hue_shift_intensity()
{
    return palettes_manager::bg_palettes_bank().hue_shift_intensity();
}

void set_hue_shift_intensity(fixed intensity)
{
    palettes_manager::bg_palettes_bank().set_hue_shift_intensity(intensity);
}

color fade_color()
{
    return palettes_manager::bg_palettes_bank().fade_color();
}

fixed fade_intensity()
{
    return palettes_manager::bg_palettes_bank().fade_intensity();
}

void set_fade_color(color color)
{
    palettes_bank& bank = palettes_manager::bg_palettes_bank();
    bank.set_fade_color(color);
}

void set_fade_intensity(fixed intensity)
{
    palettes_bank& bank = palettes_manager::bg_palettes_bank();
    bank.set_fade_intensity(intensity);
}

void set_fade(color color, fixed intensity)
{
    palettes_manager::bg_palettes_bank().set_fade(color, intensity);
}

void log_status()
{
    #if BN_CFG_LOG_ENABLED
        palettes_manager::bg_palettes_bank().log_status();
    #endif
}

}
