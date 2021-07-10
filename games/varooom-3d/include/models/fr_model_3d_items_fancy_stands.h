/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_FANCY_STANDS_H
#define FR_MODEL_3D_ITEMS_FANCY_STANDS_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d fancy_stands_vertices[] = {
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
    };

    constexpr inline int fancy_stands_roof_front_color = 7;
    constexpr inline int fancy_stands_roof_front_shading = 7;

    constexpr inline int fancy_stands_roof_side_a_color = 7;
    constexpr inline int fancy_stands_roof_side_a_shading = 5;

    constexpr inline int fancy_stands_roof_side_b_color = 7;
    constexpr inline int fancy_stands_roof_side_b_shading = 4;

    constexpr inline int fancy_stands_roof_back_color = 7;
    constexpr inline int fancy_stands_roof_back_shading = 2;

    constexpr inline int fancy_stands_stick_front_color = 6;
    constexpr inline int fancy_stands_stick_front_shading = 7;

    constexpr inline int fancy_stands_stick_side_color = 6;
    constexpr inline int fancy_stands_stick_side_shading = 6;

    constexpr inline int fancy_stands_stick_interior_color = 6;
    constexpr inline int fancy_stands_stick_interior_shading = 1;

    constexpr inline int fancy_stands_stand_front_color = 3;
    constexpr inline int fancy_stands_stand_front_shading = 7;

    constexpr inline int fancy_stands_stand_side_color = 3;
    constexpr inline int fancy_stands_stand_side_shading = 6;

    constexpr inline int fancy_stands_stand_up_color = 3;
    constexpr inline int fancy_stands_stand_up_shading = 2;

    constexpr inline face_3d fancy_stands_faces[] = {
        face_3d(fancy_stands_vertices, vertex_3d(-0.6839411288813297, 0.7295372041400852, 0.0), 0, 1, 2, 3, fancy_stands_roof_front_color, fancy_stands_roof_front_shading),
        face_3d(fancy_stands_vertices, vertex_3d(-0.6839411288813296, 0.729537204140085, 0.0), 4, 1, 0, 5, fancy_stands_roof_front_color, fancy_stands_roof_front_shading),
        face_3d(fancy_stands_vertices, vertex_3d(0.0, 0.8682431421244593, -0.49613893835683376), 6, 1, 4, fancy_stands_roof_side_a_color, fancy_stands_roof_side_a_shading),
        face_3d(fancy_stands_vertices, vertex_3d(0.0, 0.7295372041400852, -0.6839411288813297), 2, 7, 3, fancy_stands_roof_side_a_color, fancy_stands_roof_side_a_shading),
        face_3d(fancy_stands_vertices, vertex_3d(0.0, 0.7295372041400852, 0.6839411288813297), 5, 8, 4, fancy_stands_roof_side_b_color, fancy_stands_roof_side_b_shading),
        face_3d(fancy_stands_vertices, vertex_3d(0.0, 0.8682431421244593, 0.49613893835683376), 1, 6, 2, fancy_stands_roof_side_b_color, fancy_stands_roof_side_b_shading),
        face_3d(fancy_stands_vertices, vertex_3d(0.6839411288813296, 0.729537204140085, 0.0), 7, 2, 6, 9, fancy_stands_roof_back_color, fancy_stands_roof_back_shading),
        face_3d(fancy_stands_vertices, vertex_3d(0.6839411288813297, 0.7295372041400852, 0.0), 6, 4, 8, 9, fancy_stands_roof_back_color, fancy_stands_roof_back_shading),
        face_3d(fancy_stands_vertices, vertex_3d(-1.0, 0.0, 0.0), 10, 11, 12, 13, fancy_stands_stick_front_color, fancy_stands_stick_front_shading),
        face_3d(fancy_stands_vertices, vertex_3d(-1.0, 0.0, 0.0), 14, 15, 16, 17, fancy_stands_stick_front_color, fancy_stands_stick_front_shading),
        face_3d(fancy_stands_vertices, vertex_3d(-1.0, 0.0, 0.0), 18, 19, 20, 21, fancy_stands_stick_front_color, fancy_stands_stick_front_shading),
        face_3d(fancy_stands_vertices, vertex_3d(0.0, 0.0, -1.0), 22, 23, 24, 25, fancy_stands_stick_side_color, fancy_stands_stick_side_shading),
        face_3d(fancy_stands_vertices, vertex_3d(0.0, -0.0, 1.0), 26, 27, 28, 29, fancy_stands_stick_side_color, fancy_stands_stick_side_shading),
        face_3d(fancy_stands_vertices, vertex_3d(0.0, 0.0, -1.0), 30, 31, 12, 11, fancy_stands_stick_side_color, fancy_stands_stick_side_shading),
        face_3d(fancy_stands_vertices, vertex_3d(0.0, -0.0, 1.0), 32, 14, 17, 33, fancy_stands_stick_side_color, fancy_stands_stick_side_shading),
        face_3d(fancy_stands_vertices, vertex_3d(-0.7071067811865475, 0.0, 0.7071067811865475), 34, 25, 24, 35, fancy_stands_stick_interior_color, fancy_stands_stick_interior_shading),
        face_3d(fancy_stands_vertices, vertex_3d(-0.7071067811865475, 0.0, -0.7071067811865475), 36, 37, 28, 27, fancy_stands_stick_interior_color, fancy_stands_stick_interior_shading),
        face_3d(fancy_stands_vertices, vertex_3d(0.7071067811865475, -0.0, 0.7071067811865475), 30, 10, 13, 31, fancy_stands_stick_interior_color, fancy_stands_stick_interior_shading),
        face_3d(fancy_stands_vertices, vertex_3d(0.7071067811865475, 0.0, -0.7071067811865475), 32, 33, 16, 15, fancy_stands_stick_interior_color, fancy_stands_stick_interior_shading),
        face_3d(fancy_stands_vertices, vertex_3d(0.4472135954999579, -0.0, 0.8944271909999159), 42, 18, 21, 43, fancy_stands_stick_interior_color, fancy_stands_stick_interior_shading),
        face_3d(fancy_stands_vertices, vertex_3d(0.4472135954999579, 0.0, -0.8944271909999159), 42, 43, 20, 19, fancy_stands_stick_interior_color, fancy_stands_stick_interior_shading),
        face_3d(fancy_stands_vertices, vertex_3d(-0.4472135954999579, 0.0, -0.8944271909999159), 38, 41, 44, 45, fancy_stands_stick_interior_color, fancy_stands_stick_interior_shading),
        face_3d(fancy_stands_vertices, vertex_3d(-0.4472135954999579, 0.0, 0.8944271909999159), 39, 45, 44, 40, fancy_stands_stick_interior_color, fancy_stands_stick_interior_shading),
        face_3d(fancy_stands_vertices, vertex_3d(-1.0, 0.0, 0.0), 46, 47, 48, 49, fancy_stands_stand_front_color, fancy_stands_stand_front_shading),
        face_3d(fancy_stands_vertices, vertex_3d(0.0, 0.0, 1.0), 50, 51, 47, 46, fancy_stands_stand_side_color, fancy_stands_stand_side_shading),
        face_3d(fancy_stands_vertices, vertex_3d(0.0, 0.0, -1.0), 49, 48, 52, 53, fancy_stands_stand_side_color, fancy_stands_stand_side_shading),
        face_3d(fancy_stands_vertices, vertex_3d(0.0, 1.0, 0.0), 52, 48, 47, 51, fancy_stands_stand_up_color, fancy_stands_stand_up_shading),
    };

    constexpr inline face_3d fancy_stands_collision_face(fancy_stands_vertices, vertex_3d(0.0, -1.0, 0.0), 49, 53, 50, 46, 0, -1);

    constexpr inline model_3d_item fancy_stands(fancy_stands_vertices, fancy_stands_faces, &fancy_stands_collision_face);
}

#endif

