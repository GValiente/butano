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
#include "btn_sprite_actions.h"

#include "btn_input_string_stream.h"
#include "btn_sprite_text_generator.h"

#include "bf_stats.h"
#include "bf_sprite_fonts.h"

int main()
{
    btn::core::init();

    btn::bg_palettes::set_transparent_color(btn::colors::gray);

    int display_width = btn::display::width();
    int display_height = btn::display::height();
    btn::sprite_ptr sprite = btn::sprite_ptr::create(display_width / 2, display_height / 2, btn::sprite_items::hero);
    sprite.set_mosaic_enabled(true);

    auto sprite_animate_action = btn::create_sprite_animate_action_forever(sprite, 16, btn::sprite_items::hero, 0, 2);
    sprite_animate_action.run();

    btn::random random;
    btn::sprite_move_to_action sprite_move_to_action(sprite, 64, 0, 0);

    btn::sprite_text_generator text_generator(bf::variable_8x8_sprite_font);
    bf::stats stats(text_generator);
    int counter = 0;
    btn::music::play(btn::music_items::battle_clean);
    btn::core::update();

    while(true)
    {
        if(btn::keypad::pressed(btn::keypad::button_type::A))
        {
            btn::sound::play(btn::sound_items::cure);
        }

        if(btn::keypad::pressed(btn::keypad::button_type::B))
        {
            btn::display::set_green_swap_enabled(! btn::display::green_swap_enabled());
        }

        if(btn::keypad::pressed(btn::keypad::button_type::L))
        {
            sprite.set_horizontal_flip(! sprite.horizontal_flip());
        }

        if(btn::keypad::pressed(btn::keypad::button_type::R))
        {
            sprite.set_vertical_flip(! sprite.vertical_flip());
        }

        if(btn::keypad::pressed(btn::keypad::button_type::SELECT))
        {
            btn::core::sleep(btn::keypad::button_type::SELECT);
        }

        btn::fixed_point camera_position = btn::camera::position();

        if(btn::keypad::held(btn::keypad::button_type::LEFT))
        {
            camera_position.set_x(camera_position.x() - 1);
        }
        else if(btn::keypad::held(btn::keypad::button_type::RIGHT))
        {
            camera_position.set_x(camera_position.x() + 1);
        }

        if(btn::keypad::held(btn::keypad::button_type::UP))
        {
            camera_position.set_y(camera_position.y() - 1);
        }
        else if(btn::keypad::held(btn::keypad::button_type::DOWN))
        {
            camera_position.set_y(camera_position.y() + 1);
        }

        btn::camera::set_position(camera_position);

        if(counter % 64 == 0)
        {
            int x = (display_width / 4) + int(random.get() % (unsigned(display_width) / 2));
            int y = (display_height / 4) + int(random.get() % (unsigned(display_height) / 2));
            sprite_move_to_action = btn::sprite_move_to_action(sprite, 64, x, y);
            sprite_move_to_action.run();
        }

        btn::core::update();
        ++counter;
    }
}
