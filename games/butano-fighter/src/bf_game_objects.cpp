#include "bf_game_objects.h"

#include "btn_sound.h"
#include "btn_fixed_rect.h"
#include "btn_sound_items.h"
#include "btn_gem_sprite_item.h"

namespace bf::game
{

objects::objects(const btn::sprite_palette_ptr& flash_palette) :
    _flash_palette(flash_palette),
    _gem_palette(btn::sprite_items::gem.palette_item().create_palette()),
    _gem_tiles({ btn::sprite_items::gem.tiles_item().create_tiles(0),
               btn::sprite_items::gem.tiles_item().create_tiles(1),
               btn::sprite_items::gem.tiles_item().create_tiles(2),
               btn::sprite_items::gem.tiles_item().create_tiles(3) })
{
}

bool objects::check_hero_weapon(const btn::fixed_rect& hero_rect)
{
    if(_hero_weapon)
    {
        if(_hero_weapon->intersects_hero(hero_rect))
        {
            if(_messages.full())
            {
                _messages.pop_front();
            }

            _messages.push_back(object_message::create_level_up(hero_rect.position()));
            _hero_weapon.reset();
            btn::sound::play(btn::sound_items::reload);
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
                if(_messages.full())
                {
                    _messages.pop_front();
                }

                _messages.push_back(object_message::create_bomb(hero_rect.position()));
                btn::sound::play(btn::sound_items::reload);
            }

            return true;
        }
    }

    return false;
}

int objects::check_gem(const btn::fixed_rect& hero_rect, int hero_level)
{
    for(auto gems_it = _gems.begin(), gems_end = _gems.end(); gems_it != gems_end; ++gems_it)
    {
        if(gems_it->intersects_hero(hero_rect))
        {
            _gems.erase(gems_it);
            btn::sound::play(btn::sound_items::gold_3);
            return hero_level + 1;
        }
    }

    return 0;
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

void objects::spawn_gem(const btn::fixed_point& position)
{
    if(! _gems.full())
    {
        _gems.emplace_back(position, _gem_tiles, _gem_palette);
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

    int gems_count = _gems.size();
    int messages_count = _messages.size();

    for(int index = 0; index < gems_count; )
    {
        gem& gem = _gems[index];
        gem.update();

        if(gem.done())
        {
            if(index < gems_count - 1)
            {
                btn::swap(gem, _gems[gems_count - 1]);
            }

            --gems_count;
        }
        else
        {
            ++index;
        }
    }

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

    _gems.shrink(gems_count);
    _messages.shrink(messages_count);
}

}
