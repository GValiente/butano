/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * CA3
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_BUTANO_HOUSE_H
#define FR_MODEL_3D_ITEMS_BUTANO_HOUSE_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d butano_house_vertices[] = {
        vertex_3d(4.5, 44.0, 4.5),
        vertex_3d(4.5, 44.0, -4.5),
        vertex_3d(-4.5, 44.0, -4.5),
        vertex_3d(-4.5, 44.0, 4.5),
        vertex_3d(4.5, 36.5, 4.5),
        vertex_3d(4.5, 36.5, -4.5),
        vertex_3d(-4.5, 36.5, -4.5),
        vertex_3d(-4.5, 36.5, 4.5),
        vertex_3d(-12.0, 33.5, -7.5),
        vertex_3d(-7.5, 35.0, -7.5),
        vertex_3d(-5.0, 37.0, -15.0),
        vertex_3d(-9.0, 38.0, -18.0),
        vertex_3d(5.0, 37.0, -15.0),
        vertex_3d(7.5, 35.0, -7.5),
        vertex_3d(12.0, 33.5, -7.5),
        vertex_3d(9.0, 38.0, -18.0),
        vertex_3d(-12.0, 33.5, 7.5),
        vertex_3d(-9.0, 38.0, 18.0),
        vertex_3d(-5.0, 37.0, 15.0),
        vertex_3d(-7.5, 35.0, 7.5),
        vertex_3d(5.0, 37.0, 15.0),
        vertex_3d(9.0, 38.0, 18.0),
        vertex_3d(12.0, 33.5, 7.5),
        vertex_3d(7.5, 35.0, 7.5),
        vertex_3d(9.0, 36.5, -22.5),
        vertex_3d(-9.0, 36.5, -22.5),
        vertex_3d(-9.0, 36.5, 22.5),
        vertex_3d(9.0, 36.5, 22.5),
        vertex_3d(-12.0, 32.0, -12.0),
        vertex_3d(12.0, 32.0, -12.0),
        vertex_3d(-12.0, 32.0, 12.0),
        vertex_3d(12.0, 32.0, 12.0),
        vertex_3d(24.0, 30.5, 0.0),
        vertex_3d(18.0, 26.0, -18.0),
        vertex_3d(0.0, 30.5, -24.0),
        vertex_3d(-18.0, 26.0, 18.0),
        vertex_3d(0.0, 30.5, 24.0),
        vertex_3d(-24.0, 30.5, 0.0),
        vertex_3d(18.0, 26.0, 18.0),
        vertex_3d(-18.0, 26.0, -18.0),
        vertex_3d(-24.0, -35.0, 0.0),
        vertex_3d(-18.0, -35.0, 18.0), // 41
        vertex_3d(24.0, -35.0, 0.0),
        vertex_3d(18.0, -35.0, -18.0), // 43
        vertex_3d(0.0, -35.0, -24.0),
        vertex_3d(-18.0, -35.0, -18.0), // 45
        vertex_3d(18.0, -35.0, 18.0), // 46
        vertex_3d(0.0, -35.0, 24.0),
    };

    constexpr inline face_3d butano_house_collision_face(butano_house_vertices, vertex_3d(0, 0, 0), 45, 41, 46, 43, 0, -1);

    constexpr inline int butano_house_cap_top_color = 6;
    constexpr inline int butano_house_cap_top_shading = 7;

    constexpr inline int butano_house_cap_a_color = 6;
    constexpr inline int butano_house_cap_a_shading = 6;

    constexpr inline int butano_house_cap_b_color = 6;
    constexpr inline int butano_house_cap_b_shading = 0;

    constexpr inline int butano_house_handle_up_color = 6;
    constexpr inline int butano_house_handle_up_shading = 6;

    constexpr inline int butano_house_handle_ext_a_color = 6;
    constexpr inline int butano_house_handle_ext_a_shading = 6;

    constexpr inline int butano_house_handle_ext_b_color = 6;
    constexpr inline int butano_house_handle_ext_b_shading = 0;

    constexpr inline int butano_house_cylinder_top_a_color = 0;
    constexpr inline int butano_house_cylinder_top_a_shading = 6;

    constexpr inline int butano_house_cylinder_top_b_color = 0;
    constexpr inline int butano_house_cylinder_top_b_shading = 7;

    constexpr inline int butano_house_cylinder_a_color = 0;
    constexpr inline int butano_house_cylinder_a_shading = 1;

    constexpr inline int butano_house_cylinder_b_color = 0;
    constexpr inline int butano_house_cylinder_b_shading = 0;

    constexpr inline int butano_house_cylinder_bottom_color = 6;
    constexpr inline int butano_house_cylinder_bottom_shading = 0;

    constexpr inline face_3d butano_house_faces[] = {
        face_3d(butano_house_vertices, vertex_3d(0.0, 1.0, 0.0), 0, 1, 2, 3, butano_house_cap_top_color, butano_house_cap_top_shading),
        face_3d(butano_house_vertices, vertex_3d(1.0, 0.0, 0.0), 0, 4, 5, 1, butano_house_cap_a_color, butano_house_cap_a_shading),
        face_3d(butano_house_vertices, vertex_3d(-1.0, 0.0, 0.0), 3, 2, 6, 7, butano_house_cap_a_color, butano_house_cap_a_shading),
        face_3d(butano_house_vertices, vertex_3d(0.0, 0.0, 1.0), 3, 7, 4, 0, butano_house_cap_b_color, butano_house_cap_b_shading),
        face_3d(butano_house_vertices, vertex_3d(0.0, 0.0, -1.0), 2, 1, 5, 6, butano_house_cap_b_color, butano_house_cap_b_shading),
        face_3d(butano_house_vertices, vertex_3d(-0.3128396370962206, 0.9385189112886618, 0.14599183064490295), 8, 9, 10, 11, butano_house_handle_up_color, butano_house_handle_up_shading),
        face_3d(butano_house_vertices, vertex_3d(0.3128396370962206, 0.9385189112886618, 0.14599183064490295), 12, 13, 14, 15, butano_house_handle_up_color, butano_house_handle_up_shading),
        face_3d(butano_house_vertices, vertex_3d(-0.0, 0.9486832980505138, 0.31622776601683794), 12, 15, 11, 10, butano_house_handle_up_color, butano_house_handle_up_shading),
        face_3d(butano_house_vertices, vertex_3d(-0.054312544659356844, 0.9233132592090663, -0.3801878126154979), 16, 17, 18, 19, butano_house_handle_up_color, butano_house_handle_up_shading),
        face_3d(butano_house_vertices, vertex_3d(0.054312544659356844, 0.9233132592090663, -0.3801878126154979), 20, 21, 22, 23, butano_house_handle_up_color, butano_house_handle_up_shading),
        face_3d(butano_house_vertices, vertex_3d(0.0, 0.9486832980505138, -0.31622776601683794), 20, 18, 17, 21, butano_house_handle_up_color, butano_house_handle_up_shading),
        face_3d(butano_house_vertices, vertex_3d(0.0, 0.9486832980505138, -0.31622776601683794), 11, 15, 24, 25, butano_house_handle_ext_a_color, butano_house_handle_ext_a_shading),
        face_3d(butano_house_vertices, vertex_3d(0.0, 0.9486832980505138, 0.31622776601683794), 17, 26, 27, 21, butano_house_handle_ext_a_color, butano_house_handle_ext_a_shading),
        face_3d(butano_house_vertices, vertex_3d(-0.9299811099505542, 0.3487429162314578, -0.11624763874381927), 8, 11, 25, 28, butano_house_handle_ext_b_color, butano_house_handle_ext_b_shading),
        face_3d(butano_house_vertices, vertex_3d(0.9299811099505542, 0.3487429162314578, -0.11624763874381927), 29, 24, 15, 14, butano_house_handle_ext_b_color, butano_house_handle_ext_b_shading),
        face_3d(butano_house_vertices, vertex_3d(-0.9299811099505542, 0.3487429162314578, 0.11624763874381927), 16, 30, 26, 17, butano_house_handle_ext_b_color, butano_house_handle_ext_b_shading),
        face_3d(butano_house_vertices, vertex_3d(0.9299811099505542, 0.3487429162314578, 0.11624763874381927), 31, 22, 21, 27, butano_house_handle_ext_b_color, butano_house_handle_ext_b_shading),
        face_3d(butano_house_vertices, vertex_3d(0.3312945782245396, 0.8834522085987724, -0.3312945782245396), 32, 33, 34, 5, butano_house_cylinder_top_a_color, butano_house_cylinder_top_a_shading),
        face_3d(butano_house_vertices, vertex_3d(-0.33465737284726993, 0.8761938489092158, 0.34682673185989793), 35, 36, 7, 37, butano_house_cylinder_top_a_color, butano_house_cylinder_top_a_shading),
        face_3d(butano_house_vertices, vertex_3d(0.3481553119113957, 0.8703882797784892, 0.3481553119113957), 32, 4, 36, 38, butano_house_cylinder_top_a_color, butano_house_cylinder_top_a_shading),
        face_3d(butano_house_vertices, vertex_3d(-0.3312945782245396, 0.8834522085987724, -0.3312945782245396), 34, 39, 37, 6, butano_house_cylinder_top_a_color, butano_house_cylinder_top_a_shading),
        face_3d(butano_house_vertices, vertex_3d(0.2940858488375231, 0.9557790087219501, -0.0), 4, 32, 5, butano_house_cylinder_top_b_color, butano_house_cylinder_top_b_shading),
        face_3d(butano_house_vertices, vertex_3d(0.0, 0.9557790087219501, -0.2940858488375231), 34, 6, 5, butano_house_cylinder_top_b_color, butano_house_cylinder_top_b_shading),
        face_3d(butano_house_vertices, vertex_3d(-0.2940858488375231, 0.9557790087219501, 0.0), 37, 7, 6, butano_house_cylinder_top_b_color, butano_house_cylinder_top_b_shading),
        face_3d(butano_house_vertices, vertex_3d(0.0, 0.9557790087219501, 0.2940858488375231), 7, 36, 4, butano_house_cylinder_top_b_color, butano_house_cylinder_top_b_shading),
        face_3d(butano_house_vertices, vertex_3d(-0.9486832980505138, 0.0, 0.31622776601683794), 35, 37, 40, 41, butano_house_cylinder_a_color, butano_house_cylinder_a_shading),
        face_3d(butano_house_vertices, vertex_3d(0.9486832980505138, 0.0, -0.31622776601683794), 42, 43, 33, 32, butano_house_cylinder_a_color, butano_house_cylinder_a_shading),
        face_3d(butano_house_vertices, vertex_3d(-0.31622776601683794, 0.0, -0.9486832980505138), 34, 44, 45, 39, butano_house_cylinder_a_color, butano_house_cylinder_a_shading),
        face_3d(butano_house_vertices, vertex_3d(0.31622776601683794, 0.0, 0.9486832980505138), 46, 38, 36, 47, butano_house_cylinder_a_color, butano_house_cylinder_a_shading),
        face_3d(butano_house_vertices, vertex_3d(-0.9486832980505138, 0.0, -0.31622776601683794), 37, 39, 45, 40, butano_house_cylinder_b_color, butano_house_cylinder_b_shading),
        face_3d(butano_house_vertices, vertex_3d(0.31622776601683794, 0.0, -0.9486832980505138), 43, 44, 34, 33, butano_house_cylinder_b_color, butano_house_cylinder_b_shading),
        face_3d(butano_house_vertices, vertex_3d(-0.31622776601683794, 0.0, 0.9486832980505138), 36, 35, 41, 47, butano_house_cylinder_b_color, butano_house_cylinder_b_shading),
        face_3d(butano_house_vertices, vertex_3d(0.9486832980505138, 0.0, 0.31622776601683794), 38, 46, 42, 32, butano_house_cylinder_b_color, butano_house_cylinder_b_shading),
    };

    constexpr inline model_3d_item butano_house(butano_house_vertices, butano_house_faces, &butano_house_collision_face);
}

#endif

