/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_sram_data.h"

#include "bn_sram.h"
#include "bn_fixed.h"
#include "bn_sstream.h"
#include "bn_string_view.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_palettes.h"

namespace fr
{

namespace
{
    constexpr bn::string_view valid_label = "vr012";
}

void sram_data::init()
{
    sram_data copy;
    bn::sram::read(copy);

    if(bn::string_view(copy._label) == valid_label)
    {
        *this = copy;
    }
    else
    {
        bn::istring_base label_istring(_label);
        bn::ostringstream label_stream(label_istring);
        label_stream.append(valid_label);
        _stages.init();
        _viewer_items.init();
    }

    set_brightness(_brightness_level);
}

void sram_data::set_brightness(brightness_level brightness)
{
    switch(brightness)
    {

    case brightness_level::LOW:
        bn::bg_palettes::set_brightness(0);
        bn::sprite_palettes::set_brightness(0);
        break;

    case brightness_level::MEDIUM:
        bn::bg_palettes::set_brightness(0.2);
        bn::sprite_palettes::set_brightness(0.2);
        break;

    case brightness_level::HIGH:
        bn::bg_palettes::set_brightness(0.4);
        bn::sprite_palettes::set_brightness(0.4);
        break;

    default:
        BN_ERROR("Invalid brightness: ", int(brightness));
        break;
    }

    _brightness_level = brightness;
}

void sram_data::write()
{
    bn::sram::write(*this);
}

}
