/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_EXPLOSION_8_H
#define FR_MODEL_3D_ITEMS_EXPLOSION_8_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d explosion_8_vertices[] = {
        vertex_3d(24.0, 0.0, 0.0),
        vertex_3d(20.0, 4.0, 0.0),
        vertex_3d(16.0, 0.0, 0.0),
        vertex_3d(20.0, -4.0, 0.0),
        vertex_3d(12.0, 0.0, -20.78),
        vertex_3d(10.0, 4.0, -17.32),
        vertex_3d(8.0, 0.0, -13.86),
        vertex_3d(10.0, -4.0, -17.32),
        vertex_3d(-12.0, 0.0, -20.78),
        vertex_3d(-10.0, 4.0, -17.32),
        vertex_3d(-8.0, 0.0, -13.86),
        vertex_3d(-10.0, -4.0, -17.32),
        vertex_3d(-24.0, 0.0, 0.0),
        vertex_3d(-20.0, 4.0, 0.0),
        vertex_3d(-16.0, 0.0, 0.0),
        vertex_3d(-20.0, -4.0, 0.0),
        vertex_3d(-12.0, 0.0, 20.78),
        vertex_3d(-10.0, 4.0, 17.32),
        vertex_3d(-8.0, 0.0, 13.86),
        vertex_3d(-10.0, -4.0, 17.32),
        vertex_3d(12.0, 0.0, 20.78),
        vertex_3d(10.0, 4.0, 17.32),
        vertex_3d(8.0, 0.0, 13.86),
        vertex_3d(10.0, -4.0, 17.32),
    };

    constexpr inline int explosion_8_top_a_color = 7;
    constexpr inline int explosion_8_top_a_shading = 7;

    constexpr inline int explosion_8_top_b_color = 7;
    constexpr inline int explosion_8_top_b_shading = 5;

    constexpr inline int explosion_8_middle_a_color = 7;
    constexpr inline int explosion_8_middle_a_shading = 6;

    constexpr inline int explosion_8_middle_b_color = 7;
    constexpr inline int explosion_8_middle_b_shading = 4;

    constexpr inline face_3d explosion_8_faces[] = {
        face_3d(explosion_8_vertices, vertex_3d(0.6548129280934906, -0.654434860583044, -0.3780675104465881), 3, 7, 4, 0, explosion_8_top_a_color, explosion_8_top_a_shading),
        face_3d(explosion_8_vertices, vertex_3d(0.0, 0.6542104006098486, -0.7563126018610965), 4, 8, 9, 5, explosion_8_top_a_color, explosion_8_top_a_shading),
        face_3d(explosion_8_vertices, vertex_3d(-0.6546509272536523, -0.6546509272536525, -0.377973976474395), 11, 15, 12, 8, explosion_8_top_a_color, explosion_8_top_a_shading),
        face_3d(explosion_8_vertices, vertex_3d(-0.6547679368255225, 0.6544528415719972, 0.37811430423033054), 12, 16, 17, 13, explosion_8_top_a_color, explosion_8_top_a_shading),
        face_3d(explosion_8_vertices, vertex_3d(0.0, -0.6542104006098486, 0.7563126018610965), 19, 23, 20, 16, explosion_8_top_a_color, explosion_8_top_a_shading),
        face_3d(explosion_8_vertices, vertex_3d(0.6546329231329472, 0.6546329231329472, 0.37803633674664894), 20, 0, 1, 21, explosion_8_top_a_color, explosion_8_top_a_shading),
        face_3d(explosion_8_vertices, vertex_3d(0.6547679368255225, 0.6544528415719972, -0.37811430423033054), 0, 4, 5, 1, explosion_8_top_b_color, explosion_8_top_b_shading),
        face_3d(explosion_8_vertices, vertex_3d(0.0, -0.6542104006098486, -0.7563126018610965), 7, 11, 8, 4, explosion_8_top_b_color, explosion_8_top_b_shading),
        face_3d(explosion_8_vertices, vertex_3d(-0.6546329231329472, 0.6546329231329472, -0.37803633674664894), 8, 12, 13, 9, explosion_8_top_b_color, explosion_8_top_b_shading),
        face_3d(explosion_8_vertices, vertex_3d(-0.6548129280934906, -0.654434860583044, 0.3780675104465881), 15, 19, 16, 12, explosion_8_top_b_color, explosion_8_top_b_shading),
        face_3d(explosion_8_vertices, vertex_3d(0.0, 0.6542104006098486, 0.7563126018610965), 16, 20, 21, 17, explosion_8_top_b_color, explosion_8_top_b_shading),
        face_3d(explosion_8_vertices, vertex_3d(0.6546509272536523, -0.6546509272536525, 0.377973976474395), 23, 3, 0, 20, explosion_8_top_b_color, explosion_8_top_b_shading),
        face_3d(explosion_8_vertices, vertex_3d(-0.6548803885511436, -0.6544078918783074, 0.37799733826905835), 2, 6, 7, 3, explosion_8_middle_a_color, explosion_8_middle_a_shading),
        face_3d(explosion_8_vertices, vertex_3d(0.0, 0.6542104006098486, 0.7563126018610965), 5, 9, 10, 6, explosion_8_middle_a_color, explosion_8_middle_a_shading),
        face_3d(explosion_8_vertices, vertex_3d(0.6546779167345106, -0.6546779167345106, 0.37788047141963094), 10, 14, 15, 11, explosion_8_middle_a_color, explosion_8_middle_a_shading),
        face_3d(explosion_8_vertices, vertex_3d(0.6548129280934906, 0.654434860583044, -0.3780675104465881), 13, 17, 18, 14, explosion_8_middle_a_color, explosion_8_middle_a_shading),
        face_3d(explosion_8_vertices, vertex_3d(0.0, -0.6542104006098486, -0.7563126018610964), 18, 22, 23, 19, explosion_8_middle_a_color, explosion_8_middle_a_shading),
        face_3d(explosion_8_vertices, vertex_3d(-0.6546509272536523, 0.6546509272536523, -0.37797397647439507), 21, 1, 2, 22, explosion_8_middle_a_color, explosion_8_middle_a_shading),
        face_3d(explosion_8_vertices, vertex_3d(-0.6548129280934906, 0.654434860583044, 0.3780675104465881), 1, 5, 6, 2, explosion_8_middle_b_color, explosion_8_middle_b_shading),
        face_3d(explosion_8_vertices, vertex_3d(0.0, -0.6542104006098486, 0.7563126018610964), 6, 10, 11, 7, explosion_8_middle_b_color, explosion_8_middle_b_shading),
        face_3d(explosion_8_vertices, vertex_3d(0.6546509272536523, 0.6546509272536523, 0.37797397647439507), 9, 13, 14, 10, explosion_8_middle_b_color, explosion_8_middle_b_shading),
        face_3d(explosion_8_vertices, vertex_3d(0.6548803885511436, -0.6544078918783074, -0.37799733826905835), 14, 18, 19, 15, explosion_8_middle_b_color, explosion_8_middle_b_shading),
        face_3d(explosion_8_vertices, vertex_3d(0.0, 0.6542104006098486, -0.7563126018610965), 17, 21, 22, 18, explosion_8_middle_b_color, explosion_8_middle_b_shading),
        face_3d(explosion_8_vertices, vertex_3d(-0.6546779167345106, -0.6546779167345106, -0.37788047141963094), 22, 2, 3, 23, explosion_8_middle_b_color, explosion_8_middle_b_shading),
    };

    constexpr inline model_3d_item explosion_8(explosion_8_vertices, explosion_8_faces);
}

#endif

