/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_EXPLOSION_H
#define BF_GAME_EXPLOSION_H

#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_affine_mat_ptr.h"

namespace bf::game
{

class explosion
{

public:
    explosion(const bn::sprite_item& sprite_item, const bn::fixed_point& position, int animation_frames,
              int z_order, bool double_size, const bn::camera_ptr& camera);

    [[nodiscard]] bool show_target_sprite() const
    {
        return _animation_index < 3;
    }

    [[nodiscard]] bool done() const
    {
        return _sprites.empty();
    }

    void update();

private:
    const bn::sprite_item* _sprite_item;
    bn::optional<bn::sprite_affine_mat_ptr> _affine_mat;
    bn::vector<bn::sprite_ptr, 4> _sprites;
    int _animation_frames;
    int _animation_index;
    int _frame_counter;
};

}

#endif
