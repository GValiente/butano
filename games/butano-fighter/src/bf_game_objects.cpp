#include "bf_game_objects.h"

#include "btn_fixed_rect.h"
#include "btn_sound_items.h"
#include "btn_sprite_items_gem.h"

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

            _messages.push_back(object_message::create_level_up(_hero_weapon->position()));
            _hero_weapon.reset();
            btn::sound_items::reload.play();
            return true;
        }
    }

    return false;
}

objects::bomb_check_result objects::check_hero_bomb(const btn::fixed_rect& hero_rect, bool max_hero_bombs,
                                                    int hero_level)
{
    bomb_check_result result;

    if(_hero_bomb)
    {
        if(_hero_bomb->intersects_hero(hero_rect))
        {
            if(_messages.full())
            {
                _messages.pop_front();
            }

            if(max_hero_bombs)
            {
                int experience = _hero_bomb->experience(hero_level);
                result.experience_to_add = experience;
                _messages.push_back(object_message::create_experience(_hero_bomb->position(), experience));
                btn::sound_items::gold_3.play();
            }
            else
            {
                result.add_bomb = true;
                _messages.push_back(object_message::create_bomb(_hero_bomb->position()));
                btn::sound_items::reload.play();
            }

            _hero_bomb.reset();
        }
    }

    return result;
}

int objects::check_gem(const btn::fixed_rect& hero_rect, int hero_level)
{
    auto before_it = _gems.before_begin();
    auto it = _gems.begin();
    auto end = _gems.end();
    int result = 0;

    while(it != end)
    {
        gem& gem = *it;

        if(gem.intersects_hero(hero_rect))
        {
            int experience = gem.experience(hero_level);

            if(_messages.full())
            {
                _messages.pop_front();
            }

            _messages.push_back(object_message::create_experience(gem.position(), experience));
            result += experience;
            it = _gems.erase_after(before_it);
        }
        else
        {
            before_it = it;
            ++it;
        }
    }

    if(result)
    {
        btn::sound_items::gold_3.play();
    }

    return result;
}

void objects::spawn_hero_weapon(const btn::fixed_point& position, int hero_level)
{
    if(! _hero_weapon)
    {
        _hero_weapon = object::create_hero_weapon(position, hero_level, _flash_palette);
        btn::sound_items::power_up_1.play();
    }
}

void objects::spawn_hero_bomb_with_sound(const btn::fixed_point& position)
{
    if(! _hero_bomb)
    {
        spawn_hero_bomb_without_sound(position);
        btn::sound_items::cure.play();
    }
}

void objects::spawn_hero_bomb_without_sound(const btn::fixed_point& position)
{
    if(! _hero_bomb)
    {
        _hero_bomb = object::create_hero_bomb(position, _flash_palette);
    }
}

void objects::spawn_gem(const btn::fixed_point& position)
{
    if(! _gems.full())
    {
        _gems.emplace_front(position, _gem_tiles, _gem_palette);
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

    auto before_it = _gems.before_begin();
    auto it = _gems.begin();
    auto end = _gems.end();

    while(it != end)
    {
        gem& gem = *it;
        gem.update();

        if(gem.done())
        {
            it = _gems.erase_after(before_it);
        }
        else
        {
            before_it = it;
            ++it;
        }
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
