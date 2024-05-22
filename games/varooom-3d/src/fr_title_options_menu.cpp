/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_title_options_menu.h"

#include "bn_keypad.h"

#include "fr_common_stuff.h"

#include "bn_sound_items.h"

namespace fr
{

namespace
{
    constexpr bn::fixed text_x = 18;

    constexpr bn::fixed initial_y = 2;
    constexpr bn::fixed light_y = initial_y + 11 + 6;
    constexpr bn::fixed rumble_y = light_y + 11;

    [[nodiscard]] bn::sprite_ptr _create_cursor_sprite(common_stuff& common_stuff)
    {
        bn::vector<bn::sprite_ptr, 1> output_sprites;
        bn::sprite_text_generator& text_generator = common_stuff.small_variable_text_generator;
        text_generator.set_one_sprite_per_character(true);
        text_generator.generate(text_x - 10, light_y, ">", output_sprites);
        text_generator.set_one_sprite_per_character(false);
        return bn::move(output_sprites[0]);
    }

    [[nodiscard]] bn::sprite_ptr _create_sprite(bn::fixed x, bn::fixed y, const bn::string_view& text,
                                                common_stuff& common_stuff)
    {
        bn::vector<bn::sprite_ptr, 1> output_sprites;
        bn::sprite_text_generator& text_generator = common_stuff.small_variable_text_generator;
        text_generator.generate(x, y, text, output_sprites);
        return bn::move(output_sprites[0]);
    }
}

title_options_menu::title_options_menu(common_stuff& common_stuff) :
    _cursor_sprite(_create_cursor_sprite(common_stuff)),
    _low_brightness_sprite(_create_sprite(text_x + 55, light_y, "LOW", common_stuff)),
    _medium_brightness_sprite(_create_sprite(text_x + 55, light_y, "MED", common_stuff)),
    _high_brightness_sprite(_create_sprite(text_x + 55, light_y, "HI", common_stuff)),
    _rumble_enabled_sprite(_create_sprite(text_x + 55, rumble_y, "ON", common_stuff)),
    _rumble_disabled_sprite(_create_sprite(text_x + 55, rumble_y, "OFF", common_stuff))
{
    bn::sprite_text_generator& text_generator = common_stuff.small_variable_text_generator;
    text_generator.generate(text_x + 17, initial_y, "OPTIONS", _text_sprites);
    text_generator.generate(text_x, light_y, "LIGHT:", _text_sprites);
    text_generator.generate(text_x, rumble_y, "RUMBLE:", _text_sprites);

    _update_cursor();
    set_visible(common_stuff, false);
}

void title_options_menu::set_visible(const common_stuff& common_stuff, bool visible)
{
    _cursor_sprite.set_visible(visible);

    for(bn::sprite_ptr& text_sprite : _text_sprites)
    {
        text_sprite.set_visible(visible);
    }

    if(visible)
    {
        _update_rumble(common_stuff);
        _update_brightness(common_stuff);
        _menu_keypad.reset();
    }
    else
    {
        _low_brightness_sprite.set_visible(false);
        _medium_brightness_sprite.set_visible(false);
        _high_brightness_sprite.set_visible(false);
        _rumble_enabled_sprite.set_visible(false);
        _rumble_disabled_sprite.set_visible(false);
    }
}

bool title_options_menu::update(common_stuff& common_stuff)
{
    bool result = true;

    if(_cursor_sprite.visible())
    {
        _menu_keypad.update();

        sram_data& sram_data = common_stuff.storage;
        bool a_pressed = bn::keypad::a_pressed();
        bool left_pressed = _menu_keypad.left();
        bool right_pressed = _menu_keypad.right();

        if(_menu_keypad.up())
        {
            if(_cursor_index > 0)
            {
                --_cursor_index;
                _update_cursor();
                bn::sound_items::blip02.play();
            }
        }
        else if(_menu_keypad.down())
        {
            if(_cursor_index < 1)
            {
                ++_cursor_index;
                _update_cursor();
                bn::sound_items::blip02.play();
            }
        }
        else if(bn::keypad::b_pressed())
        {
            sram_data.write();
            result = false;
            bn::sound_items::blip05.play();
        }
        else if(a_pressed || left_pressed || right_pressed)
        {
            switch(_cursor_index)
            {

            case 0:
                switch(sram_data.brightness())
                {

                case brightness_level::LOW:
                    if(a_pressed || right_pressed)
                    {
                        sram_data.set_brightness(brightness_level::MEDIUM);
                        _update_brightness(common_stuff);
                        bn::sound_items::blip04.play();
                    }
                    break;

                case brightness_level::MEDIUM:
                    if(left_pressed)
                    {
                        sram_data.set_brightness(brightness_level::LOW);
                    }
                    else
                    {
                        sram_data.set_brightness(brightness_level::HIGH);
                    }

                    _update_brightness(common_stuff);
                    bn::sound_items::blip04.play();
                    break;

                case brightness_level::HIGH:
                    if(a_pressed)
                    {
                        sram_data.set_brightness(brightness_level::LOW);
                        _update_brightness(common_stuff);
                        bn::sound_items::blip04.play();
                    }
                    else if(left_pressed)
                    {
                        sram_data.set_brightness(brightness_level::MEDIUM);
                        _update_brightness(common_stuff);
                        bn::sound_items::blip04.play();
                    }
                    break;

                default:
                    BN_ERROR("Invalid brightness: ", int(sram_data.brightness()));
                    break;
                }
                break;

            case 1:
                if(sram_data.rumble_allowed())
                {
                    if(a_pressed || right_pressed)
                    {
                        sram_data.set_rumble_allowed(false);
                        _update_rumble(common_stuff);
                        bn::sound_items::blip04.play();
                    }
                }
                else
                {
                    if(a_pressed || left_pressed)
                    {
                        sram_data.set_rumble_allowed(true);
                        _update_rumble(common_stuff);
                        bn::sound_items::blip04.play();
                    }
                }
                break;

            default:
                BN_ERROR("Invalid cursor index: ", _cursor_index);
                break;
            }
        }
    }

    return result;
}

void title_options_menu::_update_cursor()
{
    switch(_cursor_index)
    {

    case 0:
        _cursor_sprite.set_y(light_y);
        break;

    case 1:
        _cursor_sprite.set_y(rumble_y);
        break;

    default:
        BN_ERROR("Invalid cursor index: ", _cursor_index);
        break;
    }
}

void title_options_menu::_update_rumble(const common_stuff& common_stuff)
{
    bool rumble_allowed = common_stuff.storage.rumble_allowed();
    _rumble_enabled_sprite.set_visible(rumble_allowed);
    _rumble_disabled_sprite.set_visible(! rumble_allowed);
}

void title_options_menu::_update_brightness(const common_stuff& common_stuff)
{
    brightness_level brightness = common_stuff.storage.brightness();

    switch(brightness)
    {

    case brightness_level::LOW:
        _low_brightness_sprite.set_visible(true);
        _medium_brightness_sprite.set_visible(false);
        _high_brightness_sprite.set_visible(false);
        break;

    case brightness_level::MEDIUM:
        _low_brightness_sprite.set_visible(false);
        _medium_brightness_sprite.set_visible(true);
        _high_brightness_sprite.set_visible(false);
        break;

    case brightness_level::HIGH:
        _low_brightness_sprite.set_visible(false);
        _medium_brightness_sprite.set_visible(false);
        _high_brightness_sprite.set_visible(true);
        break;

    default:
        BN_ERROR("Invalid brightness: ", int(brightness));
        break;
    }
}

}
