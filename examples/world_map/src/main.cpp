/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_blending.h"
#include "bn_fixed_point.h"
#include "bn_affine_bg_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_affine_bg_pa_register_hblank_effect_ptr.h"
#include "bn_affine_bg_pd_register_hblank_effect_ptr.h"
#include "bn_affine_bg_dx_register_hblank_effect_ptr.h"
#include "bn_affine_bg_dy_register_hblank_effect_ptr.h"

#include "info.h"
#include "load_attributes.h"
#include "variable_8x16_sprite_font.h"

#include "bn_affine_bg_items_land.h"
#include "bn_affine_bg_items_clouds.h"

int main()
{
    bn::core::init();

    constexpr const bn::string_view info_text_lines[] = {
        "PAD: move",
        "A: move faster",
        "",
        "START: go to next scene",
    };

    bn::sprite_text_generator text_generator(variable_8x16_sprite_font);
    info info("World map", info_text_lines, text_generator);

    bn::affine_bg_ptr land_bg = bn::affine_bg_items::land.create_bg(0, 0);
    land_bg.set_pivot_position(1424, 850);

    int x_limit = (land_bg.dimensions().width() - bn::display::width()) / 2;
    int y_limit = (land_bg.dimensions().height() - bn::display::height()) / 2;

    bn::affine_bg_ptr clouds_bg = bn::affine_bg_items::clouds.create_bg(0, 0);
    bn::blending::set_transparency_alpha(0.5);
    clouds_bg.set_blending_enabled(true);

    bn::unique_ptr<bn::array<bn::affine_bg_mat_attributes, bn::display::height()>> land_attributes_ptr(
            new bn::array<bn::affine_bg_mat_attributes, bn::display::height()>());
    bn::array<bn::affine_bg_mat_attributes, bn::display::height()>& land_attributes = *land_attributes_ptr;
    bn::affine_bg_pa_register_hblank_effect_ptr land_pa_hblank_effect =
            bn::affine_bg_pa_register_hblank_effect_ptr::create(land_bg, land_attributes._data);
    bn::affine_bg_pd_register_hblank_effect_ptr land_pd_hblank_effect =
            bn::affine_bg_pd_register_hblank_effect_ptr::create(land_bg, land_attributes._data);
    bn::affine_bg_dx_register_hblank_effect_ptr land_dx_hblank_effect =
            bn::affine_bg_dx_register_hblank_effect_ptr::create(land_bg, land_attributes._data);
    bn::affine_bg_dy_register_hblank_effect_ptr land_dy_hblank_effect =
            bn::affine_bg_dy_register_hblank_effect_ptr::create(land_bg, land_attributes._data);

    bn::unique_ptr<bn::array<bn::affine_bg_mat_attributes, bn::display::height()>> clouds_attributes_ptr(
            new bn::array<bn::affine_bg_mat_attributes, bn::display::height()>());
    bn::array<bn::affine_bg_mat_attributes, bn::display::height()>& clouds_attributes = *clouds_attributes_ptr;
    bn::affine_bg_pa_register_hblank_effect_ptr clouds_pa_hblank_effect =
            bn::affine_bg_pa_register_hblank_effect_ptr::create(clouds_bg, clouds_attributes._data);
    bn::affine_bg_pd_register_hblank_effect_ptr clouds_pd_hblank_effect =
            bn::affine_bg_pd_register_hblank_effect_ptr::create(clouds_bg, clouds_attributes._data);
    bn::affine_bg_dx_register_hblank_effect_ptr clouds_dx_hblank_effect =
            bn::affine_bg_dx_register_hblank_effect_ptr::create(clouds_bg, clouds_attributes._data);
    bn::affine_bg_dy_register_hblank_effect_ptr clouds_dy_hblank_effect =
            bn::affine_bg_dy_register_hblank_effect_ptr::create(clouds_bg, clouds_attributes._data);

    bool first_frame = true;

    while(true)
    {
        int inc = bn::keypad::a_held() ? 8 : 1;

        if(bn::keypad::left_held())
        {
            land_bg.set_pivot_x(bn::max(land_bg.pivot_x().right_shift_integer() - inc, 1 - x_limit));
            clouds_bg.set_pivot_x(clouds_bg.pivot_x().right_shift_integer() - inc);
        }
        else if(bn::keypad::right_held())
        {
            land_bg.set_pivot_x(bn::min(land_bg.pivot_x().right_shift_integer() + inc, x_limit));
            clouds_bg.set_pivot_x(clouds_bg.pivot_x().right_shift_integer() + inc);
        }

        if(bn::keypad::up_held())
        {
            land_bg.set_pivot_y(bn::max(land_bg.pivot_y().right_shift_integer() - inc, 1 - y_limit));
            clouds_bg.set_pivot_y(clouds_bg.pivot_y().right_shift_integer() - inc);
        }
        else if(bn::keypad::down_held())
        {
            land_bg.set_pivot_y(bn::min(land_bg.pivot_y().right_shift_integer() + inc, y_limit));
            clouds_bg.set_pivot_y(clouds_bg.pivot_y().right_shift_integer() + inc);
        }

        clouds_bg.set_pivot_position(clouds_bg.pivot_position() + bn::fixed_point(0.1, 0.1));

        load_attributes(land_bg.mat_attributes(), land_attributes._data);
        load_attributes(clouds_bg.mat_attributes(), clouds_attributes._data);

        if(first_frame)
        {
            land_pa_hblank_effect.reload_attributes_ref();
            land_pd_hblank_effect.reload_attributes_ref();
            clouds_pa_hblank_effect.reload_attributes_ref();
            clouds_pd_hblank_effect.reload_attributes_ref();
            first_frame = false;
        }

        land_dx_hblank_effect.reload_attributes_ref();
        land_dy_hblank_effect.reload_attributes_ref();
        clouds_dx_hblank_effect.reload_attributes_ref();
        clouds_dy_hblank_effect.reload_attributes_ref();
        info.update();
        bn::core::update();
    }
}
