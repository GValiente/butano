/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_brightness_scene.h"

#include "bn_keypad.h"
#include "bn_colors.h"
#include "bn_display.h"

#include "fr_scene_type.h"
#include "fr_common_stuff.h"

#include "bn_sound_items.h"
#include "bn_sprite_items_big_font.h"
#include "bn_sprite_items_small_variable_font.h"
#include "bn_regular_bg_items_brightness_backdrop.h"
#include "bn_sprite_palette_items_big_font_gray_border.h"
#include "bn_sprite_palette_items_small_font_gray_border.h"

namespace fr
{

namespace
{
    constexpr bn::fixed initial_text_x = -84;
    constexpr bn::fixed cursor_text_x_desp = 10;

    constexpr bn::fixed low_text_x = initial_text_x;
    constexpr bn::fixed medium_text_x = low_text_x + 70 - 8;
    constexpr bn::fixed high_text_x = medium_text_x + 70 + 8;

    constexpr bn::fixed title_text_y = -65;
    constexpr bn::fixed text_y = title_text_y + 16;

    [[nodiscard]] bn::bg_palettes_fade_to_action _create_bgs_fade_in_action()
    {
        bn::bg_palettes::set_fade(bn::colors::black, 1);
        return bn::bg_palettes_fade_to_action(60, 0);
    }

    [[nodiscard]] bn::sprite_palettes_fade_to_action _create_sprites_fade_in_action()
    {
        bn::sprite_palettes::set_fade(bn::colors::black, 1);
        return bn::sprite_palettes_fade_to_action(60, 0);
    }

    [[nodiscard]] bn::sprite_ptr _create_cursor_sprite(common_stuff& common_stuff)
    {
        bn::vector<bn::sprite_ptr, 1> output_sprites;
        bn::sprite_text_generator& small_text_generator = common_stuff.small_variable_text_generator;
        small_text_generator.set_palette_item(bn::sprite_palette_items::small_font_gray_border);
        small_text_generator.set_bg_priority(0);
        small_text_generator.set_one_sprite_per_character(true);
        small_text_generator.generate(low_text_x - cursor_text_x_desp, text_y, ">", output_sprites);
        small_text_generator.set_one_sprite_per_character(false);
        return bn::move(output_sprites[0]);
    }
}

brightness_scene::brightness_scene(common_stuff& common_stuff) :
    _sram_data(common_stuff.storage),
    _bgs_fade_in_action(_create_bgs_fade_in_action()),
    _sprites_fade_in_action(_create_sprites_fade_in_action()),
    _backdrop_bg(bn::regular_bg_items::brightness_backdrop.create_bg(
                     (256 - bn::display::width()) / 2, (256 - bn::display::height()) / 2)),
    _cursor_sprite(_create_cursor_sprite(common_stuff))
{
    bn::sprite_text_generator& big_text_generator = common_stuff.big_text_generator;
    big_text_generator.set_center_alignment();
    big_text_generator.set_palette_item(bn::sprite_palette_items::big_font_gray_border);
    big_text_generator.generate(0, title_text_y, "SELECT SCREEN BRIGHTNESS", _text_sprites);
    big_text_generator.set_left_alignment();
    big_text_generator.set_palette_item(bn::sprite_items::big_font.palette_item());

    bn::sprite_text_generator& small_text_generator = common_stuff.small_variable_text_generator;
    small_text_generator.generate(low_text_x, text_y, "LOW", _text_sprites);
    small_text_generator.generate(medium_text_x, text_y, "MEDIUM", _text_sprites);
    small_text_generator.generate(high_text_x, text_y, "HIGH", _text_sprites);
    small_text_generator.set_palette_item(bn::sprite_items::small_variable_font.palette_item());

    _update_cursor();
}

bn::optional<scene_type> brightness_scene::update()
{
    bn::optional<scene_type> result;

    if(_bgs_fade_in_action.done())
    {
        _menu_keypad.update();

        if(_bgs_fade_out_action)
        {
            _bgs_fade_out_action->update();
            _sprites_fade_out_action->update();

            if(_bgs_fade_out_action->done())
            {
                result = scene_type::BUTANO_INTRO;
            }
        }
        else if(_menu_keypad.left())
        {
            auto brightness_integer = int(_sram_data.brightness());

            if(brightness_integer > int(brightness_level::LOW))
            {
                _sram_data.set_brightness(brightness_level(brightness_integer - 1));
                _update_cursor();
                bn::sound_items::blip02.play();
            }
        }
        else if(_menu_keypad.right())
        {
            auto brightness_integer = int(_sram_data.brightness());

            if(brightness_integer < int(brightness_level::HIGH))
            {
                _sram_data.set_brightness(brightness_level(brightness_integer + 1));
                _update_cursor();
                bn::sound_items::blip02.play();
            }
        }
        else if(bn::keypad::a_pressed())
        {
            constexpr bn::color fade_color(14, 0, 0);
            bn::bg_palettes::set_fade_color(fade_color);
            bn::bg_palettes::set_transparent_color(fade_color);
            bn::sprite_palettes::set_fade_color(fade_color);
            _bgs_fade_out_action.emplace(60, 1);
            _sprites_fade_out_action.emplace(60, 1);
            bn::sound_items::blip09.play();
            _sram_data.set_brightness_scene_shown(true);
            _sram_data.write();
        }
    }
    else
    {
        _bgs_fade_in_action.update();
        _sprites_fade_in_action.update();
    }

    return result;
}

void brightness_scene::_update_cursor()
{
    switch(_sram_data.brightness())
    {

    case brightness_level::LOW:
        _cursor_sprite.set_x(low_text_x - cursor_text_x_desp);
        break;

    case brightness_level::MEDIUM:
        _cursor_sprite.set_x(medium_text_x - cursor_text_x_desp);
        break;

    case brightness_level::HIGH:
        _cursor_sprite.set_x(high_text_x - cursor_text_x_desp);
        break;

    default:
        BN_ERROR("Invalid brightness: ", int(_sram_data.brightness()));
        break;
    }
}

}
