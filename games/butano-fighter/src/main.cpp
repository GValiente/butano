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

#include "btn_bg_ptr.h"
#include "btn_tank_bg_item.h"

#include "bf_stats.h"
#include "bf_sprite_fonts.h"

int main()
{
    btn::core::init();

    btn::bg_ptr bg = btn::bg_ptr::create(0, 0, btn::bg_items::tank);
    btn::bg_palettes::set_transparent_color(btn::colors::gray);

    int display_width = btn::display::width();
    int display_height = btn::display::height();

    struct bench_sprite
    {
        btn::sprite_move_loop_action move_loop_action;
        btn::sprite_cached_animate_action<2> animate_action;
    };

    constexpr int num_sprites = 64;
    btn::random random;
    btn::vector<bench_sprite, num_sprites> bench_sprites;

    for(int index = 0; index < num_sprites; ++index)
    {
        int x = (display_width / 4) + int(random.get() % (unsigned(display_width) / 2));
        int y = (display_height / 4) + int(random.get() % (unsigned(display_height) / 2));
        btn::sprite_ptr sprite = btn::sprite_ptr::create(x, y, btn::sprite_items::hero);
        x = (display_width / 4) + int(random.get() % (unsigned(display_width) / 2));
        y = (display_height / 4) + int(random.get() % (unsigned(display_height) / 2));
        bench_sprites.push_back(bench_sprite{
                                    btn::sprite_move_loop_action(sprite, 64, x, y),
                                    btn::create_sprite_cached_animate_action_forever(sprite, 16, btn::sprite_items::hero, 0, 2) });

        bench_sprite& bench_spr = bench_sprites.back();
        bench_spr.move_loop_action.run();
        bench_spr.animate_action.run();
    }

    btn::sprite_builder sprite_builder(btn::sprite_items::hero);
    sprite_builder.set_position(display_width / 2, display_height / 2);
    sprite_builder.set_scale_x(2);
    sprite_builder.set_scale_y(2);
    sprite_builder.set_mosaic_enabled(true);

    btn::sprite_ptr sprite = sprite_builder.build_and_release();
    auto sprite_animate_action = btn::create_sprite_cached_animate_action_forever(sprite, 16, btn::sprite_items::hero, 0, 2);
    sprite_animate_action.run();

    btn::sprite_move_to_action sprite_move_to_action(sprite, 64, 0, 0);

    btn::sprite_text_generator text_generator(bf::variable_8x8_sprite_font);
    bf::stats stats(text_generator);
    int counter = 0;
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
