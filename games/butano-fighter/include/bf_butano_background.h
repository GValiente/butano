#ifndef BF_BUTANO_BACKGROUND_H
#define BF_BUTANO_BACKGROUND_H

#include "btn_optional.h"
#include "btn_sprite_actions.h"
#include "btn_regular_bg_actions.h"
#include "btn_bg_palette_hblank_effects.h"

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

    void show(const btn::fixed_point& silhouette_position);

    void hide(const btn::fixed_point& silhouette_position);

    void put_under_all();

    void update();

private:
    btn::optional<btn::regular_bg_move_by_action> _move_action;
    btn::optional<btn::bg_palette_color_hblank_effect_ptr> _palette_hblank_effect;
    btn::optional<btn::sprite_move_to_action> _silhouette_up_sprite_move_action;
    btn::optional<btn::sprite_move_to_action> _silhouette_down_sprite_move_action;
    btn::optional<btn::sprite_scale_to_action> _silhouette_up_sprite_scale_action;
    btn::optional<btn::sprite_scale_to_action> _silhouette_down_sprite_scale_action;

    void _set_visible();
};

}

#endif
