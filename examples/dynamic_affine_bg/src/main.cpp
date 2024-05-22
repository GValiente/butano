/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_memory.h"
#include "bn_bg_tiles.h"
#include "bn_affine_bg_ptr.h"
#include "bn_affine_bg_item.h"
#include "bn_affine_bg_map_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_affine_bg_map_cell_info.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#include "bn_sprite_items_cursor.h"
#include "bn_bg_palette_items_palette.h"
#include "bn_affine_bg_tiles_items_tiles.h"

namespace
{
    struct bg_map
    {
        static const int columns = 32;
        static const int rows = 32;
        static const int cells_count = columns * rows;

        alignas(int) bn::affine_bg_map_cell cells[cells_count];
        bn::affine_bg_map_item map_item;

        bg_map() :
            map_item(cells[0], bn::size(bg_map::columns, bg_map::rows))
        {
            reset();
        }

        bool dig(int x, int y)
        {
            bn::affine_bg_map_cell& current_cell = cells[map_item.cell_index(x, y)];
            bn::affine_bg_map_cell_info cell_info(current_cell);

            if(cell_info.tile_index() == 1)
            {
                return false;
            }

            cell_info.set_tile_index(1);
            current_cell = cell_info.cell();

            bn::affine_bg_map_cell& up_cell = cells[map_item.cell_index(x, y - 1)];
            cell_info.set_cell(up_cell);

            if(cell_info.tile_index() == 0)
            {
                cell_info.set_tile_index(2);
                up_cell = cell_info.cell();
            }

            return true;
        }

        void reset()
        {
            bn::memory::clear(cells_count, cells[0]);
        }
    };

    bn::fixed sprite_x(int cursor_x)
    {
        return (cursor_x * 8) - (bg_map::columns * 4) + 4;
    }

    bn::fixed sprite_y(int cursor_y)
    {
        return (cursor_y * 8) - (bg_map::rows * 4) + 4;
    }
}

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);

    constexpr bn::string_view info_text_lines[] = {
        "PAD: move cursor",
        "",
        "START: reset BG",
    };

    common::info info("Dynamic affine BG", info_text_lines, text_generator);

    bn::bg_tiles::set_allow_offset(false);

    bn::unique_ptr<bg_map> bg_map_ptr(new bg_map());
    bn::affine_bg_item bg_item(
                bn::affine_bg_tiles_items::tiles, bn::bg_palette_items::palette, bg_map_ptr->map_item);
    bn::affine_bg_ptr bg = bg_item.create_bg(0, 0);
    bn::affine_bg_map_ptr bg_map = bg.map();

    bn::bg_tiles::set_allow_offset(true);

    int cursor_x = bg_map::columns / 2;
    int cursor_y = bg_map::rows / 2;
    bn::sprite_ptr cursor_sprite = bn::sprite_items::cursor.create_sprite(sprite_x(cursor_x), sprite_y(cursor_y));
    bg_map_ptr->dig(cursor_x, cursor_y);
    bg_map.reload_cells_ref();

    while(true)
    {
        if(bn::keypad::left_pressed())
        {
            if(cursor_x > 4)
            {
                --cursor_x;

                if(bg_map_ptr->dig(cursor_x, cursor_y))
                {
                    bg_map.reload_cells_ref();
                    bg.set_scale(1.2);
                }
            }
        }
        else if(bn::keypad::right_pressed())
        {
            if(cursor_x < bg_map::columns - 4 - 1)
            {
                ++cursor_x;

                if(bg_map_ptr->dig(cursor_x, cursor_y))
                {
                    bg_map.reload_cells_ref();
                    bg.set_scale(1.2);
                }
            }
        }

        if(bn::keypad::up_pressed())
        {
            if(cursor_y > 10)
            {
                --cursor_y;

                if(bg_map_ptr->dig(cursor_x, cursor_y))
                {
                    bg_map.reload_cells_ref();
                    bg.set_scale(1.2);
                }
            }
        }
        else if(bn::keypad::down_pressed())
        {
            if(cursor_y < bg_map::rows - 10 - 1)
            {
                ++cursor_y;

                if(bg_map_ptr->dig(cursor_x, cursor_y))
                {
                    bg_map.reload_cells_ref();
                    bg.set_scale(1.2);
                }
            }
        }

        if(bn::keypad::start_pressed())
        {
            bg_map_ptr->reset();

            cursor_x = bg_map::columns / 2;
            cursor_y = bg_map::rows / 2;
            cursor_sprite.set_position(sprite_x(cursor_x), sprite_y(cursor_y));

            if(bg_map_ptr->dig(cursor_x, cursor_y))
            {
                bg_map.reload_cells_ref();
                bg.set_scale(1.2);
            }
        }
        else
        {
            bn::fixed cursor_sprite_x = cursor_sprite.x();
            bn::fixed target_cursor_sprite_x = sprite_x(cursor_x);

            if(cursor_sprite_x < target_cursor_sprite_x)
            {
                cursor_sprite.set_x(cursor_sprite_x + 1);
            }
            else if(cursor_sprite_x > target_cursor_sprite_x)
            {
                cursor_sprite.set_x(cursor_sprite_x - 1);
            }

            bn::fixed cursor_sprite_y = cursor_sprite.y();
            bn::fixed target_cursor_sprite_y = sprite_y(cursor_y);

            if(cursor_sprite_y < target_cursor_sprite_y)
            {
                cursor_sprite.set_y(cursor_sprite_y + 1);
            }
            else if(cursor_sprite_y > target_cursor_sprite_y)
            {
                cursor_sprite.set_y(cursor_sprite_y - 1);
            }

            bg.set_scale(bn::max(bg.horizontal_scale() - 0.05, bn::fixed(1)));
        }

        info.update();
        bn::core::update();
    }
}
