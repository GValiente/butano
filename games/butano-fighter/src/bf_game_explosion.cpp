/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_explosion.h"

#include "bn_sprite_builder.h"
#include "bn_affine_mat_attributes.h"

namespace bf::game
{

explosion::explosion(const bn::sprite_item& sprite_item, const bn::fixed_point& position, int animation_frames,
                     int z_order, bool double_size, const bn::camera_ptr& camera) :
    _sprite_item(&sprite_item),
    _animation_frames(animation_frames),
    _animation_index(0),
    _frame_counter(animation_frames)
{
    BN_ASSERT(_sprite_item->tiles_item().graphics_count() > 4 && _sprite_item->tiles_item().graphics_count() % 4 == 0,
               "Invalid graphics count: ", _sprite_item->tiles_item().graphics_count());
    BN_ASSERT(animation_frames > 0, "Invalid animation frames: ", animation_frames);

    bn::fixed x = position.x();
    bn::fixed y = position.y();

    if(double_size)
    {
        bn::affine_mat_attributes attributes;
        attributes.set_scale(2);
        _affine_mat = bn::sprite_affine_mat_ptr::create(attributes);
    }

    bn::sprite_builder builder(sprite_item);
    builder.set_z_order(z_order);
    builder.set_camera(camera);

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

    builder = bn::sprite_builder(sprite_item, 1);
    builder.set_z_order(z_order);
    builder.set_camera(camera);

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

    builder = bn::sprite_builder(sprite_item, 2);
    builder.set_z_order(z_order);
    builder.set_camera(camera);

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

    builder = bn::sprite_builder(sprite_item, 3);
    builder.set_z_order(z_order);
    builder.set_camera(camera);

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
    BN_ASSERT(! done(), "Explosion is done");

    --_frame_counter;

    if(! _frame_counter)
    {
        const bn::sprite_tiles_item& tiles_item = _sprite_item->tiles_item();
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
