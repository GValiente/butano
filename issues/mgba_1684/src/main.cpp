/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_core.h"
#include "btn_display.h"
#include "btn_bgs_mosaic.h"
#include "btn_regular_bg_items_stage_1.h"
#include "btn_regular_bg_position_hblank_effect_ptr.h"
#include "wave_generator.h"

namespace
{
    [[nodiscard]] constexpr btn::array<btn::fixed, btn::display::height()> _create_hblank_effect_deltas()
    {
        btn::array<btn::fixed, btn::display::height()> result;
        wave_generator().generate(result);
        return result;
    }

    constexpr const btn::array<btn::fixed, btn::display::height()> hblank_effect_deltas = _create_hblank_effect_deltas();
}

int main()
{
    btn::core::init();

    btn::regular_bg_ptr bg = btn::regular_bg_items::stage_1.create_bg(0, 0);
    bg.set_mosaic_enabled(true);
    btn::bgs_mosaic::set_stretch(0.2);

    btn::regular_bg_position_hblank_effect_ptr hblank_effect =
            btn::regular_bg_position_hblank_effect_ptr::create_horizontal(move(bg), hblank_effect_deltas);

    while(true)
    {
        btn::core::update();
    }
}
