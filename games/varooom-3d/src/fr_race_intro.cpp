/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_race_intro.h"

#include "bn_colors.h"

#include "fr_stage.h"
#include "fr_models_3d.h"
#include "fr_camera_3d.h"

#include "bn_music_items.h"
#include "models/fr_model_3d_items_character_1.h"
#include "models/fr_model_3d_items_character_2.h"
#include "models/fr_model_3d_items_character_3.h"
#include "models/fr_model_3d_items_character_g.h"
#include "models/fr_model_3d_items_character_o.h"

namespace fr
{

namespace
{
    constexpr int animation_frames = 32;

    constexpr bn::fixed target_y = 200 - constants_3d::camera_diff_y;
    constexpr bn::fixed inc_y = 4;
    constexpr bn::fixed inc_angle = 512;
    constexpr bn::fixed source_y = target_y - (animation_frames * inc_y);
}

race_intro::race_intro(models_3d& models) :
    _models(models)
{
    bn::bg_palettes::set_fade(bn::colors::black, 1);
    bn::sprite_palettes::set_fade(bn::colors::black, 1);
}

race_intro::~race_intro()
{
    if(_character_model_1)
    {
        _models.destroy_dynamic_model(*_character_model_1);
        _character_model_1 = nullptr;
    }

    if(_character_model_2)
    {
        _models.destroy_dynamic_model(*_character_model_2);
        _character_model_2 = nullptr;
    }
}

#if FR_SKIP_RACE_INTRO
    void race_intro::update(const stage&, camera_3d&)
    {
        switch(_index)
        {

        case 0:
            if(_counter < 30)
            {
                ++_counter;

                if(_counter == 3)
                {
                    _bg_fade_action.emplace(30 - 5, 0);
                    _sprite_fade_action.emplace(30 - 5, 0);
                }
            }
            else
            {
                _counter = 0;
                _index = 10;
            }

            if(_bg_fade_action)
            {
                _bg_fade_action->update();
                _sprite_fade_action->update();

                if(_bg_fade_action->done())
                {
                    _bg_fade_action.reset();
                    _sprite_fade_action.reset();
                }
            }
            break;

        case 10:
            break;

        default:
            BN_ERROR("Invalid index: ", _index);
            break;
        }
    }
#else
    void race_intro::update(const stage& stage, camera_3d& camera)
    {
        switch(_index)
        {

        case 0:
            _update_camera(camera);

            if(_counter < 30)
            {
                ++_counter;

                if(_counter == 3)
                {
                    _bg_fade_action.emplace(30 - 5, 0);
                    _sprite_fade_action.emplace(30 - 5, 0);
                }
            }
            else
            {
                _character_model_1 = &_models.create_dynamic_model(model_3d_items::character_3);

                point_3d position = camera.position();
                position.set_y(source_y + _camera_y - constants_3d::camera_min_y);
                _character_model_1->set_position(position);
                _character_model_1->set_phi(0);
                _character_model_1->set_theta(16384);
                _character_model_1->set_psi(camera.phi());

                _counter = animation_frames;
                ++_index;
            }

            if(_bg_fade_action)
            {
                _bg_fade_action->update();
                _sprite_fade_action->update();

                if(_bg_fade_action->done())
                {
                    _bg_fade_action.reset();
                    _sprite_fade_action.reset();
                }
            }
            break;

        case 1:
            _update_camera(camera);

            if(_counter)
            {
                point_3d position = camera.position();
                position.set_y(_character_model_1->position().y() + inc_y);
                _character_model_1->set_position(position);
                _character_model_1->set_phi(_character_model_1->phi() + inc_angle);
                _character_model_1->set_theta(_character_model_1->theta() - inc_angle);
                _character_model_1->set_psi(_character_model_1->psi() - inc_angle);
                --_counter;

                if(_counter == 6)
                {
                    stage.music_item().play(bn::fixed::from_data(constants_3d::music_volume_data));
                }
            }
            else
            {
                _counter = stage.countdown_wait_frames();
                ++_index;
            }

            if(_bg_fade_action)
            {
                _bg_fade_action->update();
                _sprite_fade_action->update();

                if(_bg_fade_action->done())
                {
                    _bg_fade_action.reset();
                    _sprite_fade_action.reset();
                }
            }
            break;

        case 2:
            _update_camera(camera);

            if(_counter)
            {
                --_counter;
            }
            else
            {
                _models.destroy_dynamic_model(*_character_model_1);
                _character_model_1 = &_models.create_dynamic_model(model_3d_items::character_2);

                point_3d position = camera.position();
                position.set_y(source_y + _camera_y - constants_3d::camera_min_y);
                _character_model_1->set_position(position);
                _character_model_1->set_phi(32768);
                _character_model_1->set_theta(65536 - 16384);
                _character_model_1->set_psi(32768 + camera.phi());

                _counter = animation_frames;
                ++_index;
            }
            break;

        case 3:
            _update_camera(camera);

            if(_counter)
            {
                point_3d position = _character_model_1->position();
                position.set_y(position.y() + inc_y);
                _character_model_1->set_position(position);
                _character_model_1->set_phi(_character_model_1->phi() - inc_angle);
                _character_model_1->set_theta(_character_model_1->theta() + inc_angle);
                _character_model_1->set_psi(_character_model_1->psi() + inc_angle);
                --_counter;
            }
            else
            {
                _counter = stage.countdown_wait_frames();
                ++_index;
            }
            break;

        case 4:
            _update_camera(camera);

            if(_counter)
            {
                --_counter;
            }
            else
            {
                _models.destroy_dynamic_model(*_character_model_1);
                _character_model_1 = &_models.create_dynamic_model(model_3d_items::character_1);

                point_3d position = camera.position();
                position.set_y(source_y + _camera_y - constants_3d::camera_min_y);
                _character_model_1->set_position(position);
                _character_model_1->set_phi(65536 - 16384);
                _character_model_1->set_theta(32768);
                _character_model_1->set_psi(16384 + camera.phi());

                _counter = animation_frames;
                ++_index;
            }
            break;

        case 5:
            _update_camera(camera);

            if(_counter)
            {
                point_3d position = _character_model_1->position();
                position.set_y(position.y() + inc_y);
                _character_model_1->set_position(position);
                _character_model_1->set_phi(_character_model_1->phi() + (inc_angle * 2));
                _character_model_1->set_theta(_character_model_1->theta() - (inc_angle * 2));
                _character_model_1->set_psi(_character_model_1->psi() + (inc_angle * 2));
                --_counter;
            }
            else
            {
                _counter = stage.countdown_wait_frames();
                ++_index;
            }
            break;

        case 6:
            _update_camera(camera);

            if(_counter)
            {
                --_counter;
            }
            else
            {
                point_3d position = camera.position();
                bn::fixed camera_x = position.x();
                bn::fixed camera_phi = camera.phi();
                bn::fixed phi = 16384;
                bn::fixed psi = 65536 - 16384 + camera_phi;
                position.set_x(camera_phi > 0 ? camera_x + 12 : camera_x - 12);
                position.set_y(source_y + _camera_y - constants_3d::camera_min_y);

                _models.destroy_dynamic_model(*_character_model_1);
                _character_model_1 = &_models.create_dynamic_model(model_3d_items::character_g_full);
                _character_model_1->set_position(position);
                _character_model_1->set_phi(phi);
                _character_model_1->set_psi(psi);

                position.set_x(camera_phi > 0 ? camera_x - 12 : camera_x + 12);
                _character_model_2 = &_models.create_dynamic_model(model_3d_items::character_o_full);
                _character_model_2->set_position(position);
                _character_model_2->set_phi(phi);
                _character_model_2->set_psi(psi);

                _counter = animation_frames;
                ++_index;
            }
            break;

        case 7:
            _update_camera(camera);

            if(_counter)
            {
                point_3d position = _character_model_1->position();
                position.set_y(position.y() + 3);
                _character_model_1->set_position(position);
                _character_model_1->set_phi(_character_model_1->phi() + (inc_angle * 4));

                position = _character_model_2->position();
                position.set_y(position.y() + 3);
                _character_model_2->set_position(position);
                _character_model_2->set_phi(_character_model_2->phi() + (inc_angle * 4));
                --_counter;
            }
            else
            {
                point_3d position_1 = _character_model_1->position();
                point_3d position_2 = _character_model_2->position();
                bn::fixed phi = _character_model_1->phi();
                bn::fixed psi = _character_model_1->psi();
                _models.destroy_dynamic_model(*_character_model_1);
                _models.destroy_dynamic_model(*_character_model_2);

                _character_model_1 = &_models.create_dynamic_model(model_3d_items::character_g);
                _character_model_1->set_position(position_1);
                _character_model_1->set_phi(phi);
                _character_model_1->set_psi(psi);

                _character_model_2 = &_models.create_dynamic_model(model_3d_items::character_o);
                _character_model_2->set_position(position_2);
                _character_model_2->set_phi(phi);
                _character_model_2->set_psi(psi);

                _counter = 40;
                ++_index;
            }
            break;

        case 8:
            _update_camera(camera);

            if(_counter)
            {
                --_counter;
            }
            else
            {
                _counter = 9;
                ++_index;
            }
            break;

        case 9:
            _update_camera(camera);

            if(_counter)
            {
                bn::fixed scale = _character_model_1->scale();
                _character_model_1->set_scale(scale - 0.1);
                _character_model_2->set_scale(scale - 0.1);
                --_counter;
            }
            else
            {
                _models.destroy_dynamic_model(*_character_model_1);
                _models.destroy_dynamic_model(*_character_model_2);
                _character_model_1 = nullptr;
                _character_model_2 = nullptr;
                ++_index;
            }
            break;

        case 10:
            break;

        default:
            BN_ERROR("Invalid index: ", _index);
            break;
        }
    }

    void race_intro::_update_camera(camera_3d& camera)
    {
        if(_camera_y < constants_3d::camera_min_y)
        {
            point_3d position = camera.position();
            position.set_y(_camera_y);
            camera.set_position(position);
            _camera_y += 0.5;
        }
    }
#endif

}
