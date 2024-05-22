/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_background.h"

#include "bn_colors.h"
#include "bn_blending.h"
#include "bn_bg_palettes.h"
#include "bn_rect_window.h"
#include "bn_regular_bg_builder.h"
#include "bf_game_stage.h"
#include "bf_wave_generator.h"

namespace bf::game
{

namespace
{
    constexpr bn::fixed blending_transparency = 0.4;

    bn::regular_bg_ptr _create_bottom_bg(const stage& stage, const bn::camera_ptr& camera)
    {
        bn::regular_bg_builder builder(stage.background_bottom_bg_item);
        builder.set_mosaic_enabled(true);
        builder.set_camera(camera);
        return builder.release_build();
    }

    bn::regular_bg_ptr _create_top_bg(const stage& stage, const bn::camera_ptr& camera)
    {
        bn::regular_bg_builder builder(stage.background_top_bg_item);
        builder.set_priority(2);
        builder.set_blending_enabled(true);
        builder.set_mosaic_enabled(true);
        builder.set_camera(camera);
        return builder.release_build();
    }
}

background::background(const stage& stage, const bn::camera_ptr& camera) :
    _bottom_move_action(_create_bottom_bg(stage, camera), stage.background_bottom_bg_delta_position),
    _top_move_action(_create_top_bg(stage, camera), stage.background_top_bg_delta_position),
    _hbe(bn::regular_bg_position_hbe_ptr::create_horizontal(_bottom_move_action.bg(), _hbe_deltas))
{
    bn::blending::set_transparency_alpha(blending_transparency);
    bn::window::internal().set_show_bg(_top_move_action.bg(), false);
    _hbe.set_visible(false);
}

void background::set_visible(bool visible)
{
    bn::regular_bg_ptr bottom_bg = _bottom_move_action.bg();
    bottom_bg.set_visible(visible);

    bn::regular_bg_ptr top_bg = _top_move_action.bg();
    top_bg.set_visible(visible);
}

void background::show_bomb_open(int frames)
{
    bn::bg_palette_ptr bottom_palette = _bottom_move_action.bg().palette();
    bottom_palette.set_grayscale_intensity(0);
    _bottom_palette_grayscale_action.emplace(bn::move(bottom_palette), frames / 2, 1);

    bn::bgs_mosaic::set_stretch(0.1);
    _mosaic_action = bn::bgs_mosaic_stretch_loop_action(4, 0.2);
}

void background::show_bomb_close(int frames)
{
    bn::bgs_mosaic::set_stretch(0);
    _mosaic_action.reset();

    _hbe.set_visible(true);
    _fade_frames = frames;
}

void background::hide_bomb_close(int frames)
{
    bn::bg_palette_ptr bottom_palette = _bottom_move_action.bg().palette();
    bottom_palette.set_grayscale_intensity(1);
    _bottom_palette_grayscale_action.emplace(bn::move(bottom_palette), frames, 0);
}

void background::show_explosion_open(int frames)
{
    const bn::regular_bg_ptr& bottom_bg = _bottom_move_action.bg();
    bn::bg_palette_ptr bottom_palette = bottom_bg.palette();
    bottom_palette.set_fade(bn::colors::orange, 0);
    _bottom_palette_fade_action.emplace(bn::move(bottom_palette), frames, 1);

    const bn::regular_bg_ptr& top_bg = _top_move_action.bg();
    bn::bg_palette_ptr top_palette = top_bg.palette();
    top_palette.set_fade(bn::colors::orange, 0);
    _top_palette_fade_action.emplace(bn::move(top_palette), frames, 1);

    bn::bgs_mosaic::set_stretch(0.1);
    _mosaic_action = bn::bgs_mosaic_stretch_loop_action(4, 0.2);

    bn::rect_window external_window = bn::rect_window::external();
    external_window.set_show_bg(bottom_bg, false);
    external_window.set_show_bg(top_bg, false);

    bn::bg_palettes::set_transparent_color(bn::colors::orange);
}

void background::show_explosion_close(int frames)
{
    const bn::regular_bg_ptr& bottom_bg = _bottom_move_action.bg();
    bn::bg_palette_ptr bottom_palette = bottom_bg.palette();
    _bottom_palette_fade_action.emplace(bn::move(bottom_palette), frames, 0);

    const bn::regular_bg_ptr& top_bg = _top_move_action.bg();
    bn::bg_palette_ptr top_palette = top_bg.palette();
    _top_palette_fade_action.emplace(bn::move(top_palette), frames, 0);

    bn::bgs_mosaic::set_stretch(0);
    _mosaic_action.reset();

    bn::rect_window external_window = bn::rect_window::external();
    external_window.set_show_bg(bottom_bg, true);
    external_window.set_show_bg(top_bg, true);

    _hbe.set_visible(true);
    _fade_frames = frames;
}

void background::show_top(int frames)
{
    bn::blending::set_transparency_alpha(0);
    _blending_action.emplace(frames, blending_transparency);
}

void background::show_hero_dying()
{
    _bottom_palette_inverted_action.emplace(_bottom_move_action.bg().palette(), 5);
    _top_palette_inverted_action.emplace(_top_move_action.bg().palette(), 5);
    bn::green_swap::set_enabled(true);
    _green_swap_action.emplace(5);
}

void background::show_hero_alive()
{
    bn::bg_palette_ptr bottom_palette = _bottom_move_action.bg().palette();
    bn::bg_palette_ptr top_palette = _top_move_action.bg().palette();
    bottom_palette.set_inverted(false);
    top_palette.set_inverted(false);
    bn::green_swap::set_enabled(false);
    _bottom_palette_inverted_action.reset();
    _top_palette_inverted_action.reset();
    _green_swap_action.reset();
}

void background::show_hero_dead()
{
    bn::bg_palette_ptr bottom_palette = _bottom_move_action.bg().palette();
    bn::bg_palette_ptr top_palette = _top_move_action.bg().palette();
    bottom_palette.set_inverted(true);
    top_palette.set_inverted(true);
    bn::green_swap::set_enabled(false);
    _bottom_palette_inverted_action.reset();
    _top_palette_inverted_action.reset();
    _green_swap_action.reset();
}

void background::reset()
{
    bn::blending::set_transparency_alpha(1);
}

void background::update()
{
    _bottom_move_action.update();
    _top_move_action.update();

    if(_mosaic_action)
    {
        _mosaic_action->update();
    }

    if(_blending_action)
    {
        _blending_action->update();

        if(_blending_action->done())
        {
            _blending_action.reset();
        }
    }

    if(_bottom_palette_fade_action)
    {
        _bottom_palette_fade_action->update();
        _top_palette_fade_action->update();

        if(_bottom_palette_fade_action->done())
        {
            _bottom_palette_fade_action.reset();
            _top_palette_fade_action.reset();
        }
    }

    if(_bottom_palette_grayscale_action)
    {
        _bottom_palette_grayscale_action->update();

        if(_bottom_palette_grayscale_action->done())
        {
            _bottom_palette_grayscale_action.reset();
        }
    }

    if(_bottom_palette_inverted_action)
    {
        _bottom_palette_inverted_action->update();
        _top_palette_inverted_action->update();
        _green_swap_action->update();
    }

    if(_fade_frames)
    {
        --_fade_frames;

        if(_fade_frames)
        {
            wave_generator wave_generator;
            int hbe_speed_multiplier;
            int hbe_amplitude;

            switch(_fade_frames)
            {

            case 1:
                hbe_speed_multiplier = 1;
                hbe_amplitude = 1;
                break;

            case 2:
                hbe_speed_multiplier = 2;
                hbe_amplitude = 1;
                break;

            case 3:
                hbe_speed_multiplier = 4;
                hbe_amplitude = 2;
                break;

            case 4:
                hbe_speed_multiplier = 8;
                hbe_amplitude = 2;
                break;

            case 5:
                hbe_speed_multiplier = 16;
                hbe_amplitude = 3;
                break;

            default:
                hbe_speed_multiplier = 32;
                hbe_amplitude = 3;
                break;
            }

            wave_generator.set_speed(_fade_frames * hbe_speed_multiplier);
            wave_generator.set_amplitude(hbe_amplitude);
            wave_generator.generate(_hbe_deltas);
            _hbe.reload_deltas_ref();
        }
        else
        {
            _hbe.set_visible(false);
        }
    }
}

}
