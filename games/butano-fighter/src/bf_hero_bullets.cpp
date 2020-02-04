#include "bf_hero_bullets.h"

#include "btn_sprite_builder.h"
#include "btn_hero_bullets_sprite_item.h"
#include "bf_constants.h"

namespace bf
{

void hero_bullets::add(int level, const btn::fixed_point& position)
{
    BTN_ASSERT(! _sprite_move_actions.full(), "No more space for sprite bullets");

    btn::sprite_builder builder(btn::sprite_items::hero_bullets, level);
    builder.set_position(position);
    builder.set_z_order(constants::hero_bullets_z_order);
    _sprite_move_actions.push_back(btn::sprite_move_by_action(builder.build_and_release(), 0, -2));
}

void hero_bullets::update()
{
    int remove_count = 0;

    for(size_t index = 0, size = _sprite_move_actions.size(); index < size; ++index)
    {
        btn::sprite_move_by_action& sprite_move_action = _sprite_move_actions[index];
        const btn::fixed_point& position = sprite_move_action.sprite().position();

        if(position.x() < -constants::view_width || position.x() > constants::view_width ||
                position.y() < -constants::view_height || position.y() > constants::view_height)
        {
            if(index < size - 1)
            {
                swap(sprite_move_action, _sprite_move_actions[size - 1]);
                --index;
                --size;
            }

            ++remove_count;
        }
        else
        {
            sprite_move_action.update();
        }
    }

    while(remove_count)
    {
        _sprite_move_actions.pop_back();
        --remove_count;
    }
}

}
