/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_blending.h"
#include "bn_bgs_mosaic.h"
#include "bn_rect_window.h"
#include "bn_bg_palettes.h"
#include "bn_bitmap_bg_attributes.h"
#include "bn_sprite_text_generator.h"
#include "bn_affine_bg_mat_attributes.h"
#include "bn_dp_direct_bitmap_bg_actions.h"
#include "bn_dp_direct_bitmap_bg_builder.h"
#include "bn_dp_direct_bitmap_bg_painter.h"
#include "bn_dp_direct_bitmap_bg_attributes_hbe_ptr.h"
#include "bn_dp_direct_bitmap_bg_mat_attributes_hbe_ptr.h"
#include "bn_dp_direct_bitmap_bg_pivot_position_hbe_ptr.h"

#include "bn_sprite_items_pivot.h"
#include "bn_sprite_items_turtle.h"
#include "bn_direct_bitmap_items_face.h"
#include "bn_direct_bitmap_items_escaping_criticism.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

namespace
{
    void dp_direct_bitmap_bgs_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("DP direct bitmap BGs", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_ptr bg = bn::dp_direct_bitmap_bg_ptr::create();
        bn::dp_direct_bitmap_bg_painter painter(bg);
        painter.fill(bn::color(14, 14, 14));
        painter.plot(80, 64, bn::color(31, 0, 0));
        painter.plot(96, 64, bn::color(0, 31, 0));
        painter.plot(80, 80, bn::color(0, 0, 31));
        painter.flip_page_later();

        while(! bn::keypad::start_pressed())
        {
            info.update();
            bn::core::update();
        }
    }

    void dp_direct_bitmap_bgs_painter_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("DP direct bitmap BGs painter", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_ptr bg = bn::dp_direct_bitmap_bg_ptr::create();
        bn::dp_direct_bitmap_bg_painter painter(bg);
        painter.fill(bn::color(16, 0, 16));
        painter.plot(56, 48, bn::color(31, 0, 0));
        painter.horizontal_line(12, 64, 32, bn::color(0, 31, 0));
        painter.vertical_line(8, 32, 96, bn::color(0, 0, 31));
        painter.line(12, 36, 44, 68, bn::color(31, 25, 20));

        if(bn::optional<bn::color> rectangle_color = painter.get(56, 48))
        {
            painter.rectangle(50, 64, 66, 80, *rectangle_color);
        }

        painter.blit(74, 14, bn::direct_bitmap_items::escaping_criticism);
        painter.draw(112, 32, bn::direct_bitmap_items::face, bn::color(31, 31, 31));
        painter.flip_page_later();

        while(! bn::keypad::start_pressed())
        {
            info.update();
            bn::core::update();
        }
    }

    void dp_direct_bitmap_bgs_rois_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("DP direct bitmap BGs RoIs", info_text_lines, text_generator);

        const bn::direct_bitmap_item& item = bn::direct_bitmap_items::escaping_criticism;
        bn::dp_direct_bitmap_bg_ptr bg = bn::dp_direct_bitmap_bg_ptr::create();
        bn::dp_direct_bitmap_bg_painter painter(bg);
        painter.fill(bn::color(14, 14, 14));
        painter.blit(72 - 20, 56, bn::direct_bitmap_roi(item, 22, 48, 16, 16));
        painter.blit(72, 56, bn::direct_bitmap_roi(item, 32, 12, 16, 16));
        painter.blit(72 + 20, 56, bn::direct_bitmap_roi(item, 64, 68, 16, 16));
        painter.flip_page_later();

        while(! bn::keypad::start_pressed())
        {
            info.update();
            bn::core::update();
        }
    }

    [[nodiscard]] bn::dp_direct_bitmap_bg_ptr create_bitmap_bg()
    {
        bn::dp_direct_bitmap_bg_ptr bg = bn::dp_direct_bitmap_bg_ptr::create();

        bn::dp_direct_bitmap_bg_painter painter(bg);
        painter.fill(bn::color(16, 16, 16));
        painter.blit(40, 14, bn::direct_bitmap_items::escaping_criticism);
        painter.flip_page_later();

        return bg;
    }

    void dp_direct_bitmap_bgs_visibility_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "A: hide/show BG",
            "",
            "START: go to next scene",
        };

        common::info info("DP direct bitmap BGs visibility", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_ptr bg = create_bitmap_bg();

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::a_pressed())
            {
                bg.set_visible(! bg.visible());
            }

            info.update();
            bn::core::update();
        }
    }

    void dp_direct_bitmap_bgs_visibility_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("DP direct bitmap BGs visibility actions", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_ptr bg = create_bitmap_bg();
        bn::dp_direct_bitmap_bg_visible_toggle_action action(bg, 60);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }
    }

    void dp_direct_bitmap_bgs_position_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "PAD: move BG",
            "",
            "START: go to next scene",
        };

        common::info info("DP direct bitmap BGs position", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_ptr bg = create_bitmap_bg();

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_held())
            {
                bg.set_x(bg.x() - 1);
            }
            else if(bn::keypad::right_held())
            {
                bg.set_x(bg.x() + 1);
            }

            if(bn::keypad::up_held())
            {
                bg.set_y(bg.y() - 1);
            }
            else if(bn::keypad::down_held())
            {
                bg.set_y(bg.y() + 1);
            }

            info.update();
            bn::core::update();
        }
    }

    void dp_direct_bitmap_bgs_position_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("DP direct bitmap BGs position actions", info_text_lines, text_generator);

        bn::fixed amplitude = 30;
        bn::dp_direct_bitmap_bg_ptr bg = create_bitmap_bg();
        bn::dp_direct_bitmap_bg_move_loop_action action(bg, 120, amplitude, amplitude);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }
    }

    void dp_direct_bitmap_rotation_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease rotation angle",
            "RIGHT: increase rotation angle",
            "",
            "START: go to next scene",
        };

        common::info info("DP direct bitmap BGs rotation", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_ptr bg = create_bitmap_bg();
        bg.set_rotation_angle(45);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed rotation_angle = bg.rotation_angle();

            if(bn::keypad::left_held())
            {
                bg.set_rotation_angle(bn::max(rotation_angle - 1, bn::fixed(0)));
            }
            else if(bn::keypad::right_held())
            {
                bg.set_rotation_angle(bn::min(rotation_angle + 1, bn::fixed(360)));
            }

            info.update();
            bn::core::update();
        }
    }

    void dp_direct_bitmap_bgs_rotation_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("DP direct bitmap BGs rotation actions", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_ptr bg = create_bitmap_bg();
        bn::dp_direct_bitmap_bg_rotate_loop_action action(bg, 180, 360);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }
    }

    void dp_direct_bitmap_bgs_scale_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease horizontal scale",
            "RIGHT: increase horizontal scale",
            "DOWN: decrease vertical scale",
            "UP: increase vertical scale",
            "",
            "START: go to next scene",
        };

        common::info info("DP direct bitmap BGs scale", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_ptr bg = create_bitmap_bg();
        bg.set_horizontal_scale(1.5);
        bg.set_vertical_scale(0.75);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed horizontal_scale = bg.horizontal_scale();
            bn::fixed vertical_scale = bg.vertical_scale();

            if(bn::keypad::left_held())
            {
                bg.set_horizontal_scale(bn::max(horizontal_scale - 0.01, bn::fixed(0.01)));
            }
            else if(bn::keypad::right_held())
            {
                bg.set_horizontal_scale(bn::min(horizontal_scale + 0.01, bn::fixed(2)));
            }

            if(bn::keypad::down_held())
            {
                bg.set_vertical_scale(bn::max(vertical_scale - 0.01, bn::fixed(0.01)));
            }
            else if(bn::keypad::up_held())
            {
                bg.set_vertical_scale(bn::min(vertical_scale + 0.01, bn::fixed(2)));
            }

            info.update();
            bn::core::update();
        }
    }

    void dp_direct_bitmap_bgs_expand_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "A: expand/restore BG",
            "",
            "START: go to next scene",
        };

        common::info info("DP direct bitmap BGs expand", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_ptr bg = bn::dp_direct_bitmap_bg_ptr::create();
        bg.expand();

        bn::dp_direct_bitmap_bg_painter painter(bg);
        painter.fill(bn::color(16, 0, 16));
        painter.line(0, 0, 16, 0, bn::color(31, 0, 0));
        painter.line(0, 0, 0, 16, bn::color(31, 0, 0));
        painter.line(159, 127, 159 - 16, 127, bn::color(31, 0, 0));
        painter.line(159, 127, 159, 127 - 16, bn::color(31, 0, 0));
        painter.blit(40, 14, bn::direct_bitmap_items::escaping_criticism);
        painter.flip_page_later();

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::a_pressed())
            {
                if(bg.horizontal_scale() == 1)
                {
                    bg.expand();
                }
                else
                {
                    bg.set_scale(1);
                }
            }

            info.update();
            bn::core::update();
        }
    }

    void dp_direct_bitmap_bgs_scale_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("DP direct bitmap BGs scale actions", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_ptr bg = create_bitmap_bg();
        bg.set_scale(0.01);

        bn::dp_direct_bitmap_bg_scale_loop_action action(bg, 120, 2);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }
    }

    void dp_direct_bitmap_bgs_shear_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease horizontal shear",
            "RIGHT: increase horizontal shear",
            "DOWN: decrease vertical shear",
            "UP: increase vertical shear",
            "",
            "START: go to next scene",
        };

        common::info info("DP direct bitmap BGs shear", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_ptr bg = create_bitmap_bg();
        bg.set_horizontal_shear(0.5);
        bg.set_vertical_shear(-0.75);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed horizontal_shear = bg.horizontal_shear();
            bn::fixed vertical_shear = bg.vertical_shear();

            if(bn::keypad::left_held())
            {
                bg.set_horizontal_shear(horizontal_shear - 0.01);
            }
            else if(bn::keypad::right_held())
            {
                bg.set_horizontal_shear(horizontal_shear + 0.01);
            }

            if(bn::keypad::down_held())
            {
                bg.set_vertical_shear(vertical_shear - 0.01);
            }
            else if(bn::keypad::up_held())
            {
                bg.set_vertical_shear(vertical_shear + 0.01);
            }

            info.update();
            bn::core::update();
        }
    }

    void dp_direct_bitmap_bgs_shear_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("DP direct bitmap BGs shear actions", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_ptr bg = create_bitmap_bg();
        bg.set_shear(-0.5);

        bn::dp_direct_bitmap_bg_shear_loop_action action(bg, 120, 0.5);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }
    }

    void dp_direct_bitmap_bgs_flip_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: disable horizontal flip",
            "RIGHT: enable horizontal flip",
            "DOWN: disable vertical flip",
            "UP: enable vertical flip",
            "",
            "START: go to next scene",
        };

        common::info info("DP direct bitmap BGs flip", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_ptr bg = create_bitmap_bg();
        bg.set_horizontal_flip(true);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_pressed())
            {
                bg.set_horizontal_flip(false);
            }
            else if(bn::keypad::right_pressed())
            {
                bg.set_horizontal_flip(true);
            }

            if(bn::keypad::down_pressed())
            {
                bg.set_vertical_flip(false);
            }
            else if(bn::keypad::up_pressed())
            {
                bg.set_vertical_flip(true);
            }

            info.update();
            bn::core::update();
        }
    }

    void dp_direct_bitmap_bgs_flip_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("DP direct bitmap BGs flip actions", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_ptr bg = create_bitmap_bg();
        bn::dp_direct_bitmap_bg_horizontal_flip_toggle_action action(bg, 60);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }
    }

    void dp_direct_bitmap_bgs_pivot_position_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "PAD: move BG",
            "",
            "START: go to next scene",
        };

        common::info info("DP direct bitmap BGs pivot position", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_ptr bg = create_bitmap_bg();
        bg.set_rotation_angle(45);

        bn::sprite_ptr pivot_sprite = bn::sprite_items::pivot.create_sprite(0, 0);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_held())
            {
                bg.set_pivot_x(bg.pivot_x() - 1);
            }
            else if(bn::keypad::right_held())
            {
                bg.set_pivot_x(bg.pivot_x() + 1);
            }

            if(bn::keypad::up_held())
            {
                bg.set_pivot_y(bg.pivot_y() - 1);
            }
            else if(bn::keypad::down_held())
            {
                bg.set_pivot_y(bg.pivot_y() + 1);
            }

            pivot_sprite.set_position(bg.pivot_position());
            info.update();
            bn::core::update();
        }
    }

    void dp_direct_bitmap_bgs_pivot_position_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("DPD BGs pivot position actions", info_text_lines, text_generator);

        bn::fixed amplitude = 30;
        bn::dp_direct_bitmap_bg_ptr bg = create_bitmap_bg();
        bg.set_rotation_angle(45);

        bn::sprite_ptr pivot_sprite = bn::sprite_items::pivot.create_sprite(0, 0);
        bn::dp_direct_bitmap_bg_move_pivot_loop_action action(bg, 120, amplitude, amplitude);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            pivot_sprite.set_position(bg.pivot_position());
            info.update();
            bn::core::update();
        }
    }

    void dp_direct_bitmap_bgs_pivot_position_hbe_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("DPD BGs pivot H-Blank effect", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_ptr bg = create_bitmap_bg();
        bg.set_rotation_angle(45);

        bn::array<bn::fixed, bn::display::height()> horizontal_deltas;
        bn::dp_direct_bitmap_bg_pivot_position_hbe_ptr horizontal_hbe =
                bn::dp_direct_bitmap_bg_pivot_position_hbe_ptr::create_horizontal(bg, horizontal_deltas);

        bn::fixed base_degrees_angle;

        while(! bn::keypad::start_pressed())
        {
            base_degrees_angle += 4;

            if(base_degrees_angle >= 360)
            {
                base_degrees_angle -= 360;
            }

            bn::fixed degrees_angle = base_degrees_angle;

            for(int index = 0, limit = bn::display::height() / 2; index < limit; ++index)
            {
                degrees_angle += 16;

                if(degrees_angle >= 360)
                {
                    degrees_angle -= 360;
                }

                bn::fixed desp = bn::degrees_lut_sin(degrees_angle) * 8;
                horizontal_deltas[(bn::display::height() / 2) + index] = desp;
                horizontal_deltas[(bn::display::height() / 2) - index - 1] = desp;
            }

            horizontal_hbe.reload_deltas_ref();
            info.update();
            bn::core::update();
        }
    }

    void dp_direct_bitmap_bgs_mat_attributes_hbe_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("DPD BGs matrix H-Blank effect", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_ptr bg = create_bitmap_bg();

        bn::rect_window internal_window = bn::rect_window::internal();
        internal_window.set_top_left(-(bn::display::height() / 2) + 2, -1000);
        internal_window.set_bottom_right((bn::display::height() / 2) - 2, 1000);
        bn::window::outside().set_show_bg(bg, false);

        const bn::affine_bg_mat_attributes& base_attributes = bg.mat_attributes();
        bn::affine_bg_mat_attributes attributes[bn::display::height()];

        for(int index = 0, limit = bn::display::height(); index < limit; ++index)
        {
            attributes[index] = base_attributes;
        }

        bn::dp_direct_bitmap_bg_mat_attributes_hbe_ptr attributes_hbe =
                bn::dp_direct_bitmap_bg_mat_attributes_hbe_ptr::create(bg, attributes);

        bn::fixed base_degrees_angle;

        while(! bn::keypad::start_pressed())
        {
            base_degrees_angle += 4;

            if(base_degrees_angle >= 360)
            {
                base_degrees_angle -= 360;
            }

            bn::fixed degrees_angle = base_degrees_angle;

            for(int index = 0, limit = bn::display::height() / 2; index < limit; ++index)
            {
                degrees_angle += 16;

                if(degrees_angle >= 360)
                {
                    degrees_angle -= 360;
                }

                bn::fixed rotation_inc = bn::degrees_lut_sin(degrees_angle) * 4;
                attributes[(bn::display::height() / 2) + index].set_rotation_angle(45 + rotation_inc);
                attributes[(bn::display::height() / 2) - index - 1].set_rotation_angle(45 + rotation_inc);
            }

            attributes_hbe.reload_attributes_ref();
            info.update();
            bn::core::update();
        }
    }

    void dp_direct_bitmap_bgs_priority_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease BG priority",
            "RIGHT: increase BG priority",
            "",
            "START: go to next scene",
        };

        common::info info("DP direct bitmap BGs priority", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_ptr bg = create_bitmap_bg();
        bg.set_priority(2);

        bn::sprite_ptr turtle_sprite = bn::sprite_items::turtle.create_sprite(24, 0);
        turtle_sprite.set_bg_priority(2);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_pressed())
            {
                bg.set_priority(1);
            }
            else if(bn::keypad::right_pressed())
            {
                bg.set_priority(2);
            }

            info.update();
            bn::core::update();
        }
    }

    void dp_direct_bitmap_bgs_attributes_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "B: restore old attributes",
            "A: set new attributes",
            "",
            "START: go to next scene",
        };

        common::info info("DP direct bitmap BGs attributes", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_ptr bg = create_bitmap_bg();
        bg.set_priority(2);

        bn::sprite_ptr turtle_sprite = bn::sprite_items::turtle.create_sprite(24, 0);
        turtle_sprite.set_bg_priority(2);

        bn::bgs_mosaic::set_stretch(0.5);

        bn::bitmap_bg_attributes old_attributes = bg.attributes();
        bn::bitmap_bg_attributes new_attributes = old_attributes;
        new_attributes.set_priority(1);
        new_attributes.set_mosaic_enabled(true);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::b_pressed())
            {
                bg.set_attributes(old_attributes);
            }

            if(bn::keypad::a_pressed())
            {
                bg.set_attributes(new_attributes);
            }

            info.update();
            bn::core::update();
        }

        bn::bgs_mosaic::set_stretch(0);
    }

    void dp_direct_bitmap_bgs_attributes_hbe_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("DPD BGs attributes H-Blank effect", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_ptr bg = create_bitmap_bg();
        bg.set_priority(2);

        bn::sprite_ptr turtle_sprite = bn::sprite_items::turtle.create_sprite(24, 0);
        turtle_sprite.set_bg_priority(2);

        bn::bgs_mosaic::set_stretch(0.5);

        bn::bitmap_bg_attributes old_attributes = bg.attributes();
        bn::bitmap_bg_attributes new_attributes = old_attributes;
        new_attributes.set_priority(1);
        new_attributes.set_mosaic_enabled(true);

        bn::vector<bn::bitmap_bg_attributes, bn::display::height()> attributes;

        for(int index = 0; index < bn::display::height(); ++index)
        {
            if(index % 4 < 2)
            {
                attributes.push_back(old_attributes);
            }
            else
            {
                attributes.push_back(new_attributes);
            }
        }

        bn::span<const bn::bitmap_bg_attributes> attributes_ref(attributes.data(), attributes.size());
        bn::dp_direct_bitmap_bg_attributes_hbe_ptr attributes_hbe =
                bn::dp_direct_bitmap_bg_attributes_hbe_ptr::create(bg, attributes_ref);

        while(! bn::keypad::start_pressed())
        {
            info.update();
            bn::core::update();
        }

        bn::bgs_mosaic::set_stretch(0);
    }

    void dp_direct_bitmap_bgs_mat_attributes_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "B: restore old attributes",
            "A: set new attributes",
            "",
            "START: go to next scene",
        };

        common::info info("DPD bitmap BGs matrix attributes", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_ptr bg = create_bitmap_bg();
        bg.set_rotation_angle(45);
        bg.set_scale(1.5);

        bn::affine_mat_attributes old_attributes = bg.mat_attributes().mat_attributes();
        bn::affine_mat_attributes new_attributes = old_attributes;
        new_attributes.set_scale(2);
        new_attributes.set_horizontal_shear(0.5);
        new_attributes.set_vertical_shear(-0.75);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::b_pressed())
            {
                bg.set_mat_attributes(old_attributes);
            }

            if(bn::keypad::a_pressed())
            {
                bg.set_mat_attributes(new_attributes);
            }

            info.update();
            bn::core::update();
        }
    }

    void dp_direct_bitmap_bg_builder_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("DP direct bitmap BG builder", info_text_lines, text_generator);

        bn::dp_direct_bitmap_bg_builder builder;
        builder.set_position(-8, -8);
        builder.set_rotation_angle(45);
        builder.set_scale(3);
        builder.set_blending_enabled(true);

        bn::dp_direct_bitmap_bg_ptr bg = builder.build();
        bn::dp_direct_bitmap_bg_painter painter(bg);
        painter.fill(bn::color(16, 0, 16));
        painter.plot(80, 64, bn::color(31, 0, 0));
        painter.plot(96, 64, bn::color(0, 31, 0));
        painter.plot(80, 80, bn::color(0, 0, 31));
        painter.flip_page_later();

        bn::blending::set_transparency_and_intensity_alpha(0.6, 0.8);

        while(! bn::keypad::start_pressed())
        {
            info.update();
            bn::core::update();
        }

        bn::blending::restore();
    }
}

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    while(true)
    {
        dp_direct_bitmap_bgs_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_painter_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_rois_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_visibility_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_visibility_actions_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_position_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_position_actions_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_rotation_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_rotation_actions_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_scale_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_expand_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_scale_actions_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_shear_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_shear_actions_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_flip_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_flip_actions_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_pivot_position_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_pivot_position_actions_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_pivot_position_hbe_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_mat_attributes_hbe_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_priority_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_attributes_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_attributes_hbe_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bgs_mat_attributes_scene(text_generator);
        bn::core::update();

        dp_direct_bitmap_bg_builder_scene(text_generator);
        bn::core::update();
    }
}
