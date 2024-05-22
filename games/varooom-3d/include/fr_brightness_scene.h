/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_BRIGHTNESS_SCENE_H
#define FR_BRIGHTNESS_SCENE_H

#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_bg_palettes_actions.h"
#include "bn_sprite_palettes_actions.h"

#include "fr_scene.h"
#include "fr_menu_keypad.h"

namespace fr
{

class sram_data;
class common_stuff;

class brightness_scene : public scene
{

public:
    explicit brightness_scene(common_stuff& common_stuff);

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    sram_data& _sram_data;
    bn::bg_palettes_fade_to_action _bgs_fade_in_action;
    bn::sprite_palettes_fade_to_action _sprites_fade_in_action;
    bn::optional<bn::bg_palettes_fade_to_action> _bgs_fade_out_action;
    bn::optional<bn::sprite_palettes_fade_to_action> _sprites_fade_out_action;
    bn::regular_bg_ptr _backdrop_bg;
    bn::sprite_ptr _cursor_sprite;
    bn::vector<bn::sprite_ptr, 16> _text_sprites;
    menu_keypad _menu_keypad;

    void _update_cursor();
};

}

#endif
