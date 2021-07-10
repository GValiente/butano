/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_START_FLAG_H
#define FR_MODEL_3D_ITEMS_START_FLAG_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d start_flag_vertices[] = {
        vertex_3d(-2.0, 40.0, -29.28),
        vertex_3d(2.0, 40.0, -29.28),
        vertex_3d(2.0, 40.0, -59.53),
        vertex_3d(-2.0, 40.0, -59.53),
        vertex_3d(-2.0, 40.0, 0.97),
        vertex_3d(-2.0, 40.0, 31.22),
        vertex_3d(2.0, 40.0, 31.22),
        vertex_3d(2.0, 40.0, 0.97),
        vertex_3d(-2.0, 25.0, 0.97),
        vertex_3d(-2.0, 25.0, -29.28),
        vertex_3d(-2.0, 10.0, -29.28),
        vertex_3d(-2.0, 10.0, 0.97),
        vertex_3d(-2.0, 25.0, -59.53),
        vertex_3d(2.0, 25.0, -29.28),
        vertex_3d(2.0, 25.0, -59.53),
        vertex_3d(2.0, 25.0, 0.97),
        vertex_3d(2.0, 10.0, 0.97),
        vertex_3d(2.0, 10.0, -29.28),
        vertex_3d(-2.0, 25.0, 31.22),
        vertex_3d(-2.0, 10.0, 31.22),
        vertex_3d(-2.0, 10.0, 61.47),
        vertex_3d(-2.0, 25.0, 61.47),
        vertex_3d(2.0, 25.0, 31.22),
        vertex_3d(2.0, 25.0, 61.47),
        vertex_3d(2.0, 10.0, 61.47),
        vertex_3d(2.0, 10.0, 31.22),
        vertex_3d(-2.0, 40.0, 61.47),
        vertex_3d(2.0, 40.0, 61.47),
        vertex_3d(2.0, 10.0, -59.53),
        vertex_3d(-2.0, 10.0, -59.53),
        vertex_3d(2.0, 40.0, -64.37),
        vertex_3d(-2.0, 40.0, -64.37),
        vertex_3d(2.0, -40.0, -59.53),
        vertex_3d(2.0, -40.0, -64.37),
        vertex_3d(-2.0, -40.0, -64.37),
        vertex_3d(-2.0, -40.0, -59.53),
    };

    constexpr inline int start_flag_white_top_color = 2;
    constexpr inline int start_flag_white_top_shading = 5;

    constexpr inline int start_flag_white_side_a_color = 2;
    constexpr inline int start_flag_white_side_a_shading = 3;

    constexpr inline int start_flag_white_side_b_color = 2;
    constexpr inline int start_flag_white_side_b_shading = 2;

    constexpr inline int start_flag_white_bottom_color = 2;
    constexpr inline int start_flag_white_bottom_shading = 0;

    constexpr inline int start_flag_black_top_color = 6;
    constexpr inline int start_flag_black_top_shading = 7;

    constexpr inline int start_flag_black_side_a_color = 6;
    constexpr inline int start_flag_black_side_a_shading = 2;

    constexpr inline int start_flag_black_side_b_color = 6;
    constexpr inline int start_flag_black_side_b_shading = 0;

    constexpr inline int start_flag_black_bottom_color = 6;
    constexpr inline int start_flag_black_bottom_shading = 0;

    constexpr inline int start_flag_gray_top_color = 3;
    constexpr inline int start_flag_gray_top_shading = 7;

    constexpr inline int start_flag_gray_side_a_color = 3;
    constexpr inline int start_flag_gray_side_a_shading = 4;

    constexpr inline int start_flag_gray_side_b_color = 3;
    constexpr inline int start_flag_gray_side_b_shading = 2;

    constexpr inline int start_flag_gray_bottom_color = 3;
    constexpr inline int start_flag_gray_bottom_shading = 0;

    constexpr inline face_3d start_flag_full_faces[] = {
        face_3d(start_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 0, 1, 2, 3, start_flag_white_top_color, start_flag_white_top_shading),
        face_3d(start_flag_vertices, vertex_3d(0.0, 1.0, 0.0), 4, 5, 6, 7, start_flag_white_top_color, start_flag_white_top_shading),
        face_3d(start_flag_vertices, vertex_3d(-1.0, 0.0, 0.0), 8, 9, 10, 11, start_flag_white_side_a_color, start_flag_white_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(-1.0, 0.0, 0.0), 0, 3, 12, 9, start_flag_white_side_a_color, start_flag_white_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(1.0, 0.0, 0.0), 1, 13, 14, 2, start_flag_white_side_a_color, start_flag_white_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(1.0, 0.0, 0.0), 15, 16, 17, 13, start_flag_white_side_a_color, start_flag_white_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(-1.0, 0.0, 0.0), 18, 19, 20, 21, start_flag_white_side_a_color, start_flag_white_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(1.0, -0.0, 0.0), 22, 23, 24, 25, start_flag_white_side_a_color, start_flag_white_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(1.0, 0.0, 0.0), 15, 7, 6, 22, start_flag_white_side_a_color, start_flag_white_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(-1.0, -0.0, 0.0), 8, 18, 5, 4, start_flag_white_side_a_color, start_flag_white_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(0.0, 0.0, 1.0), 20, 24, 23, 21, start_flag_white_side_b_color, start_flag_white_side_b_shading),
        face_3d(start_flag_vertices, vertex_3d(0.0, -1.0, -0.0), 10, 17, 16, 11, start_flag_white_bottom_color, start_flag_white_bottom_shading),
        face_3d(start_flag_vertices, vertex_3d(0.0, -1.0, -0.0), 19, 25, 24, 20, start_flag_white_bottom_color, start_flag_white_bottom_shading),
        face_3d(start_flag_vertices, vertex_3d(0.0, 1.0, 0.0), 7, 1, 0, 4, start_flag_black_top_color, start_flag_black_top_shading),
        face_3d(start_flag_vertices, vertex_3d(0.0, 1.0, 0.0), 6, 5, 26, 27, start_flag_black_top_color, start_flag_black_top_shading),
        face_3d(start_flag_vertices, vertex_3d(1.0, 0.0, 0.0), 13, 17, 28, 14, start_flag_black_side_a_color, start_flag_black_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(-1.0, 0.0, 0.0), 9, 12, 29, 10, start_flag_black_side_a_color, start_flag_black_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(-1.0, 0.0, 0.0), 4, 0, 9, 8, start_flag_black_side_a_color, start_flag_black_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(1.0, 0.0, 0.0), 7, 15, 13, 1, start_flag_black_side_a_color, start_flag_black_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(-1.0, 0.0, 0.0), 8, 11, 19, 18, start_flag_black_side_a_color, start_flag_black_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(1.0, -0.0, 0.0), 6, 27, 23, 22, start_flag_black_side_a_color, start_flag_black_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(-1.0, 0.0, 0.0), 5, 18, 21, 26, start_flag_black_side_a_color, start_flag_black_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(1.0, -0.0, 0.0), 15, 22, 25, 16, start_flag_black_side_a_color, start_flag_black_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(0.0, 0.0, 1.0), 26, 21, 23, 27, start_flag_black_side_b_color, start_flag_black_side_b_shading),
        face_3d(start_flag_vertices, vertex_3d(0.0, -1.0, 0.0), 17, 10, 29, 28, start_flag_black_bottom_color, start_flag_black_bottom_shading),
        face_3d(start_flag_vertices, vertex_3d(0.0, -1.0, 0.0), 16, 25, 19, 11, start_flag_black_bottom_color, start_flag_black_bottom_shading),
        face_3d(start_flag_vertices, vertex_3d(0.0, 1.0, 0.0), 2, 30, 31, 3, start_flag_gray_top_color, start_flag_gray_top_shading),
        face_3d(start_flag_vertices, vertex_3d(1.0, 0.0, 0.0), 32, 33, 30, 2, start_flag_gray_side_a_color, start_flag_gray_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(-1.0, 0.0, 0.0), 31, 34, 35, 3, start_flag_gray_side_a_color, start_flag_gray_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(-0.0, 0.0, 1.0), 32, 28, 29, 35, start_flag_gray_side_b_color, start_flag_gray_side_b_shading),
        face_3d(start_flag_vertices, vertex_3d(0.0, 0.0, -1.0), 30, 33, 34, 31, start_flag_gray_side_b_color, start_flag_gray_side_b_shading),
        face_3d(start_flag_vertices, vertex_3d(0.0, -1.0, -0.0), 34, 33, 32, 35, start_flag_gray_bottom_color, start_flag_gray_bottom_shading),
    };

    constexpr inline model_3d_item start_flag_full(start_flag_vertices, start_flag_full_faces);

    constexpr inline face_3d start_flag_faces[] = {
        face_3d(start_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 0, 1, 2, 3, start_flag_white_top_color, start_flag_white_top_shading),
        face_3d(start_flag_vertices, vertex_3d(0.0, 1.0, 0.0), 4, 5, 6, 7, start_flag_white_top_color, start_flag_white_top_shading),
        face_3d(start_flag_vertices, vertex_3d(-1.0, 0.0, 0.0), 8, 9, 10, 11, start_flag_white_side_a_color, start_flag_white_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(-1.0, 0.0, 0.0), 0, 3, 12, 9, start_flag_white_side_a_color, start_flag_white_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(1.0, 0.0, 0.0), 1, 13, 14, 2, start_flag_white_side_a_color, start_flag_white_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(1.0, 0.0, 0.0), 15, 16, 17, 13, start_flag_white_side_a_color, start_flag_white_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(-1.0, 0.0, 0.0), 18, 19, 20, 21, start_flag_white_side_a_color, start_flag_white_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(1.0, -0.0, 0.0), 22, 23, 24, 25, start_flag_white_side_a_color, start_flag_white_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(1.0, 0.0, 0.0), 15, 7, 6, 22, start_flag_white_side_a_color, start_flag_white_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(-1.0, -0.0, 0.0), 8, 18, 5, 4, start_flag_white_side_a_color, start_flag_white_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(0.0, 0.0, 1.0), 20, 24, 23, 21, start_flag_white_side_b_color, start_flag_white_side_b_shading),
        face_3d(start_flag_vertices, vertex_3d(0.0, 1.0, 0.0), 7, 1, 0, 4, start_flag_black_top_color, start_flag_black_top_shading),
        face_3d(start_flag_vertices, vertex_3d(0.0, 1.0, 0.0), 6, 5, 26, 27, start_flag_black_top_color, start_flag_black_top_shading),
        face_3d(start_flag_vertices, vertex_3d(1.0, 0.0, 0.0), 13, 17, 28, 14, start_flag_black_side_a_color, start_flag_black_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(-1.0, 0.0, 0.0), 9, 12, 29, 10, start_flag_black_side_a_color, start_flag_black_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(-1.0, 0.0, 0.0), 4, 0, 9, 8, start_flag_black_side_a_color, start_flag_black_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(1.0, 0.0, 0.0), 7, 15, 13, 1, start_flag_black_side_a_color, start_flag_black_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(-1.0, 0.0, 0.0), 8, 11, 19, 18, start_flag_black_side_a_color, start_flag_black_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(1.0, -0.0, 0.0), 6, 27, 23, 22, start_flag_black_side_a_color, start_flag_black_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(-1.0, 0.0, 0.0), 5, 18, 21, 26, start_flag_black_side_a_color, start_flag_black_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(1.0, -0.0, 0.0), 15, 22, 25, 16, start_flag_black_side_a_color, start_flag_black_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(0.0, 0.0, 1.0), 26, 21, 23, 27, start_flag_black_side_b_color, start_flag_black_side_b_shading),
        face_3d(start_flag_vertices, vertex_3d(0.0, 1.0, 0.0), 2, 30, 31, 3, start_flag_gray_top_color, start_flag_gray_top_shading),
        face_3d(start_flag_vertices, vertex_3d(1.0, 0.0, 0.0), 32, 33, 30, 2, start_flag_gray_side_a_color, start_flag_gray_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(-1.0, 0.0, 0.0), 31, 34, 35, 3, start_flag_gray_side_a_color, start_flag_gray_side_a_shading),
        face_3d(start_flag_vertices, vertex_3d(-0.0, 0.0, 1.0), 32, 28, 29, 35, start_flag_gray_side_b_color, start_flag_gray_side_b_shading),
        face_3d(start_flag_vertices, vertex_3d(0.0, 0.0, -1.0), 30, 33, 34, 31, start_flag_gray_side_b_color, start_flag_gray_side_b_shading),
    };

    constexpr inline model_3d_item start_flag(start_flag_vertices, start_flag_faces);
}

#endif

