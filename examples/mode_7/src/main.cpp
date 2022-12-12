/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_affine_bg_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_affine_bg_pa_register_hbe_ptr.h"
#include "bn_affine_bg_pc_register_hbe_ptr.h"
#include "bn_affine_bg_dx_register_hbe_ptr.h"
#include "bn_affine_bg_dy_register_hbe_ptr.h"
#include "bn_window.h"
#include "bn_rect_window.h"

#include "bn_affine_bg_items_land.h"
#include "bn_regular_bg_items_background.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"


#define M7_D        256     //!< Focal length
#define M7_LEFT     (-120)      //!< Viewport left
#define M7_RIGHT     120        //!< Viewport right
#define M7_TOP        80        //!< Viewport top (y-axis up)
#define M7_BOTTOM   (-80)       //!< Viewport bottom (y-axis up!)
#define M7_NEAR       24        //!< Near plane (objects)
#define M7_FAR       512        //!< Far plane (objects)
#define M7_FAR_BG    768        //!< Far plane (floor)

#define INT_MAX 2147483647
#define INT_MIN -2147483648

namespace
{
    struct camera
    {
        bn::fixed x = 440;
        bn::fixed y = 128;
        bn::fixed z = 320;
        int phi = 10;
        int theta = 0;
        bn::fixed cf = 0;
        bn::fixed sf = 0;
        bn::fixed ct = 0;
        bn::fixed st = 0;
    };

    void update_camera(camera& camera)
    {
        bn::fixed dir_x = 0;
        bn::fixed dir_z = 0;

        if(bn::keypad::left_held())
        {
            dir_x -= 1;
        }
        else if(bn::keypad::right_held())
        {
            dir_x += 1;
        }

        if(bn::keypad::down_held())
        {
            dir_z += 1;
        }
        else if(bn::keypad::up_held())
        {
            dir_z -= 1;
        }

        if(bn::keypad::b_held())
        {
            camera.y -= bn::fixed::from_data(2048);

            if(camera.y < 0)
            {
                camera.y = 0;
            }
        }
        else if(bn::keypad::a_held())
        {
            camera.y += bn::fixed::from_data(2048);
        }

        if(bn::keypad::l_held())
        {
            camera.phi -= 4;

            if(camera.phi < 0)
            {
                camera.phi += 2048;
            }
        }
        else if(bn::keypad::r_held())
        {
            camera.phi += 4;

            if(camera.phi >= 2048)
            {
                camera.phi -= 2048;
            }
        }

        camera.cf = bn::lut_cos(camera.phi);
        camera.sf = bn::lut_sin(camera.phi);
        camera.x += (dir_x * camera.cf) - (dir_z * camera.sf);
        camera.z += (dir_x * camera.sf) + (dir_z * camera.cf);
    }

    void update_camera_with_pitch(camera& camera)
    {
        bn::fixed dir_x = 0;
        bn::fixed dir_z = 0;
        bool strafemode = bn::keypad::l_held();

        if(strafemode)
        {
            if(bn::keypad::left_held())
            {
                dir_x -= 1;
            }
            else if(bn::keypad::right_held())
            {
                dir_x += 1;
            }

            if(bn::keypad::down_held())
            {
                dir_z += 1;
            }
            else if(bn::keypad::up_held())
            {
                dir_z -= 1;
            }
        }
        else
        {
            if(bn::keypad::left_held())
            {
                camera.phi -= 4;
                if(camera.phi < 0)
                {
                    camera.phi += 2048;
                }
            }
            else if(bn::keypad::right_held())
            {
                camera.phi += 4;
                if(camera.phi >= 2048)
                {
                    camera.phi -= 2048;
                }
            }

            if(bn::keypad::up_held())
            {
                camera.theta -= 4;

                if(camera.theta < 0)
                {
                    camera.theta += 2048;
                }
                if(camera.theta > 512 && camera.theta < 1536)
                    camera.theta = 1536;
            }
            else if(bn::keypad::down_held())
            {
                camera.theta += 4;

                if(camera.theta > 2048)
                {
                    camera.theta -= 2048;
                }
                if(camera.theta > 512 && camera.theta < 1536)
                    camera.theta = 512;
            }
        }

        if(bn::keypad::b_held())
        {
            camera.y -= bn::fixed::from_data(2048);

            if(camera.y < 0)
            {
                camera.y = 0;
            }
        }
        else if(bn::keypad::a_held())
        {
            camera.y += bn::fixed::from_data(2048);
        }

        camera.cf = bn::lut_cos(camera.phi);
        camera.sf = bn::lut_sin(camera.phi);
        camera.ct = bn::lut_cos(camera.theta);
        camera.st = bn::lut_sin(camera.theta);
        camera.x += (dir_x * camera.cf) - (dir_z * camera.sf);
        camera.z += (dir_x * camera.sf) + (dir_z * camera.cf);
    }

    void update_hbe_values(const camera& camera, int16_t* pa_values, int16_t* pc_values, int* dx_values,
                           int* dy_values)
    {
        int camera_x = camera.x.data();
        int camera_y = camera.y.data() >> 4;
        int camera_z = camera.z.data();
        int camera_cos = camera.cf.data() >> 4;
        int camera_sin = camera.sf.data() >> 4;
        int y_shift = 160;

        for(int index = 0; index < bn::display::height(); ++index)
        {
            int reciprocal = bn::reciprocal_lut[index].data() >> 4;
            int lam = camera_y * reciprocal >> 12;
            int lcf = lam * camera_cos >> 8;
            int lsf = lam * camera_sin >> 8;

            pa_values[index] = int16_t(lcf >> 4);
            pc_values[index] = int16_t(lsf >> 4);

            int lxr = (bn::display::width() / 2) * lcf;
            int lyr = y_shift * lsf;
            dx_values[index] = (camera_x - lxr + lyr) >> 4;

            lxr = (bn::display::width() / 2) * lsf;
            lyr = y_shift * lcf;
            dy_values[index] = (camera_z - lxr - lyr) >> 4;
        }
    }
    int update_hbe_values_with_pitch(const camera& camera, int16_t* pa_values, int16_t* pc_values, int* dx_values,
                           int* dy_values)
    {
        int xc = camera.x.data()>>4;
        int yc = camera.y.data() >> 4;
        int zc = camera.z.data()>>4;
        int cf = camera.cf.data() >> 4;
        int sf = camera.sf.data() >> 4;
        int ct= camera.ct.data() >> 4;
        int st= camera.st.data() >> 4;
        int horiz=0;
        if(camera.ct.data() != 0) // camera.ct = camup.y in camera coord
        {
            horiz= M7_FAR_BG*camera.st.data() - camera.y.data();
            horiz= M7_TOP - horiz*M7_D / (M7_FAR_BG*camera.ct.data());
        }
        else    // looking straight down ((camera.st = cambackward.y) > 0) means horizon at -inf scanline
        {
            horiz= camera.st.data() > 0 ? INT_MIN : INT_MAX;
        }

        for(int index = 0; index < bn::display::height(); ++index)
        {
            if(index<horiz)
            {
                // Ugly hack to be sure to have transparent color above horizon
                pa_values[index]=0;
                pc_values[index]=0;
                dx_values[index]=0xFFFFFFFF;
                dy_values[index]=0xFFFFFFFF;
                continue;
            }
            int yb= (index-M7_TOP)*ct + M7_D*st;
            int lam=0;
            if(yb != 0)
                lam=(yc<<12)/yb;     // .12f

            int lcf= lam*cf>>8;                 // .12f
            int lsf= lam*sf>>8;                 // .12f

            pa_values[index]= int16_t(lcf>>4);              // .8f
            pc_values[index]= int16_t(lsf>>4);              // .8f

            // lambda·Rx·b
            int zb= (index-M7_TOP)*st - M7_D*ct;    // .8f
            dx_values[index]= xc + (lcf>>4)*M7_LEFT - (lsf*zb>>12); // .8f
            dy_values[index]= zc + (lsf>>4)*M7_LEFT + (lcf*zb>>12); // .8f
        }
        return horiz;
    }
}

void simple_mode7_scene(bn::sprite_text_generator text_generator)
{
    constexpr bn::string_view info_text_lines[] = {
        "Left/Right: move camera x",
        "Up/Down: move camera z",
        "B/A: move camera y",
        "L/R: move camera phi",
        "",
        "START: go to next scene",
    };

    common::info info("Mode 7", info_text_lines, text_generator);

    bn::affine_bg_ptr bg = bn::affine_bg_items::land.create_bg(-376, -336);

    int16_t pa_values[bn::display::height()];
    bn::affine_bg_pa_register_hbe_ptr pa_hbe = bn::affine_bg_pa_register_hbe_ptr::create(bg, pa_values);

    int16_t pc_values[bn::display::height()];
    bn::affine_bg_pc_register_hbe_ptr pc_hbe = bn::affine_bg_pc_register_hbe_ptr::create(bg, pc_values);

    int dx_values[bn::display::height()];
    bn::affine_bg_dx_register_hbe_ptr dx_hbe = bn::affine_bg_dx_register_hbe_ptr::create(bg, dx_values);

    int dy_values[bn::display::height()];
    bn::affine_bg_dy_register_hbe_ptr dy_hbe = bn::affine_bg_dy_register_hbe_ptr::create(bg, dy_values);

    camera camera;

    while(! bn::keypad::start_pressed())
    {
        update_camera(camera);
        update_hbe_values(camera, pa_values, pc_values, dx_values, dy_values);
        pa_hbe.reload_values_ref();
        pc_hbe.reload_values_ref();
        dx_hbe.reload_values_ref();
        dy_hbe.reload_values_ref();
        info.update();
        bn::core::update();
    }
}
void advanced_mode7_scene(bn::sprite_text_generator text_generator)
{
    constexpr bn::string_view info_text_lines[] = {
        "Left/Right: move camera phi",
        "Up/Down: move camera theta",
        "L+Left/Right: move camera x",
        "L+Up/Down: move camera z",
        "B/A: move camera y",
        "",
        "START: go to next scene",
    };

    common::info info("Mode 7 Advanced", info_text_lines, text_generator);

    bn::affine_bg_ptr bg = bn::affine_bg_items::land.create_bg(-376, -336);
    bg.set_wrapping_enabled(true);

    bn::regular_bg_ptr backdrop = bn::regular_bg_items::background.create_bg(0, 0);

    int16_t pa_values[bn::display::height()];
    bn::affine_bg_pa_register_hbe_ptr pa_hbe = bn::affine_bg_pa_register_hbe_ptr::create(bg, pa_values);

    int16_t pc_values[bn::display::height()];
    bn::affine_bg_pc_register_hbe_ptr pc_hbe = bn::affine_bg_pc_register_hbe_ptr::create(bg, pc_values);

    int dx_values[bn::display::height()];
    bn::affine_bg_dx_register_hbe_ptr dx_hbe = bn::affine_bg_dx_register_hbe_ptr::create(bg, dx_values);

    int dy_values[bn::display::height()];
    bn::affine_bg_dy_register_hbe_ptr dy_hbe = bn::affine_bg_dy_register_hbe_ptr::create(bg, dy_values);
	
	bn::window outside_window = bn::window::outside();
    outside_window.set_show_bg(backdrop, false);
	bn::rect_window internal_window = bn::rect_window::internal();
	internal_window.set_boundaries(-80, -120, 80, 120);
	internal_window.set_show_bg(backdrop, true);



    camera camera;
    camera.y=10;

    while(! bn::keypad::start_pressed())
    {
        update_camera_with_pitch(camera);
        int horiz = update_hbe_values_with_pitch(camera, pa_values, pc_values, dx_values, dy_values);
        backdrop.set_y(horiz-80-backdrop.dimensions().height()/2);
        backdrop.set_x(-camera.phi);
		if(horiz >= 0 && horiz < 160)
			internal_window.set_bottom(-80+horiz);
		else
			internal_window.set_bottom(0);

        pa_hbe.reload_values_ref();
        pc_hbe.reload_values_ref();
        dx_hbe.reload_values_ref();
        dy_hbe.reload_values_ref();
        info.update();
        bn::core::update();
    }
}
int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);

    while(true)
    {
        simple_mode7_scene(text_generator);
        bn::core::update();

        advanced_mode7_scene(text_generator);
        bn::core::update();
    }
}
