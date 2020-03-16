#include "bf_hero_bomb.h"

#include "btn_sound.h"
#include "btn_keypad.h"
#include "btn_regular_bg_builder.h"
#include "btn_hero_bomb_bg_item.h"
#include "btn_sound_items.h"
#include "bf_hero.h"

namespace bf
{

namespace
{
    btn::regular_bg_ptr _create_bg()
    {
        btn::regular_bg_builder builder(btn::bg_items::hero_bomb);
        builder.set_priority(1);
        builder.set_blending_enabled(true);
        builder.set_visible(false);
        return builder.release_build();
    }
}

hero_bomb::hero_bomb() :
    _bg_move_action(_create_bg(), -0.5, 4)
{
}

void hero_bomb::update(hero& hero)
{
    switch(_status)
    {

    case status_type::INACTIVE:
        if(btn::keypad::pressed(btn::keypad::button_type::A) && hero.throw_bomb())
        {
            btn::sound::play(btn::sound_items::explosion_2);
            _status = status_type::ACTIVE;
            _counter = 40;
        }
        break;

    case status_type::ACTIVE:
        _bg_move_action.update();

        if(_counter)
        {
            --_counter;
        }
        else
        {
            _status = status_type::INACTIVE;
        }
        break;
    }
}

}
