#include "bf_game_intro.h"

#include "btn_colors.h"
#include "btn_rect_window.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_text_generator.h"
#include "btn_sprite_items_stage_1_intro.h"
#include "btn_sprite_items_stage_1_intro_alt.h"
#include "bf_constants.h"
#include "bf_butano_background.h"

namespace bf::game
{

namespace
{
    constexpr bool skip = false;

    constexpr const int wait_1_frames = 30;
    constexpr const int scale_frames = 40;
    constexpr const int wait_2_frames = 120;
    constexpr const int wait_3_frames = 40;
    constexpr const int background_move_frames = wait_2_frames + wait_3_frames + (scale_frames * 2);

    void _load_sprites(btn::ivector<btn::sprite_ptr>& sprites)
    {
        int x = -64 + 8;
        btn::sprite_builder builder(btn::sprite_items::stage_1_intro);
        builder.set_x(x);
        builder.set_scale_x(2);
        builder.set_scale_y(0.01);
        builder.set_z_order(constants::intro_sprites_z_order);
        builder.set_ignore_camera(true);
        builder.set_visible(false);
        sprites.push_back(builder.build());
        x += 128;

        builder = btn::sprite_builder(btn::sprite_items::stage_1_intro, 1);
        builder.set_x(x);
        builder.set_scale_x(2);
        builder.set_scale_y(0.01);
        builder.set_z_order(constants::intro_sprites_z_order);
        builder.set_ignore_camera(true);
        builder.set_visible(false);
        sprites.push_back(builder.build());
    }

    btn::sprite_third_attributes_hblank_effect_ptr _create_hblank_effect(
            const btn::sprite_ptr& sprite, const btn::sprite_palette_ptr& second_palette,
            btn::ivector<btn::sprite_third_attributes>& hblank_effect_attributes)
    {
        btn::sprite_third_attributes attributes = sprite.third_attributes();
        btn::sprite_third_attributes alt_attributes = attributes;
        alt_attributes.set_palette(second_palette);

        for(int index = 0; index < btn::display::height(); index += 2)
        {
            hblank_effect_attributes.push_back(attributes);
            hblank_effect_attributes.push_back(alt_attributes);
        }

        btn::span<const btn::sprite_third_attributes> attributes_span(
                    hblank_effect_attributes.data(), btn::display::height());
        return btn::sprite_third_attributes_hblank_effect_ptr::create(sprite, attributes_span);
    }
}

intro::intro(btn::sprite_text_generator& text_generator)
{
    _load_sprites(_background_sprites);

    btn::horizontal_alignment_type old_alignment = text_generator.alignment();
    int old_bg_priority = text_generator.bg_priority();
    int old_z_order = text_generator.z_order();
    text_generator.set_alignment(btn::horizontal_alignment_type::CENTER);
    text_generator.set_bg_priority(3);
    text_generator.set_z_order(constants::intro_sprites_z_order);
    text_generator.set_one_sprite_per_character(true);
    text_generator.generate(0, -16, "STAGE 1", _text_sprites);
    text_generator.generate(0, 16, "DWARF LAND", _text_sprites);
    text_generator.set_alignment(old_alignment);
    text_generator.set_bg_priority(old_bg_priority);
    text_generator.set_z_order(old_z_order);
    text_generator.set_one_sprite_per_character(false);

    for(btn::sprite_ptr& text_sprite : _text_sprites)
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
            for(btn::sprite_ptr& background_sprite : _background_sprites)
            {
                background_sprite.set_visible(true);
                _background_sprite_move_actions.emplace_back(background_sprite, background_move_frames,
                                                             background_sprite.x() - 16, background_sprite.y());
                _background_sprite_scale_y_actions.emplace_back(background_sprite, scale_frames, 2);
            }

            btn::sprite_palette_ptr first_palette = _background_sprites[0].palette();
            btn::sprite_palette_ptr second_palette = btn::sprite_items::stage_1_intro_alt.palette_item().create_palette();
            first_palette.set_fade(btn::colors::white, 0.5);
            second_palette.set_fade(btn::colors::black, 0.5);
            _background_sprite_palette_actions.emplace_back(first_palette, scale_frames, 0);
            _background_sprite_palette_actions.emplace_back(second_palette, scale_frames, 0);
            _background_sprite_hblank_effects.push_back(
                        _create_hblank_effect(
                            _background_sprites[0], second_palette, _background_sprite_hblank_effect_attributes_1));
            _background_sprite_hblank_effects.push_back(
                        _create_hblank_effect(
                            _background_sprites[1], second_palette, _background_sprite_hblank_effect_attributes_2));

            btn::rect_window internal_window = btn::rect_window::internal();
            internal_window.set_boundaries(0, -128, 0, 128);
            internal_window.set_ignore_camera(true);
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

        for(auto& background_sprite_scale_y_action : _background_sprite_scale_y_actions)
        {
            background_sprite_scale_y_action.update();
        }

        for(auto& background_sprite_palette_action : _background_sprite_palette_actions)
        {
            background_sprite_palette_action.update();
        }

        _window_move_top_action->update();
        _window_move_bottom_action->update();

        if(_background_sprite_scale_y_actions[0].done())
        {
            _background_sprite_scale_y_actions.clear();
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
            for(auto& text_sprite_scale_y_action : _text_sprite_scale_y_actions)
            {
                if(! text_sprite_scale_y_action.done())
                {
                    text_sprite_scale_y_action.update();
                }
            }

            if(_counter % 2 == 0)
            {
                int sprites_count = _text_sprites.size();
                int actions_count = _text_sprite_scale_y_actions.size();

                if(actions_count < sprites_count)
                {
                    btn::sprite_ptr text_sprite = _text_sprites[actions_count];
                    text_sprite.set_visible(true);
                    _text_sprite_scale_y_actions.emplace_back(move(text_sprite), scale_frames / 2, 2);
                }
            }
        }
        else
        {
            _text_sprite_scale_y_actions.clear();

            _state = state::WAIT_3;
            _counter = wait_3_frames;
        }
        break;

    case state::WAIT_3:
        for(auto& text_sprite_scale_y_action : _text_sprite_scale_y_actions)
        {
            if(! text_sprite_scale_y_action.done())
            {
                text_sprite_scale_y_action.update();

                if(text_sprite_scale_y_action.done())
                {
                    btn::sprite_ptr text_sprite = text_sprite_scale_y_action.sprite();
                    text_sprite.set_visible(false);
                }
            }
        }

        --_counter;

        if(_counter % 2 == 0)
        {
            int sprites_count = _text_sprites.size();
            int actions_count = _text_sprite_scale_y_actions.size();

            if(actions_count < sprites_count)
            {
                _text_sprite_scale_y_actions.emplace_back(_text_sprites[actions_count], scale_frames / 2, 0.01);
            }
        }

        if(! _counter)
        {
            for(btn::sprite_ptr& background_sprite : _background_sprites)
            {
                _background_sprite_scale_y_actions.emplace_back(background_sprite, scale_frames, 0.1);
            }

            for(auto& background_sprite_palette_action : _background_sprite_palette_actions)
            {
                background_sprite_palette_action = btn::sprite_palette_fade_to_action(
                            background_sprite_palette_action.palette(), scale_frames, 0.5);
            }

            btn::rect_window internal_window = btn::rect_window::internal();
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

            for(auto& background_sprite_scale_y_action : _background_sprite_scale_y_actions)
            {
                background_sprite_scale_y_action.update();
            }

            for(auto& background_sprite_palette_action : _background_sprite_palette_actions)
            {
                background_sprite_palette_action.update();
            }

            for(auto& text_sprite_scale_y_action : _text_sprite_scale_y_actions)
            {
                if(! text_sprite_scale_y_action.done())
                {
                    text_sprite_scale_y_action.update();

                    if(text_sprite_scale_y_action.done())
                    {
                        btn::sprite_ptr text_sprite = text_sprite_scale_y_action.sprite();
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
            _background_sprite_scale_y_actions.clear();
            _background_sprite_palette_actions.clear();
            _window_move_top_action.reset();
            _window_move_bottom_action.reset();
            _background_sprite_hblank_effects.clear();
            _background_sprite_hblank_effect_attributes_1.clear();
            _background_sprite_hblank_effect_attributes_2.clear();
            _background_sprites.clear();
            _text_sprites.clear();
            _text_sprite_scale_y_actions.clear();

            btn::rect_window internal_window = btn::rect_window::internal();
            internal_window.set_boundaries(0, 0, 0, 0);
            internal_window.set_ignore_camera(false);

            _state = state::DONE;
        }
        break;

    case state::DONE:
        break;
    }
}

}
