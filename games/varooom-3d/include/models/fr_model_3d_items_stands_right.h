/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * CA3
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_STANDS_RIGHT_H
#define FR_MODEL_3D_ITEMS_STANDS_RIGHT_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d stands_right_vertices[] = {
        vertex_3d(-35.0, -11.56, -39.39),
        vertex_3d(-35.0, 38.19, 35.61),
        vertex_3d(-29.99, 34.63, 30.25),
        vertex_3d(-29.99, -8.0, -34.03),
        vertex_3d(35.0, -11.56, -39.39),
        vertex_3d(35.0, -8.0, -34.03),
        vertex_3d(35.0, 38.19, 35.61),
        vertex_3d(35.0, 34.63, 30.25),
        vertex_3d(35.0, -28.56, -39.39),
        vertex_3d(-35.0, -28.56, -39.39),
        vertex_3d(-35.0, -28.56, 35.61),
        vertex_3d(35.0, -28.56, 35.61),
        vertex_3d(35.0, 23.25, 30.25),
        vertex_3d(35.0, -19.39, -34.03),
        vertex_3d(-29.99, -19.39, -34.03),
        vertex_3d(-29.99, 23.25, 30.25),
    };

    constexpr inline int stands_right_border_up_color = 3;
    constexpr inline int stands_right_border_up_shading = 7;

    constexpr inline int stands_right_border_front_color = 3;
    constexpr inline int stands_right_border_front_shading = 5;

    constexpr inline int stands_right_border_side_color = 3;
    constexpr inline int stands_right_border_side_shading = 3;

    constexpr inline int stands_right_interior_up_color = 3;
    constexpr inline int stands_right_interior_up_shading = 6;

    constexpr inline int stands_right_interior_top_color = 3;
    constexpr inline int stands_right_interior_top_shading = 3;

    constexpr inline int stands_right_interior_side_color = 3;
    constexpr inline int stands_right_interior_side_shading = 1;

    constexpr inline int stands_right_interior_bottom_color = 3;
    constexpr inline int stands_right_interior_bottom_shading = 0;

    constexpr inline face_3d stands_right_faces[] = {
        face_3d(stands_right_vertices, vertex_3d(0.0007540443371482793, 0.8333298814227168, -0.5527754880104022), 0, 1, 2, 3, stands_right_border_up_color, stands_right_border_up_shading),
        face_3d(stands_right_vertices, vertex_3d(0.0, 0.8330054358699549, -0.5532648044210895), 4, 0, 3, 5, stands_right_border_up_color, stands_right_border_up_shading),
        face_3d(stands_right_vertices, vertex_3d(0.0, 0.8330054358699553, -0.553264804421089), 1, 6, 7, 2, stands_right_border_up_color, stands_right_border_up_shading),
        face_3d(stands_right_vertices, vertex_3d(0.0, 0.0, -1.0), 4, 8, 9, 0, stands_right_border_front_color, stands_right_border_front_shading),
        face_3d(stands_right_vertices, vertex_3d(-1.0, 0.0, 0.0), 0, 9, 10, 1, stands_right_border_side_color, stands_right_border_side_shading),
        face_3d(stands_right_vertices, vertex_3d(0.0, 0.8333245446214539, -0.5527840476455941), 12, 13, 14, 15, stands_right_interior_up_color, stands_right_interior_up_shading),
        face_3d(stands_right_vertices, vertex_3d(0.0, 0.0, -1.0), 2, 7, 12, 15, stands_right_interior_top_color, stands_right_interior_top_shading),
        face_3d(stands_right_vertices, vertex_3d(1.0, -0.0, 0.0), 3, 2, 15, 14, stands_right_interior_side_color, stands_right_interior_side_shading),
        face_3d(stands_right_vertices, vertex_3d(0.0, 0.0, 1.0), 5, 3, 14, 13, stands_right_interior_bottom_color, stands_right_interior_bottom_shading),
    };

    constexpr inline face_3d stands_right_collision_face(stands_right_vertices, vertex_3d(0.0, -1.0, 0.0), 11, 10, 9, 8, 0, -1);

    constexpr inline model_3d_item stands_right(stands_right_vertices, stands_right_faces, &stands_right_collision_face);
}

#endif

