#include "bf_game_explosion.h"

#include "btn_sprite_builder.h"
#include "btn_sprite_affine_mat_attributes.h"

namespace bf::game
{

explosion::explosion(const btn::sprite_item& sprite_item, const btn::fixed_point& position, int animation_frames,
                     int z_order, bool double_size) :
    _sprite_item(sprite_item),
    _animation_frames(animation_frames),
    _animation_index(0),
    _frame_counter(animation_frames)
{
    BTN_ASSERT(_sprite_item.tiles_item().graphics_count() > 4 && _sprite_item.tiles_item().graphics_count() % 4 == 0,
               "Invalid graphics count: ", _sprite_item.tiles_item().graphics_count());
    BTN_ASSERT(animation_frames > 0, "Invalid animation frames: ", animation_frames);

    btn::fixed x = position.x();
    btn::fixed y = position.y();

    if(double_size)
    {
        btn::sprite_affine_mat_attributes attributes;
        attributes.set_scale(2);
        _affine_mat = btn::sprite_affine_mat_ptr::create(attributes);
    }

    btn::sprite_builder builder(sprite_item);
    builder.set_z_order(z_order);

    if(double_size)
    {
        builder.set_affine_mat(*_affine_mat);
        builder.set_position(x - 64, y - 64);
    }
    else
    {
        builder.set_position(x - 32, y - 32);
    }

    _sprites.push_back(builder.release_build());
    builder = btn::sprite_builder(sprite_item, 1);
    builder.set_z_order(z_order);

    if(double_size)
    {
        builder.set_affine_mat(*_affine_mat);
        builder.set_position(x + 64, y - 64);
    }
    else
    {
        builder.set_position(x + 32, y - 32);
    }

    _sprites.push_back(builder.release_build());
    builder = btn::sprite_builder(sprite_item, 2);
    builder.set_z_order(z_order);

    if(double_size)
    {
        builder.set_affine_mat(*_affine_mat);
        builder.set_position(x - 64, y + 64);
    }
    else
    {
        builder.set_position(x - 32, y + 32);
    }

    _sprites.push_back(builder.release_build());
    builder = btn::sprite_builder(sprite_item, 3);
    builder.set_z_order(z_order);

    if(double_size)
    {
        builder.set_affine_mat(*_affine_mat);
        builder.set_position(x + 64, y + 64);
    }
    else
    {
        builder.set_position(x + 32, y + 32);
    }

    _sprites.push_back(builder.release_build());
}

void explosion::update()
{
    BTN_ASSERT(! done(), "Explosion is done");

    --_frame_counter;

    if(! _frame_counter)
    {
        const btn::sprite_tiles_item& tiles_item = _sprite_item.tiles_item();
        int animations_count = tiles_item.graphics_count() / 4;
        int animation_index = _animation_index + 1;
        _animation_index = animation_index;
        _frame_counter = _animation_frames;

        if(animation_index < animations_count)
        {
            for(int index = 0; index < 4; ++index)
            {
                _sprites[index].set_tiles(tiles_item, (animation_index * 4) + index);
            }
        }
        else
        {
            _sprites.clear();
        }
    }
}

}
