/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_BOSS_H
#define BF_GAME_BOSS_H

#include "bn_color.h"
#include "bn_vector.h"
#include "bn_fixed_rect_fwd.h"
#include "bn_unique_ptr_fwd.h"
#include "bn_fixed_point_fwd.h"
#include "bn_sprite_palette_ptr.h"

namespace bn
{
    class camera_ptr;
}

namespace bf::game
{

class hero;
class objects;
class hero_bomb;
class scoreboard;
class background;
class enemy_bullets;
class rumble_manager;
class check_hero_bullet_data;

class boss
{

public:
    enum class type
    {
        TANK,
        GIGABAT,
        WIZARD,
        BUTANO
    };

    static bn::unique_ptr<boss> create(type type, const bn::fixed_point& hero_position,
                                        const bn::sprite_palette_ptr& damage_palette, const bn::camera_ptr& camera);

    virtual ~boss() = default;

    virtual void play_music() const;

    [[nodiscard]] bool check_hero(const bn::fixed_rect& hero_rect) const;

    [[nodiscard]] bool check_hero_bullet(const check_hero_bullet_data& data);

    [[nodiscard]] int life() const
    {
        return _life;
    }

    [[nodiscard]] bool dead() const
    {
        return ! _dead_counter;
    }

    [[nodiscard]] bool hero_should_look_down(const bn::fixed_point& hero_position, bool hero_is_looking_down) const
    {
        return _hero_should_look_down_impl(hero_position, hero_is_looking_down);
    }

    void update(const hero_bomb& hero_bomb, const bn::camera_ptr& camera, hero& hero, enemy_bullets& enemy_bullets,
                objects& objects, scoreboard& scoreboard, background& background, rumble_manager& rumble_manager);

protected:
    boss(int life, int experience, const bn::ivector<bn::fixed_rect>& rects,
         const bn::sprite_palette_ptr& damage_palette);

    [[nodiscard]] virtual bn::fixed_point _position() const = 0;

    virtual void _update_alive(const bn::fixed_point& hero_position, const hero_bomb& hero_bomb,
                               const bn::camera_ptr& camera, enemy_bullets& enemy_bullets) = 0;

    [[nodiscard]] virtual bool _update_dead(const bn::fixed_point& hero_position, const bn::camera_ptr& camera,
                                            background& background, rumble_manager& rumble_manager) = 0;

    virtual void _show_damage_palette(const bn::sprite_palette_ptr& damage_palette, const bn::camera_ptr& camera) = 0;

    virtual void _hide_damage_palette() = 0;

    [[nodiscard]] virtual bool _hero_should_look_down_impl(const bn::fixed_point& hero_position,
                                                           bool hero_is_looking_down) const = 0;

    [[nodiscard]] virtual bool _throw_bomb() const;

    [[nodiscard]] bool _death_flash() const
    {
        return _death_flash_counter > 0;
    }

private:
    const bn::ivector<bn::fixed_rect>& _rects;
    bn::sprite_palette_ptr _damage_palette;
    bn::optional<bn::color> _transparent_color;
    int _life;
    int _experience;
    int _damage_palette_counter;
    int _ignore_hero_bullet_counter;
    int _death_flash_counter;
    int _dead_counter;
    bool _hero_bomb_active;
};

}

#endif
