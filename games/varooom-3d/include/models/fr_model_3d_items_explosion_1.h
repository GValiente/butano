/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_EXPLOSION_1_H
#define FR_MODEL_3D_ITEMS_EXPLOSION_1_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d explosion_1_vertices[] = {
        vertex_3d(38.0, 0.0, 0.0),
        vertex_3d(20.0, 18.0, 0.0),
        vertex_3d(2.0, 0.0, 0.0),
        vertex_3d(20.0, -18.0, 0.0),
        vertex_3d(19.0, 0.0, -32.91),
        vertex_3d(10.0, 18.0, -17.32),
        vertex_3d(1.0, 0.0, -1.73),
        vertex_3d(10.0, -18.0, -17.32),
        vertex_3d(-19.0, 0.0, -32.91),
        vertex_3d(-10.0, 18.0, -17.32),
        vertex_3d(-1.0, 0.0, -1.73),
        vertex_3d(-10.0, -18.0, -17.32),
        vertex_3d(-38.0, 0.0, 0.0),
        vertex_3d(-20.0, 18.0, 0.0),
        vertex_3d(-2.0, 0.0, 0.0),
        vertex_3d(-20.0, -18.0, 0.0),
        vertex_3d(-19.0, 0.0, 32.91),
        vertex_3d(-10.0, 18.0, 17.32),
        vertex_3d(-1.0, 0.0, 1.73),
        vertex_3d(-10.0, -18.0, 17.32),
        vertex_3d(19.0, 0.0, 32.91),
        vertex_3d(10.0, 18.0, 17.32),
        vertex_3d(1.0, 0.0, 1.73),
        vertex_3d(10.0, -18.0, 17.32),
    };

    constexpr inline int explosion_1_top_a_color = 7;
    constexpr inline int explosion_1_top_a_shading = 7;

    constexpr inline int explosion_1_top_b_color = 7;
    constexpr inline int explosion_1_top_b_shading = 5;

    constexpr inline int explosion_1_middle_a_color = 7;
    constexpr inline int explosion_1_middle_a_shading = 6;

    constexpr inline int explosion_1_middle_b_color = 7;
    constexpr inline int explosion_1_middle_b_shading = 4;

    constexpr inline face_3d explosion_1_faces[] = {
        face_3d(explosion_1_vertices, vertex_3d(0.6546329288086787, -0.6546749247625413, -0.3779635847625166), 3, 7, 4, 0, explosion_1_top_a_color, explosion_1_top_a_shading),
        face_3d(explosion_1_vertices, vertex_3d(0.0, 0.6546906904378867, -0.7558968844055141), 4, 8, 9, 5, explosion_1_top_a_color, explosion_1_top_a_shading),
        face_3d(explosion_1_vertices, vertex_3d(-0.6546509272536523, -0.6546509272536524, -0.37797397647439507), 11, 15, 12, 8, explosion_1_top_a_color, explosion_1_top_a_shading),
        face_3d(explosion_1_vertices, vertex_3d(-0.6546471383366209, 0.6546692406067292, 0.37794881885128534), 12, 16, 17, 13, explosion_1_top_a_color, explosion_1_top_a_shading),
        face_3d(explosion_1_vertices, vertex_3d(0.0, -0.6546906904378869, 0.7558968844055141), 19, 23, 20, 16, explosion_1_top_a_color, explosion_1_top_a_shading),
        face_3d(explosion_1_vertices, vertex_3d(0.6546566109145544, 0.6546566109145545, 0.3779542876747656), 20, 0, 1, 21, explosion_1_top_a_color, explosion_1_top_a_shading),
        face_3d(explosion_1_vertices, vertex_3d(0.6546471383366209, 0.6546692406067292, -0.37794881885128534), 0, 4, 5, 1, explosion_1_top_b_color, explosion_1_top_b_shading),
        face_3d(explosion_1_vertices, vertex_3d(0.0, -0.6546906904378869, -0.7558968844055141), 7, 11, 8, 4, explosion_1_top_b_color, explosion_1_top_b_shading),
        face_3d(explosion_1_vertices, vertex_3d(-0.6546566109145544, 0.6546566109145545, -0.3779542876747656), 8, 12, 13, 9, explosion_1_top_b_color, explosion_1_top_b_shading),
        face_3d(explosion_1_vertices, vertex_3d(-0.6546329288086787, -0.6546749247625413, 0.3779635847625166), 15, 19, 16, 12, explosion_1_top_b_color, explosion_1_top_b_shading),
        face_3d(explosion_1_vertices, vertex_3d(0.0, 0.6546906904378867, 0.7558968844055141), 16, 20, 21, 17, explosion_1_top_b_color, explosion_1_top_b_shading),
        face_3d(explosion_1_vertices, vertex_3d(0.6546509272536523, -0.6546509272536524, 0.37797397647439507), 23, 3, 0, 20, explosion_1_top_b_color, explosion_1_top_b_shading),
        face_3d(explosion_1_vertices, vertex_3d(-0.6543626803519171, -0.6547829518229202, 0.37824432390284224), 2, 6, 7, 3, explosion_1_middle_a_color, explosion_1_middle_a_shading),
        face_3d(explosion_1_vertices, vertex_3d(0.0, 0.654690690437887, 0.7558968844055141), 5, 9, 10, 6, explosion_1_middle_a_color, explosion_1_middle_a_shading),
        face_3d(explosion_1_vertices, vertex_3d(0.6545427687163788, -0.6545427687163788, 0.37834842122334034), 10, 14, 15, 11, explosion_1_middle_a_color, explosion_1_middle_a_shading),
        face_3d(explosion_1_vertices, vertex_3d(0.6546329288086787, 0.6546749247625413, -0.3779635847625166), 13, 17, 18, 14, explosion_1_middle_a_color, explosion_1_middle_a_shading),
        face_3d(explosion_1_vertices, vertex_3d(0.0, -0.654690690437887, -0.755896884405514), 18, 22, 23, 19, explosion_1_middle_a_color, explosion_1_middle_a_shading),
        face_3d(explosion_1_vertices, vertex_3d(-0.6546509272536523, 0.6546509272536523, -0.3779739764743951), 21, 1, 2, 22, explosion_1_middle_a_color, explosion_1_middle_a_shading),
        face_3d(explosion_1_vertices, vertex_3d(-0.6546329288086787, 0.6546749247625413, 0.3779635847625166), 1, 5, 6, 2, explosion_1_middle_b_color, explosion_1_middle_b_shading),
        face_3d(explosion_1_vertices, vertex_3d(0.0, -0.654690690437887, 0.755896884405514), 6, 10, 11, 7, explosion_1_middle_b_color, explosion_1_middle_b_shading),
        face_3d(explosion_1_vertices, vertex_3d(0.6546509272536523, 0.6546509272536523, 0.3779739764743951), 9, 13, 14, 10, explosion_1_middle_b_color, explosion_1_middle_b_shading),
        face_3d(explosion_1_vertices, vertex_3d(0.6543626803519171, -0.6547829518229202, -0.37824432390284224), 14, 18, 19, 15, explosion_1_middle_b_color, explosion_1_middle_b_shading),
        face_3d(explosion_1_vertices, vertex_3d(0.0, 0.654690690437887, -0.7558968844055141), 17, 21, 22, 18, explosion_1_middle_b_color, explosion_1_middle_b_shading),
        face_3d(explosion_1_vertices, vertex_3d(-0.6545427687163788, -0.6545427687163788, -0.37834842122334034), 22, 2, 3, 23, explosion_1_middle_b_color, explosion_1_middle_b_shading),
    };

    constexpr inline model_3d_item explosion_1(explosion_1_vertices, explosion_1_faces);
}

#endif

