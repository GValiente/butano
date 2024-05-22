/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_boss_intro.h"

#include "bn_colors.h"
#include "bn_display.h"
#include "bn_bg_palettes.h"
#include "bn_sound_items.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_palettes.h"
#include "bn_affine_mat_attributes.h"
#include "bn_sprite_items_boss_intro.h"

namespace bf::game
{

namespace
{
    constexpr int amplitude = 32;
    constexpr bool skip = false;

    [[nodiscard]] bn::sprite_ptr _create_sprite(int x, int graphics_index,
                                                 const bn::sprite_affine_mat_ptr& affine_mat)
    {
        bn::sprite_builder builder(bn::sprite_items::boss_intro, graphics_index);
        builder.set_x(x);
        builder.set_affine_mat(affine_mat);
        builder.set_bg_priority(2);
        return builder.release_build();
    }

    constexpr bn::array<bn::affine_mat_attributes, bn::display::height()> _hbe_attributes = []{
        bn::affine_mat_attributes base_attributes;
        base_attributes.set_scale(0.001);

        bn::array<bn::affine_mat_attributes, bn::display::height()> result;
        result.fill(base_attributes);

        bn::fixed scale = 1;
        bn::fixed scale_step = 0.002;
        bn::fixed scale_dec = 0;

        for(int index = 0; index < amplitude; ++index)
        {
            int base_index = bn::display::height() / 2;
            result[base_index - index].set_scale(scale);
            result[base_index + index + 1].set_scale(scale);
            scale_dec += scale_step;
            scale -= scale_dec;
        }

        return result;
    }();
}

void boss_intro::enable()
{
    bn::sprite_affine_mat_ptr affine_mat = bn::sprite_affine_mat_ptr::create();
    int x = -84;
    _sprites.push_back(_create_sprite(x, 0, affine_mat));
    x += 31;
    _sprites.push_back(_create_sprite(x, 1, affine_mat));
    x += 30;
    _sprites.push_back(_create_sprite(x, 2, affine_mat));
    x += 30;
    _sprites.push_back(_create_sprite(x, 3, affine_mat));
    x += 24;
    _sprites.push_back(_create_sprite(x, 4, affine_mat));
    x += 24;
    _sprites.push_back(_create_sprite(x, 5, affine_mat));
    x += 30;
    _sprites.push_back(_create_sprite(x, 6, affine_mat));
    _music_volume_action.emplace(amplitude * 2, 0);
    _hbe = bn::sprite_affine_mat_attributes_hbe_ptr::create(bn::move(affine_mat), _hbe_attributes);

    _state = state::ACTIVE;
    _loops = 5;
    _counter = 1;
}

void boss_intro::update()
{
    switch(_state)
    {

    case state::INIT:
        break;

    case state::ACTIVE:
        --_counter;

        if(! _counter)
        {
            --_loops;

            if(_loops && ! skip)
            {
                bn::sound_items::space_shooter_7.play();
                _counter = amplitude * 2;
            }
            else
            {
                bn::bg_palettes::set_contrast(0);
                bn::sprite_palettes::set_contrast(0);
                _sprites.clear();
                _hbe.reset();
                _loops = 0;
                _state = state::DONE;
            }
        }

        if(_counter)
        {
            bn::fixed y = _counter - amplitude;

            for(bn::sprite_ptr& sprite : _sprites)
            {
                sprite.set_y(y);
            }

            bn::fixed contrast = (amplitude - bn::abs(y)) / amplitude;
            bn::bg_palettes::set_contrast(contrast);
            bn::sprite_palettes::set_contrast(contrast);
        }

        if(_music_volume_action)
        {
            _music_volume_action->update();

            if(_music_volume_action->done())
            {
                _music_volume_action.reset();
            }
        }
        break;

    case state::DONE:
        break;

    default:
        BN_ERROR("Invalid state: ", int(_state));
        break;
    }
}

}
