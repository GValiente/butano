/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_TITLE_FLAG_H
#define FR_MODEL_3D_ITEMS_TITLE_FLAG_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d title_flag_vertices[] = {
        vertex_3d(-12.5, 0.0, 83.33),
        vertex_3d(-4.17, 0.0, 83.33),
        vertex_3d(4.17, 0.0, 83.33),
        vertex_3d(12.5, 0.0, 83.33),
        vertex_3d(-20.83, 0.0, 75.0),
        vertex_3d(-12.5, 0.0, 75.0),
        vertex_3d(-4.17, 0.0, 75.0),
        vertex_3d(4.17, 0.0, 75.0),
        vertex_3d(12.5, 0.0, 75.0),
        vertex_3d(20.83, 0.0, 75.0),
        vertex_3d(-20.83, 0.0, 66.67),
        vertex_3d(-12.5, 0.0, 66.67),
        vertex_3d(-4.17, 0.0, 66.67),
        vertex_3d(4.17, 0.0, 66.67),
        vertex_3d(12.5, 0.0, 66.67),
        vertex_3d(20.83, 0.0, 66.67),
        vertex_3d(-20.83, 0.0, 58.33),
        vertex_3d(-12.5, 0.0, 58.33),
        vertex_3d(-4.17, 0.0, 58.33),
        vertex_3d(4.17, 0.0, 58.33),
        vertex_3d(12.5, 0.0, 58.33),
        vertex_3d(20.83, 0.0, 58.33),
        vertex_3d(-20.83, 0.0, 50.0),
        vertex_3d(-12.5, 0.0, 50.0),
        vertex_3d(-4.17, 0.0, 50.0),
        vertex_3d(4.17, 0.0, 50.0),
        vertex_3d(12.5, 0.0, 50.0),
        vertex_3d(20.83, 0.0, 50.0),
        vertex_3d(-20.83, 0.0, 41.67),
        vertex_3d(-12.5, 0.0, 41.67),
        vertex_3d(-4.17, 0.0, 41.67),
        vertex_3d(4.17, 0.0, 41.67),
        vertex_3d(12.5, 0.0, 41.67),
        vertex_3d(20.83, 0.0, 41.67),
        vertex_3d(-20.83, 0.0, 33.33),
        vertex_3d(-12.5, 0.0, 33.33),
        vertex_3d(-4.17, 0.0, 33.33),
        vertex_3d(4.17, 0.0, 33.33),
        vertex_3d(12.5, 0.0, 33.33),
        vertex_3d(20.83, 0.0, 33.33),
        vertex_3d(-20.83, 0.0, 25.0),
        vertex_3d(-12.5, 0.0, 25.0),
        vertex_3d(-4.17, 0.0, 25.0),
        vertex_3d(4.17, 0.0, 25.0),
        vertex_3d(12.5, 0.0, 25.0),
        vertex_3d(20.83, 0.0, 25.0),
        vertex_3d(-20.83, 0.0, 16.67),
        vertex_3d(-12.5, 0.0, 16.67),
        vertex_3d(-4.17, 0.0, 16.67),
        vertex_3d(4.17, 0.0, 16.67),
        vertex_3d(12.5, 0.0, 16.67),
        vertex_3d(20.83, 0.0, 16.67),
        vertex_3d(-20.83, 0.0, 8.33),
        vertex_3d(-12.5, 0.0, 8.33),
        vertex_3d(-4.17, 0.0, 8.33),
        vertex_3d(4.17, 0.0, 8.33),
        vertex_3d(12.5, 0.0, 8.33),
        vertex_3d(20.83, 0.0, 8.33),
        vertex_3d(-20.83, 0.0, 0.0),
        vertex_3d(-12.5, 0.0, 0.0),
        vertex_3d(-4.17, 0.0, 0.0),
        vertex_3d(4.17, 0.0, 0.0),
        vertex_3d(12.5, 0.0, 0.0),
        vertex_3d(20.83, 0.0, 0.0),
    };

    constexpr inline int title_flag_white_color = 0;
    constexpr inline int title_flag_white_shading = -1;

    constexpr inline face_3d title_flag_faces[] = {
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 0, 1, 6, 5, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 2, 3, 8, 7, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 4, 5, 11, 10, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 6, 7, 13, 12, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 8, 9, 15, 14, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 11, 12, 18, 17, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 13, 14, 20, 19, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 16, 17, 23, 22, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, 0.0), 19, 25, 24, 18, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 23, 24, 30, 29, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 25, 26, 32, 31, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, 0.0), 27, 26, 20, 21, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 28, 29, 35, 34, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 30, 31, 37, 36, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 32, 33, 39, 38, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 35, 36, 42, 41, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 37, 38, 44, 43, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 40, 41, 47, 46, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 42, 43, 49, 48, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 44, 45, 51, 50, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 47, 48, 54, 53, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 49, 50, 56, 55, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 52, 53, 59, 58, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 54, 55, 61, 60, title_flag_white_color, title_flag_white_shading),
        face_3d(title_flag_vertices, vertex_3d(0.0, 1.0, -0.0), 56, 57, 63, 62, title_flag_white_color, title_flag_white_shading),
    };

    constexpr inline model_3d_item title_flag(title_flag_vertices, title_flag_faces);
}

#endif

