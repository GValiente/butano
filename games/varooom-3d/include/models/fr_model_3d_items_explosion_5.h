/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_EXPLOSION_5_H
#define FR_MODEL_3D_ITEMS_EXPLOSION_5_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d explosion_5_vertices[] = {
        vertex_3d(30.0, 0.0, 0.0),
        vertex_3d(20.0, 10.0, 0.0),
        vertex_3d(10.0, 0.0, 0.0),
        vertex_3d(20.0, -10.0, 0.0),
        vertex_3d(15.0, 0.0, -25.98),
        vertex_3d(10.0, 10.0, -17.32),
        vertex_3d(5.0, 0.0, -8.66),
        vertex_3d(10.0, -10.0, -17.32),
        vertex_3d(-15.0, 0.0, -25.98),
        vertex_3d(-10.0, 10.0, -17.32),
        vertex_3d(-5.0, 0.0, -8.66),
        vertex_3d(-10.0, -10.0, -17.32),
        vertex_3d(-30.0, 0.0, 0.0),
        vertex_3d(-20.0, 10.0, 0.0),
        vertex_3d(-10.0, 0.0, 0.0),
        vertex_3d(-20.0, -10.0, 0.0),
        vertex_3d(-15.0, 0.0, 25.98),
        vertex_3d(-10.0, 10.0, 17.32),
        vertex_3d(-5.0, 0.0, 8.66),
        vertex_3d(-10.0, -10.0, 17.32),
        vertex_3d(15.0, 0.0, 25.98),
        vertex_3d(10.0, 10.0, 17.32),
        vertex_3d(5.0, 0.0, 8.66),
        vertex_3d(10.0, -10.0, 17.32),
    };

    constexpr inline int explosion_5_top_a_color = 7;
    constexpr inline int explosion_5_top_a_shading = 7;

    constexpr inline int explosion_5_top_b_color = 7;
    constexpr inline int explosion_5_top_b_shading = 5;

    constexpr inline int explosion_5_middle_a_color = 7;
    constexpr inline int explosion_5_middle_a_shading = 6;

    constexpr inline int explosion_5_middle_b_color = 7;
    constexpr inline int explosion_5_middle_b_shading = 4;

    constexpr inline face_3d explosion_5_faces[] = {
        face_3d(explosion_5_vertices, vertex_3d(0.6546509272536524, -0.6546509272536523, -0.3779739764743951), 3, 7, 4, 0, explosion_5_top_a_color, explosion_5_top_a_shading),
        face_3d(explosion_5_vertices, vertex_3d(0.0, 0.6546426970976187, -0.7559384493044096), 4, 8, 9, 5, explosion_5_top_a_color, explosion_5_top_a_shading),
        face_3d(explosion_5_vertices, vertex_3d(-0.6546509272536523, -0.6546509272536523, -0.3779739764743951), 11, 15, 12, 8, explosion_5_top_a_color, explosion_5_top_a_shading),
        face_3d(explosion_5_vertices, vertex_3d(-0.6546509272536523, 0.6546509272536523, 0.37797397647439507), 12, 16, 17, 13, explosion_5_top_a_color, explosion_5_top_a_shading),
        face_3d(explosion_5_vertices, vertex_3d(0.0, -0.6546426970976187, 0.7559384493044097), 19, 23, 20, 16, explosion_5_top_a_color, explosion_5_top_a_shading),
        face_3d(explosion_5_vertices, vertex_3d(0.6546509272536523, 0.6546509272536523, 0.37797397647439507), 20, 0, 1, 21, explosion_5_top_a_color, explosion_5_top_a_shading),
        face_3d(explosion_5_vertices, vertex_3d(0.6546509272536523, 0.6546509272536523, -0.37797397647439507), 0, 4, 5, 1, explosion_5_top_b_color, explosion_5_top_b_shading),
        face_3d(explosion_5_vertices, vertex_3d(0.0, -0.6546426970976187, -0.7559384493044097), 7, 11, 8, 4, explosion_5_top_b_color, explosion_5_top_b_shading),
        face_3d(explosion_5_vertices, vertex_3d(-0.6546509272536523, 0.6546509272536523, -0.37797397647439507), 8, 12, 13, 9, explosion_5_top_b_color, explosion_5_top_b_shading),
        face_3d(explosion_5_vertices, vertex_3d(-0.6546509272536524, -0.6546509272536523, 0.3779739764743951), 15, 19, 16, 12, explosion_5_top_b_color, explosion_5_top_b_shading),
        face_3d(explosion_5_vertices, vertex_3d(0.0, 0.6546426970976187, 0.7559384493044096), 16, 20, 21, 17, explosion_5_top_b_color, explosion_5_top_b_shading),
        face_3d(explosion_5_vertices, vertex_3d(0.6546509272536523, -0.6546509272536523, 0.3779739764743951), 23, 3, 0, 20, explosion_5_top_b_color, explosion_5_top_b_shading),
        face_3d(explosion_5_vertices, vertex_3d(-0.6546509272536523, -0.6546509272536523, 0.3779739764743951), 2, 6, 7, 3, explosion_5_middle_a_color, explosion_5_middle_a_shading),
        face_3d(explosion_5_vertices, vertex_3d(0.0, 0.6546426970976187, 0.7559384493044097), 5, 9, 10, 6, explosion_5_middle_a_color, explosion_5_middle_a_shading),
        face_3d(explosion_5_vertices, vertex_3d(0.6546509272536523, -0.6546509272536524, 0.3779739764743951), 10, 14, 15, 11, explosion_5_middle_a_color, explosion_5_middle_a_shading),
        face_3d(explosion_5_vertices, vertex_3d(0.6546509272536523, 0.6546509272536524, -0.3779739764743951), 13, 17, 18, 14, explosion_5_middle_a_color, explosion_5_middle_a_shading),
        face_3d(explosion_5_vertices, vertex_3d(0.0, -0.6546426970976188, -0.7559384493044097), 18, 22, 23, 19, explosion_5_middle_a_color, explosion_5_middle_a_shading),
        face_3d(explosion_5_vertices, vertex_3d(-0.6546509272536523, 0.6546509272536523, -0.3779739764743951), 21, 1, 2, 22, explosion_5_middle_a_color, explosion_5_middle_a_shading),
        face_3d(explosion_5_vertices, vertex_3d(-0.6546509272536523, 0.6546509272536524, 0.3779739764743951), 1, 5, 6, 2, explosion_5_middle_b_color, explosion_5_middle_b_shading),
        face_3d(explosion_5_vertices, vertex_3d(0.0, -0.6546426970976188, 0.7559384493044097), 6, 10, 11, 7, explosion_5_middle_b_color, explosion_5_middle_b_shading),
        face_3d(explosion_5_vertices, vertex_3d(0.6546509272536523, 0.6546509272536523, 0.3779739764743951), 9, 13, 14, 10, explosion_5_middle_b_color, explosion_5_middle_b_shading),
        face_3d(explosion_5_vertices, vertex_3d(0.6546509272536523, -0.6546509272536523, -0.3779739764743951), 14, 18, 19, 15, explosion_5_middle_b_color, explosion_5_middle_b_shading),
        face_3d(explosion_5_vertices, vertex_3d(0.0, 0.6546426970976187, -0.7559384493044097), 17, 21, 22, 18, explosion_5_middle_b_color, explosion_5_middle_b_shading),
        face_3d(explosion_5_vertices, vertex_3d(-0.6546509272536523, -0.6546509272536524, -0.3779739764743951), 22, 2, 3, 23, explosion_5_middle_b_color, explosion_5_middle_b_shading),
    };

    constexpr inline model_3d_item explosion_5(explosion_5_vertices, explosion_5_faces);
}

#endif

