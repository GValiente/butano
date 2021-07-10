/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_SPAM_H
#define FR_MODEL_3D_ITEMS_SPAM_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d spam_vertices[] = {
        vertex_3d(5.0, 30.0, 34.73),
        vertex_3d(5.0, -50.0, 34.73),
        vertex_3d(5.0, 30.0, 54.95),
        vertex_3d(5.0, -50.0, 54.95),
        vertex_3d(-5.0, 30.0, 34.73),
        vertex_3d(-5.0, -50.0, 34.73),
        vertex_3d(-5.0, 30.0, 54.95),
        vertex_3d(-5.0, -50.0, 54.95),
        vertex_3d(-5.0, 5.0, -107.05),
        vertex_3d(5.0, 5.0, -107.05),
        vertex_3d(-5.0, 30.0, -107.05),
        vertex_3d(5.0, 30.0, -107.05),
        vertex_3d(-5.0, 5.0, 34.73),
        vertex_3d(5.0, 5.0, 34.73),
    };

    constexpr inline int spam_yellow_top_color = 7;
    constexpr inline int spam_yellow_top_shading = 5;

    constexpr inline int spam_yellow_side_a_color = 7;
    constexpr inline int spam_yellow_side_a_shading = 2;

    constexpr inline int spam_yellow_side_b_color = 7;
    constexpr inline int spam_yellow_side_b_shading = 3;

    constexpr inline int spam_yellow_bottom_color = 7;
    constexpr inline int spam_yellow_bottom_shading = 0;

    constexpr inline int spam_gray_top_color = 3;
    constexpr inline int spam_gray_top_shading = 7;

    constexpr inline int spam_gray_side_a_color = 3;
    constexpr inline int spam_gray_side_a_shading = 5;

    constexpr inline int spam_gray_side_b_color = 3;
    constexpr inline int spam_gray_side_b_shading = 3;

    constexpr inline int spam_gray_bottom_color = 3;
    constexpr inline int spam_gray_bottom_shading = 1;

    constexpr inline face_3d spam_full_faces[] = {
        face_3d(spam_vertices, vertex_3d(0.0, 1.0, -0.0), 4, 0, 11, 10, spam_yellow_top_color, spam_yellow_top_shading),
        face_3d(spam_vertices, vertex_3d(1.0, 0.0, 0.0), 0, 13, 9, 11, spam_yellow_side_a_color, spam_yellow_side_a_shading),
        face_3d(spam_vertices, vertex_3d(-1.0, 0.0, 0.0), 4, 10, 8, 12, spam_yellow_side_a_color, spam_yellow_side_a_shading),
        face_3d(spam_vertices, vertex_3d(-0.0, 0.0, -1.0), 8, 10, 11, 9, spam_yellow_side_b_color, spam_yellow_side_b_shading),
        face_3d(spam_vertices, vertex_3d(0.0, -1.0, 0.0), 9, 13, 12, 8, spam_yellow_bottom_color, spam_yellow_bottom_shading),
        face_3d(spam_vertices, vertex_3d(0.0, 1.0, 0.0), 2, 0, 4, 6, spam_gray_top_color, spam_gray_top_shading),
        face_3d(spam_vertices, vertex_3d(-1.0, 0.0, 0.0), 7, 6, 4, 5, spam_gray_side_a_color, spam_gray_side_a_shading),
        face_3d(spam_vertices, vertex_3d(1.0, 0.0, 0.0), 1, 0, 2, 3, spam_gray_side_a_color, spam_gray_side_a_shading),
        face_3d(spam_vertices, vertex_3d(-0.0, 0.0, 1.0), 3, 2, 6, 7, spam_gray_side_b_color, spam_gray_side_b_shading),
        face_3d(spam_vertices, vertex_3d(0.0, 0.0, -1.0), 13, 1, 5, 12, spam_gray_side_b_color, spam_gray_side_b_shading),
        face_3d(spam_vertices, vertex_3d(0.0, -1.0, -0.0), 5, 1, 3, 7, spam_gray_bottom_color, spam_gray_bottom_shading),
    };

    constexpr inline const face_3d* spam_collision_face_ptr = &spam_full_faces[10];

    constexpr inline model_3d_item spam_full(spam_vertices, spam_full_faces, spam_collision_face_ptr);

    constexpr inline face_3d spam_faces[] = {
        face_3d(spam_vertices, vertex_3d(0.0, 1.0, -0.0), 4, 0, 11, 10, spam_yellow_top_color, spam_yellow_top_shading),
        face_3d(spam_vertices, vertex_3d(1.0, 0.0, 0.0), 0, 13, 9, 11, spam_yellow_side_a_color, spam_yellow_side_a_shading),
        face_3d(spam_vertices, vertex_3d(-1.0, 0.0, 0.0), 4, 10, 8, 12, spam_yellow_side_a_color, spam_yellow_side_a_shading),
        face_3d(spam_vertices, vertex_3d(-0.0, 0.0, -1.0), 8, 10, 11, 9, spam_yellow_side_b_color, spam_yellow_side_b_shading),
        face_3d(spam_vertices, vertex_3d(0.0, 1.0, 0.0), 2, 0, 4, 6, spam_gray_top_color, spam_gray_top_shading),
        face_3d(spam_vertices, vertex_3d(-1.0, 0.0, 0.0), 7, 6, 4, 5, spam_gray_side_a_color, spam_gray_side_a_shading),
        face_3d(spam_vertices, vertex_3d(1.0, 0.0, 0.0), 1, 0, 2, 3, spam_gray_side_a_color, spam_gray_side_a_shading),
        face_3d(spam_vertices, vertex_3d(-0.0, 0.0, 1.0), 3, 2, 6, 7, spam_gray_side_b_color, spam_gray_side_b_shading),
        face_3d(spam_vertices, vertex_3d(0.0, 0.0, -1.0), 13, 1, 5, 12, spam_gray_side_b_color, spam_gray_side_b_shading),
    };

    constexpr inline model_3d_item spam(spam_vertices, spam_faces, spam_collision_face_ptr);
}

#endif

