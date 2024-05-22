/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_foreground_3d.h"

#include "bn_display.h"

#include "fr_div_lut.h"
#include "fr_camera_3d.h"

namespace fr
{

int foreground_3d::_reload_static_model_items_impl(const camera_3d& camera, const model_3d_item* model_items,
                                                   const visible_model_3d_grid::cell& visible_cell)
{
    constexpr int display_width = bn::display::width();
    constexpr int display_height = bn::display::height();
    constexpr int focal_length_shift = constants_3d::focal_length_shift;

    constexpr bn::fixed vry = (0 - 256) / 16;
    constexpr int vcz = -vry.data();

    // constexpr int scale = (1 << (focal_length_shift + 16)) / vcz;
    constexpr auto scale = int((calculate_div_lut_value(vcz >> 6) << (focal_length_shift - 8)) >> 6);

    bn::fixed camera_x = camera.position().x();
    bn::fixed camera_z = camera.position().z();
    bn::fixed camera_u_x = camera.u().x();
    bn::fixed camera_u_z = camera.u().z();
    bn::fixed camera_v_x = camera.v().x();
    bn::fixed camera_v_z = camera.v().z();
    int static_model_items_count = 0;

    for(int index = visible_cell.models_count - 1; index >= 0; --index)
    {
        uint16_t model_index = visible_cell.model_indexes[index];
        const model_3d_item& model_item = model_items[model_index];
        const model_3d_vertical_cylinder* vertical_cylinder = model_item.vertical_cylinder();
        int radius = vertical_cylinder->integer_radius();

        bn::fixed vrx = (vertical_cylinder->centroid_x() - camera_x) / 16;
        bn::fixed vrz = (vertical_cylinder->centroid_z() - camera_z) / 16;
        int vcy = -(vrx.unsafe_multiplication(camera_v_x) + vrz.unsafe_multiplication(camera_v_z)).data();
        int screen_y = ((vcy * scale) >> 16) + (display_height / 2);

        if(screen_y + radius >= 0 && screen_y - radius < display_height) [[likely]]
        {
            int vcx = (vrx.unsafe_multiplication(camera_u_x) + vrz.unsafe_multiplication(camera_u_z)).data();
            int screen_x = ((vcx * scale) >> 16) + (display_width / 2);

            if(screen_x + radius >= 0 && screen_x - radius < display_width) [[likely]]
            {
                _static_model_items[static_model_items_count] = &model_item;
                ++static_model_items_count;
            }
        }
    }

    return static_model_items_count;
}

}
