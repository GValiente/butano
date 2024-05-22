/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_butano_background.h"

#include "bn_color.h"
#include "bn_window.h"
#include "bn_display.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_items_butano_big_silhouette.h"
#include "bn_regular_bg_items_butano_background.h"

namespace bf
{

namespace
{
    constexpr int show_hide_frames = 10;

    constexpr bn::array<bn::color, bn::display::height()> palette_hbe_colors = []{
        bn::array<bn::color, bn::display::height()> result;
        bn::color color(14, 0, 0);
        int redInc = 16;
        int greenInc = 16;
        int blueInc = 8;

        for(int index = 0; index < bn::display::height(); ++index)
        {
            result[index] = color;
            --redInc;
            --greenInc;
            --blueInc;

            if(! redInc)
            {
                color.set_red(color.red() + (index <= 80 ? 1 : -1));
                redInc = 16;
            }

            if(! greenInc)
            {
                color.set_green(bn::max(color.green() + (index <= 80 ? 1 : -1), 0));
                greenInc = 16;
            }

            if(! blueInc)
            {
                color.set_blue(bn::max(color.blue() + (index <= 80 ? 1 : -1), 0));
                blueInc = 16;
            }
        }

        return result;
    }();
}

void butano_background::show(const bn::fixed_point& silhouette_position, const bn::camera_ptr& camera)
{
    _set_visible();

    bn::regular_bg_ptr bg = _move_action->bg();
    bg.set_priority(0);
    bn::window::sprites().set_show_bg(bg, false);

    bn::sprite_builder builder(bn::sprite_items::butano_big_silhouette, 0);
    builder.set_position(silhouette_position.x(), silhouette_position.y() - 63);
    builder.set_scale(2);
    builder.set_window_enabled(true);
    builder.set_camera(camera);

    bn::sprite_ptr silhouette_up_sprite = builder.release_build();
    _silhouette_up_sprite_move_action.emplace(silhouette_up_sprite, show_hide_frames, silhouette_position);
    _silhouette_up_sprite_scale_action.emplace(bn::move(silhouette_up_sprite), show_hide_frames, bn::fixed(0.01));

    builder = bn::sprite_builder(bn::sprite_items::butano_big_silhouette, 1);
    builder.set_position(silhouette_position.x(), silhouette_position.y() + 63);
    builder.set_scale(2);
    builder.set_window_enabled(true);
    builder.set_camera(camera);

    bn::sprite_ptr silhouette_down_sprite = builder.release_build();
    _silhouette_down_sprite_move_action.emplace(silhouette_down_sprite, show_hide_frames, silhouette_position);
    _silhouette_down_sprite_scale_action.emplace(bn::move(silhouette_down_sprite), show_hide_frames, bn::fixed(0.01));
}

void butano_background::hide(const bn::fixed_point& silhouette_position, const bn::camera_ptr& camera)
{
    _set_visible();

    bn::regular_bg_ptr bg = _move_action->bg();
    bg.set_priority(0);
    bn::window::sprites().set_show_bg(bg, false);

    bn::sprite_builder builder(bn::sprite_items::butano_big_silhouette, 0);
    builder.set_position(silhouette_position);
    builder.set_scale(0.01);
    builder.set_window_enabled(true);
    builder.set_camera(camera);

    bn::sprite_ptr silhouette_up_sprite = builder.release_build();
    _silhouette_up_sprite_move_action.emplace(silhouette_up_sprite, show_hide_frames, silhouette_position.x(),
                                              silhouette_position.y() - 63);
    _silhouette_up_sprite_scale_action.emplace(bn::move(silhouette_up_sprite), show_hide_frames, 2);

    builder = bn::sprite_builder(bn::sprite_items::butano_big_silhouette, 1);
    builder.set_position(silhouette_position);
    builder.set_scale(0.01);
    builder.set_window_enabled(true);
    builder.set_camera(camera);

    bn::sprite_ptr silhouette_down_sprite = builder.release_build();
    _silhouette_down_sprite_move_action.emplace(silhouette_down_sprite, show_hide_frames, silhouette_position.x(),
                                                silhouette_position.y() + 63);
    _silhouette_down_sprite_scale_action.emplace(bn::move(silhouette_down_sprite), show_hide_frames, 2);
}

void butano_background::put_under_all()
{
    _set_visible();

    bn::regular_bg_ptr bg = _move_action->bg();
    bg.set_priority(3);
}

void butano_background::update()
{
    if(_move_action)
    {
        _move_action->update();

        if(_silhouette_up_sprite_move_action)
        {
            _silhouette_up_sprite_move_action->update();
            _silhouette_up_sprite_scale_action->update();
            _silhouette_down_sprite_move_action->update();
            _silhouette_down_sprite_scale_action->update();

            if(_silhouette_up_sprite_move_action->done())
            {
                if(_silhouette_up_sprite_move_action->sprite().horizontal_scale() > 1)
                {
                    _move_action.reset();
                    _palette_hbe.reset();
                }

                _silhouette_up_sprite_move_action.reset();
                _silhouette_up_sprite_scale_action.reset();
                _silhouette_down_sprite_move_action.reset();
                _silhouette_down_sprite_scale_action.reset();
            }
        }
    }
}

void butano_background::_set_visible()
{
    if(! _move_action)
    {
        bn::regular_bg_ptr bg = bn::regular_bg_items::butano_background.create_bg(0, 0);
        _palette_hbe = bn::bg_palette_color_hbe_ptr::create(bg.palette(), 1, palette_hbe_colors);
        _move_action.emplace(bn::move(bg), 1, -1);
    }
}

}
