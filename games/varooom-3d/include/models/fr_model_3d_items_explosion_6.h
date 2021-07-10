/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_EXPLOSION_6_H
#define FR_MODEL_3D_ITEMS_EXPLOSION_6_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d explosion_6_vertices[] = {
        vertex_3d(28.0, 0.0, 0.0),
        vertex_3d(20.0, 8.0, 0.0),
        vertex_3d(12.0, 0.0, 0.0),
        vertex_3d(20.0, -8.0, 0.0),
        vertex_3d(14.0, 0.0, -24.25),
        vertex_3d(10.0, 8.0, -17.32),
        vertex_3d(6.0, 0.0, -10.39),
        vertex_3d(10.0, -8.0, -17.32),
        vertex_3d(-14.0, 0.0, -24.25),
        vertex_3d(-10.0, 8.0, -17.32),
        vertex_3d(-6.0, 0.0, -10.39),
        vertex_3d(-10.0, -8.0, -17.32),
        vertex_3d(-28.0, 0.0, 0.0),
        vertex_3d(-20.0, 8.0, 0.0),
        vertex_3d(-12.0, 0.0, 0.0),
        vertex_3d(-20.0, -8.0, 0.0),
        vertex_3d(-14.0, 0.0, 24.25),
        vertex_3d(-10.0, 8.0, 17.32),
        vertex_3d(-6.0, 0.0, 10.39),
        vertex_3d(-10.0, -8.0, 17.32),
        vertex_3d(14.0, 0.0, 24.25),
        vertex_3d(10.0, 8.0, 17.32),
        vertex_3d(6.0, 0.0, 10.39),
        vertex_3d(10.0, -8.0, 17.32),
    };

    constexpr inline int explosion_6_top_a_color = 7;
    constexpr inline int explosion_6_top_a_shading = 7;

    constexpr inline int explosion_6_top_b_color = 7;
    constexpr inline int explosion_6_top_b_shading = 5;

    constexpr inline int explosion_6_middle_a_color = 7;
    constexpr inline int explosion_6_middle_a_shading = 6;

    constexpr inline int explosion_6_middle_b_color = 7;
    constexpr inline int explosion_6_middle_b_shading = 4;

    constexpr inline face_3d explosion_6_faces[] = {
        face_3d(explosion_6_vertices, vertex_3d(0.6546104312164889, -0.6547049188653364, -0.37795059539058246), 3, 7, 4, 0, explosion_6_top_a_color, explosion_6_top_a_shading),
        face_3d(explosion_6_vertices, vertex_3d(0.0, 0.6547506709810516, -0.7558449304254563), 4, 8, 9, 5, explosion_6_top_a_color, explosion_6_top_a_shading),
        face_3d(explosion_6_vertices, vertex_3d(-0.6546509272536523, -0.6546509272536523, -0.37797397647439507), 11, 15, 12, 8, explosion_6_top_a_color, explosion_6_top_a_shading),
        face_3d(explosion_6_vertices, vertex_3d(-0.6546297160301848, 0.6546972036297755, 0.37793055770814793), 12, 16, 17, 13, explosion_6_top_a_color, explosion_6_top_a_shading),
        face_3d(explosion_6_vertices, vertex_3d(0.0, -0.6547506709810514, 0.7558449304254563), 19, 23, 20, 16, explosion_6_top_a_color, explosion_6_top_a_shading),
        face_3d(explosion_6_vertices, vertex_3d(0.6546586405782042, 0.6546586405782042, 0.37794725641628285), 20, 0, 1, 21, explosion_6_top_a_color, explosion_6_top_a_shading),
        face_3d(explosion_6_vertices, vertex_3d(0.6546297160301848, 0.6546972036297755, -0.37793055770814793), 0, 4, 5, 1, explosion_6_top_b_color, explosion_6_top_b_shading),
        face_3d(explosion_6_vertices, vertex_3d(0.0, -0.6547506709810514, -0.7558449304254563), 7, 11, 8, 4, explosion_6_top_b_color, explosion_6_top_b_shading),
        face_3d(explosion_6_vertices, vertex_3d(-0.6546586405782042, 0.6546586405782042, -0.37794725641628285), 8, 12, 13, 9, explosion_6_top_b_color, explosion_6_top_b_shading),
        face_3d(explosion_6_vertices, vertex_3d(-0.6546104312164889, -0.6547049188653364, 0.37795059539058246), 15, 19, 16, 12, explosion_6_top_b_color, explosion_6_top_b_shading),
        face_3d(explosion_6_vertices, vertex_3d(0.0, 0.6547506709810516, 0.7558449304254563), 16, 20, 21, 17, explosion_6_top_b_color, explosion_6_top_b_shading),
        face_3d(explosion_6_vertices, vertex_3d(0.6546509272536523, -0.6546509272536523, 0.37797397647439507), 23, 3, 0, 20, explosion_6_top_b_color, explosion_6_top_b_shading),
        face_3d(explosion_6_vertices, vertex_3d(-0.6545654232434233, -0.6547229221421824, 0.37799735702218856), 2, 6, 7, 3, explosion_6_middle_a_color, explosion_6_middle_a_shading),
        face_3d(explosion_6_vertices, vertex_3d(0.0, 0.6547506709810514, 0.7558449304254563), 5, 9, 10, 6, explosion_6_middle_a_color, explosion_6_middle_a_shading),
        face_3d(explosion_6_vertices, vertex_3d(0.6546329231329472, -0.6546329231329472, 0.37803633674664894), 10, 14, 15, 11, explosion_6_middle_a_color, explosion_6_middle_a_shading),
        face_3d(explosion_6_vertices, vertex_3d(0.6546104312164889, 0.6547049188653365, -0.37795059539058246), 13, 17, 18, 14, explosion_6_middle_a_color, explosion_6_middle_a_shading),
        face_3d(explosion_6_vertices, vertex_3d(0.0, -0.6547506709810516, -0.7558449304254563), 18, 22, 23, 19, explosion_6_middle_a_color, explosion_6_middle_a_shading),
        face_3d(explosion_6_vertices, vertex_3d(-0.6546509272536523, 0.6546509272536523, -0.37797397647439507), 21, 1, 2, 22, explosion_6_middle_a_color, explosion_6_middle_a_shading),
        face_3d(explosion_6_vertices, vertex_3d(-0.6546104312164889, 0.6547049188653365, 0.37795059539058246), 1, 5, 6, 2, explosion_6_middle_b_color, explosion_6_middle_b_shading),
        face_3d(explosion_6_vertices, vertex_3d(0.0, -0.6547506709810516, 0.7558449304254563), 6, 10, 11, 7, explosion_6_middle_b_color, explosion_6_middle_b_shading),
        face_3d(explosion_6_vertices, vertex_3d(0.6546509272536523, 0.6546509272536523, 0.37797397647439507), 9, 13, 14, 10, explosion_6_middle_b_color, explosion_6_middle_b_shading),
        face_3d(explosion_6_vertices, vertex_3d(0.6545654232434233, -0.6547229221421824, -0.37799735702218856), 14, 18, 19, 15, explosion_6_middle_b_color, explosion_6_middle_b_shading),
        face_3d(explosion_6_vertices, vertex_3d(0.0, 0.6547506709810514, -0.7558449304254563), 17, 21, 22, 18, explosion_6_middle_b_color, explosion_6_middle_b_shading),
        face_3d(explosion_6_vertices, vertex_3d(-0.6546329231329472, -0.6546329231329472, -0.37803633674664894), 22, 2, 3, 23, explosion_6_middle_b_color, explosion_6_middle_b_shading),
    };

    constexpr inline model_3d_item explosion_6(explosion_6_vertices, explosion_6_faces);
}

#endif

