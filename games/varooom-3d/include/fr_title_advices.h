/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_TITLE_ADVICES_H
#define FR_TITLE_ADVICES_H

#include "bn_deque.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_affine_mat_ptr.h"

namespace fr
{

class common_stuff;

class title_advices
{

public:
    explicit title_advices(common_stuff& common_stuff);

    [[nodiscard]] bool animating() const
    {
        return ! _intro_done || ! _outro_done;
    }

    void set_visible(bool visible);

    void update(common_stuff& common_stuff);

private:
    struct message_type
    {
        bn::deque<bn::sprite_ptr, 32> sprites;
    };

    bn::sprite_affine_mat_ptr _affine_mat;
    bn::deque<message_type, 2> _messages;
    bn::fixed _vertical_scale;
    int _item_index = 0;
    int _wait_frames = 0;
    bool _intro_done = false;
    bool _outro_done = true;

    void _set_sprites_visible(bool visible);

    void _set_sprites_vertical_scale(bn::fixed vertical_scale);

    void _update_intro();

    void _update_outro();
};

}

#endif
