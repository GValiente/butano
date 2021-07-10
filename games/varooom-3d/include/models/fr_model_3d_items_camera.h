/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_CAMERA_H
#define FR_MODEL_3D_ITEMS_CAMERA_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d camera_vertices[] = {
        vertex_3d(-6.25, -32.32, 39.46),
        vertex_3d(6.25, -32.32, 39.46),
        vertex_3d(12.5, -32.32, 33.21),
        vertex_3d(-12.5, -32.32, 33.21),
        vertex_3d(12.5, -32.32, 70.71),
        vertex_3d(6.25, -32.32, 64.46),
        vertex_3d(-12.5, -32.32, 70.71),
        vertex_3d(-6.25, -32.32, 64.46),
        vertex_3d(12.5, -51.07, 33.21),
        vertex_3d(12.5, -51.07, 70.71),
        vertex_3d(-12.5, -51.07, 33.21),
        vertex_3d(-12.5, -51.07, 70.71),
        vertex_3d(-7.5, 22.68, -30.54),
        vertex_3d(-7.5, 27.68, -30.54),
        vertex_3d(-7.5, 27.68, -53.04),
        vertex_3d(-7.5, 22.68, -53.04),
        vertex_3d(7.5, 22.68, -53.04),
        vertex_3d(7.5, 27.68, -53.04),
        vertex_3d(7.5, 27.68, -30.54),
        vertex_3d(7.5, 22.68, -30.54),
        vertex_3d(-7.5, 37.68, -30.54),
        vertex_3d(-2.5, 32.68, -30.54),
        vertex_3d(2.5, 32.68, -30.54),
        vertex_3d(7.5, 37.68, -30.54),
        vertex_3d(-2.5, 27.68, -30.54),
        vertex_3d(2.5, 27.68, -30.54),
        vertex_3d(7.5, 32.68, -53.04),
        vertex_3d(-7.5, 32.68, -53.04),
    };

    constexpr inline int camera_base_up_color = 0;
    constexpr inline int camera_base_up_shading = 7;

    constexpr inline int camera_base_side_a_color = 0;
    constexpr inline int camera_base_side_a_shading = 4;

    constexpr inline int camera_base_side_b_color = 0;
    constexpr inline int camera_base_side_b_shading = 2;

    constexpr inline int camera_elevator_up_color = 6;
    constexpr inline int camera_elevator_up_shading = 7;

    constexpr inline int camera_elevator_side_color = 6;
    constexpr inline int camera_elevator_side_shading = 4;

    constexpr inline int camera_crystal_up_color = 1;
    constexpr inline int camera_crystal_up_shading = 4;

    constexpr inline int camera_crystal_side_a_color = 1;
    constexpr inline int camera_crystal_side_a_shading = 2;

    constexpr inline int camera_crystal_side_b_color = 1;
    constexpr inline int camera_crystal_side_b_shading = 0;

    constexpr inline face_3d camera_faces[] = {
        face_3d(camera_vertices, vertex_3d(0.0, 1.0, 0.0), 3, 6, 4, 2, camera_base_up_color, camera_base_up_shading),
        face_3d(camera_vertices, vertex_3d(1.0, 0.0, 0.0), 8, 2, 4, 9, camera_base_side_a_color, camera_base_side_a_shading),
        face_3d(camera_vertices, vertex_3d(-1.0, -0.0, 0.0), 10, 11, 6, 3, camera_base_side_a_color, camera_base_side_a_shading),
        face_3d(camera_vertices, vertex_3d(-1.0, 0.0, 0.0), 12, 13, 14, 15, camera_base_side_a_color, camera_base_side_a_shading),
        face_3d(camera_vertices, vertex_3d(1.0, 0.0, 0.0), 16, 17, 18, 19, camera_base_side_a_color, camera_base_side_a_shading),
        face_3d(camera_vertices, vertex_3d(0.0, 0.0, -1.0), 16, 15, 14, 17, camera_base_side_b_color, camera_base_side_b_shading),
        face_3d(camera_vertices, vertex_3d(0.0, 0.0, 1.0), 11, 9, 4, 6, camera_base_side_b_color, camera_base_side_b_shading),
        face_3d(camera_vertices, vertex_3d(0.0, 0.0, -1.0), 8, 10, 3, 2, camera_base_side_b_color, camera_base_side_b_shading),
        face_3d(camera_vertices, vertex_3d(-0.0, 0.0, 1.0), 19, 23, 20, 12, camera_base_side_b_color, camera_base_side_b_shading),
        face_3d(camera_vertices, vertex_3d(0.0, 0.8253072612498318, 0.5646839155919902), 22, 21, 7, 5, camera_elevator_up_color, camera_elevator_up_shading),
        face_3d(camera_vertices, vertex_3d(0.9983399416756089, 0.057596535096669746, 0.0), 22, 5, 1, 25, camera_elevator_side_color, camera_elevator_side_shading),
        face_3d(camera_vertices, vertex_3d(-0.9985681322700888, 0.0, -0.0534947213716119), 21, 24, 0, 7, camera_elevator_side_color, camera_elevator_side_shading),
        face_3d(camera_vertices, vertex_3d(0.0, 0.9761870601839527, -0.21693045781865616), 20, 23, 26, 27, camera_crystal_up_color, camera_crystal_up_shading),
        face_3d(camera_vertices, vertex_3d(-1.0, 0.0, 0.0), 20, 27, 14, 13, camera_crystal_side_a_color, camera_crystal_side_a_shading),
        face_3d(camera_vertices, vertex_3d(1.0, 0.0, 0.0), 23, 18, 17, 26, camera_crystal_side_a_color, camera_crystal_side_a_shading),
        face_3d(camera_vertices, vertex_3d(0.0, 0.0, -1.0), 17, 14, 27, 26, camera_crystal_side_b_color, camera_crystal_side_b_shading),
    };

    constexpr inline face_3d camera_collision_face(camera_vertices, vertex_3d(0.0, -1.0, -0.0), 10, 8, 9, 11, 0, -1);

    constexpr inline model_3d_item camera(camera_vertices, camera_faces, &camera_collision_face);
}

#endif

