/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_intro.h"

#include "bn_colors.h"
#include "bn_rect_window.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_text_generator.h"
#include "bf_game_stage.h"
#include "bf_butano_background.h"

namespace bf::game
{

namespace
{
    constexpr bool skip = false;

    constexpr int wait_1_frames = 30;
    constexpr int scale_frames = 40;
    constexpr int wait_2_frames = 120;
    constexpr int wait_3_frames = 40;
    constexpr int background_move_frames = wait_2_frames + wait_3_frames + (scale_frames * 2);

    bn::vector<bn::sprite_ptr, 2> _create_background_sprites(const stage& stage)
    {
        bn::vector<bn::sprite_ptr, 2> result;
        int x = -64 + 8;

        bn::sprite_builder builder(stage.intro_sprite_item);
        builder.set_x(x);
        builder.set_horizontal_scale(2);
        builder.set_vertical_scale(0.01);
        builder.set_z_order(constants::intro_sprites_z_order);
        builder.set_visible(false);
        result.push_back(builder.build());

        builder = bn::sprite_builder(stage.intro_sprite_item, 1);
        builder.set_x(x + 128);
        builder.set_horizontal_scale(2);
        builder.set_vertical_scale(0.01);
        builder.set_z_order(constants::intro_sprites_z_order);
        builder.set_visible(false);
        result.push_back(builder.build());

        return result;
    }

    bn::sprite_third_attributes_hbe_ptr _create_hbe(
            const bn::sprite_ptr& sprite, const bn::sprite_palette_ptr& second_palette,
            bn::ivector<bn::sprite_third_attributes>& hbe_attributes)
    {
        bn::sprite_third_attributes attributes = sprite.third_attributes();
        bn::sprite_third_attributes alt_attributes = attributes;
        alt_attributes.set_palette(second_palette);

        for(int index = 0; index < bn::display::height(); index += 2)
        {
            hbe_attributes.push_back(attributes);
            hbe_attributes.push_back(alt_attributes);
        }

        bn::span<const bn::sprite_third_attributes> attributes_span(hbe_attributes.data(), bn::display::height());
        return bn::sprite_third_attributes_hbe_ptr::create(sprite, attributes_span);
    }
}

intro::intro(const stage& stage, bn::sprite_text_generator& text_generator) :
    _background_sprites(_create_background_sprites(stage)),
    _alt_palette(stage.intro_alt_sprite_item.palette_item().create_palette())
{
    int old_bg_priority = text_generator.bg_priority();
    int old_z_order = text_generator.z_order();
    text_generator.set_center_alignment();
    text_generator.set_bg_priority(3);
    text_generator.set_z_order(constants::intro_sprites_z_order);
    text_generator.set_one_sprite_per_character(true);
    text_generator.generate(0, -16, stage.intro_top_label, _text_sprites);
    text_generator.generate(0, 16, stage.intro_bottom_label, _text_sprites);
    text_generator.set_bg_priority(old_bg_priority);
    text_generator.set_z_order(old_z_order);
    text_generator.set_one_sprite_per_character(false);

    for(bn::sprite_ptr& text_sprite : _text_sprites)
    {
        text_sprite.set_x(text_sprite.x() * 2);
        text_sprite.set_scale(2, 0.01);
        text_sprite.set_visible(false);
    }
}

void intro::update(const butano_background& butano_background)
{
    switch(_state)
    {

    case state::INIT:
        if(! butano_background.silhouette_visible())
        {
            if(skip)
            {
                _state = state::OUT;
                _counter = 1;
            }
            else
            {
                _state = state::WAIT_1;
                _counter = wait_1_frames;
            }
        }
        break;

    case state::WAIT_1:
        --_counter;

        if(! _counter)
        {
            for(bn::sprite_ptr& background_sprite : _background_sprites)
            {
                background_sprite.set_visible(true);
                _background_sprite_move_actions.emplace_back(background_sprite, background_move_frames,
                                                             background_sprite.x() - 16, background_sprite.y());
                _background_sprite_vertical_scale_actions.emplace_back(background_sprite, scale_frames, 2);
            }

            bn::sprite_palette_ptr first_palette = _background_sprites[0].palette();
            bn::sprite_palette_ptr& second_palette = *_alt_palette;
            first_palette.set_fade(bn::colors::white, 0.5);
            second_palette.set_fade(bn::colors::black, 0.5);
            _background_sprite_palette_actions.emplace_back(first_palette, scale_frames, 0);
            _background_sprite_palette_actions.emplace_back(second_palette, scale_frames, 0);
            _background_sprite_hbes.push_back(
                        _create_hbe(_background_sprites[0], second_palette, _background_sprite_hbe_attributes_1));
            _background_sprite_hbes.push_back(
                        _create_hbe(_background_sprites[1], second_palette, _background_sprite_hbe_attributes_2));

            bn::rect_window internal_window = bn::rect_window::internal();
            internal_window.set_boundaries(0, -128, 0, 128);
            _window_move_top_action.emplace(internal_window, scale_frames, -64);
            _window_move_bottom_action.emplace(internal_window, scale_frames, 64);

            _state = state::IN;
        }
        break;

    case state::IN:
        for(auto& background_sprite_move_action : _background_sprite_move_actions)
        {
            background_sprite_move_action.update();
        }

        for(auto& background_sprite_vertical_scale_action : _background_sprite_vertical_scale_actions)
        {
            background_sprite_vertical_scale_action.update();
        }

        for(auto& background_sprite_palette_action : _background_sprite_palette_actions)
        {
            background_sprite_palette_action.update();
        }

        _window_move_top_action->update();
        _window_move_bottom_action->update();

        if(_background_sprite_vertical_scale_actions[0].done())
        {
            _background_sprite_vertical_scale_actions.clear();
            _window_move_top_action.reset();
            _window_move_bottom_action.reset();

            _state = state::WAIT_2;
            _counter = wait_2_frames;
        }
        break;

    case state::WAIT_2:
        for(auto& background_sprite_move_action : _background_sprite_move_actions)
        {
            background_sprite_move_action.update();
        }

        --_counter;

        if(_counter)
        {
            for(auto& text_sprite_vertical_scale_action : _text_sprite_vertical_scale_actions)
            {
                if(! text_sprite_vertical_scale_action.done())
                {
                    text_sprite_vertical_scale_action.update();
                }
            }

            if(_counter % 2 == 0)
            {
                int sprites_count = _text_sprites.size();
                int actions_count = _text_sprite_vertical_scale_actions.size();

                if(actions_count < sprites_count)
                {
                    bn::sprite_ptr text_sprite = _text_sprites[actions_count];
                    text_sprite.set_visible(true);
                    _text_sprite_vertical_scale_actions.emplace_back(bn::move(text_sprite), scale_frames / 2, 2);
                }
            }
        }
        else
        {
            _text_sprite_vertical_scale_actions.clear();

            _state = state::WAIT_3;
            _counter = wait_3_frames;
        }
        break;

    case state::WAIT_3:
        for(auto& text_sprite_vertical_scale_action : _text_sprite_vertical_scale_actions)
        {
            if(! text_sprite_vertical_scale_action.done())
            {
                text_sprite_vertical_scale_action.update();

                if(text_sprite_vertical_scale_action.done())
                {
                    bn::sprite_ptr text_sprite = text_sprite_vertical_scale_action.sprite();
                    text_sprite.set_visible(false);
                }
            }
        }

        --_counter;

        if(_counter % 2 == 0)
        {
            int sprites_count = _text_sprites.size();
            int actions_count = _text_sprite_vertical_scale_actions.size();

            if(actions_count < sprites_count)
            {
                _text_sprite_vertical_scale_actions.emplace_back(
                            _text_sprites[actions_count], scale_frames / 2, bn::fixed(0.01));
            }
        }

        if(! _counter)
        {
            for(bn::sprite_ptr& background_sprite : _background_sprites)
            {
                _background_sprite_vertical_scale_actions.emplace_back(background_sprite, scale_frames, bn::fixed(0.1));
            }

            for(auto& background_sprite_palette_action : _background_sprite_palette_actions)
            {
                background_sprite_palette_action = bn::sprite_palette_fade_to_action(
                            background_sprite_palette_action.palette(), scale_frames, 0.5);
            }

            bn::rect_window internal_window = bn::rect_window::internal();
            _window_move_top_action.emplace(internal_window, scale_frames, 0);
            _window_move_bottom_action.emplace(internal_window, scale_frames, 0);

            _state = state::OUT;
            _counter = scale_frames;
        }
        break;

    case state::OUT:
        --_counter;

        if(_counter)
        {
            for(auto& background_sprite_move_action : _background_sprite_move_actions)
            {
                background_sprite_move_action.update();
            }

            for(auto& background_sprite_vertical_scale_action : _background_sprite_vertical_scale_actions)
            {
                background_sprite_vertical_scale_action.update();
            }

            for(auto& background_sprite_palette_action : _background_sprite_palette_actions)
            {
                background_sprite_palette_action.update();
            }

            for(auto& text_sprite_vertical_scale_action : _text_sprite_vertical_scale_actions)
            {
                if(! text_sprite_vertical_scale_action.done())
                {
                    text_sprite_vertical_scale_action.update();

                    if(text_sprite_vertical_scale_action.done())
                    {
                        bn::sprite_ptr text_sprite = text_sprite_vertical_scale_action.sprite();
                        text_sprite.set_visible(false);
                    }
                }
            }

            _window_move_top_action->update();
            _window_move_bottom_action->update();
        }
        else
        {
            _background_sprite_move_actions.clear();
            _background_sprite_vertical_scale_actions.clear();
            _background_sprite_palette_actions.clear();
            _window_move_top_action.reset();
            _window_move_bottom_action.reset();
            _background_sprite_hbes.clear();
            _background_sprite_hbe_attributes_1.clear();
            _background_sprite_hbe_attributes_2.clear();
            _background_sprites.clear();
            _alt_palette.reset();
            _text_sprites.clear();
            _text_sprite_vertical_scale_actions.clear();

            bn::rect_window internal_window = bn::rect_window::internal();
            internal_window.set_boundaries(0, 0, 0, 0);

            _state = state::DONE;
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
