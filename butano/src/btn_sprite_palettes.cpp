#include "btn_sprite_palettes.h"

#include "btn_palettes_bank.h"
#include "btn_palettes_manager.h"

namespace btn::sprite_palettes
{

fixed brightness()
{
    return palettes_manager::sprite_palettes_bank().brightness();
}

void set_brightness(fixed brightness)
{
    palettes_manager::sprite_palettes_bank().set_brightness(brightness);
}

fixed contrast()
{
    return palettes_manager::sprite_palettes_bank().contrast();
}

void set_contrast(fixed contrast)
{
    palettes_manager::sprite_palettes_bank().set_contrast(contrast);
}

fixed intensity()
{
    return palettes_manager::sprite_palettes_bank().intensity();
}

void set_intensity(fixed intensity)
{
    palettes_manager::sprite_palettes_bank().set_intensity(intensity);
}

fixed inverse_intensity()
{
    return palettes_manager::sprite_palettes_bank().inverse_intensity();
}

void set_inverse_intensity(fixed intensity)
{
    palettes_manager::sprite_palettes_bank().set_inverse_intensity(intensity);
}

fixed grayscale_intensity()
{
    return palettes_manager::sprite_palettes_bank().grayscale_intensity();
}

void set_grayscale_intensity(fixed intensity)
{
    palettes_manager::sprite_palettes_bank().set_grayscale_intensity(intensity);
}

color fade_color()
{
    return palettes_manager::sprite_palettes_bank().fade_color();
}

fixed fade_intensity()
{
    return palettes_manager::sprite_palettes_bank().fade_intensity();
}

void set_fade(color color, fixed intensity)
{
    palettes_manager::sprite_palettes_bank().set_fade(color, intensity);
}

}
