#include "btn_sprite_palettes.h"

#include "btn_palettes_bank.h"
#include "btn_palettes_manager.h"

namespace btn::sprite_palettes
{

int used_count()
{
    return palettes_manager::sprite_palettes_bank().used_count();
}

int available_count()
{
    return palettes_manager::sprite_palettes_bank().available_count();
}

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

bool inverted()
{
    return palettes_manager::sprite_palettes_bank().inverted();
}

void set_inverted(bool inverted)
{
    palettes_manager::sprite_palettes_bank().set_inverted(inverted);
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

void set_fade_color(color color)
{
    palettes_bank& bank = palettes_manager::sprite_palettes_bank();
    bank.set_fade(color, bank.fade_intensity());
}

fixed fade_intensity()
{
    return palettes_manager::sprite_palettes_bank().fade_intensity();
}

void set_fade_intensity(fixed intensity)
{
    palettes_bank& bank = palettes_manager::sprite_palettes_bank();
    bank.set_fade(bank.fade_color(), intensity);
}

void set_fade(color color, fixed intensity)
{
    palettes_manager::sprite_palettes_bank().set_fade(color, intensity);
}

}
