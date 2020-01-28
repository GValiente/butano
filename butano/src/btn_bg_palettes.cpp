#include "btn_bg_palettes.h"

#include "btn_palettes_bank.h"
#include "btn_palettes_manager.h"

namespace btn::bg_palettes
{

int used_count()
{
    return palettes_manager::bg_palettes_bank().used_count();
}

int available_count()
{
    return palettes_manager::bg_palettes_bank().available_count();
}

const optional<color>& transparent_color()
{
    return palettes_manager::bg_palettes_bank().transparent_color();
}

void set_transparent_color(const optional<color>& transparent_color)
{
    palettes_manager::bg_palettes_bank().set_transparent_color(transparent_color);
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

fixed inverse_intensity()
{
    return palettes_manager::bg_palettes_bank().inverse_intensity();
}

void set_inverse_intensity(fixed intensity)
{
    palettes_manager::bg_palettes_bank().set_inverse_intensity(intensity);
}

fixed grayscale_intensity()
{
    return palettes_manager::bg_palettes_bank().grayscale_intensity();
}

void set_grayscale_intensity(fixed intensity)
{
    palettes_manager::bg_palettes_bank().set_grayscale_intensity(intensity);
}

color fade_color()
{
    return palettes_manager::bg_palettes_bank().fade_color();
}

fixed fade_intensity()
{
    return palettes_manager::bg_palettes_bank().fade_intensity();
}

void set_fade(color color, fixed intensity)
{
    palettes_manager::bg_palettes_bank().set_fade(color, intensity);
}

}
