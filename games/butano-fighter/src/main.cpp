#include "btn_core.h"
#include "btn_music.h"
#include "btn_sound.h"
#include "btn_camera.h"
#include "btn_colors.h"
#include "btn_keypad.h"
#include "btn_memory.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_algorithm.h"
#include "btn_bg_palettes.h"
#include "btn_fixed_point.h"

#include "btn_music_items.h"
#include "btn_sound_items.h"
#include "btn_hero_sprite_item.h"

#include "btn_random.h"
#include "btn_green_swap.h"
#include "btn_sprite_actions.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_text_generator.h"

#include "btn_bg_actions.h"
#include "btn_stage_1_bg_item.h"

#include "bf_stats.h"
#include "bf_sprite_fonts.h"

int main()
{
    btn::core::init();

    btn::bg_ptr bg = btn::bg_ptr::create(0, 0, btn::bg_items::stage_1);
    btn::bg_move_by_action bg_move_action(btn::move(bg), 0, 0.5);
    bg_move_action.run();

    int center_x = btn::display::width() / 2;
    int center_y = btn::display::height() / 2;

    btn::sprite_builder sprite_builder(btn::sprite_items::hero);
    sprite_builder.set_position(center_x, center_y);
    sprite_builder.set_mosaic_enabled(true);

    btn::sprite_ptr sprite = sprite_builder.build_and_release();
    auto sprite_animate_action = btn::create_sprite_cached_animate_action_forever(sprite, 16, btn::sprite_items::hero, 0, 2);
    sprite_animate_action.run();

    btn::sprite_text_generator text_generator(bf::variable_8x8_sprite_font);
    bf::stats stats(text_generator);
    // btn::music::play(btn::music_items::battle_clean);
    btn::core::update();

    while(true)
    {
        if(btn::keypad::pressed(btn::keypad::button_type::A))
        {
        }

        if(btn::keypad::pressed(btn::keypad::button_type::B))
        {
            btn::green_swap::set_enabled(! btn::green_swap::enabled());
        }

        if(btn::keypad::held(btn::keypad::button_type::L))
        {
            sprite.set_rotation_angle(btn::max(sprite.rotation_angle() - 1, btn::fixed(0)));
        }

        if(btn::keypad::held(btn::keypad::button_type::R))
        {
            sprite.set_rotation_angle(btn::min(sprite.rotation_angle() + 1, btn::fixed(360)));
        }

        if(btn::keypad::pressed(btn::keypad::button_type::SELECT))
        {
            btn::core::sleep(btn::keypad::button_type::SELECT);
        }

        if(btn::keypad::held(btn::keypad::button_type::LEFT))
        {
            btn::fixed sprite_x = btn::max(sprite.x() - 1, btn::fixed(center_x - 72));
            sprite.set_x(sprite_x);

            if(sprite_x < center_x + 32)
            {
                btn::camera::set_x(btn::max(btn::camera::x() - 1, btn::fixed(-32)));
            }
        }
        else if(btn::keypad::held(btn::keypad::button_type::RIGHT))
        {
            btn::fixed sprite_x = btn::min(sprite.x() + 1, btn::fixed(center_x + 72));
            sprite.set_x(sprite_x);

            if(sprite_x > center_x - 32)
            {
                btn::camera::set_x(btn::min(btn::camera::x() + 1, btn::fixed(32)));
            }
        }

        if(btn::keypad::held(btn::keypad::button_type::UP))
        {
            sprite.set_y(btn::max(sprite.y() - 1, btn::fixed(32)));
        }
        else if(btn::keypad::held(btn::keypad::button_type::DOWN))
        {
            sprite.set_y(btn::min(sprite.y() + 1, btn::fixed(128)));
        }

        // btn::camera::set_position(camera_position);
        btn::core::update();
    }
}
