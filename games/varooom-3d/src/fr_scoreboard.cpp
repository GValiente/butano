/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_scoreboard.h"

#include "bn_core.h"
#include "bn_display.h"

#include "fr_stage.h"
#include "fr_race_state.h"
#include "fr_common_stuff.h"

#include "bn_sound_items.h"
#include "bn_sprite_items_inverse.h"
#include "bn_sprite_items_turbo_bar.h"
#include "bn_sprite_items_big_digits.h"
#include "bn_sprite_items_race_labels.h"
#include "bn_sprite_items_small_fixed_font.h"
#include "bn_sprite_items_small_variable_font.h"
#include "bn_sprite_palette_items_big_digits_red.h"
#include "bn_sprite_palette_items_big_digits_orange.h"

namespace fr
{

namespace
{
    constexpr int intro_frames = 51;
    constexpr int intro_x_inc = 1;
    constexpr int intro_width = intro_frames * intro_x_inc;

    constexpr bn::fixed left_x = 8 - (bn::display::width() / 2);
    constexpr bn::fixed left_y = 13 - (bn::display::height() / 2);

    constexpr bn::fixed pending_time_x = left_x + 6 - intro_width;
    constexpr bn::fixed pending_time_y = left_y;

    constexpr bn::fixed cpu_x = left_x;
    constexpr bn::fixed cpu_y = (bn::display::height() / 2) - 32;

    constexpr bn::fixed lap_x = left_x - intro_width;
    constexpr bn::fixed lap_y = (bn::display::height() / 2) - 13;

    constexpr bn::fixed right_x = (bn::display::width() / 2) - 42;
    constexpr bn::fixed right_y = left_y;

    constexpr bn::fixed position_x = right_x + 3 + intro_width;
    constexpr bn::fixed position_y = right_y;

    constexpr bn::fixed turbo_x = right_x + 28 + intro_width;
    constexpr bn::fixed turbo_y = (bn::display::height() / 2) - 27;
}

scoreboard::scoreboard(const stage& stage, common_stuff& common_stuff) :
    _pending_time_palette(bn::sprite_items::big_digits.palette_item().create_palette()),
    _numbers_palette(bn::sprite_items::small_fixed_font.palette_item().create_palette()),
    _pending_time_number(bn::sprite_items::big_digits.shape_size(), bn::sprite_items::big_digits.tiles_item(), 0,
                         _pending_time_palette, pending_time_x, pending_time_y + 13, 13),
    _position_number(bn::sprite_items::big_digits.shape_size(), bn::sprite_items::big_digits.tiles_item(), 0,
                     _numbers_palette, position_x, position_y + 13, 13),
    _current_lap_number(bn::sprite_items::small_fixed_font.shape_size(),
                        bn::sprite_items::small_fixed_font.tiles_item(), 15, _numbers_palette,
                        lap_x + (8 * 4) - 6, lap_y),
    _total_laps_number(bn::sprite_items::small_fixed_font.shape_size(),
                       bn::sprite_items::small_fixed_font.tiles_item(), 15, _numbers_palette,
                       lap_x + (8 * 6) - 6, lap_y),
    _cpu_number(bn::sprite_items::small_fixed_font.shape_size(), bn::sprite_items::small_fixed_font.tiles_item(), 15,
                _numbers_palette, cpu_x, cpu_y),
    _turbo_bar_top_sprite(bn::sprite_items::turbo_bar.create_sprite(turbo_x, turbo_y - 16)),
    _turbo_bar_bottom_sprite(bn::sprite_items::turbo_bar.create_sprite(turbo_x, turbo_y + 16, 1)),
    _inverse_sprite(bn::sprite_items::inverse.create_sprite(0, 9 - 64)),
    _previous_pending_time(stage.total_time())
{
    _pending_time_number.show(stage.total_time());
    _position_number.show(stage.rival_car_infos().size());

    _current_lap_number.show(1);
    _current_lap_number.set_visible(false);

    _total_laps_number.show(stage.total_laps());
    _total_laps_number.set_visible(false);

    _turbo_bar_top_sprite.set_bg_priority(1);
    _turbo_bar_top_sprite.set_double_size_mode(bn::sprite_double_size_mode::DISABLED);

    _turbo_bar_bottom_sprite.set_bg_priority(1);
    _turbo_bar_bottom_sprite.set_double_size_mode(bn::sprite_double_size_mode::DISABLED);

    _inverse_sprite.set_bg_priority(1);
    _inverse_sprite.set_visible(false);

    bn::sprite_palette_ptr turbo_bar_palette = _turbo_bar_top_sprite.palette();
    turbo_bar_palette.set_fade_color(bn::color(28, 14, 2));

    bn::fixed time_label_x = pending_time_x + 3;
    _left_label_sprites.push_back(bn::sprite_items::race_labels.create_sprite(time_label_x, pending_time_y));
    _left_label_sprites.push_back(bn::sprite_items::race_labels.create_sprite(time_label_x + 17, pending_time_y, 1));

    bn::sprite_text_generator& text_generator = common_stuff.small_variable_text_generator;
    text_generator.set_palette_item(bn::sprite_items::race_labels.palette_item());
    text_generator.generate(lap_x, lap_y, "LAP", _left_label_sprites);
    text_generator.set_palette_item(bn::sprite_items::small_variable_font.palette_item());
    text_generator.set_one_sprite_per_character(true);
    text_generator.generate(lap_x + (8 * 5) - 6, lap_y, "/", _left_label_sprites);
    text_generator.set_one_sprite_per_character(false);
    _left_label_sprites.back().set_visible(false);

    bn::fixed position_label_x = position_x + 5;
    _right_label_sprites.push_back(bn::sprite_items::race_labels.create_sprite(position_label_x, position_y, 2));
    _right_label_sprites.push_back(bn::sprite_items::race_labels.create_sprite(position_label_x + 17, position_y, 3));

    #if ! FR_SHOW_CPU_USAGE
        _cpu_number.set_visible(false);
    #endif
}

void scoreboard::update(const stage& stage, const race_state& state, bn::fixed turbo_energy, bool intro_done)
{
    if(intro_done)
    {
        _even_update = ! _even_update;

        if(_even_update)
        {
            _update_pending_time(state);
            _update_inverse(state);
            _current_lap_number.show(bn::min(state.current_lap() + 1, stage.total_laps()));
        }
        else
        {
            _update_turbo_bar(turbo_energy);
            _position_number.show(state.current_position() + 1);
        }

        _update_turbo_bar_actions();
        _update_intro();
    }

    #if FR_SHOW_CPU_USAGE
        #if FR_SHOW_CPU_USAGE_MAX
            static_assert(! FR_SHOW_CPU_USAGE_MIN);
            static_assert(! FR_SHOW_CPU_USAGE_CURRENT);

            if(_cpu_counter < 4)
            {
               ++_cpu_counter;
            }
            else
            {
                _cpu_usage = bn::max(_cpu_usage, bn::core::last_cpu_usage());

                bn::fixed max_cpu_pct = _cpu_usage * 100;
                _cpu_number.show(max_cpu_pct.right_shift_integer());
            }
        #elif FR_SHOW_CPU_USAGE_MIN
            static_assert(! FR_SHOW_CPU_USAGE_CURRENT);

            if(! _cpu_counter)
            {
                _cpu_usage = bn::core::last_cpu_usage();
                ++_cpu_counter;
            }
            else
            {
                _cpu_usage = bn::min(_cpu_usage, bn::core::last_cpu_usage());

                bn::fixed min_cpu_pct = _cpu_usage * 100;
                _cpu_number.show(min_cpu_pct.right_shift_integer());
            }
        #elif FR_SHOW_CPU_USAGE_CURRENT
            bn::fixed cpu_pct = bn::core::last_cpu_usage() * 100;
            _cpu_number.show(cpu_pct.right_shift_integer());
        #else
            _cpu_usage = bn::max(_cpu_usage, bn::core::last_cpu_usage());

            if(_cpu_counter)
            {
               --_cpu_counter;
            }
            else
            {
                bn::fixed max_cpu_pct = _cpu_usage * 100;
                _cpu_number.show(max_cpu_pct.right_shift_integer());
                _cpu_usage = 0;
                _cpu_counter = 30;
            }
        #endif
    #endif
}

void scoreboard::_update_pending_time(const race_state& state)
{
    int previous_pending_time = _previous_pending_time;
    int current_pending_time = state.pending_time();

    if(previous_pending_time != current_pending_time)
    {
        _pending_time_number.show(current_pending_time);
        _previous_pending_time = current_pending_time;

        if(current_pending_time > previous_pending_time)
        {
            _pending_time_effect_counter = 64;
        }
        else if(current_pending_time < 10)
        {
            if(! _pending_time_red_palette)
            {
                _pending_time_palette.set_colors(bn::sprite_palette_items::big_digits_red);
                _pending_time_red_palette = true;
            }
        }
    }

    if(int counter = _pending_time_effect_counter)
    {
        _pending_time_effect_counter = counter - 1;

        if(counter % 8 == 0)
        {
            _pending_time_palette.set_colors(bn::sprite_palette_items::big_digits_orange);
        }
        else if(counter % 4 == 0)
        {
            if(current_pending_time >= 10)
            {
                _pending_time_palette.set_colors(bn::sprite_items::big_digits.palette_item());
                _pending_time_red_palette = false;
            }
            else
            {
                _pending_time_palette.set_colors(bn::sprite_palette_items::big_digits_red);
                _pending_time_red_palette = true;
            }
        }
    }
}

void scoreboard::_update_turbo_bar(bn::fixed turbo_energy)
{
    constexpr int last_graphics_index = 22;
    int graphics_index = (turbo_energy * last_graphics_index).right_shift_integer();

    if(_turbo_graphics_index < graphics_index)
    {
        ++_turbo_graphics_index;

        if(_turbo_graphics_index == last_graphics_index)
        {
            _turbo_bar_top_sprite.set_horizontal_scale(1.6);
            _turbo_bar_top_scale_action.emplace(_turbo_bar_top_sprite, 30, bn::fixed(1));

            _turbo_bar_bottom_sprite.set_horizontal_scale(1.6);
            _turbo_bar_bottom_scale_action.emplace(_turbo_bar_bottom_sprite, 30, bn::fixed(1));

            bn::sprite_palette_ptr turbo_bar_palette = _turbo_bar_top_sprite.palette();
            turbo_bar_palette.set_fade_intensity(1);
            _turbo_bar_palette_action.emplace(bn::move(turbo_bar_palette), 30, bn::fixed(0));
        }

        graphics_index = _turbo_graphics_index * 2;
        _turbo_bar_top_sprite.set_tiles(bn::sprite_items::turbo_bar.tiles_item(), graphics_index);
        _turbo_bar_bottom_sprite.set_tiles(bn::sprite_items::turbo_bar.tiles_item(), graphics_index + 1);
    }
    else if(_turbo_graphics_index > graphics_index)
    {
        if(_turbo_graphics_index == last_graphics_index)
        {
            bn::sprite_palette_ptr turbo_bar_palette = _turbo_bar_top_sprite.palette();
            turbo_bar_palette.set_fade_intensity(1);
            _turbo_bar_palette_action.emplace(bn::move(turbo_bar_palette), 30, bn::fixed(0));
        }

        --_turbo_graphics_index;

        graphics_index = _turbo_graphics_index * 2;
        _turbo_bar_top_sprite.set_tiles(bn::sprite_items::turbo_bar.tiles_item(), graphics_index);
        _turbo_bar_bottom_sprite.set_tiles(bn::sprite_items::turbo_bar.tiles_item(), graphics_index + 1);
    }
}

void scoreboard::_update_turbo_bar_actions()
{
    if(_turbo_bar_palette_action)
    {
        _turbo_bar_palette_action->update();

        if(_turbo_bar_palette_action->done())
        {
            _turbo_bar_palette_action.reset();
        }
    }

    if(_turbo_bar_top_scale_action)
    {
        _turbo_bar_top_scale_action->update();
        _turbo_bar_bottom_scale_action->update();

        if(_turbo_bar_top_scale_action->done())
        {
            _turbo_bar_top_scale_action.reset();
            _turbo_bar_bottom_scale_action.reset();
        }
    }
}

void scoreboard::_update_intro()
{
    if(_intro_counter < intro_frames)
    {
        ++_intro_counter;

        for(bn::sprite_ptr& left_label : _left_label_sprites)
        {
            left_label.set_x(left_label.x() + intro_x_inc);
        }

        _pending_time_number.set_x(_pending_time_number.x() + intro_x_inc);
        _current_lap_number.set_x(_current_lap_number.x() + intro_x_inc);
        _total_laps_number.set_x(_total_laps_number.x() + intro_x_inc);

        for(bn::sprite_ptr& right_label : _right_label_sprites)
        {
            right_label.set_x(right_label.x() - intro_x_inc);
        }

        _position_number.set_x(_position_number.x() - intro_x_inc);

        bn::fixed turbo_bar_x = _turbo_bar_top_sprite.x() - intro_x_inc;
        _turbo_bar_top_sprite.set_x(turbo_bar_x);
        _turbo_bar_bottom_sprite.set_x(turbo_bar_x);

        if(_intro_counter == intro_frames)
        {
            _current_lap_number.set_visible(true);
            _total_laps_number.set_visible(true);
            _left_label_sprites.back().set_visible(true);
        }
    }
}

void scoreboard::_update_inverse(const race_state& state)
{
    int inverse_frames = state.inverse_frames();

    if(int inverse_counter = _inverse_counter)
    {
        if(inverse_frames)
        {
            ++_inverse_counter;

            if(inverse_counter % 16 == 0)
            {
                bool visible = ! _inverse_sprite.visible();
                _inverse_sprite.set_visible(visible);

                if(visible)
                {
                    bn::sound_items::backwards02.play();
                }
            }
        }
        else
        {
            _inverse_sprite.set_visible(false);
            _inverse_counter = 0;
        }
    }
    else
    {
        if(inverse_frames >= 30)
        {
            _inverse_sprite.set_visible(true);
            _inverse_counter = 1;
            bn::sound_items::backwards02.play();
        }
    }
}

}
