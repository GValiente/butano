/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_TITLE_OPTIONS_MENU_H
#define FR_TITLE_OPTIONS_MENU_H

#include "bn_vector.h"
#include "bn_sprite_ptr.h"

#include "fr_menu_keypad.h"

namespace fr
{

class common_stuff;

class title_options_menu
{

public:
    explicit title_options_menu(common_stuff& common_stuff);

    [[nodiscard]] bool visible() const
    {
        return _cursor_sprite.visible();
    }

    void set_visible(const common_stuff& common_stuff, bool visible);

    [[nodiscard]] bool update(common_stuff& common_stuff);

private:
    bn::sprite_ptr _cursor_sprite;
    bn::sprite_ptr _low_brightness_sprite;
    bn::sprite_ptr _medium_brightness_sprite;
    bn::sprite_ptr _high_brightness_sprite;
    bn::sprite_ptr _rumble_enabled_sprite;
    bn::sprite_ptr _rumble_disabled_sprite;
    bn::vector<bn::sprite_ptr, 16> _text_sprites;
    menu_keypad _menu_keypad;
    int _cursor_index = 0;

    void _update_cursor();

    void _update_rumble(const common_stuff& common_stuff);

    void _update_brightness(const common_stuff& common_stuff);
};

}

#endif
