/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_POOL_H
#define FR_MODEL_3D_ITEMS_POOL_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d pool_vertices[] = {
        vertex_3d(-25.0, 4.5, -50.0),
        vertex_3d(-25.0, 4.5, 50.0),
        vertex_3d(-21.0, 4.5, 46.0),
        vertex_3d(-21.0, 4.5, -46.0),
        vertex_3d(25.0, 4.5, -50.0),
        vertex_3d(21.0, 4.5, -46.0),
        vertex_3d(25.0, 4.5, 50.0),
        vertex_3d(21.0, 4.5, 46.0),
        vertex_3d(25.0, -10.5, -50.0),
        vertex_3d(-25.0, -10.5, -50.0),
        vertex_3d(25.0, -10.5, 50.0),
        vertex_3d(-25.0, -10.5, 50.0),
        vertex_3d(21.0, 1.5, 46.0),
        vertex_3d(21.0, 1.5, -46.0),
        vertex_3d(-21.0, 1.5, -46.0),
        vertex_3d(-21.0, 1.5, 46.0),
    };

    constexpr inline int pool_border_up_color = 3;
    constexpr inline int pool_border_up_shading = 7;

    constexpr inline int pool_border_front_color = 3;
    constexpr inline int pool_border_front_shading = 6;

    constexpr inline int pool_border_side_color = 3;
    constexpr inline int pool_border_side_shading = 4;

    constexpr inline int pool_border_back_color = 3;
    constexpr inline int pool_border_back_shading = 2;

    constexpr inline int pool_border_down_color = 3;
    constexpr inline int pool_border_down_shading = 0;

    constexpr inline int pool_interior_up_color = 1;
    constexpr inline int pool_interior_up_shading = 7;

    constexpr inline int pool_interior_top_color = 1;
    constexpr inline int pool_interior_top_shading = 5;

    constexpr inline int pool_interior_side_color = 1;
    constexpr inline int pool_interior_side_shading = 3;

    constexpr inline int pool_interior_bottom_color = 1;
    constexpr inline int pool_interior_bottom_shading = 1;

    constexpr inline face_3d pool_full_faces[] = {
        face_3d(pool_vertices, vertex_3d(0.0, 1.0, 0.0), 0, 1, 2, 3, pool_border_up_color, pool_border_up_shading),
        face_3d(pool_vertices, vertex_3d(0.0, 1.0, 0.0), 4, 0, 3, 5, pool_border_up_color, pool_border_up_shading),
        face_3d(pool_vertices, vertex_3d(0.0, 1.0, -0.0), 1, 6, 7, 2, pool_border_up_color, pool_border_up_shading),
        face_3d(pool_vertices, vertex_3d(-0.0, 1.0, 0.0), 6, 4, 5, 7, pool_border_up_color, pool_border_up_shading),
        face_3d(pool_vertices, vertex_3d(0.0, 0.0, -1.0), 4, 8, 9, 0, pool_border_front_color, pool_border_front_shading),
        face_3d(pool_vertices, vertex_3d(1.0, 0.0, 0.0), 6, 10, 8, 4, pool_border_side_color, pool_border_side_shading),
        face_3d(pool_vertices, vertex_3d(-1.0, 0.0, 0.0), 0, 9, 11, 1, pool_border_side_color, pool_border_side_shading),
        face_3d(pool_vertices, vertex_3d(0.0, 0.0, 1.0), 6, 1, 11, 10, pool_border_back_color, pool_border_back_shading),
        face_3d(pool_vertices, vertex_3d(0.0, -1.0, 0.0), 10, 11, 9, 8, pool_border_down_color, pool_border_down_shading),
        face_3d(pool_vertices, vertex_3d(0.0, 1.0, 0.0), 12, 13, 14, 15, pool_interior_up_color, pool_interior_up_shading),
        face_3d(pool_vertices, vertex_3d(0.0, 0.0, -1.0), 2, 7, 12, 15, pool_interior_top_color, pool_interior_top_shading),
        face_3d(pool_vertices, vertex_3d(1.0, -0.0, 0.0), 3, 2, 15, 14, pool_interior_side_color, pool_interior_side_shading),
        face_3d(pool_vertices, vertex_3d(-1.0, 0.0, 0.0), 7, 5, 13, 12, pool_interior_side_color, pool_interior_side_shading),
        face_3d(pool_vertices, vertex_3d(0.0, 0.0, 1.0), 5, 3, 14, 13, pool_interior_bottom_color, pool_interior_bottom_shading),
    };

    constexpr inline const face_3d* pool_collision_face_ptr = &pool_full_faces[8];

    constexpr inline model_3d_item pool_full(pool_vertices, pool_full_faces, pool_collision_face_ptr);

    constexpr inline face_3d pool_faces[] = {
        face_3d(pool_vertices, vertex_3d(0.0, 1.0, 0.0), 0, 1, 2, 3, pool_border_up_color, pool_border_up_shading),
        face_3d(pool_vertices, vertex_3d(0.0, 1.0, 0.0), 4, 0, 3, 5, pool_border_up_color, pool_border_up_shading),
        face_3d(pool_vertices, vertex_3d(0.0, 1.0, -0.0), 1, 6, 7, 2, pool_border_up_color, pool_border_up_shading),
        face_3d(pool_vertices, vertex_3d(-0.0, 1.0, 0.0), 6, 4, 5, 7, pool_border_up_color, pool_border_up_shading),
        face_3d(pool_vertices, vertex_3d(0.0, 0.0, -1.0), 4, 8, 9, 0, pool_border_front_color, pool_border_front_shading),
        face_3d(pool_vertices, vertex_3d(1.0, 0.0, 0.0), 6, 10, 8, 4, pool_border_side_color, pool_border_side_shading),
        face_3d(pool_vertices, vertex_3d(-1.0, 0.0, 0.0), 0, 9, 11, 1, pool_border_side_color, pool_border_side_shading),
        face_3d(pool_vertices, vertex_3d(0.0, 0.0, 1.0), 6, 1, 11, 10, pool_border_back_color, pool_border_back_shading),
        face_3d(pool_vertices, vertex_3d(0.0, 1.0, 0.0), 12, 13, 14, 15, pool_interior_up_color, pool_interior_up_shading),
        face_3d(pool_vertices, vertex_3d(0.0, 0.0, -1.0), 2, 7, 12, 15, pool_interior_top_color, pool_interior_top_shading),
        face_3d(pool_vertices, vertex_3d(1.0, -0.0, 0.0), 3, 2, 15, 14, pool_interior_side_color, pool_interior_side_shading),
        face_3d(pool_vertices, vertex_3d(-1.0, 0.0, 0.0), 7, 5, 13, 12, pool_interior_side_color, pool_interior_side_shading),
        face_3d(pool_vertices, vertex_3d(0.0, 0.0, 1.0), 5, 3, 14, 13, pool_interior_bottom_color, pool_interior_bottom_shading),
    };

    constexpr inline model_3d_item pool(pool_vertices, pool_faces, pool_collision_face_ptr);
}

#endif

