#include "bf_game_objects.h"

#include "btn_sound.h"
#include "btn_fixed_rect.h"
#include "btn_sound_items.h"

namespace bf::game
{

objects::objects(const btn::sprite_palette_ptr& flash_palette) :
    _flash_palette(flash_palette)
{
}

bool objects::check_hero_weapon(const btn::fixed_rect& hero_rect)
{
    if(_hero_weapon)
    {
        if(_hero_weapon->intersects_hero(hero_rect))
        {
            _hero_weapon.reset();

            if(! _messages.full())
            {
                _messages.emplace_back(hero_rect.position(), 0);
            }

            return true;
        }
    }

    return false;
}

void objects::spawn_hero_weapon(const btn::fixed_point& position, int hero_level)
{
    if(! _hero_weapon)
    {
        _hero_weapon.emplace(position, hero_level, _flash_palette);
        btn::sound::play(btn::sound_items::power_up_1);
    }
}

void objects::update()
{
    if(_hero_weapon)
    {
        _hero_weapon->update();
    }

    int messages_count = _messages.size();

    for(int index = 0; index < messages_count; )
    {
        object_message& message = _messages[index];
        message.update();

        if(message.done())
        {
            if(index < messages_count - 1)
            {
                btn::swap(message, _messages[messages_count - 1]);
            }

            --messages_count;
        }
        else
        {
            ++index;
        }
    }

    _messages.shrink(messages_count);
}

}
