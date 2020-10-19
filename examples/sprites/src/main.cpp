#include "btn_core.h"
#include "btn_math.h"
#include "btn_keypad.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_blending.h"
#include "btn_bg_palettes.h"
#include "btn_regular_bg_ptr.h"
#include "btn_sprites_mosaic.h"
#include "btn_sprite_actions.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_text_generator.h"
#include "btn_sprite_first_attributes.h"
#include "btn_sprite_third_attributes.h"
#include "btn_sprite_affine_second_attributes.h"
#include "btn_sprite_regular_second_attributes.h"
#include "btn_sprite_position_hblank_effect_ptr.h"
#include "btn_sprite_first_attributes_hblank_effect_ptr.h"
#include "btn_sprite_third_attributes_hblank_effect_ptr.h"
#include "btn_sprite_affine_second_attributes_hblank_effect_ptr.h"
#include "btn_sprite_regular_second_attributes_hblank_effect_ptr.h"

#include "btn_sprite_items_caveman.h"
#include "btn_sprite_items_red_sprite.h"
#include "btn_sprite_items_blue_sprite.h"
#include "btn_sprite_items_green_sprite.h"
#include "btn_sprite_items_yellow_sprite.h"
#include "btn_regular_bg_items_red_bg.h"
#include "btn_regular_bg_items_blue_bg.h"
#include "btn_regular_bg_items_green_bg.h"
#include "btn_regular_bg_items_yellow_bg.h"

#include "info.h"
#include "variable_8x16_sprite_font.h"

namespace
{
    void sprites_visibility_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "A: hide/show sprite",
            "",
            "START: go to next scene",
        };

        info info("Sprites visibility", info_text_lines, text_generator);

        btn::sprite_ptr red_sprite = btn::sprite_items::red_sprite.create_sprite(0, 0);

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::a_pressed())
            {
                red_sprite.set_visible(! red_sprite.visible());
            }

            info.update();
            btn::core::update();
        }
    }

    void sprites_visibility_actions_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Sprites visibility actions", info_text_lines, text_generator);

        btn::sprite_ptr green_sprite = btn::sprite_items::green_sprite.create_sprite(0, 0);
        btn::sprite_visible_toggle_action action(green_sprite, 60);

        while(! btn::keypad::start_pressed())
        {
            action.update();
            info.update();
            btn::core::update();
        }
    }

    void sprites_position_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "PAD: move sprite",
            "",
            "START: go to next scene",
        };

        info info("Sprites position", info_text_lines, text_generator);

        btn::sprite_ptr blue_sprite = btn::sprite_items::blue_sprite.create_sprite(0, 0);

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::left_held())
            {
                blue_sprite.set_x(blue_sprite.x() - 1);
            }
            else if(btn::keypad::right_held())
            {
                blue_sprite.set_x(blue_sprite.x() + 1);
            }

            if(btn::keypad::up_held())
            {
                blue_sprite.set_y(blue_sprite.y() - 1);
            }
            else if(btn::keypad::down_held())
            {
                blue_sprite.set_y(blue_sprite.y() + 1);
            }

            info.update();
            btn::core::update();
        }
    }

    void sprites_position_actions_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Sprites position actions", info_text_lines, text_generator);

        btn::fixed amplitude = 30;
        btn::sprite_ptr yellow_sprite = btn::sprite_items::yellow_sprite.create_sprite(-amplitude, -amplitude);
        btn::sprite_move_loop_action action(yellow_sprite, 120, amplitude, amplitude);

        while(! btn::keypad::start_pressed())
        {
            action.update();
            info.update();
            btn::core::update();
        }
    }

    void sprites_position_hblank_effect_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Sprites position H-Blank effect", info_text_lines, text_generator);

        btn::sprite_ptr red_sprite = btn::sprite_items::red_sprite.create_sprite(0, 0);

        btn::array<btn::fixed, btn::display::height()> horizontal_deltas;

        btn::sprite_position_hblank_effect_ptr horizontal_hblank_effect =
                btn::sprite_position_hblank_effect_ptr::create_horizontal(red_sprite, horizontal_deltas);

        btn::fixed base_degrees_angle;

        while(! btn::keypad::start_pressed())
        {
            base_degrees_angle += 4;

            if(base_degrees_angle >= 360)
            {
                base_degrees_angle -= 360;
            }

            btn::fixed degrees_angle = base_degrees_angle;

            for(int index = 0, limit = btn::display::height() / 2; index < limit; ++index)
            {
                degrees_angle += 16;

                if(degrees_angle >= 360)
                {
                    degrees_angle -= 360;
                }

                btn::fixed desp = btn::degrees_sin(degrees_angle) * 6;
                horizontal_deltas[(btn::display::height() / 2) + index] = desp;
                horizontal_deltas[(btn::display::height() / 2) - index - 1] = desp;
            }

            horizontal_hblank_effect.reload_deltas_ref();
            info.update();
            btn::core::update();
        }
    }

    void sprites_rotation_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: decrease rotation angle",
            "RIGHT: increase rotation angle",
            "",
            "START: go to next scene",
        };

        info info("Sprites rotation", info_text_lines, text_generator);

        btn::sprite_ptr green_sprite = btn::sprite_items::green_sprite.create_sprite(0, 0);
        green_sprite.set_rotation_angle(45);

        while(! btn::keypad::start_pressed())
        {
            btn::fixed rotation_angle = green_sprite.rotation_angle();

            if(btn::keypad::left_held())
            {
                green_sprite.set_rotation_angle(btn::max(rotation_angle - 1, btn::fixed(0)));
            }
            else if(btn::keypad::right_held())
            {
                green_sprite.set_rotation_angle(btn::min(rotation_angle + 1, btn::fixed(360)));
            }

            info.update();
            btn::core::update();
        }
    }

    void sprites_rotation_actions_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Sprites rotation actions", info_text_lines, text_generator);

        btn::sprite_ptr blue_sprite = btn::sprite_items::blue_sprite.create_sprite(0, 0);
        btn::sprite_rotate_loop_action action(blue_sprite, 180, 360);

        while(! btn::keypad::start_pressed())
        {
            action.update();
            info.update();
            btn::core::update();
        }
    }

    void sprites_scale_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: decrease horizontal scale",
            "RIGHT: increase horizontal scale",
            "DOWN: decrease vertical scale",
            "UP: increase vertical scale",
            "",
            "START: go to next scene",
        };

        info info("Sprites scale", info_text_lines, text_generator);

        btn::sprite_ptr yellow_sprite = btn::sprite_items::yellow_sprite.create_sprite(0, 0);
        yellow_sprite.set_scale_x(1.5);
        yellow_sprite.set_scale_y(0.75);

        while(! btn::keypad::start_pressed())
        {
            btn::fixed scale_x = yellow_sprite.scale_x();
            btn::fixed scale_y = yellow_sprite.scale_y();

            if(btn::keypad::left_held())
            {
                yellow_sprite.set_scale_x(btn::max(scale_x - 0.01, btn::fixed(0.01)));
            }
            else if(btn::keypad::right_held())
            {
                yellow_sprite.set_scale_x(btn::min(scale_x + 0.01, btn::fixed(2)));
            }

            if(btn::keypad::down_held())
            {
                yellow_sprite.set_scale_y(btn::max(scale_y - 0.01, btn::fixed(0.01)));
            }
            else if(btn::keypad::up_held())
            {
                yellow_sprite.set_scale_y(btn::min(scale_y + 0.01, btn::fixed(2)));
            }

            info.update();
            btn::core::update();
        }
    }

    void sprites_scale_actions_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Sprites scale actions", info_text_lines, text_generator);

        btn::sprite_ptr red_sprite = btn::sprite_items::red_sprite.create_sprite(0, 0);
        red_sprite.set_scale(0.01);

        btn::sprite_scale_loop_action action(red_sprite, 120, 2);

        while(! btn::keypad::start_pressed())
        {
            action.update();
            info.update();
            btn::core::update();
        }
    }

    void sprites_flip_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: disable horizontal flip",
            "RIGHT: enable horizontal flip",
            "DOWN: disable vertical flip",
            "UP: enable vertical flip",
            "",
            "START: go to next scene",
        };

        info info("Sprites flip", info_text_lines, text_generator);

        btn::sprite_ptr green_sprite = btn::sprite_items::green_sprite.create_sprite(0, 0);
        green_sprite.set_horizontal_flip(true);

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::left_pressed())
            {
                green_sprite.set_horizontal_flip(false);
            }
            else if(btn::keypad::right_pressed())
            {
                green_sprite.set_horizontal_flip(true);
            }

            if(btn::keypad::down_pressed())
            {
                green_sprite.set_vertical_flip(false);
            }
            else if(btn::keypad::up_pressed())
            {
                green_sprite.set_vertical_flip(true);
            }

            info.update();
            btn::core::update();
        }
    }

    void sprites_flip_actions_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Sprites flip actions", info_text_lines, text_generator);

        btn::sprite_ptr blue_sprite = btn::sprite_items::blue_sprite.create_sprite(0, 0);
        btn::sprite_horizontal_flip_toggle_action action(blue_sprite, 120);

        while(! btn::keypad::start_pressed())
        {
            action.update();
            info.update();
            btn::core::update();
        }
    }

    void sprites_double_size_mode_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "A: toggle double size mode",
            "B: enable auto double size mode",
            "",
            "START: go to next scene",
        };

        info info("Sprites double size mode", info_text_lines, text_generator);

        btn::sprite_ptr yellow_sprite = btn::sprite_items::yellow_sprite.create_sprite(0, 0);
        yellow_sprite.set_scale(2);
        yellow_sprite.set_double_size_mode(btn::sprite_double_size_mode::DISABLED);

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::a_pressed())
            {
                if(yellow_sprite.double_size_mode() == btn::sprite_double_size_mode::DISABLED)
                {
                    yellow_sprite.set_double_size_mode(btn::sprite_double_size_mode::ENABLED);
                }
                else
                {
                    yellow_sprite.set_double_size_mode(btn::sprite_double_size_mode::DISABLED);
                }
            }

            if(btn::keypad::b_pressed())
            {
                yellow_sprite.set_double_size_mode(btn::sprite_double_size_mode::AUTO);
            }

            info.update();
            btn::core::update();
        }
    }

    void sprites_bg_priority_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: decrease caveman BG priority",
            "RIGHT: increase caveman BG priority",
            "",
            "START: go to next scene",
        };

        info info("Sprites BG priority", info_text_lines, text_generator);

        btn::regular_bg_ptr green_bg = btn::regular_bg_items::green_bg.create_bg(-56, -56);
        green_bg.set_priority(0);

        btn::regular_bg_ptr blue_bg = btn::regular_bg_items::blue_bg.create_bg(-56, 56);
        blue_bg.set_priority(1);

        btn::regular_bg_ptr yellow_bg = btn::regular_bg_items::yellow_bg.create_bg(56, -56);
        yellow_bg.set_priority(2);

        btn::regular_bg_ptr red_bg = btn::regular_bg_items::red_bg.create_bg(56, 56);
        red_bg.set_priority(3);

        btn::sprite_ptr caveman_sprite = btn::sprite_items::caveman.create_sprite(0, 0);
        caveman_sprite.set_bg_priority(2);
        caveman_sprite.set_z_order(1);

        while(! btn::keypad::start_pressed())
        {
            int priority = caveman_sprite.bg_priority();

            if(btn::keypad::left_pressed())
            {
                caveman_sprite.set_bg_priority(btn::max(priority - 1, 0));
            }
            else if(btn::keypad::right_pressed())
            {
                caveman_sprite.set_bg_priority(btn::min(priority + 1, 3));
            }

            info.update();
            btn::core::update();
        }
    }

    void sprites_z_order_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: decrease red sprite Z order",
            "RIGHT: increase red sprite Z order",
            "",
            "START: go to next scene",
        };

        info info("Sprites Z order", info_text_lines, text_generator);

        btn::sprite_ptr green_sprite = btn::sprite_items::green_sprite.create_sprite(-32, -32);
        green_sprite.set_z_order(0);

        btn::sprite_ptr blue_sprite = btn::sprite_items::blue_sprite.create_sprite(-32, 32);
        blue_sprite.set_z_order(1);

        btn::sprite_ptr yellow_sprite = btn::sprite_items::yellow_sprite.create_sprite(32, 32);
        yellow_sprite.set_z_order(2);

        btn::sprite_ptr red_sprite = btn::sprite_items::red_sprite.create_sprite(0, 0);
        red_sprite.set_z_order(2);

        while(! btn::keypad::start_pressed())
        {
            int z_order = red_sprite.z_order();

            if(btn::keypad::left_pressed())
            {
                red_sprite.set_z_order(btn::max(z_order - 1, 0));
            }
            else if(btn::keypad::right_pressed())
            {
                red_sprite.set_z_order(btn::min(z_order + 1, 3));
            }

            info.update();
            btn::core::update();
        }
    }

    void sprites_put_above_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "B: put blue sprite above red sprite",
            "A: put yellow sprite above red sprite",
            "L: put green sprite above red sprite",
            "R: put red sprite above all",
            "",
            "START: go to next scene",
        };

        info info("Sprites put above", info_text_lines, text_generator);

        btn::sprite_ptr green_sprite = btn::sprite_items::green_sprite.create_sprite(-32, -32);
        btn::sprite_ptr blue_sprite = btn::sprite_items::blue_sprite.create_sprite(-32, 32);
        btn::sprite_ptr yellow_sprite = btn::sprite_items::yellow_sprite.create_sprite(32, 32);
        btn::optional<btn::sprite_ptr> red_sprite = btn::sprite_items::red_sprite.create_sprite(0, 0);

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::b_pressed())
            {
                blue_sprite.put_above(*red_sprite);
            }

            if(btn::keypad::a_pressed())
            {
                yellow_sprite.put_above(*red_sprite);
            }

            if(btn::keypad::l_pressed())
            {
                green_sprite.put_above(*red_sprite);
            }

            if(btn::keypad::r_pressed())
            {
                red_sprite.reset();
                red_sprite = btn::sprite_items::red_sprite.create_sprite(0, 0);
            }

            info.update();
            btn::core::update();
        }
    }

    void sprites_first_attributes_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "B: copy blue attributes to red",
            "A: copy yellow attributes to red",
            "L: copy green attributes to red",
            "R: restore red attributes",
            "",
            "START: go to next scene",
        };

        info info("First attributes", info_text_lines, text_generator);

        btn::sprites_mosaic::set_stretch(0.1);

        btn::sprite_ptr green_sprite = btn::sprite_items::green_sprite.create_sprite(-32, -32);
        btn::sprite_ptr blue_sprite = btn::sprite_items::blue_sprite.create_sprite(-32, 32);
        btn::sprite_ptr yellow_sprite = btn::sprite_items::yellow_sprite.create_sprite(32, 32);
        btn::sprite_ptr red_sprite = btn::sprite_items::red_sprite.create_sprite(0, 0);
        btn::sprite_first_attributes red_sprite_attributes = red_sprite.first_attributes();
        green_sprite.set_mosaic_enabled(true);
        yellow_sprite.set_mosaic_enabled(true);

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::b_pressed())
            {
                red_sprite.set_first_attributes(blue_sprite.first_attributes());
            }

            if(btn::keypad::a_pressed())
            {
                red_sprite.set_first_attributes(yellow_sprite.first_attributes());
            }

            if(btn::keypad::l_pressed())
            {
                red_sprite.set_first_attributes(green_sprite.first_attributes());
            }

            if(btn::keypad::r_pressed())
            {
                red_sprite.set_first_attributes(red_sprite_attributes);
            }

            info.update();
            btn::core::update();
        }

        btn::sprites_mosaic::set_stretch(0);
    }

    void sprites_first_attributes_hblank_effect_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("First attributes H-Blank effect", info_text_lines, text_generator);

        btn::sprites_mosaic::set_stretch(0.2);
        btn::blending::set_transparency_alpha(0.6);

        btn::sprite_ptr red_sprite = btn::sprite_items::red_sprite.create_sprite(0, 0);
        red_sprite.set_scale(2);

        btn::sprite_first_attributes red_attributes = red_sprite.first_attributes();
        btn::sprite_first_attributes alt_attributes = red_attributes;
        alt_attributes.set_mosaic_enabled(true);
        alt_attributes.set_blending_enabled(true);

        btn::vector<btn::sprite_first_attributes, btn::display::height()> attributes;

        for(int index = 0; index < btn::display::height(); ++index)
        {
            if(index % 2)
            {
                attributes.push_back(alt_attributes);
            }
            else
            {
                attributes.push_back(red_attributes);
            }
        }

        btn::span<const btn::sprite_first_attributes> attributes_ref(attributes.data(), attributes.size());
        btn::sprite_first_attributes_hblank_effect_ptr hblank_effect =
                btn::sprite_first_attributes_hblank_effect_ptr::create(red_sprite, attributes_ref);

        while(! btn::keypad::start_pressed())
        {
            info.update();
            btn::core::update();
        }

        btn::sprites_mosaic::set_stretch(0);
        btn::blending::set_transparency_alpha(1);
    }

    void sprites_regular_second_attributes_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "B: copy blue attributes to red",
            "A: copy yellow attributes to red",
            "L: copy green attributes to red",
            "R: restore red attributes",
            "",
            "START: go to next scene",
        };

        info info("Regular second attributes", info_text_lines, text_generator);

        btn::sprite_ptr green_sprite = btn::sprite_items::green_sprite.create_sprite(-32, -32);
        btn::sprite_ptr blue_sprite = btn::sprite_items::blue_sprite.create_sprite(-32, 32);
        btn::sprite_ptr yellow_sprite = btn::sprite_items::yellow_sprite.create_sprite(32, 32);
        btn::sprite_ptr red_sprite = btn::sprite_items::red_sprite.create_sprite(0, 0);
        btn::sprite_regular_second_attributes red_sprite_attributes = red_sprite.regular_second_attributes();
        green_sprite.set_horizontal_flip(true);
        yellow_sprite.set_vertical_flip(true);

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::b_pressed())
            {
                red_sprite.set_regular_second_attributes(blue_sprite.regular_second_attributes());
            }

            if(btn::keypad::a_pressed())
            {
                red_sprite.set_regular_second_attributes(yellow_sprite.regular_second_attributes());
            }

            if(btn::keypad::l_pressed())
            {
                red_sprite.set_regular_second_attributes(green_sprite.regular_second_attributes());
            }

            if(btn::keypad::r_pressed())
            {
                red_sprite.set_regular_second_attributes(red_sprite_attributes);
            }

            info.update();
            btn::core::update();
        }
    }

    void sprites_regular_second_attributes_hblank_effect_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Reg. second attribs. H-Blank effect", info_text_lines, text_generator);

        btn::sprite_ptr red_sprite = btn::sprite_items::red_sprite.create_sprite(0, 0);
        btn::sprite_regular_second_attributes red_attributes = red_sprite.regular_second_attributes();
        btn::sprite_regular_second_attributes alt_attributes = red_attributes;
        alt_attributes.set_horizontal_flip(true);
        alt_attributes.set_vertical_flip(true);

        btn::vector<btn::sprite_regular_second_attributes, btn::display::height()> attributes;

        for(int index = 0; index < btn::display::height(); ++index)
        {
            if(index % 2)
            {
                attributes.push_back(alt_attributes);
            }
            else
            {
                attributes.push_back(red_attributes);
            }
        }

        btn::span<const btn::sprite_regular_second_attributes> attributes_ref(attributes.data(), attributes.size());
        btn::sprite_regular_second_attributes_hblank_effect_ptr hblank_effect =
                btn::sprite_regular_second_attributes_hblank_effect_ptr::create(red_sprite, attributes_ref);

        while(! btn::keypad::start_pressed())
        {
            info.update();
            btn::core::update();
        }
    }

    void sprites_affine_second_attributes_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "B: copy blue attributes to red",
            "A: copy yellow attributes to red",
            "L: copy green attributes to red",
            "R: restore red attributes",
            "",
            "START: go to next scene",
        };

        info info("Affine second attributes", info_text_lines, text_generator);

        btn::sprite_ptr green_sprite = btn::sprite_items::green_sprite.create_sprite(-32, -32);
        btn::sprite_ptr blue_sprite = btn::sprite_items::blue_sprite.create_sprite(-32, 32);
        btn::sprite_ptr yellow_sprite = btn::sprite_items::yellow_sprite.create_sprite(32, 32);
        btn::sprite_ptr red_sprite = btn::sprite_items::red_sprite.create_sprite(0, 0);
        green_sprite.set_rotation_angle(45);
        blue_sprite.set_rotation_angle(200);
        yellow_sprite.set_scale(1.5);
        red_sprite.set_scale(2);

        btn::sprite_affine_second_attributes red_sprite_attributes = red_sprite.affine_second_attributes();

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::b_pressed())
            {
                red_sprite.set_affine_second_attributes(blue_sprite.affine_second_attributes());
            }

            if(btn::keypad::a_pressed())
            {
                red_sprite.set_affine_second_attributes(yellow_sprite.affine_second_attributes());
            }

            if(btn::keypad::l_pressed())
            {
                red_sprite.set_affine_second_attributes(green_sprite.affine_second_attributes());
            }

            if(btn::keypad::r_pressed())
            {
                red_sprite.set_affine_second_attributes(red_sprite_attributes);
            }

            info.update();
            btn::core::update();
        }
    }

    void sprites_affine_second_attributes_hblank_effect_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Affine second attribs. H-Blank effect", info_text_lines, text_generator);

        btn::sprite_ptr red_sprite = btn::sprite_items::red_sprite.create_sprite(0, 0);
        red_sprite.set_scale(2);

        btn::sprite_affine_second_attributes red_attributes = red_sprite.affine_second_attributes();
        btn::sprite_affine_second_attributes alt_attributes = red_attributes;
        btn::sprite_affine_mat_ptr affine_mat = btn::sprite_affine_mat_ptr::create();
        affine_mat.set_rotation_angle(45);
        affine_mat.set_scale(1.5);
        alt_attributes.set_affine_mat(affine_mat);

        btn::vector<btn::sprite_affine_second_attributes, btn::display::height()> attributes;

        for(int index = 0; index < btn::display::height(); ++index)
        {
            if(index % 2)
            {
                attributes.push_back(alt_attributes);
            }
            else
            {
                attributes.push_back(red_attributes);
            }
        }

        btn::span<const btn::sprite_affine_second_attributes> attributes_ref(attributes.data(), attributes.size());
        btn::sprite_affine_second_attributes_hblank_effect_ptr hblank_effect =
                btn::sprite_affine_second_attributes_hblank_effect_ptr::create(red_sprite, attributes_ref);

        while(! btn::keypad::start_pressed())
        {
            info.update();
            btn::core::update();
        }
    }

    void sprites_third_attributes_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "B: copy blue attributes to red",
            "A: copy yellow attributes to red",
            "L: copy green attributes to red",
            "R: restore red attributes",
            "",
            "START: go to next scene",
        };

        info info("Third attributes", info_text_lines, text_generator);

        btn::sprite_ptr green_sprite = btn::sprite_items::green_sprite.create_sprite(-32, -32);
        btn::sprite_ptr blue_sprite = btn::sprite_items::blue_sprite.create_sprite(-32, 32);
        btn::sprite_ptr yellow_sprite = btn::sprite_items::yellow_sprite.create_sprite(32, 32);
        btn::sprite_ptr red_sprite = btn::sprite_items::red_sprite.create_sprite(0, 0);
        btn::sprite_third_attributes red_sprite_attributes = red_sprite.third_attributes();

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::b_pressed())
            {
                red_sprite.set_third_attributes(blue_sprite.third_attributes());
            }

            if(btn::keypad::a_pressed())
            {
                red_sprite.set_third_attributes(yellow_sprite.third_attributes());
            }

            if(btn::keypad::l_pressed())
            {
                red_sprite.set_third_attributes(green_sprite.third_attributes());
            }

            if(btn::keypad::r_pressed())
            {
                red_sprite.set_third_attributes(red_sprite_attributes);
            }

            info.update();
            btn::core::update();
        }
    }

    void sprites_third_attributes_hblank_effect_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Third attributes H-Blank effect", info_text_lines, text_generator);

        btn::sprite_ptr red_sprite = btn::sprite_items::red_sprite.create_sprite(0, 0);
        red_sprite.set_scale(2);

        btn::sprite_palette_ptr green_palette = btn::sprite_items::green_sprite.create_sprite(0, 0).palette();
        btn::sprite_third_attributes red_attributes = red_sprite.third_attributes();
        btn::sprite_third_attributes green_attributes = red_attributes;
        green_attributes.set_palette(green_palette);

        btn::vector<btn::sprite_third_attributes, btn::display::height()> attributes;

        for(int index = 0; index < btn::display::height(); ++index)
        {
            if(index % 2)
            {
                attributes.push_back(green_attributes);
            }
            else
            {
                attributes.push_back(red_attributes);
            }
        }

        btn::span<const btn::sprite_third_attributes> attributes_ref(attributes.data(), attributes.size());
        btn::sprite_third_attributes_hblank_effect_ptr hblank_effect =
                btn::sprite_third_attributes_hblank_effect_ptr::create(red_sprite, attributes_ref);

        while(! btn::keypad::start_pressed())
        {
            info.update();
            btn::core::update();
        }
    }

    void sprite_builder_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Sprite builder", info_text_lines, text_generator);

        btn::regular_bg_ptr red_bg = btn::regular_bg_items::red_bg.create_bg(0, 0);
        btn::sprites_mosaic::set_stretch(0.1);
        btn::blending::set_transparency_alpha(0.6);

        btn::sprite_builder builder(btn::sprite_items::blue_sprite);
        builder.set_position(48, 24);
        builder.set_scale(2);
        builder.set_rotation_angle(45);
        builder.set_horizontal_flip(true);
        builder.set_mosaic_enabled(true);
        builder.set_blending_enabled(true);

        btn::sprite_ptr blue_sprite = builder.build();

        while(! btn::keypad::start_pressed())
        {
            info.update();
            btn::core::update();
        }

        btn::sprites_mosaic::set_stretch(0);
        btn::blending::set_transparency_alpha(1);
    }
}

int main()
{
    btn::core::init();

    btn::sprite_text_generator text_generator(variable_8x16_sprite_font);
    btn::bg_palettes::set_transparent_color(btn::color(16, 16, 16));

    while(true)
    {
        sprites_visibility_scene(text_generator);
        btn::core::update();

        sprites_visibility_actions_scene(text_generator);
        btn::core::update();

        sprites_position_scene(text_generator);
        btn::core::update();

        sprites_position_actions_scene(text_generator);
        btn::core::update();

        sprites_position_hblank_effect_scene(text_generator);
        btn::core::update();

        sprites_rotation_scene(text_generator);
        btn::core::update();

        sprites_rotation_actions_scene(text_generator);
        btn::core::update();

        sprites_scale_scene(text_generator);
        btn::core::update();

        sprites_scale_actions_scene(text_generator);
        btn::core::update();

        sprites_flip_scene(text_generator);
        btn::core::update();

        sprites_flip_actions_scene(text_generator);
        btn::core::update();

        sprites_double_size_mode_scene(text_generator);
        btn::core::update();

        sprites_bg_priority_scene(text_generator);
        btn::core::update();

        sprites_z_order_scene(text_generator);
        btn::core::update();

        sprites_put_above_scene(text_generator);
        btn::core::update();

        sprites_first_attributes_scene(text_generator);
        btn::core::update();

        sprites_first_attributes_hblank_effect_scene(text_generator);
        btn::core::update();

        sprites_regular_second_attributes_scene(text_generator);
        btn::core::update();

        sprites_regular_second_attributes_hblank_effect_scene(text_generator);
        btn::core::update();

        sprites_affine_second_attributes_scene(text_generator);
        btn::core::update();

        sprites_affine_second_attributes_hblank_effect_scene(text_generator);
        btn::core::update();

        sprites_third_attributes_scene(text_generator);
        btn::core::update();

        sprites_third_attributes_hblank_effect_scene(text_generator);
        btn::core::update();

        sprite_builder_scene(text_generator);
        btn::core::update();
    }
}
