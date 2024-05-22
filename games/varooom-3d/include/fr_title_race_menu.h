/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_TITLE_RACE_MENU_H
#define FR_TITLE_RACE_MENU_H

#include "bn_vector.h"
#include "bn_sprite_ptr.h"

#include "fr_menu_keypad.h"

namespace bn
{
    class regular_bg_ptr;
}

namespace fr
{

class common_stuff;

class title_race_menu
{

public:
    explicit title_race_menu(common_stuff& common_stuff);

    [[nodiscard]] bool visible() const
    {
        return _cursor_sprites[0].visible();
    }

    void set_visible(bool visible);

    [[nodiscard]] bn::optional<bool> update(common_stuff& common_stuff, bn::regular_bg_ptr& backdrop);

    void blink_cursor();

private:
    bn::vector<bn::sprite_ptr, 6> _locked_sprites;
    bn::vector<bn::sprite_ptr, 2> _cursor_sprites;
    bn::vector<bn::sprite_ptr, 16> _label_sprites;
    bn::vector<bn::sprite_ptr, 8> _position_time_sprites;
    menu_keypad _menu_keypad;
    int _show_frames = 0;
    int _hide_frames = 0;
    int _blink_wait_frames = 0;
    bool _return = false;

    void _update_cursor(common_stuff& common_stuff);

    void _show(bn::regular_bg_ptr& backdrop);

    void _hide(bn::regular_bg_ptr& backdrop);
};

}

#endif
