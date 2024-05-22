/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_player_car.h"

#include "bn_music.h"
#include "bn_keypad.h"
#include "bn_rumble.h"
#include "bn_bgs_mosaic.h"
#include "bn_green_swap.h"
#include "bn_affine_bg_item.h"
#include "bn_sprites_mosaic.h"

#include "fr_stage.h"
#include "fr_sin_cos.h"
#include "fr_models_3d.h"
#include "fr_camera_3d.h"
#include "fr_announcer.h"
#include "fr_race_state.h"
#include "fr_background_3d.h"

#include "bn_sound_items.h"

// #include "bn_log.h"

namespace fr
{

namespace
{
    constexpr bn::fixed car_y = 5;
    constexpr bn::fixed turbo_y = car_y + 1;
    constexpr bn::fixed explosion_y = 20;

    constexpr bn::fixed pi_2(3.14159265358979 + 3.14159265358979);

    constexpr bn::fixed turn_speed_base = 0.0125;
    constexpr bn::fixed turn_speed_inc = 0.025 - turn_speed_base;
    constexpr bn::fixed turn_power_reduction_factor = 0.75;

    constexpr bn::fixed fast_power_drag = 0.95;
    constexpr bn::fixed slow_power_drag = 0.91;
    constexpr bn::fixed fast_angular_drag = 0.95;
    constexpr bn::fixed slow_angular_drag = 0.875;

    constexpr bn::fixed turbo_energy_inc = bn::fixed::from_data(9);
    constexpr bn::fixed turbo_energy_dec = bn::fixed::from_data(8);
    constexpr bn::fixed turbo_energy_length = 0.15;

    constexpr int turbo_allowed_rumble_frames = 8;
    constexpr int turbo_enabled_rumble_frames = 30;

    constexpr int total_bump_frames = 64;
    constexpr bn::fixed bump_penalization_factor = 0.36;

    constexpr int fade_crash_frames = 64;
    constexpr int static_crash_frames = 64;
    constexpr int total_crash_frames = fade_crash_frames + static_crash_frames;

    constexpr bn::color fade_color(28, 14, 2);
}

player_car::player_car(const stage& stage, const race_state& state, models_3d& models, camera_3d& camera,
                       announcer& announcer, background_3d& background) :
    _models(models),
    _car_sprite_3d_item(stage.player_car_sprite_item(), 0),
    _turbo_explosion_sprite_3d_item(stage.player_car_sprite_item(), 1),
    _car_sprite(&models.create_sprite(_car_sprite_3d_item)),
    _angle(stage.start_angle())
{
    bn::sprite_palette_ptr& palette = _car_sprite_3d_item.palette();
    palette.set_fade_color(fade_color);

    const point_3d& start_position = stage.start_position();
    _car_sprite->set_position(point_3d(start_position.x(), car_y, start_position.z()));
    update(stage, state, false, false, announcer, background);
    update_camera(stage, false, camera);
}

player_car::~player_car()
{
    if(_car_sprite)
    {
        _models.destroy_sprite(*_car_sprite);
        _car_sprite = nullptr;
    }

    if(_turbo_sprite)
    {
        _models.destroy_sprite(*_turbo_sprite);
        _turbo_sprite = nullptr;
    }

    if(_explosion_sprite)
    {
        _models.destroy_sprite(*_explosion_sprite);
        _explosion_sprite = nullptr;
    }

    bn::green_swap::set_enabled(false);
    bn::rumble::set_enabled(false);
    bn::sprites_mosaic::set_stretch(0);
    bn::bgs_mosaic::set_stretch(0);
}

const point_3d& player_car::position() const
{
    return _car_sprite->position();
}

bn::array<point_3d, 5> player_car::collision_points() const
{
    return { _car_sprite->transform(vertex_3d(6.0, 0.0, -14.0)),
                _car_sprite->transform(vertex_3d(0.0, 0.0, -14.0)),
                _car_sprite->transform(vertex_3d(-6.0, 0.0, -14.0)),

                _car_sprite->transform(vertex_3d(6.0, 0.0, 14.0)),
                _car_sprite->transform(vertex_3d(-6.0, 0.0, 14.0)) };
}

void player_car::bump()
{
    _bump_frames = total_bump_frames;
    _rumble_frames = 16;

    _x_velocity = _x_velocity.unsafe_multiplication(bump_penalization_factor);
    _y_velocity = _y_velocity.unsafe_multiplication(bump_penalization_factor);
    _power = _power.unsafe_multiplication(bump_penalization_factor);
    _reverse = _reverse.unsafe_multiplication(bump_penalization_factor);
    _angular_velocity = _angular_velocity.unsafe_multiplication(bump_penalization_factor);

    _stop_turbo();
}

void player_car::crash()
{
    _bump_frames = total_bump_frames;
    _crash_frames = total_crash_frames;
}

void player_car::set_win_state(int win_state)
{
    _stop_turbo();

    if(_explosion_sprite)
    {
        _models.destroy_sprite(*_explosion_sprite);
        _explosion_sprite = nullptr;
    }

    bn::rumble::set_enabled(false);
    _win_state = win_state;
}

void player_car::update(const stage& stage, const race_state& state, bool read_keypad, bool rumble_allowed,
                        announcer& announcer, background_3d& background)
{
    if(_crash_frames)
    {
        _update_crash(stage, state, background);
        read_keypad = false;
    }
    else if(_bump_frames)
    {
        --_bump_frames;
    }

    const bn::affine_bg_map_cell* map_cells_ptr = &stage.ground_bg_item().map_item().cells_ref();
    const point_3d& position = _car_sprite->position();
    int tile_x = position.x().data() >> 15;
    int tile_y = position.z().data() >> 15;
    int tile_index = map_cells_ptr[(tile_y << 7) + tile_x];
    bool fast_tile = tile_index != stage.slow_ground_tile_index();
    bool engine_enabled = read_keypad && state.pending_time() > 0;
    bn::fixed turbo_energy = _turbo_energy;
    bn::fixed power_drag = fast_tile ? fast_power_drag : slow_power_drag;
    bn::fixed angular_drag = fast_tile ? fast_angular_drag : slow_angular_drag;
    bn::fixed power;
    bn::fixed reverse;

    if(read_keypad && turbo_energy == 1 && bn::keypad::r_pressed())
    {
        turbo_energy = -turbo_energy_length;
        _rumble_frames = turbo_enabled_rumble_frames;

        bn::sprite_palette_ptr& palette = _car_sprite_3d_item.palette();
        palette.set_fade_intensity(1);
        _palette_action.emplace(palette, turbo_enabled_rumble_frames, bn::fixed(0));

        bn::sound_items::turbo2.play();
        announcer.play_boost_used_voice(45);
    }

    const car_engine& engine = stage.player_car_engine();
    bn::fixed max_power = engine.max_power();
    bool turbo_enabled = turbo_energy < 0;

    if(turbo_enabled)
    {
        power = bn::min(_power + engine.turbo_acceleration(), engine.max_turbo_power());
        turbo_energy += turbo_energy_dec;

        if(turbo_energy > 0)
        {
            turbo_energy = 0;
        }
    }
    else
    {
        if(engine_enabled && bn::keypad::a_held())
        {
            power = bn::min(_power + engine.acceleration(), max_power);

            if(fast_tile)
            {
                if(turbo_energy < 1)
                {
                    turbo_energy += turbo_energy_inc;

                    if(turbo_energy >= 1)
                    {
                        turbo_energy = 1;
                        _rumble_frames = turbo_allowed_rumble_frames;
                        bn::sound_items::nitro03.play(0.6);
                        announcer.play_boost_charged_voice(55);
                    }
                }
            }
        }
        else
        {
            power = bn::max(_power - engine.acceleration(), bn::fixed(0));
        }
    }

    if(engine_enabled && bn::keypad::b_held())
    {
        reverse = bn::min(_reverse + engine.reverse_acceleration(), engine.max_reverse_power());
    }
    else
    {
        reverse = bn::max(_reverse - engine.reverse_acceleration(), bn::fixed(0));
    }

    bn::fixed power_minus_reverse = power - reverse;
    _power = power;
    _reverse = reverse;
    _turbo_energy = turbo_energy;

    bn::fixed angular_velocity = _angular_velocity;

    if(read_keypad)
    {
        bn::fixed power_factor = bn::abs(power_minus_reverse).unsafe_division(max_power);

        if(power_factor > 1)
        {
            power_factor = 1;
        }

        bn::fixed turn_factor = 1 - power_factor;
        bn::fixed turn_speed = turn_factor.unsafe_multiplication(turn_speed_inc);
        turn_speed += turn_speed_base;

        if(bn::keypad::left_held())
        {
            angular_velocity -= power_minus_reverse.unsafe_multiplication(turn_speed);
        }
        else if(bn::keypad::right_held())
        {
            angular_velocity += power_minus_reverse.unsafe_multiplication(turn_speed);
        }
    }

    bn::fixed angle = _angle + angular_velocity;
    _angular_velocity = angular_velocity.unsafe_multiplication(angular_drag);

    if(angle < 0)
    {
        angle += pi_2;
    }
    else if(angle >= pi_2)
    {
        angle -= pi_2;
    }

    constexpr bn::rule_of_three_approximation angle_16_rule_of_three(pi_2.data(), 65536);
    int angle_16_data = angle_16_rule_of_three.calculate(angle.data());
    bn::fixed angle_sin = sin(angle_16_data);
    bn::fixed angle_cos = cos(angle_16_data);
    _angle = angle;
    _angle_sin = angle_sin;
    _angle_cos = angle_cos;

    bn::fixed acceleration = power_minus_reverse.unsafe_multiplication(
                1 - bn::abs(angular_velocity.unsafe_multiplication(turn_power_reduction_factor)));
    bn::fixed x_velocity = _x_velocity + angle_sin.unsafe_multiplication(acceleration);
    bn::fixed y_velocity = _y_velocity + angle_cos.unsafe_multiplication(acceleration);
    _x_velocity = x_velocity.unsafe_multiplication(power_drag);
    _y_velocity = y_velocity.unsafe_multiplication(power_drag);

    point_3d car_position(position.x() + x_velocity, car_y, position.z() - y_velocity);
    _car_sprite->set_position(car_position);
    _car_sprite->set_theta(-angle_16_data);

    _update_turbo_gfx(car_position, turbo_enabled);

    if(bn::music::playing())
    {
        bn::fixed music_volume = bn::music::volume();
        bn::fixed max_music_volume = bn::fixed::from_data(constants_3d::music_volume_data);

        if(music_volume < max_music_volume)
        {
            music_volume = bn::min(music_volume + bn::fixed(0.01), max_music_volume);
            bn::music::set_volume(music_volume);
        }
    }

    if(_rumble_frames)
    {
        --_rumble_frames;
        bn::rumble::set_enabled(rumble_allowed);
    }
    else
    {
        bn::rumble::set_enabled(false);
    }

    // BN_LOG("checkpoint(", car_position.x().right_shift_integer(), ", ",
    //        car_position.z().right_shift_integer(), ", ", angle, "),");
}

void player_car::update_camera(const stage& stage, bool add_delay, camera_3d& camera)
{
    bn::fixed power_minus_reverse;

    if(int crash_frames = _crash_frames)
    {
        crash_frames = total_crash_frames - crash_frames;
        power_minus_reverse = bn::fixed(-crash_frames) / (total_crash_frames * 4);
    }
    else
    {
        power_minus_reverse = _power - _reverse;

        if(power_minus_reverse < 0)
        {
            power_minus_reverse = 0;
        }
    }

    bn::fixed target_phi = -_car_sprite->theta();
    bn::fixed target_y;
    bn::fixed x_desp;
    bn::fixed z_desp;

    switch(_win_state)
    {

    case 0:
        {
            bn::fixed max_turbo_power = stage.player_car_engine().max_turbo_power();
            bn::fixed relative_power = power_minus_reverse.unsafe_division(max_turbo_power);
            bn::fixed target_distance = 48 + (relative_power * 15);
            target_y = constants_3d::camera_min_y + (relative_power * constants_3d::camera_diff_y);
            x_desp = _angle_sin.unsafe_multiplication(target_distance);
            z_desp = -_angle_cos.unsafe_multiplication(target_distance);
        }
        break;

    case 1:
        target_y = 220;
        x_desp = -_angle_sin.unsafe_multiplication(15);
        z_desp = _angle_cos.unsafe_multiplication(15);
        add_delay = false;
        break;

    case 2:
        target_phi += 12000;
        target_y = 204;
        x_desp = -_angle_sin.unsafe_multiplication(19);
        z_desp = _angle_cos.unsafe_multiplication(19);
        add_delay = false;
        break;

    case 3:
        target_phi += 26000;
        target_y = 188;
        x_desp = -_angle_sin.unsafe_multiplication(19);
        z_desp = _angle_cos.unsafe_multiplication(19);
        add_delay = false;
        break;

    case 4:
        target_phi += 44000;
        target_y = 172;
        x_desp = -_angle_sin.unsafe_multiplication(19);
        z_desp = _angle_cos.unsafe_multiplication(19);
        add_delay = false;
        break;

    default:
        BN_ERROR("Invalid win state: ", _win_state);
        break;
    }

    const point_3d& model_position = _car_sprite->position();
    point_3d target_position(model_position.x() + x_desp, target_y, model_position.z() + z_desp);

    if(add_delay)
    {
        point_3d camera_position = camera.position();
        camera.set_position(camera_position + ((target_position - camera_position) / 4));

        if(target_phi < 0)
        {
            target_phi += 0xFFFF;
        }

        bn::fixed camera_phi = camera.phi();
        bn::fixed phi_diff = target_phi - camera_phi;

        if(phi_diff >= 32768)
        {
            phi_diff = phi_diff - 0xFFFF;
        }
        else if(phi_diff <= -32768)
        {
            phi_diff = 0xFFFF + phi_diff;
        }

        camera.set_phi(camera_phi + (phi_diff / 8));
    }
    else
    {
        camera.set_position(target_position);
        camera.set_phi(target_phi);
    }
}

void player_car::_update_turbo_gfx(const point_3d& car_position, bool turbo_enabled)
{
    if(turbo_enabled && ! _turbo_sprite)
    {
        point_3d turbo_position(car_position.x() - (_angle_sin * 14), turbo_y, car_position.z() + (_angle_cos * 14));
        bn::fixed turbo_scale = bn::fixed::from_data(1) - _turbo_energy.unsafe_division(turbo_energy_length);
        bn::fixed turbo_explosion_angle = _turbo_explosion_angle + 4000;

        if(turbo_explosion_angle >= 65536)
        {
            turbo_explosion_angle -= 65536;
        }

        _turbo_sprite = &_models.create_sprite(_turbo_explosion_sprite_3d_item);
        _turbo_sprite->set_position(turbo_position);
        _turbo_sprite->set_theta(turbo_explosion_angle);
        _turbo_explosion_angle = turbo_explosion_angle;

        bn::green_swap::set_enabled(turbo_scale > 0.7);

        if(turbo_scale < 0.5)
        {
            _turbo_sprite->set_scale(turbo_scale * 2);
        }
    }
    else if(_turbo_sprite)
    {
        _models.destroy_sprite(*_turbo_sprite);
        _turbo_sprite = nullptr;
    }

    if(_palette_action)
    {
        _palette_action->update();

        if(_palette_action->done())
        {
            _palette_action.reset();
        }
    }
}

void player_car::_stop_turbo()
{
    if(_turbo_energy < 0)
    {
        _turbo_energy = 0;
    }

    _turbo_explosion_angle = 0;

    if(_palette_action)
    {
        bn::sprite_palette_ptr& palette = _car_sprite_3d_item.palette();
        palette.set_fade_intensity(0);
        _palette_action.reset();
    }

    if(_turbo_sprite)
    {
        _models.destroy_sprite(*_turbo_sprite);
        _turbo_sprite = nullptr;
    }

    bn::green_swap::set_enabled(false);
}

void player_car::_update_crash(const stage& stage, const race_state& state, background_3d& background)
{
    int crash_frames = _crash_frames;

    if(crash_frames == total_crash_frames)
    {
        _start_crash(stage, background);
    }
    else if(crash_frames == 1)
    {
        _end_crash(stage, state);
    }
    else
    {
        _continue_crash(stage, background);
    }
}

void player_car::_start_crash(const stage& stage, background_3d& background)
{
    bn::sprite_palette_ptr& palette = _car_sprite_3d_item.palette();
    palette.set_fade_intensity(1);
    _models.set_fade(fade_color, 1);
    background.set_fade(fade_color, 1);

    const bn::sprite_tiles_item& tiles_item = stage.player_car_sprite_item().tiles_item();
    bn::sprite_tiles_ptr& car_tiles = _car_sprite_3d_item.tiles();
    car_tiles.set_tiles_ref(tiles_item, 2);

    bn::sprite_tiles_ptr& explosion_tiles = _turbo_explosion_sprite_3d_item.tiles();
    explosion_tiles.set_tiles_ref(tiles_item, 3);

    _stop_turbo();

    _x_velocity = 0;
    _y_velocity = 0;
    _power = 0;
    _reverse = 0;
    _angular_velocity = 0;

    _rumble_frames = fade_crash_frames;
    --_crash_frames;

    if(bn::music::playing())
    {
        bn::music::set_volume(bn::fixed::from_data(constants_3d::music_volume_data) / 4);
    }

    bn::sound_items::varoomkaboom5.play();
}

void player_car::_continue_crash(const stage& stage, background_3d& background)
{
    bn::sprite_palette_ptr& palette = _car_sprite_3d_item.palette();
    int crash_frames = _crash_frames - 1;
    _crash_frames = crash_frames;
    crash_frames -= static_crash_frames;

    if(crash_frames > 0)
    {
        bn::fixed fade_intensity = bn::fixed(crash_frames) / fade_crash_frames;
        palette.set_fade_intensity(fade_intensity);
        _models.set_fade(fade_color, fade_intensity);
        background.set_fade(fade_color, fade_intensity);

        if(_explosion_sprite)
        {
            _models.destroy_sprite(*_explosion_sprite);
            _explosion_sprite = nullptr;
        }
        else
        {
            const bn::sprite_tiles_item& tiles_item = stage.player_car_sprite_item().tiles_item();
            bn::sprite_tiles_ptr& explosion_tiles = _turbo_explosion_sprite_3d_item.tiles();
            int explosion_graphics_index = ((fade_crash_frames - crash_frames) * 9) / fade_crash_frames;
            explosion_tiles.set_tiles_ref(tiles_item, explosion_graphics_index + 3);

            _turbo_explosion_angle += 4000;

            if(_turbo_explosion_angle > 65536)
            {
                _turbo_explosion_angle -= 65536;
            }

            point_3d explosion_position = _car_sprite->position();
            explosion_position.set_y(explosion_y);
            _explosion_sprite = &_models.create_sprite(_turbo_explosion_sprite_3d_item);
            _explosion_sprite->set_position(explosion_position);
            _explosion_sprite->set_theta(_turbo_explosion_angle);
            _explosion_sprite->set_scale(1 + ((1 - fade_intensity).unsafe_multiplication(bn::fixed(0.6))));
        }

        bn::fixed mosaic_stretch = bn::fixed(crash_frames - (fade_crash_frames / 2)) / fade_crash_frames;
        mosaic_stretch = bn::max(mosaic_stretch, bn::fixed(0));
        bn::sprites_mosaic::set_stretch(mosaic_stretch);
        bn::bgs_mosaic::set_stretch(mosaic_stretch);

        bool green_swap = fade_intensity >= 0.5 || (fade_intensity < 0.4 && fade_intensity >= 0.3) ||
                (fade_intensity < 0.2 && fade_intensity >= 0.1);
        bn::green_swap::set_enabled(green_swap);

        if(crash_frames == fade_crash_frames - 8)
        {
            bn::sound_items::crashboombang3.play(0.75);
        }
    }
    else
    {
        palette.set_fade_intensity(0);
        _models.set_fade(fade_color, 0);
        background.set_fade(fade_color, 0);

        if(_explosion_sprite)
        {
            _models.destroy_sprite(*_explosion_sprite);
            _explosion_sprite = nullptr;
        }
    }
}

void player_car::_end_crash(const stage& stage, const race_state& state)
{
    _crash_frames = 0;

    const bn::sprite_tiles_item& tiles_item = stage.player_car_sprite_item().tiles_item();
    bn::sprite_tiles_ptr& car_tiles = _car_sprite_3d_item.tiles();
    car_tiles.set_tiles_ref(tiles_item, 0);

    bn::sprite_tiles_ptr& turbo_tiles = _turbo_explosion_sprite_3d_item.tiles();
    turbo_tiles.set_tiles_ref(tiles_item, 1);

    const bn::span<const checkpoint>& checkpoints = stage.player_checkpoints();
    const checkpoint& next_checkpoint = checkpoints[state.next_checkpoint_index()];
    point_3d car_position(next_checkpoint.x(), car_y, next_checkpoint.y());
    bn::fixed angle = next_checkpoint.angle();

    constexpr bn::rule_of_three_approximation angle_16_rule_of_three(pi_2.data(), 65536);
    int angle_16_data = angle_16_rule_of_three.calculate(angle.data());
    bn::fixed angle_sin = sin(angle_16_data);
    bn::fixed angle_cos = cos(angle_16_data);
    _angle = angle;
    _angle_sin = angle_sin;
    _angle_cos = angle_cos;

    _car_sprite->set_position(car_position);
    _car_sprite->set_theta(-angle_16_data);
}

}
