/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_title_scene.h"

#include "bn_colors.h"
#include "bn_display.h"
#include "bn_blending.h"
#include "bn_regular_bg_map_ptr.h"

#include "fr_sin_cos.h"
#include "fr_scene_type.h"
#include "fr_common_stuff.h"
#include "fr_model_colors.h"

#include "bn_music_items.h"
#include "bn_sound_items.h"
#include "bn_regular_bg_items_title_backdrop_1.h"
#include "bn_regular_bg_items_title_backdrop_2.h"
#include "bn_regular_bg_items_title_backdrop_3.h"
#include "bn_regular_bg_items_title_backdrop_4.h"
#include "bn_regular_bg_items_title_backdrop_5.h"
#include "bn_regular_bg_items_title_backdrop_6.h"
#include "bn_regular_bg_items_title_backdrop_7.h"
#include "bn_regular_bg_items_title_backdrop_8.h"
#include "bn_regular_bg_items_title_backdrop_9.h"
#include "bn_regular_bg_items_title_backdrop_10.h"
#include "bn_regular_bg_items_title_backdrop_fade.h"
#include "bn_regular_bg_items_title_backdrop_mini.h"

#include "models/fr_model_3d_items_player_car.h"
#include "models/fr_model_3d_items_character_v.h"
#include "models/fr_model_3d_items_character_a.h"
#include "models/fr_model_3d_items_character_r.h"
#include "models/fr_model_3d_items_character_o.h"
#include "models/fr_model_3d_items_character_m.h"
#include "models/fr_model_3d_items_character_3.h"
#include "models/fr_model_3d_items_character_d.h"

namespace fr
{

namespace
{
    constexpr int fade_frames = 60;
    constexpr int varooom_model_anim_frames = 8;
    constexpr int three_d_model_anim_frames = 64 + 12;

    constexpr bn::fixed model_source_y = 196;
    constexpr bn::fixed model_target_y = 4;
    constexpr bn::fixed varooom_model_inc_y = (model_target_y - model_source_y) / varooom_model_anim_frames;
    constexpr bn::fixed three_d_model_inc_y = (model_target_y - model_source_y) / three_d_model_anim_frames;

    constexpr bn::fixed model_source_z = 0;
    constexpr bn::fixed model_target_z = -48;
    constexpr bn::fixed varooom_model_inc_z = (model_target_z - model_source_z) / varooom_model_anim_frames;
    constexpr bn::fixed three_d_model_inc_z = (model_target_z - model_source_z) / three_d_model_anim_frames;

    constexpr bn::fixed model_source_phi = 0;
    constexpr bn::fixed model_target_phi = 16384;
    constexpr bn::fixed varooom_model_inc_phi = (model_target_phi - model_source_phi) / varooom_model_anim_frames;
    constexpr bn::fixed three_d_model_inc_phi = (65536 + model_target_phi - model_source_phi) / three_d_model_anim_frames;

    constexpr bn::fixed model_psi = 65536 - 16384;

    constexpr bn::fixed model_x = -99.5;
    constexpr bn::fixed model_inc_x = 24;
}

title_scene::title_scene(common_stuff& common_stuff) :
    _common_stuff(common_stuff),
    _base_menu(common_stuff),
    _race_menu(common_stuff),
    _options_menu(common_stuff),
    _advices(common_stuff),
    _flag(_models)
{
    _models.load_colors(default_model_colors);

    bn::color fade_color = bn::bg_palettes::fade_color();
    bn::fixed fade_intensity = bn::bg_palettes::fade_intensity();
    bn::bg_palettes::set_transparent_color(bn::colors::black);

    if(fade_color == bn::colors::black && fade_intensity == 1)
    {
        bn::bg_palettes::set_fade_intensity(0);
        bn::sprite_palettes::set_fade_intensity(0);
    }
    else if(fade_intensity > 0)
    {
        _bgs_fade_action.emplace(fade_frames, bn::fixed(0));
        bn::sprite_palettes::set_fade_intensity(0);
    }

    _wait_frames = fade_frames;
}

title_scene::~title_scene()
{
    bn::blending::set_intensity_alpha(0);
}

bn::optional<scene_type> title_scene::update()
{
    bn::optional<scene_type> result;
    bool update_advices = true;

    if(! _intro_done)
    {
        _update_intro();
        update_advices = false;
    }
    else if(_bgs_fade_out_action)
    {
        if(_bgs_fade_out_action->done())
        {
            if(! _wait_frames)
            {
                bn::music::stop();

                switch(*_base_menu_selected_option)
                {

                case title_base_menu::option::RACE:
                    if(_common_stuff.storage.how_to_play_scene_shown())
                    {
                        result = scene_type::RACE;
                    }
                    else
                    {
                        result = scene_type::HOW_TO_PLAY_AND_RACE;
                    }
                    break;

                case title_base_menu::option::MODEL_VIEWER:
                    result = scene_type::MODEL_VIEWER;
                    break;

                case title_base_menu::option::OPTIONS:
                    BN_ERROR("Unexpected option: ", int(*_base_menu_selected_option));
                    break;

                case title_base_menu::option::HOW_TO_PLAY:
                    result = scene_type::HOW_TO_PLAY_AND_TITLE;
                    break;

                case title_base_menu::option::CREDITS:
                    result = scene_type::CREDITS;
                    break;

                default:
                    BN_ERROR("Invalid option: ", int(*_base_menu_selected_option));
                    break;
                }
            }
        }
        else
        {
            _bgs_fade_out_action->update();
            _sprites_fade_out_action->update();
            _music_volume_action->update();
            _race_menu.blink_cursor();
        }

        if(_wait_frames)
        {
            --_wait_frames;
        }
    }
    else if(_base_menu.animating())
    {
        [[maybe_unused]] bn::optional<title_base_menu::option> unused = _base_menu.update();
    }
    else if(_race_menu.visible())
    {
        if(bn::optional<bool> race_menu_result = _race_menu.update(_common_stuff, *_front_backdrop_bg))
        {
            if(*race_menu_result)
            {
                _exit();
                _announcer.play_race_start_voice(55);
                _wait_frames = 55 + 100;
            }
            else
            {
                _race_menu.set_visible(false);
                _base_menu.set_visible(true, true);
                _advices.set_visible(true);
            }
        }

        update_advices = false;
    }
    else if(_options_menu.visible())
    {
        if(! _options_menu.update(_common_stuff))
        {
            _options_menu.set_visible(_common_stuff, false);
            _base_menu.set_visible(true, false);
        }
    }
    else
    {
        _base_menu_selected_option = _base_menu.update();

        if(_base_menu_selected_option)
        {
            switch(*_base_menu_selected_option)
            {

            case title_base_menu::option::RACE:
                _base_menu.set_visible(false, true);
                _advices.set_visible(false);
                _race_menu.set_visible(true);
                break;

            case title_base_menu::option::MODEL_VIEWER:
                _exit();
                break;

            case title_base_menu::option::OPTIONS:
                _base_menu.set_visible(false, false);
                _options_menu.set_visible(_common_stuff, true);
                break;

            case title_base_menu::option::HOW_TO_PLAY:
                _exit();
                break;

            case title_base_menu::option::CREDITS:
                _exit();
                break;

            default:
                BN_ERROR("Invalid option: ", int(*_base_menu_selected_option));
                break;
            }
        }
    }

    _models.update(_camera);

    if(_middle_backdrop_action)
    {
        _middle_backdrop_action->update();
    }

    _announcer.refesh_choice<5>();
    _announcer.update();
    _flag.update();

    if(update_advices)
    {
        _advices.update(_common_stuff);
    }

    if(result)
    {
        _common_stuff.storage.write();
    }

    return result;
}

void title_scene::_update_intro()
{
    switch(_intro_index)
    {

    case -1:
        _update_intro_minus_1();
        break;

    case 0:
        _update_intro_0();
        break;

    case 1:
        _update_intro_1();
        break;

    case 2:
        _update_intro_2();
        break;

    case 3:
        _update_intro_3();
        break;

    case 4:
        _update_intro_4();
        break;

    case 5:
        _update_intro_5();
        break;

    case 6:
        _update_intro_6();
        break;

    case 7:
        _update_intro_7();
        break;

    case 8:
        _update_intro_8();
        break;

    case 9:
        _update_intro_9();
        break;

    case 10:
        _update_intro_10();
        break;

    case 11:
        _update_intro_11();
        break;

    default:
        BN_ERROR("Invalid intro index: ", _intro_index);
        break;
    }
}

void title_scene::_update_intro_minus_1()
{
    if(_bgs_fade_action && ! _bgs_fade_action->done())
    {
        _bgs_fade_action->update();
    }

    if(_wait_frames)
    {
        --_wait_frames;

        if(_wait_frames == fade_frames - 10)
        {
            bn::music_items::carpass.play(0.7);
        }

        return;
    }

    _bgs_fade_action.reset();
    ++_intro_index;
}

void title_scene::_update_intro_0()
{
    auto set_car_position = [](model_3d& car_model) {
        bn::fixed car_angle = car_model.phi();
        int angle = car_angle.right_shift_integer();
        bn::fixed sf = sin(angle);
        bn::fixed cf = cos(angle);
        car_model.set_position(point_3d((cf * 128) - 92, (sf * 128) + 224, 8));
    };

    if(! _model)
    {
        _model = &_models.create_dynamic_model(model_3d_items::player_car);
        _model->set_phi(65535);
        _model->set_psi(-16384);
        set_car_position(*_model);
        return;
    }

    bn::fixed car_angle = _model->phi();

    if(car_angle >= 65536 - 16384)
    {
        _model->set_phi(car_angle - 330);
        set_car_position(*_model);
        _wait_frames = 16;
        return;
    }

    if(_wait_frames)
    {
        --_wait_frames;
        return;
    }

    _intro_scene_started = false;
    ++_intro_index;
}

void title_scene::_update_intro_1()
{
    auto set_car_position = [](model_3d& car_model) {
        bn::fixed car_angle = car_model.phi() + 8192;
        int angle = car_angle.right_shift_integer();
        bn::fixed sf = sin(angle);
        bn::fixed cf = cos(angle);
        car_model.set_position(point_3d((sf * 16) - 16, 192, (-cf * 128) - 128));
    };

    if(! _intro_scene_started)
    {
        _model->set_phi(32768);
        _model->set_psi(65536 - 16384 - 7000);
        set_car_position(*_model);

        bn::sound_items::brake2_8bit.play(1, 1, -0.5);
        _intro_scene_started = true;
        return;
    }

    bn::fixed car_angle = _model->phi();

    if(car_angle >= 32768 - 8192)
    {
        _model->set_phi(car_angle - 330);
        set_car_position(*_model);
        _wait_frames = 16;
        return;
    }

    if(_wait_frames)
    {
        --_wait_frames;
        return;
    }

    _intro_scene_started = false;
    ++_intro_index;
}

void title_scene::_update_intro_2()
{
    if(! _intro_scene_started)
    {
        _models.destroy_dynamic_model(*_model);
        _model = nullptr;

        _back_backdrop_bg = bn::regular_bg_items::title_backdrop_fade.create_bg(0, 0);

        _middle_backdrop_bg = bn::regular_bg_items::title_backdrop_mini.create_bg(0, 0);
        _middle_backdrop_bg->set_blending_enabled(true);
        _middle_backdrop_action.emplace(*_middle_backdrop_bg, 1, -1);
        bn::blending::set_intensity_alpha(0.5);

        _front_backdrop_bg = bn::regular_bg_items::title_backdrop_1.create_bg(
                    (256 - bn::display::width()) / 2, (256 - bn::display::height()) / 2);

        bn::bg_palettes::remove_transparent_color();
        bn::bg_palettes::set_fade(bn::colors::white, 1);
        _bgs_fade_action.emplace(30, bn::fixed(0));

        bn::music_items::titletheme.play(0.4);
        _intro_scene_started = true;
        return;
    }

    if(_bgs_fade_action)
    {
        _bgs_fade_action->update();

        if(_bgs_fade_action->done())
        {
            _bgs_fade_action.reset();
        }

        return;
    }

    if(! _model)
    {
        _model = &_models.create_dynamic_model(model_3d_items::character_v);
        _model->set_position(point_3d(0, model_source_y, model_source_z));
        _model->set_phi(model_source_phi);
        _model->set_psi(model_psi);
        _model_inc_x = model_x / varooom_model_anim_frames;
        _announcer.play_title_voice();
        return;
    }

    point_3d position = _model->position();

    if(position.y() > model_target_y)
    {
        position.set_x(position.x() + _model_inc_x);
        position.set_y(position.y() + varooom_model_inc_y);
        position.set_z(position.z() + varooom_model_inc_z);
        _model->set_position(position);
        _model->set_phi(_model->phi() + varooom_model_inc_phi);
        return;
    }

    _models.destroy_dynamic_model(*_model);
    _model = nullptr;

    _front_backdrop_bg->set_map(bn::regular_bg_items::title_backdrop_2.create_map());
    _intro_scene_started = false;
    ++_intro_index;
}

void title_scene::_update_intro_3()
{
    if(! _model)
    {
        _model = &_models.create_dynamic_model(model_3d_items::character_a);
        _model->set_position(point_3d(0, model_source_y, model_source_z));
        _model->set_phi(model_source_phi);
        _model->set_psi(model_psi);
        _model_inc_x = (model_x + (model_inc_x * 1)) / varooom_model_anim_frames;
        return;
    }

    point_3d position = _model->position();

    if(position.y() > model_target_y)
    {
        position.set_x(position.x() + _model_inc_x);
        position.set_y(position.y() + varooom_model_inc_y);
        position.set_z(position.z() + varooom_model_inc_z);
        _model->set_position(position);
        _model->set_phi(_model->phi() + varooom_model_inc_phi);
        return;
    }

    _models.destroy_dynamic_model(*_model);
    _model = nullptr;

    _front_backdrop_bg->set_map(bn::regular_bg_items::title_backdrop_3.create_map());
    _intro_scene_started = false;
    ++_intro_index;
}

void title_scene::_update_intro_4()
{
    if(! _model)
    {
        _model = &_models.create_dynamic_model(model_3d_items::character_r);
        _model->set_position(point_3d(0, model_source_y, model_source_z));
        _model->set_phi(model_source_phi);
        _model->set_psi(model_psi);
        _model_inc_x = (model_x + (model_inc_x * 2)) / varooom_model_anim_frames;
        return;
    }

    point_3d position = _model->position();

    if(position.y() > model_target_y)
    {
        position.set_x(position.x() + _model_inc_x);
        position.set_y(position.y() + varooom_model_inc_y);
        position.set_z(position.z() + varooom_model_inc_z);
        _model->set_position(position);
        _model->set_phi(_model->phi() + varooom_model_inc_phi);
        return;
    }

    _models.destroy_dynamic_model(*_model);
    _model = nullptr;

    _front_backdrop_bg->set_map(bn::regular_bg_items::title_backdrop_4.create_map());
    _intro_scene_started = false;
    ++_intro_index;
}

void title_scene::_update_intro_5()
{
    if(! _model)
    {
        _model = &_models.create_dynamic_model(model_3d_items::character_o_full);
        _model->set_position(point_3d(0, model_source_y, model_source_z));
        _model->set_phi(model_source_phi);
        _model->set_psi(model_psi);
        _model_inc_x = (model_x + (model_inc_x * 3)) / varooom_model_anim_frames;
        return;
    }

    point_3d position = _model->position();

    if(position.y() > model_target_y)
    {
        position.set_x(position.x() + _model_inc_x);
        position.set_y(position.y() + varooom_model_inc_y);
        position.set_z(position.z() + varooom_model_inc_z);
        _model->set_position(position);
        _model->set_phi(_model->phi() + varooom_model_inc_phi);
        return;
    }

    _models.destroy_dynamic_model(*_model);
    _model = nullptr;

    _front_backdrop_bg->set_map(bn::regular_bg_items::title_backdrop_5.create_map());
    _intro_scene_started = false;
    ++_intro_index;
}

void title_scene::_update_intro_6()
{
    if(! _model)
    {
        _model = &_models.create_dynamic_model(model_3d_items::character_o_full);
        _model->set_position(point_3d(0, model_source_y, model_source_z));
        _model->set_phi(model_source_phi);
        _model->set_psi(model_psi);
        _model_inc_x = (model_x + (model_inc_x * 4)) / varooom_model_anim_frames;
        return;
    }

    point_3d position = _model->position();

    if(position.y() > model_target_y)
    {
        position.set_x(position.x() + _model_inc_x);
        position.set_y(position.y() + varooom_model_inc_y);
        position.set_z(position.z() + varooom_model_inc_z);
        _model->set_position(position);
        _model->set_phi(_model->phi() + varooom_model_inc_phi);
        return;
    }

    _models.destroy_dynamic_model(*_model);
    _model = nullptr;

    _front_backdrop_bg->set_map(bn::regular_bg_items::title_backdrop_6.create_map());
    _intro_scene_started = false;
    ++_intro_index;
}

void title_scene::_update_intro_7()
{
    if(! _model)
    {
        _model = &_models.create_dynamic_model(model_3d_items::character_o_full);
        _model->set_position(point_3d(0, model_source_y, model_source_z));
        _model->set_phi(model_source_phi);
        _model->set_psi(model_psi);
        _model_inc_x = (model_x + (model_inc_x * 5)) / varooom_model_anim_frames;
        return;
    }

    point_3d position = _model->position();

    if(position.y() > model_target_y)
    {
        position.set_x(position.x() + _model_inc_x);
        position.set_y(position.y() + varooom_model_inc_y);
        position.set_z(position.z() + varooom_model_inc_z);
        _model->set_position(position);
        _model->set_phi(_model->phi() + varooom_model_inc_phi);
        return;
    }

    _models.destroy_dynamic_model(*_model);
    _model = nullptr;

    _front_backdrop_bg->set_map(bn::regular_bg_items::title_backdrop_7.create_map());
    _intro_scene_started = false;
    ++_intro_index;
}

void title_scene::_update_intro_8()
{
    if(! _model)
    {
        _model = &_models.create_dynamic_model(model_3d_items::character_m);
        _model->set_position(point_3d(0, model_source_y, model_source_z));
        _model->set_phi(model_source_phi);
        _model->set_psi(model_psi);
        _model_inc_x = (model_x + (model_inc_x * 6)) / varooom_model_anim_frames;
        return;
    }

    point_3d position = _model->position();

    if(position.y() > model_target_y)
    {
        position.set_x(position.x() + _model_inc_x);
        position.set_y(position.y() + varooom_model_inc_y);
        position.set_z(position.z() + varooom_model_inc_z);
        _model->set_position(position);
        _model->set_phi(_model->phi() + varooom_model_inc_phi);
        return;
    }

    _models.destroy_dynamic_model(*_model);
    _model = nullptr;

    _front_backdrop_bg->set_map(bn::regular_bg_items::title_backdrop_8.create_map());
    _intro_scene_started = false;
    ++_intro_index;
}

void title_scene::_update_intro_9()
{
    if(! _model)
    {
        _model = &_models.create_dynamic_model(model_3d_items::character_3_red);
        _model->set_position(point_3d(0, model_source_y, model_source_z));
        _model->set_phi(model_source_phi);
        _model->set_psi(model_psi);
        _model_inc_x = (model_x + (model_inc_x * 6) + 30) / three_d_model_anim_frames;
        return;
    }

    point_3d position = _model->position();

    if(position.y() > model_target_y)
    {
        position.set_x(position.x() + _model_inc_x);
        position.set_y(position.y() + three_d_model_inc_y);
        position.set_z(position.z() + three_d_model_inc_z);
        _model->set_position(position);
        _model->set_phi(_model->phi() + three_d_model_inc_phi);
        return;
    }

    _models.destroy_dynamic_model(*_model);
    _model = nullptr;

    _front_backdrop_bg->set_map(bn::regular_bg_items::title_backdrop_9.create_map());
    _intro_scene_started = false;
    ++_intro_index;
}

void title_scene::_update_intro_10()
{
    if(! _model)
    {
        _model = &_models.create_dynamic_model(model_3d_items::character_d_red);
        _model->set_position(point_3d(0, model_source_y, model_source_z));
        _model->set_phi(model_source_phi);
        _model->set_psi(model_psi);
        _model_inc_x = (model_x + (model_inc_x * 7) + 31) / three_d_model_anim_frames;
        return;
    }

    point_3d position = _model->position();

    if(position.y() > model_target_y)
    {
        position.set_x(position.x() + _model_inc_x);
        position.set_y(position.y() + three_d_model_inc_y);
        position.set_z(position.z() + three_d_model_inc_z);
        _model->set_position(position);
        _model->set_phi(_model->phi() + three_d_model_inc_phi);
        return;
    }

    _models.destroy_dynamic_model(*_model);
    _model = nullptr;

    _front_backdrop_bg = bn::regular_bg_items::title_backdrop_10.create_bg(
                (512 - bn::display::width()) / 2, (256 - bn::display::height()) / 2);
    _front_backdrop_bg->set_priority(2);

    ++_intro_index;
}

void title_scene::_update_intro_11()
{
    if(! _flag.visible())
    {
        _flag.show();
        _wait_frames = 10;
        return;
    }

    if(_wait_frames)
    {
        --_wait_frames;
        return;
    }

    _base_menu.set_visible(true, true);
    _advices.set_visible(true);
    _intro_done = true;
}

void title_scene::_exit()
{
    bn::bg_palettes::set_fade_color(bn::colors::black);
    bn::sprite_palettes::set_fade_color(bn::colors::black);
    _bgs_fade_out_action.emplace(fade_frames, 1);
    _sprites_fade_out_action.emplace(fade_frames, 1);
    _music_volume_action.emplace(fade_frames, 0);
}

}
