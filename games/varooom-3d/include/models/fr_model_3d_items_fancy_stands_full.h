/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_FANCY_STANDS_FULL_H
#define FR_MODEL_3D_ITEMS_FANCY_STANDS_FULL_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d fancy_stands_full_vertices[] = {
        vertex_3d(-32.0, 10.48, 0.0),
        vertex_3d(-25.0, 20.48, 0.0),
        vertex_3d(0.0, 40.48, -35.0),
        vertex_3d(-32.0, 10.48, -67.0),
        vertex_3d(0.0, 40.48, 35.0),
        vertex_3d(-32.0, 10.48, 67.0),
        vertex_3d(25.0, 20.48, 0.0),
        vertex_3d(32.0, 10.48, -67.0),
        vertex_3d(32.0, 10.48, 67.0),
        vertex_3d(32.0, 10.48, 0.0),
        vertex_3d(-30.0, 10.48, -60.0),
        vertex_3d(-30.0, 10.48, -65.0),
        vertex_3d(-30.0, -9.52, -65.0),
        vertex_3d(-30.0, -9.52, -60.0),
        vertex_3d(-30.0, 10.48, 65.0),
        vertex_3d(-30.0, 10.48, 60.0),
        vertex_3d(-30.0, -9.52, 60.0),
        vertex_3d(-30.0, -9.52, 65.0),
        vertex_3d(-30.0, 10.48, 2.5),
        vertex_3d(-30.0, 10.48, -2.5),
        vertex_3d(-30.0, -9.52, -2.5),
        vertex_3d(-30.0, -9.52, 2.5),
        vertex_3d(30.0, 10.48, -65.0),
        vertex_3d(30.0, -9.52, -65.0),
        vertex_3d(25.0, -9.52, -65.0),
        vertex_3d(25.0, 10.48, -65.0),
        vertex_3d(30.0, 10.48, 65.0),
        vertex_3d(25.0, 10.48, 65.0),
        vertex_3d(25.0, -9.52, 65.0),
        vertex_3d(30.0, -9.52, 65.0),
        vertex_3d(-25.0, 10.48, -65.0),
        vertex_3d(-25.0, -9.52, -65.0),
        vertex_3d(-25.0, 10.48, 65.0),
        vertex_3d(-25.0, -9.52, 65.0),
        vertex_3d(30.0, 10.48, -60.0),
        vertex_3d(30.0, -9.52, -60.0),
        vertex_3d(30.0, 10.48, 60.0),
        vertex_3d(30.0, -9.52, 60.0),
        vertex_3d(30.0, 10.48, -2.5),
        vertex_3d(30.0, 10.48, 2.5),
        vertex_3d(30.0, -9.52, 2.5),
        vertex_3d(30.0, -9.52, -2.5),
        vertex_3d(-25.0, 10.48, 0.0),
        vertex_3d(-25.0, -9.52, 0.0),
        vertex_3d(25.0, -9.52, 0.0),
        vertex_3d(25.0, 10.48, 0.0),
        vertex_3d(-35.0, -39.52, 70.0),
        vertex_3d(-35.0, -9.52, 70.0),
        vertex_3d(-35.0, -9.52, -70.0),
        vertex_3d(-35.0, -39.52, -70.0),
        vertex_3d(35.0, -39.52, 70.0),
        vertex_3d(35.0, -9.52, 70.0),
        vertex_3d(35.0, -9.52, -70.0),
        vertex_3d(35.0, -39.52, -70.0),
        vertex_3d(35.0, -39.52, 0.0),
        vertex_3d(35.0, -9.52, 0.0),
        vertex_3d(-35.0, -9.52, 0.0),
        vertex_3d(-35.0, -39.52, 0.0),
    };

    constexpr inline int fancy_stands_full_roof_front_color = 7;
    constexpr inline int fancy_stands_full_roof_front_shading = 7;

    constexpr inline int fancy_stands_full_roof_side_a_color = 7;
    constexpr inline int fancy_stands_full_roof_side_a_shading = 5;

    constexpr inline int fancy_stands_full_roof_side_b_color = 7;
    constexpr inline int fancy_stands_full_roof_side_b_shading = 4;

    constexpr inline int fancy_stands_full_roof_back_color = 7;
    constexpr inline int fancy_stands_full_roof_back_shading = 2;

    constexpr inline int fancy_stands_full_roof_bottom_color = 7;
    constexpr inline int fancy_stands_full_roof_bottom_shading = 0;

    constexpr inline int fancy_stands_full_stick_front_color = 6;
    constexpr inline int fancy_stands_full_stick_front_shading = 7;

    constexpr inline int fancy_stands_full_stick_side_color = 6;
    constexpr inline int fancy_stands_full_stick_side_shading = 6;

    constexpr inline int fancy_stands_full_stick_back_color = 6;
    constexpr inline int fancy_stands_full_stick_back_shading = 1;

    constexpr inline int fancy_stands_full_stick_interior_color = 6;
    constexpr inline int fancy_stands_full_stick_interior_shading = 1;

    constexpr inline int fancy_stands_full_stand_front_color = 3;
    constexpr inline int fancy_stands_full_stand_front_shading = 7;

    constexpr inline int fancy_stands_full_stand_side_color = 3;
    constexpr inline int fancy_stands_full_stand_side_shading = 6;

    constexpr inline int fancy_stands_full_stand_back_color = 3;
    constexpr inline int fancy_stands_full_stand_back_shading = 4;

    constexpr inline int fancy_stands_full_stand_up_color = 3;
    constexpr inline int fancy_stands_full_stand_up_shading = 2;

    constexpr inline int fancy_stands_full_stand_down_color = 3;
    constexpr inline int fancy_stands_full_stand_down_shading = 0;

    constexpr inline face_3d fancy_stands_full_faces[] = {
        face_3d(fancy_stands_full_vertices, vertex_3d(-0.6839411288813297, 0.7295372041400852, 0.0), 0, 1, 2, 3, fancy_stands_full_roof_front_color, fancy_stands_full_roof_front_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(-0.6839411288813296, 0.729537204140085, 0.0), 4, 1, 0, 5, fancy_stands_full_roof_front_color, fancy_stands_full_roof_front_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(0.0, 0.8682431421244593, -0.49613893835683376), 6, 1, 4, fancy_stands_full_roof_side_a_color, fancy_stands_full_roof_side_a_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(0.0, 0.7295372041400852, -0.6839411288813297), 2, 7, 3, fancy_stands_full_roof_side_a_color, fancy_stands_full_roof_side_a_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(0.0, 0.7295372041400852, 0.6839411288813297), 5, 8, 4, fancy_stands_full_roof_side_b_color, fancy_stands_full_roof_side_b_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(0.0, 0.8682431421244593, 0.49613893835683376), 1, 6, 2, fancy_stands_full_roof_side_b_color, fancy_stands_full_roof_side_b_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(0.6839411288813296, 0.729537204140085, 0.0), 7, 2, 6, 9, fancy_stands_full_roof_back_color, fancy_stands_full_roof_back_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(0.6839411288813297, 0.7295372041400852, 0.0), 6, 4, 8, 9, fancy_stands_full_roof_back_color, fancy_stands_full_roof_back_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(0.0, -1.0, 0.0), 7, 9, 0, 3, fancy_stands_full_roof_bottom_color, fancy_stands_full_roof_bottom_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(-0.0, -1.0, 0.0), 5, 0, 9, 8, fancy_stands_full_roof_bottom_color, fancy_stands_full_roof_bottom_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(-1.0, 0.0, 0.0), 10, 11, 12, 13, fancy_stands_full_stick_front_color, fancy_stands_full_stick_front_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(-1.0, 0.0, 0.0), 14, 15, 16, 17, fancy_stands_full_stick_front_color, fancy_stands_full_stick_front_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(-1.0, 0.0, 0.0), 18, 19, 20, 21, fancy_stands_full_stick_front_color, fancy_stands_full_stick_front_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(0.0, 0.0, -1.0), 22, 23, 24, 25, fancy_stands_full_stick_side_color, fancy_stands_full_stick_side_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(0.0, -0.0, 1.0), 26, 27, 28, 29, fancy_stands_full_stick_side_color, fancy_stands_full_stick_side_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(0.0, 0.0, -1.0), 30, 31, 12, 11, fancy_stands_full_stick_side_color, fancy_stands_full_stick_side_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(0.0, -0.0, 1.0), 32, 14, 17, 33, fancy_stands_full_stick_side_color, fancy_stands_full_stick_side_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(1.0, 0.0, 0.0), 22, 34, 35, 23, fancy_stands_full_stick_back_color, fancy_stands_full_stick_back_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(1.0, 0.0, 0.0), 36, 26, 29, 37, fancy_stands_full_stick_back_color, fancy_stands_full_stick_back_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(1.0, 0.0, 0.0), 38, 39, 40, 41, fancy_stands_full_stick_back_color, fancy_stands_full_stick_back_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(-0.7071067811865475, 0.0, 0.7071067811865475), 34, 25, 24, 35, fancy_stands_full_stick_interior_color, fancy_stands_full_stick_interior_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(-0.7071067811865475, 0.0, -0.7071067811865475), 36, 37, 28, 27, fancy_stands_full_stick_interior_color, fancy_stands_full_stick_interior_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(0.7071067811865475, -0.0, 0.7071067811865475), 30, 10, 13, 31, fancy_stands_full_stick_interior_color, fancy_stands_full_stick_interior_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(0.7071067811865475, 0.0, -0.7071067811865475), 32, 33, 16, 15, fancy_stands_full_stick_interior_color, fancy_stands_full_stick_interior_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(0.4472135954999579, -0.0, 0.8944271909999159), 42, 18, 21, 43, fancy_stands_full_stick_interior_color, fancy_stands_full_stick_interior_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(0.4472135954999579, 0.0, -0.8944271909999159), 42, 43, 20, 19, fancy_stands_full_stick_interior_color, fancy_stands_full_stick_interior_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(-0.4472135954999579, 0.0, -0.8944271909999159), 38, 41, 44, 45, fancy_stands_full_stick_interior_color, fancy_stands_full_stick_interior_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(-0.4472135954999579, 0.0, 0.8944271909999159), 39, 45, 44, 40, fancy_stands_full_stick_interior_color, fancy_stands_full_stick_interior_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(-1.0, 0.0, 0.0), 49, 57, 56, 48, fancy_stands_full_stand_front_color, fancy_stands_full_stand_front_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(-1.0, 0.0, 0.0), 46, 47, 56, 57, fancy_stands_full_stand_front_color, fancy_stands_full_stand_front_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(0.0, 0.0, 1.0), 50, 51, 47, 46, fancy_stands_full_stand_side_color, fancy_stands_full_stand_side_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(0.0, 0.0, -1.0), 49, 48, 52, 53, fancy_stands_full_stand_side_color, fancy_stands_full_stand_side_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(1.0, 0.0, -0.0), 51, 50, 54, 55, fancy_stands_full_stand_back_color, fancy_stands_full_stand_back_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(1.0, 0.0, 0.0), 52, 55, 54, 53, fancy_stands_full_stand_back_color, fancy_stands_full_stand_back_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(0.0, 1.0, 0.0), 48, 56, 55, 52, fancy_stands_full_stand_up_color, fancy_stands_full_stand_up_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(0.0, 1.0, 0.0), 47, 51, 55, 56, fancy_stands_full_stand_up_color, fancy_stands_full_stand_up_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(0.0, -1.0, 0.0), 49, 53, 54, 57, fancy_stands_full_stand_down_color, fancy_stands_full_stand_down_shading),
        face_3d(fancy_stands_full_vertices, vertex_3d(-0.0, -1.0, 0.0), 46, 57, 54, 50, fancy_stands_full_stand_down_color, fancy_stands_full_stand_down_shading),
    };

    constexpr inline model_3d_item fancy_stands_full(fancy_stands_full_vertices, fancy_stands_full_faces);
}

#endif

