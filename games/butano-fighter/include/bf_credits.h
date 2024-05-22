/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_CREDITS_H
#define BF_CREDITS_H

#include "bn_deque.h"
#include "bn_random.h"
#include "bn_music_actions.h"
#include "bn_sprite_actions.h"
#include "bn_blending_actions.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_affine_mat_attributes_hbe_ptr.h"
#include "bf_scene.h"

namespace bn
{
    class sprite_text_generator;
}

namespace bf
{

class butano_background;

class credits : public scene
{

public:
    credits(bn::sprite_text_generator& text_generator, butano_background& butano_background);

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    struct background_sprite
    {
        bn::sprite_move_to_action move_action;
        bn::sprite_animate_action<4> animate_action;
    };

    struct text_entry
    {
        bn::vector<bn::sprite_ptr, 12> sprites;
        bn::fixed y;
    };

    bn::sprite_text_generator& _text_generator;
    bn::sprite_affine_mat_attributes_hbe_ptr _hbe;
    bn::deque<text_entry, 16> _text_entries;
    bn::deque<background_sprite, 8> _background_sprites;
    bn::optional<bn::blending_transparency_alpha_to_action> _blending_action;
    bn::optional<bn::music_volume_to_action> _music_volume_action;
    bn::random _random;
    int _text_index = 0;
    int _text_counter = 1;
    int _background_sprite_counter = 1;
};

}

#endif
