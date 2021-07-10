/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_CHECKPOINT_FLAG_BACKWARDS_H
#define FR_MODEL_3D_ITEMS_CHECKPOINT_FLAG_BACKWARDS_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d checkpoint_flag_backwards_vertices[] = {
        vertex_3d(-2.0, 41.76, 17.22),
        vertex_3d(2.0, 41.76, 17.22),
        vertex_3d(2.0, 41.76, -43.28),
        vertex_3d(-2.0, 41.76, -43.28),
        vertex_3d(-2.0, 34.26, -43.28),
        vertex_3d(-2.0, 34.26, 17.22),
        vertex_3d(-2.0, 11.76, -43.28),
        vertex_3d(-2.0, 11.76, 17.22),
        vertex_3d(-2.0, 19.26, 17.22),
        vertex_3d(-2.0, 19.26, -43.28),
        vertex_3d(2.0, 34.26, 17.22),
        vertex_3d(2.0, 34.26, -43.28),
        vertex_3d(2.0, 11.76, 17.22),
        vertex_3d(2.0, 11.76, -43.28),
        vertex_3d(2.0, 19.26, -43.28),
        vertex_3d(2.0, 19.26, 17.22),
        vertex_3d(2.0, 34.26, 2.1),
        vertex_3d(2.0, 19.26, 2.1),
        vertex_3d(-2.0, 19.26, 2.1),
        vertex_3d(-2.0, 34.26, 2.1),
        vertex_3d(2.0, 41.76, -73.53),
        vertex_3d(-2.0, 41.76, -73.53),
        vertex_3d(-2.0, 41.76, 47.47),
        vertex_3d(2.0, 41.76, 47.47),
        vertex_3d(-2.0, 11.76, -73.53),
        vertex_3d(2.0, 11.76, -73.53),
        vertex_3d(-2.0, 11.76, 47.47),
        vertex_3d(2.0, 11.76, 47.47),
        vertex_3d(2.0, 41.76, -76.62),
        vertex_3d(-2.0, 41.76, -76.62),
        vertex_3d(2.0, -38.24, -73.53),
        vertex_3d(2.0, -38.24, -76.62),
        vertex_3d(-2.0, -38.24, -73.53),
        vertex_3d(-2.0, -38.24, -76.62),
    };

    constexpr inline int checkpoint_flag_backwards_white_top_color = 0;
    constexpr inline int checkpoint_flag_backwards_white_top_shading = 7;

    constexpr inline int checkpoint_flag_backwards_white_side_color = 0;
    constexpr inline int checkpoint_flag_backwards_white_side_shading = 5;

    constexpr inline int checkpoint_flag_backwards_white_bottom_color = 0;
    constexpr inline int checkpoint_flag_backwards_white_bottom_shading = 2;

    constexpr inline int checkpoint_flag_backwards_black_top_color = 7;
    constexpr inline int checkpoint_flag_backwards_black_top_shading = 7;

    constexpr inline int checkpoint_flag_backwards_black_side_1_color = 7;
    constexpr inline int checkpoint_flag_backwards_black_side_1_shading = 5;

    constexpr inline int checkpoint_flag_backwards_black_side_2_color = 7;
    constexpr inline int checkpoint_flag_backwards_black_side_2_shading = 3;

    constexpr inline int checkpoint_flag_backwards_black_bottom_color = 7;
    constexpr inline int checkpoint_flag_backwards_black_bottom_shading = 1;

    constexpr inline int checkpoint_flag_backwards_gray_top_color = 3;
    constexpr inline int checkpoint_flag_backwards_gray_top_shading = 7;

    constexpr inline int checkpoint_flag_backwards_gray_side_a_color = 3;
    constexpr inline int checkpoint_flag_backwards_gray_side_a_shading = 4;

    constexpr inline int checkpoint_flag_backwards_gray_side_b_color = 3;
    constexpr inline int checkpoint_flag_backwards_gray_side_b_shading = 2;

    constexpr inline int checkpoint_flag_backwards_gray_bottom_color = 3;
    constexpr inline int checkpoint_flag_backwards_gray_bottom_shading = 0;

    constexpr inline face_3d checkpoint_flag_backwards_full_faces[] = {
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(0.0, 1.0, 0.0), 0, 1, 2, 3, checkpoint_flag_backwards_white_top_color, checkpoint_flag_backwards_white_top_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(-1.0, 0.0, 0.0), 0, 3, 4, 5, checkpoint_flag_backwards_white_side_color, checkpoint_flag_backwards_white_side_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(-1.0, 0.0, 0.0), 6, 7, 8, 9, checkpoint_flag_backwards_white_side_color, checkpoint_flag_backwards_white_side_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(1.0, 0.0, 0.0), 2, 1, 10, 11, checkpoint_flag_backwards_white_side_color, checkpoint_flag_backwards_white_side_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(1.0, 0.0, 0.0), 12, 13, 14, 15, checkpoint_flag_backwards_white_side_color, checkpoint_flag_backwards_white_side_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(1.0, 0.0, 0.0), 16, 10, 15, 17, checkpoint_flag_backwards_white_side_color, checkpoint_flag_backwards_white_side_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(-1.0, 0.0, 0.0), 18, 8, 5, 19, checkpoint_flag_backwards_white_side_color, checkpoint_flag_backwards_white_side_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(-0.0, -1.0, 0.0), 7, 6, 13, 12, checkpoint_flag_backwards_white_bottom_color, checkpoint_flag_backwards_white_bottom_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(0.0, 1.0, 0.0), 3, 2, 20, 21, checkpoint_flag_backwards_black_top_color, checkpoint_flag_backwards_black_top_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(0.0, 1.0, 0.0), 0, 22, 23, 1, checkpoint_flag_backwards_black_top_color, checkpoint_flag_backwards_black_top_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(-1.0, 0.0, -0.0), 21, 24, 6, 3, checkpoint_flag_backwards_black_side_1_color, checkpoint_flag_backwards_black_side_1_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(1.0, 0.0, 0.0), 25, 20, 2, 13, checkpoint_flag_backwards_black_side_1_color, checkpoint_flag_backwards_black_side_1_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(-1.0, 0.0, 0.0), 22, 0, 7, 26, checkpoint_flag_backwards_black_side_1_color, checkpoint_flag_backwards_black_side_1_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(1.0, 0.0, -0.0), 23, 27, 12, 1, checkpoint_flag_backwards_black_side_1_color, checkpoint_flag_backwards_black_side_1_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(1.0, 0.0, 0.0), 11, 16, 17, 14, checkpoint_flag_backwards_black_side_1_color, checkpoint_flag_backwards_black_side_1_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(-1.0, 0.0, -0.0), 4, 9, 18, 19, checkpoint_flag_backwards_black_side_1_color, checkpoint_flag_backwards_black_side_1_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(0.0, 0.0, 1.0), 26, 27, 23, 22, checkpoint_flag_backwards_black_side_2_color, checkpoint_flag_backwards_black_side_2_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(0.0, -1.0, 0.0), 13, 6, 24, 25, checkpoint_flag_backwards_black_bottom_color, checkpoint_flag_backwards_black_bottom_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(0.0, -1.0, 0.0), 12, 27, 26, 7, checkpoint_flag_backwards_black_bottom_color, checkpoint_flag_backwards_black_bottom_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(0.0, 1.0, 0.0), 21, 20, 28, 29, checkpoint_flag_backwards_gray_top_color, checkpoint_flag_backwards_gray_top_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(1.0, 0.0, 0.0), 28, 20, 30, 31, checkpoint_flag_backwards_gray_side_a_color, checkpoint_flag_backwards_gray_side_a_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(-1.0, 0.0, 0.0), 32, 21, 29, 33, checkpoint_flag_backwards_gray_side_a_color, checkpoint_flag_backwards_gray_side_a_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(0.0, 0.0, -1.0), 33, 29, 28, 31, checkpoint_flag_backwards_gray_side_b_color, checkpoint_flag_backwards_gray_side_b_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(0.0, 0.0, 1.0), 24, 32, 30, 25, checkpoint_flag_backwards_gray_side_b_color, checkpoint_flag_backwards_gray_side_b_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(-0.0, -1.0, 0.0), 32, 33, 31, 30, checkpoint_flag_backwards_gray_bottom_color, checkpoint_flag_backwards_gray_bottom_shading),
    };

    constexpr inline model_3d_item checkpoint_flag_backwards_full(checkpoint_flag_backwards_vertices, checkpoint_flag_backwards_full_faces);

    constexpr inline face_3d checkpoint_flag_backwards_faces[] = {
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(0.0, 1.0, 0.0), 0, 1, 2, 3, checkpoint_flag_backwards_white_top_color, checkpoint_flag_backwards_white_top_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(-1.0, 0.0, 0.0), 0, 3, 4, 5, checkpoint_flag_backwards_white_side_color, checkpoint_flag_backwards_white_side_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(-1.0, 0.0, 0.0), 6, 7, 8, 9, checkpoint_flag_backwards_white_side_color, checkpoint_flag_backwards_white_side_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(1.0, 0.0, 0.0), 2, 1, 10, 11, checkpoint_flag_backwards_white_side_color, checkpoint_flag_backwards_white_side_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(1.0, 0.0, 0.0), 12, 13, 14, 15, checkpoint_flag_backwards_white_side_color, checkpoint_flag_backwards_white_side_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(1.0, 0.0, 0.0), 16, 10, 15, 17, checkpoint_flag_backwards_white_side_color, checkpoint_flag_backwards_white_side_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(-1.0, 0.0, 0.0), 18, 8, 5, 19, checkpoint_flag_backwards_white_side_color, checkpoint_flag_backwards_white_side_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(0.0, 1.0, 0.0), 3, 2, 20, 21, checkpoint_flag_backwards_black_top_color, checkpoint_flag_backwards_black_top_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(0.0, 1.0, 0.0), 0, 22, 23, 1, checkpoint_flag_backwards_black_top_color, checkpoint_flag_backwards_black_top_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(-1.0, 0.0, -0.0), 21, 24, 6, 3, checkpoint_flag_backwards_black_side_1_color, checkpoint_flag_backwards_black_side_1_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(1.0, 0.0, 0.0), 25, 20, 2, 13, checkpoint_flag_backwards_black_side_1_color, checkpoint_flag_backwards_black_side_1_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(-1.0, 0.0, 0.0), 22, 0, 7, 26, checkpoint_flag_backwards_black_side_1_color, checkpoint_flag_backwards_black_side_1_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(1.0, 0.0, -0.0), 23, 27, 12, 1, checkpoint_flag_backwards_black_side_1_color, checkpoint_flag_backwards_black_side_1_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(1.0, 0.0, 0.0), 11, 16, 17, 14, checkpoint_flag_backwards_black_side_1_color, checkpoint_flag_backwards_black_side_1_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(-1.0, 0.0, -0.0), 4, 9, 18, 19, checkpoint_flag_backwards_black_side_1_color, checkpoint_flag_backwards_black_side_1_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(0.0, 0.0, 1.0), 26, 27, 23, 22, checkpoint_flag_backwards_black_side_2_color, checkpoint_flag_backwards_black_side_2_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(0.0, 1.0, 0.0), 21, 20, 28, 29, checkpoint_flag_backwards_gray_top_color, checkpoint_flag_backwards_gray_top_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(1.0, 0.0, 0.0), 28, 20, 30, 31, checkpoint_flag_backwards_gray_side_a_color, checkpoint_flag_backwards_gray_side_a_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(-1.0, 0.0, 0.0), 32, 21, 29, 33, checkpoint_flag_backwards_gray_side_a_color, checkpoint_flag_backwards_gray_side_a_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(0.0, 0.0, -1.0), 33, 29, 28, 31, checkpoint_flag_backwards_gray_side_b_color, checkpoint_flag_backwards_gray_side_b_shading),
        face_3d(checkpoint_flag_backwards_vertices, vertex_3d(0.0, 0.0, 1.0), 24, 32, 30, 25, checkpoint_flag_backwards_gray_side_b_color, checkpoint_flag_backwards_gray_side_b_shading),
    };

    constexpr inline model_3d_item checkpoint_flag_backwards(checkpoint_flag_backwards_vertices, checkpoint_flag_backwards_faces);
}

#endif

