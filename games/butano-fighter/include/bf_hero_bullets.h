#ifndef BF_HERO_BULLETS_H
#define BF_HERO_BULLETS_H

#include "btn_sound_item.h"
#include "btn_sprite_actions.h"
#include "bf_config.h"

namespace btn
{
    class sound_item;
}

namespace bf
{

class hero;

class hero_bullets
{

public:
    void update(const hero& hero);

private:
    btn::vector<btn::sprite_move_by_action, BTN_MAX_HERO_BULLETS> _sprite_move_actions;
    int _counter = 0;

    void _remove_bullets();

    void _add_bullets(const hero& hero);

    void _add_bullet(int level, const btn::fixed_point& position, const btn::fixed_point& direction,
                     btn::optional<btn::sound_item> sound_item = btn::nullopt);

    void _reset_counter(const hero& hero);
};

}

#endif
