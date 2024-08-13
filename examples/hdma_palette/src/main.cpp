/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"

#include "bn_hdma.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_unique_ptr.h"
#include "bn_affine_bg_ptr.h"
#include "bn_sprite_text_generator.h"

#include "../../butano/hw/include/bn_hw_palettes.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#include "bn_affine_bg_items_butane_cylinder.h"

int main()
{
    bn::core::init();

    constexpr bn::string_view info_text_lines[] = {
        "A: pause/resume HDMA",
    };

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    common::info info("Palette updated with HDMA", info_text_lines, text_generator);

    bn::affine_bg_ptr bg = bn::affine_bg_items::butane_cylinder.create_bg(0, 0);
    bg.set_wrapping_enabled(false);

    constexpr const bn::bg_palette_item& bg_palette_item = bn::affine_bg_items::butane_cylinder.palette_item();
    constexpr int colors_count = bg_palette_item.colors_ref().size();
    using hdma_source_type = bn::array<uint16_t, bn::display::height() * colors_count>;
    bn::unique_ptr<hdma_source_type> hdma_source_ptr(new hdma_source_type());
    hdma_source_type& hdma_source = *hdma_source_ptr;
    uint16_t* hdma_source_data = hdma_source.data();

    for(int scanline = 0; scanline < bn::display::height(); ++scanline)
    {
        uint16_t* hdma_scanline_data = hdma_source_data + (scanline * colors_count);
        int red_reduction = (scanline + 1) % 32;

        if(red_reduction > 16)
        {
            red_reduction = 32 - red_reduction;
        }

        for(bn::color color : bg_palette_item.colors_ref())
        {
            color.set_red(bn::max(color.red() - red_reduction, 0));
            *hdma_scanline_data = uint16_t(color.data());
            ++hdma_scanline_data;
        }
    }

    bn::hdma::start(hdma_source, *bn::hw::palettes::bg_color_register(0));

    while(true)
    {
        if(bn::keypad::a_pressed())
        {
            if(bn::hdma::running())
            {
                bn::hdma::stop();
            }
            else
            {
                bn::hdma::start(hdma_source, *bn::hw::palettes::bg_color_register(0));
            }
        }

        bg.set_rotation_angle((bg.rotation_angle() + 1) % 360);

        info.update();
        bn::core::update();
    }
}
