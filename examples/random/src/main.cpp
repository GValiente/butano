/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_random.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#include "bn_sprite_items_dice.h"

int main()
{
    bn::core::init();
    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    constexpr bn::string_view info_text_lines[] = {
        "A: roll the dice",
    };

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    common::info info("Random", info_text_lines, text_generator);

    bn::random random;
    int dice_graphics_index = random.get_int(6);

    bn::sprite_ptr dice_sprite = bn::sprite_items::dice.create_sprite(0, 0, dice_graphics_index);

    while(true)
    {
        if(bn::keypad::a_pressed())
        {
            dice_graphics_index = random.get_int(6);
            dice_sprite.set_item(bn::sprite_items::dice, dice_graphics_index);
        }

        info.update();
        bn::core::update();
    }
}
