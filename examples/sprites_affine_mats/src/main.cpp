#include "btn_core.h"
#include "btn_math.h"
#include "btn_keypad.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_bg_palettes.h"
#include "btn_sprite_text_generator.h"
#include "btn_sprite_double_size_mode.h"
#include "btn_sprite_affine_mat_actions.h"
#include "btn_sprite_affine_mat_attributes.h"
#include "btn_sprite_affine_mat_attributes_hblank_effect_ptr.h"

#include "btn_sprite_items_red_sprite.h"
#include "btn_sprite_items_blue_sprite.h"
#include "btn_sprite_items_green_sprite.h"
#include "btn_sprite_items_yellow_sprite.h"

#include "info.h"
#include "variable_8x16_sprite_font.h"

namespace
{
    void sprite_affine_mats_rotation_scene(btn::sprite_affine_mat_ptr& affine_mat,
                                           btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: decrease rotation angle",
            "RIGHT: increase rotation angle",
            "",
            "START: go to next scene",
        };

        info info("Sprite affine mats rotation", info_text_lines, text_generator);

        affine_mat.set_rotation_angle(45);

        while(! btn::keypad::start_pressed())
        {
            btn::fixed rotation_angle = affine_mat.rotation_angle();

            if(btn::keypad::left_held())
            {
                affine_mat.set_rotation_angle(btn::max(rotation_angle - 1, btn::fixed(0)));
            }
            else if(btn::keypad::right_held())
            {
                affine_mat.set_rotation_angle(btn::min(rotation_angle + 1, btn::fixed(360)));
            }

            info.update();
            btn::core::update();
        }

        affine_mat.set_rotation_angle(0);
    }

    void sprite_affine_mats_rotation_actions_scene(btn::sprite_affine_mat_ptr& affine_mat,
                                                   btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Sprite affine mats rotation actions", info_text_lines, text_generator);

        btn::sprite_affine_mat_rotate_loop_action action(affine_mat, 180, 360);

        while(! btn::keypad::start_pressed())
        {
            action.update();
            info.update();
            btn::core::update();
        }

        affine_mat.set_rotation_angle(0);
    }

    void sprite_affine_mats_scale_scene(btn::sprite_affine_mat_ptr& affine_mat,
                                        btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: decrease horizontal scale",
            "RIGHT: increase horizontal scale",
            "DOWN: decrease vertical scale",
            "UP: increase vertical scale",
            "",
            "START: go to next scene",
        };

        info info("Sprite affine mats scale", info_text_lines, text_generator);

        affine_mat.set_scale_x(1.5);
        affine_mat.set_scale_y(0.75);

        while(! btn::keypad::start_pressed())
        {
            btn::fixed scale_x = affine_mat.scale_x();
            btn::fixed scale_y = affine_mat.scale_y();

            if(btn::keypad::left_held())
            {
                affine_mat.set_scale_x(btn::max(scale_x - 0.01, btn::fixed(0.01)));
            }
            else if(btn::keypad::right_held())
            {
                affine_mat.set_scale_x(btn::min(scale_x + 0.01, btn::fixed(2)));
            }

            if(btn::keypad::down_held())
            {
                affine_mat.set_scale_y(btn::max(scale_y - 0.01, btn::fixed(0.01)));
            }
            else if(btn::keypad::up_held())
            {
                affine_mat.set_scale_y(btn::min(scale_y + 0.01, btn::fixed(2)));
            }

            info.update();
            btn::core::update();
        }

        affine_mat.set_scale(1);
    }

    void sprite_affine_mats_scale_actions_scene(btn::sprite_affine_mat_ptr& affine_mat,
                                                btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Sprite affine mats scale actions", info_text_lines, text_generator);

        affine_mat.set_scale(0.01);

        btn::sprite_affine_mat_scale_loop_action action(affine_mat, 120, 2);

        while(! btn::keypad::start_pressed())
        {
            action.update();
            info.update();
            btn::core::update();
        }

        affine_mat.set_scale(1);
    }

    void sprite_affine_mats_flip_scene(btn::sprite_affine_mat_ptr& affine_mat,
                                       btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: disable horizontal flip",
            "RIGHT: enable horizontal flip",
            "DOWN: disable vertical flip",
            "UP: enable vertical flip",
            "",
            "START: go to next scene",
        };

        info info("Sprite affine mats flip", info_text_lines, text_generator);

        affine_mat.set_horizontal_flip(true);

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::left_pressed())
            {
                affine_mat.set_horizontal_flip(false);
            }
            else if(btn::keypad::right_pressed())
            {
                affine_mat.set_horizontal_flip(true);
            }

            if(btn::keypad::down_pressed())
            {
                affine_mat.set_vertical_flip(false);
            }
            else if(btn::keypad::up_pressed())
            {
                affine_mat.set_vertical_flip(true);
            }

            info.update();
            btn::core::update();
        }

        affine_mat.set_horizontal_flip(false);
        affine_mat.set_vertical_flip(false);
    }

    void sprite_affine_mats_flip_actions_scene(btn::sprite_affine_mat_ptr& affine_mat,
                                               btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Sprite affine mats flip actions", info_text_lines, text_generator);

        btn::sprite_affine_mat_horizontal_flip_toggle_action action(affine_mat, 120);

        while(! btn::keypad::start_pressed())
        {
            action.update();
            info.update();
            btn::core::update();
        }

        affine_mat.set_horizontal_flip(false);
    }

    void sprite_affine_mats_attributes_scene(btn::sprite_affine_mat_ptr& affine_mat,
                                             btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "PAD: change scale",
            "L: decrease rotation angle",
            "R: increase rotation angle",
            "",
            "START: go to next scene",
        };

        info info("Sprite affine mats attributes", info_text_lines, text_generator);

        affine_mat.set_rotation_angle(45);
        affine_mat.set_scale_x(1.5);
        affine_mat.set_scale_y(0.75);

        while(! btn::keypad::start_pressed())
        {
            btn::sprite_affine_mat_attributes attributes = affine_mat.attributes();
            btn::fixed scale_x = attributes.scale_x();
            btn::fixed scale_y = attributes.scale_y();
            btn::fixed rotation_angle = attributes.rotation_angle();

            if(btn::keypad::left_held())
            {
                attributes.set_scale_x(btn::max(scale_x - 0.01, btn::fixed(0.01)));
            }
            else if(btn::keypad::right_held())
            {
                attributes.set_scale_x(btn::min(scale_x + 0.01, btn::fixed(2)));
            }

            if(btn::keypad::down_held())
            {
                attributes.set_scale_y(btn::max(scale_y - 0.01, btn::fixed(0.01)));
            }
            else if(btn::keypad::up_held())
            {
                attributes.set_scale_y(btn::min(scale_y + 0.01, btn::fixed(2)));
            }

            if(btn::keypad::l_held())
            {
                attributes.set_rotation_angle(btn::max(rotation_angle - 1, btn::fixed(0)));
            }
            else if(btn::keypad::r_held())
            {
                attributes.set_rotation_angle(btn::min(rotation_angle + 1, btn::fixed(360)));
            }

            affine_mat.set_attributes(attributes);
            info.update();
            btn::core::update();
        }

        affine_mat.set_attributes(btn::sprite_affine_mat_attributes());
    }

    void sprite_affine_mats_attributes_hblank_effect_scene(btn::sprite_affine_mat_ptr& affine_mat,
                                                           btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Attributes H-Blank effect", info_text_lines, text_generator);

        btn::sprite_affine_mat_attributes attributes[btn::display::height()];

        btn::sprite_affine_mat_attributes_hblank_effect_ptr hblank_effect =
                btn::sprite_affine_mat_attributes_hblank_effect_ptr::create(affine_mat, attributes);

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
                degrees_angle += 4;

                if(degrees_angle >= 360)
                {
                    degrees_angle -= 360;
                }

                btn::fixed scale_inc = btn::degrees_sin(degrees_angle) / 2;
                attributes[(btn::display::height() / 2) + index].set_scale_x(1.5 + scale_inc);
                attributes[(btn::display::height() / 2) - index - 1].set_scale_x(1.5 + scale_inc);
            }

            hblank_effect.reload_attributes_ref();
            info.update();
            btn::core::update();
        }
    }
}

int main()
{
    btn::core::init();

    btn::sprite_text_generator text_generator(variable_8x16_sprite_font);
    btn::bg_palettes::set_transparent_color(btn::color(16, 16, 16));

    btn::sprite_affine_mat_ptr affine_mat = btn::sprite_affine_mat_ptr::create();

    btn::sprite_ptr green_sprite = btn::sprite_items::green_sprite.create_sprite(-32, -32 - 8);
    green_sprite.set_affine_mat(affine_mat);
    green_sprite.set_double_size_mode(btn::sprite_double_size_mode::ENABLED);

    btn::sprite_ptr yellow_sprite = btn::sprite_items::yellow_sprite.create_sprite(32, -32 + 8);
    yellow_sprite.set_affine_mat(affine_mat);
    yellow_sprite.set_double_size_mode(btn::sprite_double_size_mode::DISABLED);

    btn::sprite_ptr blue_sprite = btn::sprite_items::blue_sprite.create_sprite(-32, 32 - 8);
    blue_sprite.set_affine_mat(affine_mat);
    blue_sprite.set_double_size_mode(btn::sprite_double_size_mode::DISABLED);

    btn::sprite_ptr red_sprite = btn::sprite_items::red_sprite.create_sprite(32, 32 + 8);
    red_sprite.set_affine_mat(affine_mat);
    red_sprite.set_double_size_mode(btn::sprite_double_size_mode::ENABLED);

    while(true)
    {
        sprite_affine_mats_rotation_scene(affine_mat, text_generator);
        btn::core::update();

        sprite_affine_mats_rotation_actions_scene(affine_mat, text_generator);
        btn::core::update();

        sprite_affine_mats_scale_scene(affine_mat, text_generator);
        btn::core::update();

        sprite_affine_mats_scale_actions_scene(affine_mat, text_generator);
        btn::core::update();

        sprite_affine_mats_flip_scene(affine_mat, text_generator);
        btn::core::update();

        sprite_affine_mats_flip_actions_scene(affine_mat, text_generator);
        btn::core::update();

        sprite_affine_mats_attributes_scene(affine_mat, text_generator);
        btn::core::update();

        sprite_affine_mats_attributes_hblank_effect_scene(affine_mat, text_generator);
        btn::core::update();
    }
}
