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
class hero_bomb;
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

    [[nodiscard]] bool done() const
    {
        return _done;
    }

    void update(const btn::fixed_point& hero_position, const hero_bomb& hero_bomb, enemy_bullets& enemy_bullets);

protected:
    boss(int life, int experience, const btn::ivector<btn::fixed_rect>& rects,
         const btn::sprite_palette_ptr& damage_palette);

    virtual void _update_alive(const btn::fixed_point& hero_position, enemy_bullets& enemy_bullets) = 0;

    [[nodiscard]] virtual bool _update_dead(const btn::fixed_point& hero_position) = 0;

    virtual void _show_damage_palette(const btn::sprite_palette_ptr& damage_palette) = 0;

    virtual void _hide_damage_palette() = 0;

private:
    const btn::ivector<btn::fixed_rect>& _rects;
    btn::sprite_palette_ptr _damage_palette;
    int _life;
    int _experience;
    int _damage_palette_counter;
    int _ignore_hero_bullet_counter;
    bool _hero_bomb_active;
    bool _done;
};

}

#endif
