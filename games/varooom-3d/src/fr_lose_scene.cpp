/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_lose_scene.h"

#include "bn_keypad.h"
#include "bn_colors.h"

#include "fr_scene_type.h"
#include "fr_common_stuff.h"

#include "bn_sound_items.h"
#include "bn_sprite_items_small_variable_font.h"
#include "bn_sprite_palette_items_small_font_gray_border.h"

#include "models/fr_model_3d_items_character_l.h"
#include "models/fr_model_3d_items_character_o.h"
#include "models/fr_model_3d_items_character_s.h"
#include "models/fr_model_3d_items_character_e.h"

namespace fr
{

namespace
{
    constexpr int model_anim_frames = 16;

    constexpr bn::fixed model_source_y = 0;
    constexpr bn::fixed model_target_y = 128;
    constexpr bn::fixed model_inc_y = (model_target_y - model_source_y) / model_anim_frames;
    constexpr bn::fixed model_z = -11;

    constexpr bn::fixed model_source_phi = -16384;
    constexpr bn::fixed model_target_phi = 16384;
    constexpr bn::fixed model_inc_phi = (model_target_phi - model_source_phi) / model_anim_frames;

    constexpr bn::fixed model_psi = 65536 - 16384;

    constexpr bn::fixed text_x = -22;
    constexpr bn::fixed restart_text_y = 39;
    constexpr bn::fixed exit_text_y = restart_text_y + 11;

    constexpr bn::color model_colors[] = {
        bn::color(31, 22, 0)
    };

    [[nodiscard]] bn::sprite_ptr _create_cursor_sprite(common_stuff& common_stuff)
    {
        bn::vector<bn::sprite_ptr, 1> output_sprites;
        bn::sprite_text_generator& text_generator = common_stuff.small_variable_text_generator;
        text_generator.set_palette_item(bn::sprite_palette_items::small_font_gray_border);
        text_generator.set_one_sprite_per_character(true);
        text_generator.generate(text_x - 10, restart_text_y, ">", output_sprites);
        text_generator.set_one_sprite_per_character(false);
        return bn::move(output_sprites[0]);
    }
}

lose_scene::lose_scene(common_stuff& common_stuff) :
    _cursor_sprite(_create_cursor_sprite(common_stuff))
{
    bn::sprite_text_generator& text_generator = common_stuff.small_variable_text_generator;
    text_generator.generate(text_x, restart_text_y, "RESTART", _text_sprites);
    text_generator.generate(text_x, exit_text_y, "EXIT", _text_sprites);
    text_generator.set_palette_item(bn::sprite_items::small_variable_font.palette_item());

    _cursor_sprite.set_visible(false);

    for(bn::sprite_ptr& text_sprite : _text_sprites)
    {
        text_sprite.set_visible(false);
    }

    _models.load_colors(model_colors);

    bn::bg_palettes::set_fade(bn::colors::black, 0);
    bn::sprite_palettes::set_fade(bn::colors::black, 0);
}

bn::optional<scene_type> lose_scene::update()
{
    bn::optional<scene_type> result;

    if(! _animation_done)
    {
        _animate();
    }
    else if(_wait_frames)
    {
        if(_bgs_fade_action)
        {
            _bgs_fade_action->update();
            _sprites_fade_action->update();

            if(_bgs_fade_action->done())
            {
                _bgs_fade_action.reset();
                _sprites_fade_action.reset();

                _models.destroy_dynamic_model(*_l_model);
                _models.destroy_dynamic_model(*_o_model);
                _models.destroy_dynamic_model(*_s_model);
                _models.destroy_dynamic_model(*_e_model);
            }
        }

        --_wait_frames;

        if(_wait_frames == 130)
        {
            if(_cursor_index == 0)
            {
                _announcer.play_restart_voice(0);
            }
            else
            {
                _announcer.play_exit_voice(0);
            }
        }
        else if(! _wait_frames)
        {
            if(_cursor_index == 0)
            {
                result = scene_type::RACE;
            }
            else
            {
                result = scene_type::TITLE;
            }
        }
    }
    else if(bn::keypad::a_pressed())
    {
        _bgs_fade_action.emplace(60, 1);
        _sprites_fade_action.emplace(60, 1);
        _wait_frames = 130 + 30;
        bn::sound_items::blip09.play();
    }
    else if(bn::keypad::up_pressed())
    {
        if(_cursor_index > 0)
        {
            --_cursor_index;
            _cursor_sprite.set_y(restart_text_y);
            bn::sound_items::blip02.play();
        }
    }
    else if(bn::keypad::down_pressed())
    {
        if(_cursor_index < 1)
        {
            ++_cursor_index;
            _cursor_sprite.set_y(exit_text_y);
            bn::sound_items::blip02.play();
        }
    }

    _announcer.refesh_choice<3>();
    _announcer.update();
    _models.update(_camera);
    return result;
}

void lose_scene::_animate()
{
    if(! _l_model)
    {
        _l_model = &_models.create_dynamic_model(model_3d_items::character_l);
        _l_model->set_position(point_3d(-31, model_source_y, model_z));
        _l_model->set_phi(model_source_phi);
        _l_model->set_psi(model_psi);

        bn::sound_items::brake2_8bit.play();
        return;
    }

    point_3d position = _l_model->position();

    if(position.y() < model_target_y)
    {
        position.set_y(position.y() + model_inc_y);
        _l_model->set_position(position);
        _l_model->set_phi(_l_model->phi() + model_inc_phi);
        return;
    }

    if(! _o_model)
    {
        _o_model = &_models.create_dynamic_model(model_3d_items::character_o_first_color);
        _o_model->set_position(point_3d(-10, model_source_y, model_z));
        _o_model->set_phi(model_source_phi);
        _o_model->set_psi(model_psi);
        return;
    }

    position = _o_model->position();

    if(position.y() < model_target_y)
    {
        position.set_y(position.y() + model_inc_y);
        _o_model->set_position(position);
        _o_model->set_phi(_o_model->phi() + model_inc_phi);

        if(position.y() == model_source_y + (model_inc_y * 6))
        {
            bn::sound_items::brake8bit_4.play();
        }

        return;
    }

    if(! _s_model)
    {
        _s_model = &_models.create_dynamic_model(model_3d_items::character_s);
        _s_model->set_position(point_3d(9, model_source_y, model_z));
        _s_model->set_phi(model_source_phi);
        _s_model->set_psi(model_psi);
        return;
    }

    position = _s_model->position();

    if(position.y() < model_target_y)
    {
        position.set_y(position.y() + model_inc_y);
        _s_model->set_position(position);
        _s_model->set_phi(_s_model->phi() + model_inc_phi);
        return;
    }

    if(! _e_model)
    {
        _e_model = &_models.create_dynamic_model(model_3d_items::character_e);
        _e_model->set_position(point_3d(32, model_source_y, model_z));
        _e_model->set_phi(model_source_phi);
        _e_model->set_psi(model_psi);
        return;
    }

    position = _e_model->position();

    if(position.y() < model_target_y)
    {
        position.set_y(position.y() + model_inc_y);
        _e_model->set_position(position);
        _e_model->set_phi(_e_model->phi() + model_inc_phi);
        _wait_frames = 65 + 40;

        if(position.y() == model_source_y + (model_inc_y * 4))
        {
            bn::sound_items::crashboombang3.play();
        }

        return;
    }

    if(_wait_frames)
    {
        --_wait_frames;

        if(_wait_frames == 65)
        {
            bn::sound_items::v_04youlose.play();
        }

        return;
    }

    _cursor_sprite.set_visible(true);

    for(bn::sprite_ptr& text_sprite : _text_sprites)
    {
        text_sprite.set_visible(true);
    }

    _animation_done = true;
}

}
