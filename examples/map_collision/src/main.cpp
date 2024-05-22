/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_regular_bg_map_cell_info.h"

#include "bn_sprite_items_dog.h"
#include "bn_regular_bg_items_map.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

int main()
{
    bn::core::init();

    constexpr bn::string_view info_text_lines[] = {
        "PAD: move dog"
    };

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    common::info info("Map collision", info_text_lines, text_generator);

    bn::regular_bg_ptr map_bg = bn::regular_bg_items::map.create_bg(0, 0);
    bn::sprite_ptr dog_sprite = bn::sprite_items::dog.create_sprite(0, 0);

    const bn::regular_bg_map_item& map_item = bn::regular_bg_items::map.map_item();
    bn::regular_bg_map_cell valid_map_cell = map_item.cell(0, 0);
    int valid_tile_index = bn::regular_bg_map_cell_info(valid_map_cell).tile_index();
    bn::point dog_map_position(16, 16);

    while(true)
    {
        bn::point new_dog_map_position = dog_map_position;

        if(bn::keypad::left_pressed())
        {
            new_dog_map_position.set_x(new_dog_map_position.x() - 1);
            dog_sprite.set_horizontal_flip(true);
        }
        else if(bn::keypad::right_pressed())
        {
            new_dog_map_position.set_x(new_dog_map_position.x() + 1);
            dog_sprite.set_horizontal_flip(false);
        }

        if(bn::keypad::up_pressed())
        {
            new_dog_map_position.set_y(new_dog_map_position.y() - 1);
        }
        else if(bn::keypad::down_pressed())
        {
            new_dog_map_position.set_y(new_dog_map_position.y() + 1);
        }

        bn::regular_bg_map_cell dog_map_cell = map_item.cell(new_dog_map_position);
        int dog_tile_index = bn::regular_bg_map_cell_info(dog_map_cell).tile_index();

        if(dog_tile_index == valid_tile_index)
        {
            dog_map_position = new_dog_map_position;
        }

        bn::fixed dog_sprite_x = (dog_map_position.x() * 8) - (map_item.dimensions().width() * 4) + 4;
        bn::fixed dog_sprite_y = (dog_map_position.y() * 8) - (map_item.dimensions().height() * 4) + 4;
        dog_sprite.set_position(dog_sprite_x, dog_sprite_y);

        info.update();
        bn::core::update();
    }
}
