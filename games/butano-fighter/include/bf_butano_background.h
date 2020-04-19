#ifndef BF_BUTANO_BACKGROUND_H
#define BF_BUTANO_BACKGROUND_H

#include "btn_color.h"
#include "btn_optional.h"
#include "btn_regular_bg_actions.h"
#include "btn_bg_palettes_hblank_effects.h"

namespace bf
{

class butano_background
{

public:
    butano_background();

    butano_background(const butano_background& other) = delete;

    butano_background& operator=(const butano_background& other) = delete;

    void update();

private:
    btn::optional<btn::regular_bg_move_by_action> _move_action;
    btn::color _palette_hblank_effect_colors[160];
    btn::bg_palettes_transparent_color_hblank_effect_ptr _palette_hblank_effect;
};

}

#endif
