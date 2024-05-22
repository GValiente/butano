/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_ENDING_H
#define BF_ENDING_H

#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_blending_actions.h"
#include "bf_scene.h"

namespace bn
{
    class sprite_text_generator;
}

namespace bf
{

class butano_background;

class ending : public scene
{

public:
    ending(bn::sprite_text_generator& text_generator, butano_background& butano_background);

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    bn::regular_bg_ptr _bg;
    bn::vector<bn::sprite_ptr, 32> _text_sprites;
    bn::optional<bn::blending_transparency_alpha_to_action> _blending_action;
    bn::fixed _bg_y_inc;
    bool wait_for_key = false;
};

}

#endif
