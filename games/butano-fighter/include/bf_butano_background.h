/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_BUTANO_BACKGROUND_H
#define BF_BUTANO_BACKGROUND_H

#include "bn_sprite_actions.h"
#include "bn_regular_bg_actions.h"
#include "bn_bg_palette_color_hbe_ptr.h"

namespace bf
{

class butano_background
{

public:
    butano_background()
    {
        _set_visible();
    }

    [[nodiscard]] bool silhouette_visible() const
    {
        return _silhouette_up_sprite_move_action.has_value();
    }

    void show(const bn::fixed_point& silhouette_position, const bn::camera_ptr& camera);

    void hide(const bn::fixed_point& silhouette_position, const bn::camera_ptr& camera);

    void put_under_all();

    void update();

private:
    bn::optional<bn::regular_bg_move_by_action> _move_action;
    bn::optional<bn::bg_palette_color_hbe_ptr> _palette_hbe;
    bn::optional<bn::sprite_move_to_action> _silhouette_up_sprite_move_action;
    bn::optional<bn::sprite_move_to_action> _silhouette_down_sprite_move_action;
    bn::optional<bn::sprite_scale_to_action> _silhouette_up_sprite_scale_action;
    bn::optional<bn::sprite_scale_to_action> _silhouette_down_sprite_scale_action;

    void _set_visible();
};

}

#endif
