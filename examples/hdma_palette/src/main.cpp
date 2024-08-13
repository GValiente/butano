/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"

#include "bn_hdma.h"
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

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    common::info info("Palette updated with HDMA", bn::span<const bn::string_view>(), text_generator);

    bn::affine_bg_ptr bg = bn::affine_bg_items::butane_cylinder.create_bg(0, 8);
    bg.set_wrapping_enabled(false);

    constexpr const bn::bg_palette_item& bg_palette_item = bn::affine_bg_items::butane_cylinder.palette_item();
    constexpr int colors_count = bg_palette_item.colors_ref().size();
    using hdma_source_type = bn::array<uint16_t, bn::display::height() * colors_count>;
    bn::unique_ptr<hdma_source_type> hdma_source(new hdma_source_type());
    uint16_t* hdma_source_data = hdma_source->data();

    for(int scanline = 0; scanline < bn::display::height(); ++scanline)
    {
        uint16_t* hdma_scanline_data = hdma_source_data + (scanline * colors_count);
        int color_dec = (scanline + 1) % 32;

        if(color_dec > 16)
        {
            color_dec = 32 - color_dec;
        }

        for(bn::color color : bg_palette_item.colors_ref())
        {
            color.set_red(bn::max(color.red() - color_dec, 0));
            color.set_green(bn::max(color.green() - color_dec, 0));
            color.set_blue(bn::max(color.blue() - color_dec, 0));
            *hdma_scanline_data = uint16_t(color.data());
            ++hdma_scanline_data;
        }
    }

    bn::hdma::start(*hdma_source_data, colors_count, *bn::hw::palettes::bg_color_register(0));

    while(true)
    {
        bg.set_rotation_angle((bg.rotation_angle() + 1) % 360);

        info.update();
        bn::core::update();
    }
}
