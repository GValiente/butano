/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_EXPLOSION_2_H
#define FR_MODEL_3D_ITEMS_EXPLOSION_2_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d explosion_2_vertices[] = {
        vertex_3d(36.0, 0.0, 0.0),
        vertex_3d(20.0, 16.0, 0.0),
        vertex_3d(4.0, 0.0, 0.0),
        vertex_3d(20.0, -16.0, 0.0),
        vertex_3d(18.0, 0.0, -31.18),
        vertex_3d(10.0, 16.0, -17.32),
        vertex_3d(2.0, 0.0, -3.46),
        vertex_3d(10.0, -16.0, -17.32),
        vertex_3d(-18.0, 0.0, -31.18),
        vertex_3d(-10.0, 16.0, -17.32),
        vertex_3d(-2.0, 0.0, -3.46),
        vertex_3d(-10.0, -16.0, -17.32),
        vertex_3d(-36.0, 0.0, 0.0),
        vertex_3d(-20.0, 16.0, 0.0),
        vertex_3d(-4.0, 0.0, 0.0),
        vertex_3d(-20.0, -16.0, 0.0),
        vertex_3d(-18.0, 0.0, 31.18),
        vertex_3d(-10.0, 16.0, 17.32),
        vertex_3d(-2.0, 0.0, 3.46),
        vertex_3d(-10.0, -16.0, 17.32),
        vertex_3d(18.0, 0.0, 31.18),
        vertex_3d(10.0, 16.0, 17.32),
        vertex_3d(2.0, 0.0, 3.46),
        vertex_3d(10.0, -16.0, 17.32),
    };

    constexpr inline int explosion_2_top_a_color = 7;
    constexpr inline int explosion_2_top_a_shading = 7;

    constexpr inline int explosion_2_top_b_color = 7;
    constexpr inline int explosion_2_top_b_shading = 5;

    constexpr inline int explosion_2_middle_a_color = 7;
    constexpr inline int explosion_2_middle_a_shading = 6;

    constexpr inline int explosion_2_middle_b_color = 7;
    constexpr inline int explosion_2_middle_b_shading = 4;

    constexpr inline face_3d explosion_2_faces[] = {
        face_3d(explosion_2_vertices, vertex_3d(0.6546104312164889, -0.6547049188653365, -0.37795059539058246), 3, 7, 4, 0, explosion_2_top_a_color, explosion_2_top_a_shading),
        face_3d(explosion_2_vertices, vertex_3d(0.0, 0.6547506709810516, -0.7558449304254564), 4, 8, 9, 5, explosion_2_top_a_color, explosion_2_top_a_shading),
        face_3d(explosion_2_vertices, vertex_3d(-0.6546509272536523, -0.6546509272536523, -0.37797397647439507), 11, 15, 12, 8, explosion_2_top_a_color, explosion_2_top_a_shading),
        face_3d(explosion_2_vertices, vertex_3d(-0.6546404286964415, 0.6546929175056699, 0.3779194264443857), 12, 16, 17, 13, explosion_2_top_a_color, explosion_2_top_a_shading),
        face_3d(explosion_2_vertices, vertex_3d(0.0, -0.6547506709810514, 0.7558449304254563), 19, 23, 20, 16, explosion_2_top_a_color, explosion_2_top_a_shading),
        face_3d(explosion_2_vertices, vertex_3d(0.6546629250517128, 0.6546629250517128, 0.3779324134358829), 20, 0, 1, 21, explosion_2_top_a_color, explosion_2_top_a_shading),
        face_3d(explosion_2_vertices, vertex_3d(0.6546404286964415, 0.6546929175056699, -0.3779194264443857), 0, 4, 5, 1, explosion_2_top_b_color, explosion_2_top_b_shading),
        face_3d(explosion_2_vertices, vertex_3d(0.0, -0.6547506709810514, -0.7558449304254563), 7, 11, 8, 4, explosion_2_top_b_color, explosion_2_top_b_shading),
        face_3d(explosion_2_vertices, vertex_3d(-0.6546629250517128, 0.6546629250517128, -0.3779324134358829), 8, 12, 13, 9, explosion_2_top_b_color, explosion_2_top_b_shading),
        face_3d(explosion_2_vertices, vertex_3d(-0.6546104312164889, -0.6547049188653365, 0.37795059539058246), 15, 19, 16, 12, explosion_2_top_b_color, explosion_2_top_b_shading),
        face_3d(explosion_2_vertices, vertex_3d(0.0, 0.6547506709810516, 0.7558449304254564), 16, 20, 21, 17, explosion_2_top_b_color, explosion_2_top_b_shading),
        face_3d(explosion_2_vertices, vertex_3d(0.6546509272536523, -0.6546509272536523, 0.37797397647439507), 23, 3, 0, 20, explosion_2_top_b_color, explosion_2_top_b_shading),
        face_3d(explosion_2_vertices, vertex_3d(-0.6543401716307062, -0.6548129607720579, 0.378231313081333), 2, 6, 7, 3, explosion_2_middle_a_color, explosion_2_middle_a_shading),
        face_3d(explosion_2_vertices, vertex_3d(0.0, 0.6547506709810514, 0.7558449304254563), 5, 9, 10, 6, explosion_2_middle_a_color, explosion_2_middle_a_shading),
        face_3d(explosion_2_vertices, vertex_3d(0.6545427687163787, -0.6545427687163787, 0.37834842122334034), 10, 14, 15, 11, explosion_2_middle_a_color, explosion_2_middle_a_shading),
        face_3d(explosion_2_vertices, vertex_3d(0.6546104312164889, 0.6547049188653364, -0.37795059539058246), 13, 17, 18, 14, explosion_2_middle_a_color, explosion_2_middle_a_shading),
        face_3d(explosion_2_vertices, vertex_3d(0.0, -0.6547506709810514, -0.7558449304254562), 18, 22, 23, 19, explosion_2_middle_a_color, explosion_2_middle_a_shading),
        face_3d(explosion_2_vertices, vertex_3d(-0.6546509272536523, 0.6546509272536523, -0.37797397647439507), 21, 1, 2, 22, explosion_2_middle_a_color, explosion_2_middle_a_shading),
        face_3d(explosion_2_vertices, vertex_3d(-0.6546104312164889, 0.6547049188653364, 0.37795059539058246), 1, 5, 6, 2, explosion_2_middle_b_color, explosion_2_middle_b_shading),
        face_3d(explosion_2_vertices, vertex_3d(0.0, -0.6547506709810514, 0.7558449304254562), 6, 10, 11, 7, explosion_2_middle_b_color, explosion_2_middle_b_shading),
        face_3d(explosion_2_vertices, vertex_3d(0.6546509272536523, 0.6546509272536523, 0.37797397647439507), 9, 13, 14, 10, explosion_2_middle_b_color, explosion_2_middle_b_shading),
        face_3d(explosion_2_vertices, vertex_3d(0.6543401716307062, -0.6548129607720579, -0.378231313081333), 14, 18, 19, 15, explosion_2_middle_b_color, explosion_2_middle_b_shading),
        face_3d(explosion_2_vertices, vertex_3d(0.0, 0.6547506709810514, -0.7558449304254563), 17, 21, 22, 18, explosion_2_middle_b_color, explosion_2_middle_b_shading),
        face_3d(explosion_2_vertices, vertex_3d(-0.6545427687163787, -0.6545427687163787, -0.37834842122334034), 22, 2, 3, 23, explosion_2_middle_b_color, explosion_2_middle_b_shading),
    };

    constexpr inline model_3d_item explosion_2(explosion_2_vertices, explosion_2_faces);
}

#endif

