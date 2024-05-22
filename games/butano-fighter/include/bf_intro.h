/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_INTRO_H
#define BF_INTRO_H

#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_blending_actions.h"
#include "bn_bg_palettes_actions.h"
#include "bn_sprite_palettes_actions.h"
#include "bf_scene.h"

namespace bn
{
    class sprite_text_generator;
}

namespace bf
{

class butano_background;

class intro : public scene
{

public:
    intro(bn::sprite_text_generator& text_generator, butano_background& butano_background);

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    bn::bg_palettes_fade_to_action _bg_fade_action;
    bn::sprite_palettes_fade_to_action _sprite_fade_action;
    bn::blending_transparency_alpha_to_action _blending_action;
    bn::vector<bn::sprite_ptr, 20> _text_sprites;
    int _counter = 60 * 4;
};

}

#endif
