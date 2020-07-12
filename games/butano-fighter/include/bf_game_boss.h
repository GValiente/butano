#ifndef BF_GAME_BOSS_H
#define BF_GAME_BOSS_H

#include "btn_vector.h"
#include "btn_memory.h"
#include "btn_sprite_palette_ptr.h"

namespace btn
{
    class fixed_rect;
    class fixed_point;
}

namespace bf::game
{

class hero;
class objects;
class hero_bomb;
class scoreboard;
class enemy_bullets;
class check_hero_bullet_data;

class boss
{

public:
    enum class type
    {
        TANK
    };

    static btn::unique_ptr<boss> create(type type, const btn::fixed_point& hero_position,
                                        const btn::sprite_palette_ptr& damage_palette);

    virtual ~boss() = default;

    [[nodiscard]] bool check_hero(const btn::fixed_rect& hero_rect) const;

    [[nodiscard]] bool check_hero_bullet(const check_hero_bullet_data& data);

    [[nodiscard]] int life() const
    {
        return _life;
    }

    [[nodiscard]] bool dead() const
    {
        return _dead;
    }

    [[nodiscard]] bool hero_should_look_down(const btn::fixed_point& hero_position, bool hero_is_looking_down) const
    {
        return _hero_should_look_down_impl(hero_position, hero_is_looking_down);
    }

    void update(const hero_bomb& hero_bomb, hero& hero, enemy_bullets& enemy_bullets, objects& objects,
                scoreboard& scoreboard);

protected:
    boss(int life, int experience, const btn::ivector<btn::fixed_rect>& rects,
         const btn::sprite_palette_ptr& damage_palette);

    [[nodiscard]] virtual btn::fixed_point _position() const = 0;

    virtual void _update_alive(const btn::fixed_point& hero_position, enemy_bullets& enemy_bullets) = 0;

    [[nodiscard]] virtual bool _update_dead(const btn::fixed_point& hero_position) = 0;

    virtual void _show_damage_palette(const btn::sprite_palette_ptr& damage_palette) = 0;

    virtual void _hide_damage_palette() = 0;

    [[nodiscard]] virtual bool _hero_should_look_down_impl(const btn::fixed_point& hero_position,
                                                           bool hero_is_looking_down) const = 0;

    [[nodiscard]] bool _death_flash() const
    {
        return _death_flash_counter > 0;
    }

private:
    const btn::ivector<btn::fixed_rect>& _rects;
    btn::sprite_palette_ptr _damage_palette;
    int _life;
    int _experience;
    int _damage_palette_counter;
    int _ignore_hero_bullet_counter;
    int _death_flash_counter;
    bool _hero_bomb_active;
    bool _dead;
};

}

#endif
