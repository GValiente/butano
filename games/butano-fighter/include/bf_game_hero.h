#ifndef BF_GAME_HERO_H
#define BF_GAME_HERO_H

#include "btn_sprite_actions.h"
#include "btn_sprite_palette_actions.h"

namespace bf::game
{

class objects;

class hero
{

public:
    hero();

    [[nodiscard]] int level() const
    {
        return _level;
    }

    [[nodiscard]] int experience() const
    {
        return _experience;
    }

    [[nodiscard]] btn::fixed next_level_experience_ratio() const;

    [[nodiscard]] bool add_experience(int experience);

    [[nodiscard]] int bombs_count() const
    {
        return _bombs_count;
    }

    [[nodiscard]] const btn::fixed_point& weapon_position() const
    {
        return _weapon_position;
    }

    [[nodiscard]] bool is_shooting() const
    {
        return _is_shooting;
    }

    void set_is_shooting(bool is_shooting)
    {
        _is_shooting = is_shooting;
    }

    void show_shoot(btn::color fade_color);

    [[nodiscard]] bool add_bomb();

    [[nodiscard]] bool throw_bomb();

    void update(objects& objects);

private:
    int _level = 0;
    int _experience = 0;
    int _bombs_count = 2;
    btn::sprite_cached_animate_action<2> _body_sprite_animate_action;
    btn::fixed_point _weapon_position;
    btn::sprite_ptr _weapon_sprite;
    btn::optional<btn::sprite_palette_fade_to_action> _body_palette_fade_action;
    int _show_shoot_counter = 0;
    int _scale_weapon_counter = 0;
    bool _is_shooting = false;

    [[nodiscard]] btn::fixed_point _move(const btn::fixed_point& body_position, btn::sprite_ptr& body_sprite);

    void _animate(const btn::fixed_point& old_body_position, const btn::fixed_point& new_body_position);
};

}

#endif
