/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_hero.h"

#include "bn_keypad.h"
#include "bn_colors.h"
#include "bn_fixed_rect.h"
#include "bn_sound_items.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_items_hero_death.h"
#include "bn_sprite_items_hero_shield.h"
#include "bn_sprite_items_hero_weapons.h"
#include "bn_sprite_items_hero_bomb_icon.h"
#include "bn_sprite_items_hero_body_flying.h"
#include "bn_sprite_items_hero_body_walking.h"
#include "bf_scene_type.h"
#include "bf_game_enemies.h"
#include "bf_game_objects.h"
#include "bf_game_hero_bomb.h"
#include "bf_game_background.h"
#include "bf_butano_background.h"
#include "bf_game_enemy_bullets.h"
#include "bf_game_rumble_manager.h"
#include "bf_game_hero_bullet_level.h"

namespace bf::game
{

namespace
{
    constexpr int body_delta_y = 48;
    constexpr int weapon_delta_x = 2;
    constexpr int weapon_delta_y = -13;
    constexpr int shoot_frames = 5;
    constexpr int scale_weapon_frames = 30;
    constexpr int scale_weapon_half_frames = scale_weapon_frames / 2;
    constexpr int body_shadows_multiplier = 4;
    constexpr bn::fixed_size dimensions(12, 12);

    bn::vector<bn::sprite_ptr, 3> _create_body_shadows(const bn::sprite_item& body_sprite_item,
                                                         const bn::camera_ptr& camera)
    {
        bn::vector<bn::sprite_ptr, 3> result;

        for(int index = 0; index < 3; ++index)
        {
            bn::sprite_builder builder(body_sprite_item);
            builder.set_z_order(constants::hero_shadows_z_order);
            builder.set_camera(camera);
            result.push_back(builder.release_build());
        }

        return result;
    }

    bn::sprite_cached_animate_action<2> _create_body_sprite_animate_action(const bn::sprite_item& body_sprite_item,
                                                                            const bn::camera_ptr& camera)
    {
        bn::sprite_builder builder(body_sprite_item);
        builder.set_position(0, body_delta_y);
        builder.set_camera(camera);

        return bn::create_sprite_cached_animate_action_forever(builder.release_build(), 16,
                                                                body_sprite_item.tiles_item(), 0, 1);
    }

    bn::sprite_ptr _create_weapon_sprite(int level, const bn::fixed_point& position, const bn::camera_ptr& camera)
    {
        bn::sprite_builder builder(bn::sprite_items::hero_weapons, level);
        builder.set_position(position);
        builder.set_camera(camera);
        return builder.release_build();
    }

    bn::sprite_ptr _create_shield_sprite(const bn::camera_ptr& camera)
    {
        bn::sprite_builder builder(bn::sprite_items::hero_shield);
        builder.set_z_order(constants::hero_shield_z_order);
        builder.set_camera(camera);
        builder.set_visible(false);
        return builder.release_build();
    }
}

hero::hero(const bn::camera_ptr& camera, status& status) :
    _status(status),
    _body_sprite_item(status.current_stage().in_air ?
                          bn::sprite_items::hero_body_flying : bn::sprite_items::hero_body_walking),
    _body_shadows(_create_body_shadows(_body_sprite_item, camera)),
    _body_sprite_animate_action(_create_body_sprite_animate_action(_body_sprite_item, camera)),
    _body_snapshots(body_snapshots_count, body_snapshot_type{ _body_sprite_animate_action.sprite().position(), 0 }),
    _body_position(0, body_delta_y),
    _weapon_position(weapon_delta_x, body_delta_y + weapon_delta_y),
    _weapon_sprite(_create_weapon_sprite(status.level(), _weapon_position, camera)),
    _shield_sprite(_create_shield_sprite(camera)),
    _bomb_sprites_affine_mat(bn::sprite_affine_mat_ptr::create())
{
}

void hero::show_shoot(bn::color fade_color)
{
    bn::sprite_palette_ptr body_palette = _body_sprite_animate_action.sprite().palette();
    body_palette.set_fade(fade_color, 0.5);
    _body_palette_fade_action.emplace(bn::move(body_palette), shoot_frames, 0);

    bn::sprite_palette_ptr weapon_palette = _weapon_sprite.palette();
    weapon_palette.set_fade(fade_color, 0.75);
    _weapon_palette_fade_action.emplace(bn::move(weapon_palette), shoot_frames, 0);

    if(! _show_shoot_counter)
    {
        _show_shoot_counter = shoot_frames * 2;
    }
}

bn::optional<scene_type> hero::update(const hero_bomb& hero_bomb, const enemies& enemies,
                                      enemy_bullets& enemy_bullets, objects& objects, background& background,
                                      butano_background& butano_background, bn::camera_ptr& camera,
                                      rumble_manager& rumble_manager)
{
    bn::optional<scene_type> result;

    if(alive())
    {
        bn::fixed_point old_body_position = _body_position;
        bool looking_down = _looking_down;
        _looking_down = enemies.hero_should_look_down(old_body_position, looking_down);
        _move(camera);

        bn::fixed_rect new_body_rect(_body_position, dimensions);
        _animate_alive(old_body_position);

        if(objects.check_hero_weapon(new_body_rect, camera))
        {
            [[maybe_unused]] bool level_added = _status.add_level();
            BN_ASSERT(level_added, "Level add failed");

            _scale_weapon_counter = scale_weapon_frames;
            _weapon_sprite.set_item(bn::sprite_items::hero_weapons, _status.level());
            _weapon_sprite.set_scale(2);
        }

        bool max_bombs_count = _status.bombs_count() == constants::max_hero_bombs;
        int level = _status.level();

        objects::bomb_check_result bomb_check_result =
                objects.check_hero_bomb(new_body_rect, max_bombs_count, level, camera);

        if(bomb_check_result.add_bomb)
        {
            [[maybe_unused]] bool bomb_added = _status.add_bomb();
            BN_ASSERT(bomb_added, "Bomb add failed");
        }

        int experience_to_add = bomb_check_result.experience_to_add + objects.check_gem(new_body_rect, level, camera);

        if(experience_to_add && add_experience(experience_to_add))
        {
            objects.spawn_hero_weapon_with_sound(bn::fixed_point(0, -constants::view_height), level + 1, camera);
        }

        if(_shield_counter)
        {
            _animate_shield(background, rumble_manager);
        }
        else
        {
            if(! hero_bomb.active())
            {
                if(enemies.check_hero(new_body_rect) || enemy_bullets.check_hero(new_body_rect))
                {
                    int old_bombs_count = _status.bombs_count();
                    rumble_manager.set_enabled(true);

                    if(_status.throw_shield())
                    {
                        _show_shield(old_bombs_count, camera, background);
                    }
                    else
                    {
                        ++_death_counter;
                    }
                }
            }
        }

        if(enemies.boss_dead())
        {
            if(! _stage_done)
            {
                butano_background.show(_weapon_sprite.position(), camera);
                _stage_done = true;
            }
            else if(! butano_background.silhouette_visible())
            {
                if(_status.go_to_next_stage())
                {
                    result = scene_type::GAME;
                }
                else
                {
                    _status.update_high_experience();
                    _status = status();
                    result = scene_type::ENDING;
                }
            }
        }
    }
    else
    {
        result = _animate_dead(camera, background, butano_background, rumble_manager);
        _body_position = _body_sprite_animate_action.sprite().position();
    }

    return result;
}

void hero::_move(bn::camera_ptr& camera)
{
    bn::sprite_ptr body_sprite = _body_sprite_animate_action.sprite();
    bn::fixed speed = _shooting ? 1 : 2;

    if(bn::keypad::left_held())
    {
        bn::fixed sprite_x = bn::max(_body_position.x() - speed, bn::fixed(-constants::play_width));
        body_sprite.set_x(sprite_x);
        _body_position.set_x(sprite_x);

        if(sprite_x < constants::camera_width)
        {
            camera.set_x(bn::max(camera.x() - speed, bn::fixed(-constants::camera_width)));
        }
    }
    else if(bn::keypad::right_held())
    {
        bn::fixed sprite_x = bn::min(_body_position.x() + speed, bn::fixed(constants::play_width));
        body_sprite.set_x(sprite_x);
        _body_position.set_x(sprite_x);

        if(sprite_x > -constants::camera_width)
        {
            camera.set_x(bn::min(camera.x() + speed, bn::fixed(constants::camera_width)));
        }
    }

    if(bn::keypad::up_held())
    {
        bn::fixed sprite_y = bn::max(_body_position.y() - speed, bn::fixed(-constants::play_height));
        body_sprite.set_y(sprite_y);
        _body_position.set_y(sprite_y);
    }
    else if(bn::keypad::down_held())
    {
        bn::fixed sprite_y = bn::min(_body_position.y() + speed, bn::fixed(constants::play_height));
        body_sprite.set_y(sprite_y);
        _body_position.set_y(sprite_y);
    }

    bool looking_down = _looking_down;
    body_sprite.set_horizontal_flip(looking_down);
    body_sprite.set_vertical_flip(looking_down);
}

void hero::_animate_alive(const bn::fixed_point& old_body_position)
{
    int weapon_shift_y = 0;

    if(_show_shoot_counter)
    {
        if(_show_shoot_counter > 5)
        {
            weapon_shift_y = -1;
        }

        --_show_shoot_counter;
    }

    const bn::fixed_point& new_body_position = _body_position;
    bool looking_down = _looking_down;

    if(looking_down)
    {
        _weapon_position = new_body_position + bn::fixed_point(-weapon_delta_x, -weapon_delta_y);
    }
    else
    {
        _weapon_position = new_body_position + bn::fixed_point(weapon_delta_x, weapon_delta_y);
    }

    _weapon_sprite.set_position(_weapon_position + bn::fixed_point(0, weapon_shift_y));
    _weapon_sprite.set_horizontal_flip(looking_down);
    _weapon_sprite.set_vertical_flip(looking_down);

    if(! _shooting && old_body_position != new_body_position)
    {
        _body_sprite_animate_action.update();
    }

    _body_sprite_animate_action.update();

    if(_body_palette_fade_action)
    {
        _body_palette_fade_action->update();
        _weapon_palette_fade_action->update();

        if(_body_palette_fade_action->done())
        {
            _body_palette_fade_action.reset();
            _weapon_palette_fade_action.reset();
        }
    }

    if(_scale_weapon_counter)
    {
        --_scale_weapon_counter;

        if(_scale_weapon_counter)
        {
            if(_scale_weapon_counter <= scale_weapon_half_frames)
            {
                _weapon_sprite.set_scale(1 + (bn::fixed(_scale_weapon_counter) / scale_weapon_half_frames));
            }
        }
        else
        {
            _weapon_sprite.set_scale(1);
        }
    }

    int current_graphics_index = _body_sprite_animate_action.current_index();
    int shadows_count = _body_shadows.size();
    _body_snapshots.pop_back();
    _body_snapshots.push_front(body_snapshot_type{ new_body_position, int16_t(current_graphics_index), looking_down });

    if(_shooting)
    {
        _body_shadows_counter = bn::max(_body_shadows_counter - 1, 0);
    }
    else
    {
        _body_shadows_counter = bn::min(_body_shadows_counter + 1, shadows_count * body_shadows_multiplier);
    }

    const bn::sprite_tiles_item& body_tiles_item = _body_sprite_item.tiles_item();
    int visible_shadows_count = _body_shadows_counter / body_shadows_multiplier;

    for(int index = 0, limit = _body_shadows.size(); index < limit; ++index)
    {
        const body_snapshot_type& body_snapshot = _body_snapshots[(index + 1) * body_shadows_multiplier];
        bn::sprite_ptr& body_shadow = _body_shadows[shadows_count - index - 1];

        if(index >= visible_shadows_count || body_snapshot.position == new_body_position)
        {
            body_shadow.set_visible(false);
        }
        else
        {
            int graphics_index = ((index + 1) * 2) + body_snapshot.graphics_index;
            body_shadow.set_position(body_snapshot.position);
            body_shadow.set_tiles(body_tiles_item.create_tiles(graphics_index));
            body_shadow.set_horizontal_flip(body_snapshot.looking_down);
            body_shadow.set_vertical_flip(body_snapshot.looking_down);
            body_shadow.set_visible(true);
        }
    }
}

void hero::_show_shield(int old_bombs_count, const bn::camera_ptr& camera, background& background)
{
    bool looking_down = _looking_down;
    _shield_toggle_action.emplace(_shield_sprite, 1);
    _shield_rotate_action.emplace(_shield_sprite, 5);
    _shield_counter = 210;

    for(int index = 0; index < old_bombs_count; ++index)
    {
        bn::fixed x = (index % 2) ? bn::fixed(0.5) : bn::fixed(-0.5);
        bn::fixed y = (index / 2) ? bn::fixed(-0.5) : bn::fixed(0.5);
        bn::sprite_builder builder(bn::sprite_items::hero_bomb_icon);
        builder.set_position(_body_position);
        builder.set_z_order(constants::hero_shield_z_order);
        builder.set_affine_mat(_bomb_sprites_affine_mat);
        builder.set_camera(camera);

        if(looking_down)
        {
            _bomb_sprite_move_actions.emplace_back(builder.release_build(), -x, -y);
        }
        else
        {
            _bomb_sprite_move_actions.emplace_back(builder.release_build(), x, y);
        }
    }

    _bomb_sprites_affine_mat.set_rotation_angle(0);
    _bomb_sprites_affine_mat.set_scale(1);
    _bomb_sprites_rotate_action.emplace(_bomb_sprites_affine_mat, 4);
    background.show_hero_dying();
    bn::sound_items::explosion_2.play();
}

void hero::_animate_shield(background& background, rumble_manager& rumble_manager)
{
    --_shield_counter;

    if(int shield_counter = _shield_counter)
    {
        _shield_sprite.set_position(_body_position);
        _shield_toggle_action->update();
        _shield_rotate_action->update();

        if(shield_counter > 150)
        {
            _bomb_sprites_rotate_action->update();

            for(bn::sprite_move_by_action& bomb_sprite_move_action : _bomb_sprite_move_actions)
            {
                bomb_sprite_move_action.update();
            }

            int scale_counter = shield_counter - 150;

            if(scale_counter < 30)
            {
                _bomb_sprites_affine_mat.set_scale(scale_counter * bn::fixed(1.0 / 30));
            }
        }
        else if(shield_counter == 150)
        {
            _bomb_sprites_rotate_action.reset();
            _bomb_sprite_move_actions.clear();
            background.show_hero_alive();
        }
        else if(shield_counter == 135)
        {
            rumble_manager.set_enabled(false);
        }
        else if(shield_counter < 60)
        {
            _shield_sprite.set_scale(shield_counter * bn::fixed(1.0 / 60));
        }

        if(shield_counter % 16 == 0)
        {
            bn::sound_items::flame_thrower.play();
        }
    }
    else
    {
        _shield_sprite.set_scale(1);
        _shield_sprite.set_visible(false);
        _shield_toggle_action.reset();
        _shield_rotate_action.reset();
    }
}

bn::optional<scene_type> hero::_animate_dead(const bn::camera_ptr& camera, background& background,
                                             butano_background& butano_background, rumble_manager& rumble_manager)
{
    bn::sprite_ptr body_sprite = _body_sprite_animate_action.sprite();
    bn::optional<scene_type> result;

    if(_death_counter == 1)
    {
        bn::sprite_palette_ptr body_palette = body_sprite.palette();
        body_palette.set_fade(bn::colors::yellow, 0.75);
        _body_palette_fade_action.emplace(bn::move(body_palette), 30, 0);
        _body_rotate_action.emplace(body_sprite, bn::fixed(0.5));

        bn::sprite_palette_ptr weapon_palette = _weapon_sprite.palette();
        weapon_palette.set_fade(bn::colors::yellow, 0.75);
        _weapon_palette_fade_action.emplace(bn::move(weapon_palette), 30, 0);
        _weapon_sprite.set_scale(1);
        _weapon_move_action.emplace(_weapon_sprite, 70, _weapon_sprite.position() + bn::fixed_point(10, -10));
        _weapon_rotate_action.emplace(_weapon_sprite, -10);

        _body_shadows.clear();
        background.show_hero_dying();
        _music_volume_action.emplace(50, 0);
        bn::sound_items::boss_shoot.play();
    }
    else if(_death_counter == 70)
    {
        bn::fixed_point explosion_position = body_sprite.position() + bn::fixed_point(2, 4);
        _death_explosion.emplace(bn::sprite_items::hero_death, explosion_position, 4, 0, false, camera);
        _weapon_move_action.emplace(_weapon_sprite, 70, _weapon_sprite.position() + bn::fixed_point(5, -5));
        _weapon_rotate_action.emplace(_weapon_sprite, -5);
        background.show_hero_dead();
        bn::sound_items::death.play();
    }
    else if(_death_counter == 100)
    {
        rumble_manager.set_enabled(false);
    }
    else if(_death_counter == 220)
    {
        butano_background.show(_weapon_sprite.position(), camera);
    }
    else if(_death_counter > 220 && ! butano_background.silhouette_visible())
    {
        _status.update_high_experience();
        _status = status();
        result = scene_type::TITLE;
    }

    ++_death_counter;

    if(_death_explosion && ! _death_explosion->done())
    {
        _death_explosion->update();
        body_sprite.set_visible(_death_explosion->show_target_sprite());
    }

    if(body_sprite.visible())
    {
        _body_rotate_action->update();
        body_sprite.set_y(body_sprite.y() + constants::background_speed);

        if(_death_counter % 8 == 0)
        {
            body_sprite.set_x(body_sprite.x() + 2);
        }
        else if(_death_counter % 4 == 0)
        {
            body_sprite.set_x(body_sprite.x() - 2);
        }
    }

    if(_weapon_move_action)
    {
        _weapon_move_action->update();
        _weapon_rotate_action->update();

        if(_weapon_move_action->done())
        {
            _weapon_move_action.reset();
            _weapon_rotate_action.reset();
        }
    }
    else
    {
        bn::fixed weapon_y = _weapon_sprite.y();

        if(weapon_y < constants::view_height)
        {
            _weapon_sprite.set_y(weapon_y + constants::background_speed);
        }
    }

    if(_body_palette_fade_action)
    {
        _body_palette_fade_action->update();
        _weapon_palette_fade_action->update();

        if(_body_palette_fade_action->done())
        {
            _body_palette_fade_action.reset();
            _weapon_palette_fade_action.reset();
        }
    }

    if(_music_volume_action)
    {
        _music_volume_action->update();

        if(_music_volume_action->done())
        {
            _music_volume_action.reset();
        }
    }

    return result;
}

}
