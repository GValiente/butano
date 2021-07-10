/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_SMALL_TREE_H
#define FR_MODEL_3D_ITEMS_SMALL_TREE_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d small_tree_vertices[] = {
        vertex_3d(20.0, -12.0 - 15.0, 20.0),
        vertex_3d(0.0, 48.0 - 15.0, 0.0),
        vertex_3d(-20.0, -12.0 - 15.0, 20.0),
        vertex_3d(20.0, -12.0 - 15.0, -20.0),
        vertex_3d(-20.0, -12.0 - 15.0, -20.0),
    };

    constexpr inline int small_tree_green_a_color = 5;
    constexpr inline int small_tree_green_a_shading = 4;

    constexpr inline int small_tree_green_b_color = 5;
    constexpr inline int small_tree_green_b_shading = 6;

    constexpr inline int small_tree_green_c_color = 5;
    constexpr inline int small_tree_green_c_shading = 7;

    constexpr inline int small_tree_green_down_color = 5;
    constexpr inline int small_tree_green_down_shading = 1;

    constexpr inline face_3d small_tree_full_faces[] = {
        face_3d(small_tree_vertices, vertex_3d(-0.0, 0.3162277660168379, 0.9486832980505138), 0, 1, 2, small_tree_green_a_color, small_tree_green_a_shading),
        face_3d(small_tree_vertices, vertex_3d(0.9486832980505138, 0.3162277660168379, 0.0), 3, 1, 0, small_tree_green_b_color, small_tree_green_b_shading),
        face_3d(small_tree_vertices, vertex_3d(-0.9486832980505138, 0.3162277660168379, 0.0), 4, 2, 1, small_tree_green_b_color, small_tree_green_b_shading),
        face_3d(small_tree_vertices, vertex_3d(0.0, 0.3162277660168379, -0.9486832980505138), 1, 3, 4, small_tree_green_c_color, small_tree_green_c_shading),
        face_3d(small_tree_vertices, vertex_3d(0.0, -1.0, 0.0), 3, 0, 2, 4, small_tree_green_down_color, small_tree_green_down_shading),
    };

    constexpr inline model_3d_item small_tree_full(small_tree_vertices, small_tree_full_faces);

    constexpr inline face_3d small_tree_faces[] = {
        face_3d(small_tree_vertices, vertex_3d(-0.0, 0.3162277660168379, 0.9486832980505138), 0, 1, 2, small_tree_green_a_color, small_tree_green_a_shading),
        face_3d(small_tree_vertices, vertex_3d(0.9486832980505138, 0.3162277660168379, 0.0), 3, 1, 0, small_tree_green_b_color, small_tree_green_b_shading),
        face_3d(small_tree_vertices, vertex_3d(-0.9486832980505138, 0.3162277660168379, 0.0), 4, 2, 1, small_tree_green_b_color, small_tree_green_b_shading),
        face_3d(small_tree_vertices, vertex_3d(0.0, 0.3162277660168379, -0.9486832980505138), 1, 3, 4, small_tree_green_c_color, small_tree_green_c_shading),
    };

    constexpr inline model_3d_item small_tree(small_tree_vertices, small_tree_faces);
}

#endif

