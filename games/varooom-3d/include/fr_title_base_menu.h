/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_TITLE_BASE_MENU_H
#define FR_TITLE_BASE_MENU_H

#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_affine_mat_ptr.h"

#include "fr_menu_keypad.h"

namespace fr
{

class common_stuff;

class title_base_menu
{

public:
    enum class option
    {
        RACE,
        MODEL_VIEWER,
        OPTIONS,
        HOW_TO_PLAY,
        CREDITS
    };

    explicit title_base_menu(common_stuff& common_stuff);

    [[nodiscard]] bool animating() const
    {
        return ! _intro_done || ! _outro_done;
    }

    void set_visible(bool visible, bool animate);

    [[nodiscard]] bn::optional<option> update();

private:
    bn::sprite_affine_mat_ptr _affine_mat;
    bn::sprite_ptr _cursor_sprite;
    bn::vector<bn::sprite_ptr, 16> _text_sprites;
    menu_keypad _menu_keypad;
    int _cursor_index = 0;
    int _y_inc = 0;
    bool _all_models_viewed = false;
    bool _show_credits = false;
    bool _intro_done = false;
    bool _outro_done = true;

    void _set_sprites_visible(bool visible);

    void _update_intro();

    void _update_outro();

    void _update_cursor();
};

}

#endif
