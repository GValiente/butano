/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_EXPLOSION_9_H
#define FR_MODEL_3D_ITEMS_EXPLOSION_9_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d explosion_9_vertices[] = {
        vertex_3d(22.0, 0.0, 0.0),
        vertex_3d(20.0, 2.0, 0.0),
        vertex_3d(18.0, 0.0, 0.0),
        vertex_3d(20.0, -2.0, 0.0),
        vertex_3d(11.0, 0.0, -19.05),
        vertex_3d(10.0, 2.0, -17.32),
        vertex_3d(9.0, 0.0, -15.59),
        vertex_3d(10.0, -2.0, -17.32),
        vertex_3d(-11.0, 0.0, -19.05),
        vertex_3d(-10.0, 2.0, -17.32),
        vertex_3d(-9.0, 0.0, -15.59),
        vertex_3d(-10.0, -2.0, -17.32),
        vertex_3d(-22.0, 0.0, 0.0),
        vertex_3d(-20.0, 2.0, 0.0),
        vertex_3d(-18.0, 0.0, 0.0),
        vertex_3d(-20.0, -2.0, 0.0),
        vertex_3d(-11.0, 0.0, 19.05),
        vertex_3d(-10.0, 2.0, 17.32),
        vertex_3d(-9.0, 0.0, 15.59),
        vertex_3d(-10.0, -2.0, 17.32),
        vertex_3d(11.0, 0.0, 19.05),
        vertex_3d(10.0, 2.0, 17.32),
        vertex_3d(9.0, 0.0, 15.59),
        vertex_3d(10.0, -2.0, 17.32),
    };

    constexpr inline int explosion_9_top_a_color = 7;
    constexpr inline int explosion_9_top_a_shading = 7;

    constexpr inline int explosion_9_top_b_color = 7;
    constexpr inline int explosion_9_top_b_shading = 5;

    constexpr inline int explosion_9_middle_a_color = 7;
    constexpr inline int explosion_9_middle_a_shading = 6;

    constexpr inline int explosion_9_middle_b_color = 7;
    constexpr inline int explosion_9_middle_b_shading = 4;

    constexpr inline face_3d explosion_9_faces[] = {
        face_3d(explosion_9_vertices, vertex_3d(0.6548129280934906, -0.654434860583044, -0.3780675104465881), 3, 7, 4, 0, explosion_9_top_a_color, explosion_9_top_a_shading),
        face_3d(explosion_9_vertices, vertex_3d(0.0, 0.6542104006098486, -0.7563126018610964), 4, 8, 9, 5, explosion_9_top_a_color, explosion_9_top_a_shading),
        face_3d(explosion_9_vertices, vertex_3d(-0.6546509272536523, -0.6546509272536525, -0.377973976474395), 11, 15, 12, 8, explosion_9_top_a_color, explosion_9_top_a_shading),
        face_3d(explosion_9_vertices, vertex_3d(-0.654788389150739, 0.6544446682115524, 0.3780930331054136), 12, 16, 17, 13, explosion_9_top_a_color, explosion_9_top_a_shading),
        face_3d(explosion_9_vertices, vertex_3d(0.0, -0.6542104006098486, 0.7563126018610965), 19, 23, 20, 16, explosion_9_top_a_color, explosion_9_top_a_shading),
        face_3d(explosion_9_vertices, vertex_3d(0.6546411079287983, 0.6546411079287983, 0.3780079888302772), 20, 0, 1, 21, explosion_9_top_a_color, explosion_9_top_a_shading),
        face_3d(explosion_9_vertices, vertex_3d(0.654788389150739, 0.6544446682115524, -0.3780930331054136), 0, 4, 5, 1, explosion_9_top_b_color, explosion_9_top_b_shading),
        face_3d(explosion_9_vertices, vertex_3d(0.0, -0.6542104006098486, -0.7563126018610965), 7, 11, 8, 4, explosion_9_top_b_color, explosion_9_top_b_shading),
        face_3d(explosion_9_vertices, vertex_3d(-0.6546411079287983, 0.6546411079287983, -0.3780079888302772), 8, 12, 13, 9, explosion_9_top_b_color, explosion_9_top_b_shading),
        face_3d(explosion_9_vertices, vertex_3d(-0.6548129280934906, -0.654434860583044, 0.3780675104465881), 15, 19, 16, 12, explosion_9_top_b_color, explosion_9_top_b_shading),
        face_3d(explosion_9_vertices, vertex_3d(0.0, 0.6542104006098486, 0.7563126018610964), 16, 20, 21, 17, explosion_9_top_b_color, explosion_9_top_b_shading),
        face_3d(explosion_9_vertices, vertex_3d(0.6546509272536523, -0.6546509272536525, 0.377973976474395), 23, 3, 0, 20, explosion_9_top_b_color, explosion_9_top_b_shading),
        face_3d(explosion_9_vertices, vertex_3d(-0.6548429144359043, -0.6544228740802687, 0.37803632007204224), 2, 6, 7, 3, explosion_9_middle_a_color, explosion_9_middle_a_shading),
        face_3d(explosion_9_vertices, vertex_3d(0.0, 0.6542104006098486, 0.7563126018610965), 5, 9, 10, 6, explosion_9_middle_a_color, explosion_9_middle_a_shading),
        face_3d(explosion_9_vertices, vertex_3d(0.6546629250517126, -0.6546629250517129, 0.37793241343588285), 10, 14, 15, 11, explosion_9_middle_a_color, explosion_9_middle_a_shading),
        face_3d(explosion_9_vertices, vertex_3d(0.6548129280934906, 0.654434860583044, -0.3780675104465881), 13, 17, 18, 14, explosion_9_middle_a_color, explosion_9_middle_a_shading),
        face_3d(explosion_9_vertices, vertex_3d(0.0, -0.6542104006098486, -0.7563126018610965), 18, 22, 23, 19, explosion_9_middle_a_color, explosion_9_middle_a_shading),
        face_3d(explosion_9_vertices, vertex_3d(-0.6546509272536523, 0.6546509272536523, -0.37797397647439507), 21, 1, 2, 22, explosion_9_middle_a_color, explosion_9_middle_a_shading),
        face_3d(explosion_9_vertices, vertex_3d(-0.6548129280934906, 0.654434860583044, 0.3780675104465881), 1, 5, 6, 2, explosion_9_middle_b_color, explosion_9_middle_b_shading),
        face_3d(explosion_9_vertices, vertex_3d(0.0, -0.6542104006098486, 0.7563126018610965), 6, 10, 11, 7, explosion_9_middle_b_color, explosion_9_middle_b_shading),
        face_3d(explosion_9_vertices, vertex_3d(0.6546509272536523, 0.6546509272536523, 0.37797397647439507), 9, 13, 14, 10, explosion_9_middle_b_color, explosion_9_middle_b_shading),
        face_3d(explosion_9_vertices, vertex_3d(0.6548429144359043, -0.6544228740802687, -0.37803632007204224), 14, 18, 19, 15, explosion_9_middle_b_color, explosion_9_middle_b_shading),
        face_3d(explosion_9_vertices, vertex_3d(0.0, 0.6542104006098486, -0.7563126018610965), 17, 21, 22, 18, explosion_9_middle_b_color, explosion_9_middle_b_shading),
        face_3d(explosion_9_vertices, vertex_3d(-0.6546629250517126, -0.6546629250517129, -0.37793241343588285), 22, 2, 3, 23, explosion_9_middle_b_color, explosion_9_middle_b_shading),
    };

    constexpr inline model_3d_item explosion_9(explosion_9_vertices, explosion_9_faces);
}

#endif

