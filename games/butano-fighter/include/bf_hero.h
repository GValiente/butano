#ifndef BF_HERO_H
#define BF_HERO_H

#include "btn_sprite_actions.h"
#include "btn_sprite_palette_actions.h"

namespace bf
{

class hero
{

public:
    hero();

    [[nodiscard]] int level() const
    {
        return _level;
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

    void update();

private:
    int _level = 1;
    btn::sprite_cached_animate_action<2> _body_sprite_animate_action;
    btn::fixed_point _weapon_position;
    btn::sprite_ptr _weapon_sprite;
    btn::optional<btn::sprite_palette_fade_to_action> _body_palette_fade_action;
    btn::optional<btn::sprite_palette_fade_to_action> _weapon_palette_fade_action;
    int _show_shoot_counter = 0;
    bool _is_shooting = false;
};

}

#endif
