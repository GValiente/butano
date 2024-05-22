/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_PAUSE_H
#define FR_PAUSE_H

#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_bg_palettes_actions.h"
#include "bn_sprite_palettes_actions.h"

#include "fr_menu_keypad.h"

namespace fr
{

class announcer;
class common_stuff;
enum class scene_type;

class pause
{

public:
    explicit pause(common_stuff& common_stuff);

    [[nodiscard]] bool paused() const
    {
        return _animation_index > 0;
    }

    [[nodiscard]] bn::optional<scene_type> update(common_stuff& common_stuff, announcer& announcer);

private:
    bn::sprite_tiles_ptr _background_tiles;
    bn::sprite_palette_ptr _background_palette;
    bn::sprite_ptr _cursor_sprite;
    bn::sprite_ptr _low_brightness_sprite;
    bn::sprite_ptr _medium_brightness_sprite;
    bn::sprite_ptr _high_brightness_sprite;
    bn::sprite_ptr _rumble_enabled_sprite;
    bn::sprite_ptr _rumble_disabled_sprite;
    bn::vector<bn::sprite_ptr, 36> _background_sprites;
    bn::vector<bn::sprite_ptr, 16> _text_sprites;
    bn::optional<bn::bg_palettes_fade_to_action> _bg_fade_action;
    bn::optional<bn::sprite_palettes_fade_to_action> _sprite_fade_action;
    menu_keypad _menu_keypad;
    int _animation_index = 0;
    int _cursor_index = 0;
    int _wait_counter = 0;
    bool _rumble = false;
    bool _green_swap = false;
    bool _animation_inc = false;
    bool _animation_dec = false;
    bool _restart = false;

    void _show_animation(const common_stuff& common_stuff);

    void _show_background(int size);

    void _show_cursor();

    void _show_rumble(const common_stuff& common_stuff);

    void _show_brightness(const common_stuff& common_stuff);

    void _show_text(const common_stuff& common_stuff);

    void _hide_text();

    void _exit(bool restart, announcer& announcer);
};

}

#endif
