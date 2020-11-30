/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_affine_bg_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_affine_bg_pa_register_hblank_effect_ptr.h"
#include "bn_affine_bg_pc_register_hblank_effect_ptr.h"
#include "bn_affine_bg_dx_register_hblank_effect_ptr.h"
#include "bn_affine_bg_dy_register_hblank_effect_ptr.h"

#include "bn_affine_bg_items_land.h"

#include "info.h"
#include "variable_8x16_sprite_font.h"

#include "../../butano/hw/include/bn_hw_tonc.h"

namespace
{
    struct camera
    {
        int x = 400 << 8;
        int y = 128 << 8;
        int z = 280 << 8;
        int phi = 10;
        int cos = 0;
        int sin = 0;
    };

    void update_camera(camera& camera)
    {
        int dir_x = 0;
        int dir_z = 0;

        if(bn::keypad::left_held())
        {
            dir_x -= 2;
        }
        else if(bn::keypad::right_held())
        {
            dir_x += 2;
        }

        if(bn::keypad::down_held())
        {
            dir_z += 2;
        }
        else if(bn::keypad::up_held())
        {
            dir_z -= 2;
        }

        if(bn::keypad::b_held())
        {
            camera.y -= 64;

            if(camera.y < 0)
            {
                camera.y = 0;
            }
        }
        else if(bn::keypad::a_held())
        {
            camera.y += 64;
        }

        if(bn::keypad::l_held())
        {
            camera.phi -= 1;

            if(camera.phi < 0)
            {
                camera.phi = 511;
            }
        }
        else if(bn::keypad::r_held())
        {
            camera.phi += 1;

            if(camera.phi > 511)
            {
                camera.phi = 0;
            }
        }

        camera.cos = bn::lut_cos(camera.phi).data() >> 4;
        camera.sin = bn::lut_sin(camera.phi).data() >> 4;
        camera.x += (dir_x * camera.cos) - (dir_z * camera.sin);
        camera.z += (dir_x * camera.sin) + (dir_z * camera.cos);
    }

    void update_hblank_effect_values(const camera& camera, int16_t* pa_values, int16_t* pc_values, int* dx_values,
                                     int* dy_values)
    {
        int camera_x = camera.x;
        int camera_y = camera.y;
        int camera_z = camera.z;
        int camera_cos = camera.cos;
        int camera_sin = camera.sin;
        int y_shift = 160;

        for(int index = 0; index < bn::display::height(); ++index)
        {
            int lam = camera_y * lu_div(index) >> 12;
            int lcf = lam * camera_cos >> 8;
            int lsf = lam * camera_sin >> 8;

            pa_values[index] = lcf >> 4;
            pc_values[index] = lsf >> 4;

            int lxr = (bn::display::width() / 2) * (lcf >> 4);
            int lyr = (y_shift * lsf) >> 4;
            dx_values[index] = camera_x - lxr + lyr;

            lxr = (bn::display::width() / 2) * (lsf >> 4);
            lyr = (y_shift * lcf) >> 4;
            dy_values[index] = camera_z - lxr - lyr;
        }
    }
}

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(variable_8x16_sprite_font);

    constexpr const bn::string_view info_text_lines[] = {
        "Left/Right: move camera x",
        "Up/Down: move camera z",
        "B/A: move camera y",
        "L/R: move camera phi",
    };

    info info("Mode 7", info_text_lines, text_generator);

    bn::affine_bg_ptr bg = bn::affine_bg_items::land.create_bg(-376, -336);

    int16_t pa_values[bn::display::height()];
    bn::affine_bg_pa_register_hblank_effect_ptr pa_hblank_effect =
            bn::affine_bg_pa_register_hblank_effect_ptr::create(bg, pa_values);

    int16_t pc_values[bn::display::height()];
    bn::affine_bg_pc_register_hblank_effect_ptr pc_hblank_effect =
            bn::affine_bg_pc_register_hblank_effect_ptr::create(bg, pc_values);

    int dx_values[bn::display::height()];
    bn::affine_bg_dx_register_hblank_effect_ptr dx_hblank_effect =
            bn::affine_bg_dx_register_hblank_effect_ptr::create(bg, dx_values);

    int dy_values[bn::display::height()];
    bn::affine_bg_dy_register_hblank_effect_ptr dy_hblank_effect =
            bn::affine_bg_dy_register_hblank_effect_ptr::create(bg, dy_values);

    camera camera;

    while(true)
    {
        update_camera(camera);
        update_hblank_effect_values(camera, pa_values, pc_values, dx_values, dy_values);
        pa_hblank_effect.reload_values_ref();
        pc_hblank_effect.reload_values_ref();
        dx_hblank_effect.reload_values_ref();
        dy_hblank_effect.reload_values_ref();
        info.update();
        bn::core::update();
    }
}
