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
            btn::sound::play(btn::sound_items::reload);

            if(! _messages.full())
            {
                _messages.push_back(object_message::create_level_up(hero_rect.position()));
            }

            return true;
        }
    }

    return false;
}

bool objects::check_hero_bomb(const btn::fixed_rect& hero_rect, bool max_hero_bombs)
{
    if(_hero_bomb)
    {
        if(_hero_bomb->intersects_hero(hero_rect))
        {
            _hero_bomb.reset();

            if(max_hero_bombs)
            {
                //
            }
            else
            {
                btn::sound::play(btn::sound_items::reload);

                if(! _messages.full())
                {
                    _messages.push_back(object_message::create_bomb(hero_rect.position()));
                }
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
        _hero_weapon = object::create_hero_weapon(position, hero_level, _flash_palette);
        btn::sound::play(btn::sound_items::power_up_1);
    }
}

void objects::spawn_hero_bomb(const btn::fixed_point& position)
{
    if(! _hero_bomb)
    {
        _hero_bomb = object::create_hero_bomb(position, _flash_palette);
        btn::sound::play(btn::sound_items::cure);
    }
}

void objects::update()
{
    if(_hero_weapon)
    {
        _hero_weapon->update();
    }

    if(_hero_bomb)
    {
        _hero_bomb->update();
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
