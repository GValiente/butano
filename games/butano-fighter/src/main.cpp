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
#include "btn_sprite_move_to_action.h"
#include "btn_sprite_animate_action.h"

#include "btn_input_string_stream.h"
#include "btn_sprite_text_generator.h"
#include "btn_fixed_8x8_font_sprite_item.h"
#include "btn_fixed_8x16_font_sprite_item.h"

int main()
{
    btn::core::init();

    btn::bg_palettes::set_transparent_color(btn::colors::gray);

    int display_width = btn::display::width();
    int display_height = btn::display::height();
    btn::sprite_ptr sprite = btn::sprite_ptr::create(display_width / 2, display_height / 2, btn::sprite_items::hero);
    auto sprite_animate_action = btn::create_sprite_animate_action_forever(sprite, 16, btn::sprite_items::hero, 0, 2);
    sprite_animate_action.run();

    btn::random random;
    btn::sprite_move_to_action sprite_move_to_action(sprite, 64, 0, 0);

    static constexpr btn::string_view utf8_characters[] = {
        "Á", "É", "Í", "Ó", "Ú", "Ü", "Ñ", "á", "é", "í", "ó", "ú", "ü", "ñ", "¡", "¿"
    };

    constexpr btn::sprite_font font(btn::sprite_items::fixed_8x8_font, utf8_characters);
    btn::sprite_text_generator text_generator(font);
    int text_height = font.item().shape_size().height();

    btn::string<32> text;
    btn::input_string_stream text_stream(text);
    text_stream.append("IWRAM: ");
    text_stream.append(btn::used_static_iwram());
    text_stream.append("B");

    auto iwram_text_sprites = text_generator.generate<4>(8, 160 - (text_height * 2) - (text_height / 2), text);

    text_stream.clear();
    text_stream.append("EWRAM: ");
    text_stream.append(btn::used_static_ewram());
    text_stream.append("B");

    auto ewram_text_sprites = text_generator.generate<4>(8, 160 - text_height - (text_height / 2), text);

    btn::vector<btn::sprite_ptr, 4> cpu_text_sprites;
    btn::fixed max_cpu_usage = 0;
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

        max_cpu_usage = btn::max(max_cpu_usage, btn::core::cpu_usage());

        if(counter % 64 == 0)
        {
            int x = (display_width / 4) + int(random.get() % (unsigned(display_width) / 2));
            int y = (display_height / 4) + int(random.get() % (unsigned(display_height) / 2));
            sprite_move_to_action = btn::sprite_move_to_action(sprite, 64, x, y);
            sprite_move_to_action.run();

            cpu_text_sprites.clear();
            text_stream.clear();
            text_stream.append("CPU: ");
            text_stream.append(max_cpu_usage * 100);
            text_stream.append("%");
            text_generator.generate(8, 160 - (text_height * 3) - (text_height / 2), text, cpu_text_sprites);
            max_cpu_usage = 0;
        }

        btn::core::update();
        ++counter;
    }
}
