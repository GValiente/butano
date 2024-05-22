/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_how_to_play_scene.h"

#include "bn_keypad.h"
#include "bn_colors.h"
#include "bn_blending.h"

#include "fr_scene_type.h"
#include "fr_common_stuff.h"

#include "bn_sound_items.h"
#include "bn_regular_bg_items_title_backdrop_fade.h"
#include "bn_regular_bg_items_title_backdrop_mini.h"

#include "models/fr_model_3d_items_gba.h"

namespace fr
{

namespace
{
    constexpr int total_animation_frames = 16;

    constexpr bn::fixed model_y = 128;
    constexpr bn::fixed model_z_inc = 5;

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
}

how_to_play_scene::how_to_play_scene(scene_type next_scene, common_stuff& common_stuff) :
    _common_stuff(common_stuff),
    _bgs_fade_in_action(_create_bgs_fade_in_action()),
    _sprites_fade_in_action(_create_sprites_fade_in_action()),
    _backdrop_bg(bn::regular_bg_items::title_backdrop_fade.create_bg(0, 0)),
    _background_bg(bn::regular_bg_items::title_backdrop_mini.create_bg(0, 0)),
    _background_move_action(_background_bg, 1, -1),
    _next_scene(next_scene)
{
    _background_bg.set_blending_enabled(true);
    bn::blending::set_intensity_alpha(0.5);

    _models.load_colors(model_3d_items::gba_colors);
    _gba_model = &_models.create_dynamic_model(model_3d_items::gba);
    _gba_model->set_position(point_3d(0, model_y, model_z_inc));

    bn::sprite_text_generator& big_text_generator = common_stuff.big_text_generator;
    big_text_generator.set_center_alignment();
    big_text_generator.generate(0, -65, "HOW TO PLAY", _title_sprites);
    big_text_generator.generate(-22, 19, "PAD: steer", _left_right_text_sprites);
    big_text_generator.generate(26, 18, "A: accelerate", _a_text_sprites);
    big_text_generator.generate(22, 22, "B: brake", _b_text_sprites);
    big_text_generator.generate(5, 13, "R: turbo boost!", _r_text_sprites);
    big_text_generator.generate(5, 13 + 12, "(when the bar is full)", _r_text_sprites);
    big_text_generator.generate(-21, 30, "START: pause", _start_text_sprites);
    big_text_generator.set_left_alignment();

    for(bn::sprite_ptr& text_sprite : _left_right_text_sprites)
    {
        text_sprite.set_visible(false);
    }

    for(bn::sprite_ptr& text_sprite : _a_text_sprites)
    {
        text_sprite.set_visible(false);
    }

    for(bn::sprite_ptr& text_sprite : _b_text_sprites)
    {
        text_sprite.set_visible(false);
    }

    for(bn::sprite_ptr& text_sprite : _r_text_sprites)
    {
        text_sprite.set_visible(false);
    }

    for(bn::sprite_ptr& text_sprite : _start_text_sprites)
    {
        text_sprite.set_visible(false);
    }
}

bn::optional<scene_type> how_to_play_scene::update()
{
    bn::optional<scene_type> result;

    if(_bgs_fade_in_action.done())
    {
        if(_animation_frames)
        {
            _animate();
        }
        else if(_animation_index < 6)
        {
            if(bn::keypad::a_pressed())
            {
                _setup_animation();
            }
        }
        else
        {
            sram_data& sram_data = _common_stuff.storage;
            sram_data.set_how_to_play_scene_shown(true);
            sram_data.write();
            bn::blending::set_intensity_alpha(0);
            result = _next_scene;
        }

        if(_bgs_fade_out_action)
        {
            _bgs_fade_out_action->update();
            _sprites_fade_out_action->update();

            if(_bgs_fade_out_action->done())
            {
                _bgs_fade_out_action.reset();
                _sprites_fade_out_action.reset();
            }
        }
    }
    else
    {
        _bgs_fade_in_action.update();
        _sprites_fade_in_action.update();
    }

    _background_move_action.update();
    _models.update(_camera);

    return result;
}

void how_to_play_scene::_setup_animation()
{
    point_3d current_position = _gba_model->position();
    bn::fixed current_phi = _gba_model->phi();
    bn::fixed current_psi = _gba_model->psi();
    bn::fixed target_x;
    bn::fixed target_z;
    bn::fixed target_phi;
    bn::fixed target_psi;

    _models.destroy_dynamic_model(*_gba_model);
    _gba_model = &_models.create_dynamic_model(model_3d_items::gba);
    _gba_model->set_position(current_position);
    _gba_model->set_phi(current_phi);
    _gba_model->set_psi(current_psi);

    if(current_phi > 32768)
    {
        current_phi -= 65536;
    }

    switch(_animation_index)
    {

    case 0:
        target_x = bn::fixed(33) / 2;
        target_z = bn::fixed(4) / 2;
        target_phi = -6000;
        target_psi = 0;
        _animation_frames = total_animation_frames;
        break;

    case 1:
        target_x = bn::fixed(-36.5) / 2;
        target_z = bn::fixed(5.5) / 2;
        target_phi = 6000;
        target_psi = 0;
        _animation_frames = total_animation_frames;

        for(bn::sprite_ptr& text_sprite : _left_right_text_sprites)
        {
            text_sprite.set_visible(false);
        }
        break;

    case 2:
        target_x = bn::fixed(-28.5) / 2;
        target_z = bn::fixed(2.5) / 2;
        target_phi = 5000;
        target_psi = 0;
        _animation_frames = total_animation_frames;

        for(bn::sprite_ptr& text_sprite : _a_text_sprites)
        {
            text_sprite.set_visible(false);
        }
        break;

    case 3:
        target_x = bn::fixed(-36.5) / 2;
        target_z = bn::fixed(21.5) / 2;
        target_phi = 6000;
        target_psi = 6000;
        _animation_frames = total_animation_frames;

        for(bn::sprite_ptr& text_sprite : _b_text_sprites)
        {
            text_sprite.set_visible(false);
        }
        break;

    case 4:
        target_x = bn::fixed(27) / 2;
        target_z = bn::fixed(-8) / 2;
        target_phi = -5000;
        target_psi = 0;
        _animation_frames = total_animation_frames;

        for(bn::sprite_ptr& text_sprite : _r_text_sprites)
        {
            text_sprite.set_visible(false);
        }
        break;

    case 5:
        target_x = 0;
        target_z = 0;
        target_phi = 0;
        target_psi = 16000;
        _animation_frames = total_animation_frames * 4;

        for(bn::sprite_ptr& text_sprite : _start_text_sprites)
        {
            text_sprite.set_visible(false);
        }

        _bgs_fade_out_action.emplace(_animation_frames, 1);
        _sprites_fade_out_action.emplace(_animation_frames, 1);
        break;

    default:
        BN_ERROR("Invalid animation index: ", _animation_index);
        break;
    }

    target_z += model_z_inc;
    _model_x_inc = (target_x - current_position.x()) / _animation_frames;
    _model_z_inc = (target_z - current_position.z()) / _animation_frames;
    _model_phi_inc = (target_phi - current_phi) / _animation_frames;
    _model_psi_inc = (target_psi - current_psi) / _animation_frames;
}

void how_to_play_scene::_animate()
{
    point_3d position = _gba_model->position();
    position.set_x(position.x() + _model_x_inc);
    position.set_z(position.z() + _model_z_inc);

    bn::fixed phi = _gba_model->phi() + _model_phi_inc;
    bn::fixed psi = _gba_model->psi() + _model_psi_inc;
    --_animation_frames;

    if(! _animation_frames)
    {
        _models.destroy_dynamic_model(*_gba_model);

        switch(_animation_index)
        {

        case 0:
            _gba_model = &_models.create_dynamic_model(model_3d_items::gba_left_right_held);

            for(bn::sprite_ptr& text_sprite : _left_right_text_sprites)
            {
                text_sprite.set_visible(true);
            }

            bn::sound_items::blip02.play();
            break;

        case 1:
            _gba_model = &_models.create_dynamic_model(model_3d_items::gba_a_held);

            for(bn::sprite_ptr& text_sprite : _a_text_sprites)
            {
                text_sprite.set_visible(true);
            }

            bn::sound_items::blip02.play();
            break;

        case 2:
            _gba_model = &_models.create_dynamic_model(model_3d_items::gba_b_held);

            for(bn::sprite_ptr& text_sprite : _b_text_sprites)
            {
                text_sprite.set_visible(true);
            }

            bn::sound_items::blip02.play();
            break;

        case 3:
            _gba_model = &_models.create_dynamic_model(model_3d_items::gba_r_held);

            for(bn::sprite_ptr& text_sprite : _r_text_sprites)
            {
                text_sprite.set_visible(true);
            }

            bn::sound_items::blip02.play();
            break;

        case 4:
            _gba_model = &_models.create_dynamic_model(model_3d_items::gba_start_held);

            for(bn::sprite_ptr& text_sprite : _start_text_sprites)
            {
                text_sprite.set_visible(true);
            }

            bn::sound_items::blip02.play();
            break;

        case 5:
            _gba_model = nullptr;
            break;

        default:
            BN_ERROR("Invalid animation index: ", _animation_index);
            break;
        }

        ++_animation_index;
    }

    if(_gba_model)
    {
        _gba_model->set_position(position);
        _gba_model->set_phi(phi);
        _gba_model->set_psi(psi);
    }
}

}
