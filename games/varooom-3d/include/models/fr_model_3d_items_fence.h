/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_FENCE_H
#define FR_MODEL_3D_ITEMS_FENCE_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d fence_vertices[] = {
        vertex_3d(-35.0, -10.0, 5.0),
        vertex_3d(35.0, -10.0, 5.0),
        vertex_3d(35.0, 7.0, 0.0),
        vertex_3d(-35.0, 7.0, 0.0),
        vertex_3d(35.0, -10.0, -5.0),
        vertex_3d(-35.0, -10.0, -5.0),
    };

    // Red:

    constexpr inline int fence_red_color = 0;
    constexpr inline int fence_red_a_shading = 3;
    constexpr inline int fence_red_b_shading = 6;
    constexpr inline int fence_red_lateral_shading = 1;
    constexpr inline int fence_red_down_shading = 0;

    constexpr inline face_3d fence_full_red_faces[] = {
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, 0.9593655015712707), 0, 1, 2, 3, fence_red_color, fence_red_a_shading),
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, -0.9593655015712707), 3, 2, 4, 5, fence_red_color, fence_red_b_shading),
        face_3d(fence_vertices, vertex_3d(1.0, 0.0, 0.0), 2, 1, 4, fence_red_color, fence_red_lateral_shading),
        face_3d(fence_vertices, vertex_3d(-1.0, 0.0, 0.0), 3, 5, 0, fence_red_color, fence_red_lateral_shading),
        face_3d(fence_vertices, vertex_3d(0.0, -1.0, 0.0), 0, 5, 4, 1, fence_red_color, fence_red_down_shading),
    };

    constexpr inline const face_3d* fence_collision_face_ptr = &fence_full_red_faces[4];

    constexpr inline model_3d_item fence_full_red(fence_vertices, fence_full_red_faces, fence_collision_face_ptr);

    constexpr inline face_3d fence_solid_red_faces[] = {
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, 0.9593655015712707), 0, 1, 2, 3, fence_red_color, fence_red_a_shading),
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, -0.9593655015712707), 3, 2, 4, 5, fence_red_color, fence_red_b_shading),
        face_3d(fence_vertices, vertex_3d(1.0, 0.0, 0.0), 2, 1, 4, fence_red_color, fence_red_lateral_shading),
        face_3d(fence_vertices, vertex_3d(-1.0, 0.0, 0.0), 3, 5, 0, fence_red_color, fence_red_lateral_shading),
    };

    constexpr inline model_3d_item fence_solid_red(fence_vertices, fence_solid_red_faces, fence_collision_face_ptr);

    constexpr inline face_3d fence_red_faces[] = {
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, 0.9593655015712707), 0, 1, 2, 3, fence_red_color, fence_red_a_shading),
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, -0.9593655015712707), 3, 2, 4, 5, fence_red_color, fence_red_b_shading),
    };

    constexpr inline model_3d_item fence_red(fence_vertices, fence_red_faces, fence_collision_face_ptr);

    // White:

    constexpr inline int fence_white_color = 2;
    constexpr inline int fence_white_a_shading = 3;
    constexpr inline int fence_white_b_shading = 6;
    constexpr inline int fence_white_lateral_shading = 1;
    constexpr inline int fence_white_down_shading = 0;

    constexpr inline face_3d fence_full_white_faces[] = {
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, 0.9593655015712707), 0, 1, 2, 3, fence_white_color, fence_white_a_shading),
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, -0.9593655015712707), 3, 2, 4, 5, fence_white_color, fence_white_b_shading),
        face_3d(fence_vertices, vertex_3d(1.0, 0.0, 0.0), 2, 1, 4, fence_white_color, fence_white_lateral_shading),
        face_3d(fence_vertices, vertex_3d(-1.0, 0.0, 0.0), 3, 5, 0, fence_white_color, fence_white_lateral_shading),
        face_3d(fence_vertices, vertex_3d(0.0, -1.0, 0.0), 0, 5, 4, 1, fence_white_color, fence_white_down_shading),
    };

    constexpr inline model_3d_item fence_full_white(fence_vertices, fence_full_white_faces, fence_collision_face_ptr);

    constexpr inline face_3d fence_solid_white_faces[] = {
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, 0.9593655015712707), 0, 1, 2, 3, fence_white_color, fence_white_a_shading),
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, -0.9593655015712707), 3, 2, 4, 5, fence_white_color, fence_white_b_shading),
        face_3d(fence_vertices, vertex_3d(1.0, 0.0, 0.0), 2, 1, 4, fence_white_color, fence_white_lateral_shading),
        face_3d(fence_vertices, vertex_3d(-1.0, 0.0, 0.0), 3, 5, 0, fence_white_color, fence_white_lateral_shading),
    };

    constexpr inline model_3d_item fence_solid_white(fence_vertices, fence_solid_white_faces, fence_collision_face_ptr);

    constexpr inline face_3d fence_white_faces[] = {
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, 0.9593655015712707), 0, 1, 2, 3, fence_white_color, fence_white_a_shading),
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, -0.9593655015712707), 3, 2, 4, 5, fence_white_color, fence_white_b_shading),
    };

    constexpr inline model_3d_item fence_white(fence_vertices, fence_white_faces, fence_collision_face_ptr);

    // Yellow:

    constexpr inline int fence_yellow_color = 7;
    constexpr inline int fence_yellow_a_shading = 4;
    constexpr inline int fence_yellow_b_shading = 7;
    constexpr inline int fence_yellow_lateral_shading = 2;
    constexpr inline int fence_yellow_down_shading = 0;

    constexpr inline face_3d fence_full_yellow_faces[] = {
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, 0.9593655015712707), 0, 1, 2, 3, fence_yellow_color, fence_yellow_a_shading),
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, -0.9593655015712707), 3, 2, 4, 5, fence_yellow_color, fence_yellow_b_shading),
        face_3d(fence_vertices, vertex_3d(1.0, 0.0, 0.0), 2, 1, 4, fence_yellow_color, fence_yellow_lateral_shading),
        face_3d(fence_vertices, vertex_3d(-1.0, 0.0, 0.0), 3, 5, 0, fence_yellow_color, fence_yellow_lateral_shading),
        face_3d(fence_vertices, vertex_3d(0.0, -1.0, 0.0), 0, 5, 4, 1, fence_yellow_color, fence_yellow_down_shading),
    };

    constexpr inline model_3d_item fence_full_yellow(fence_vertices, fence_full_yellow_faces, fence_collision_face_ptr);

    constexpr inline face_3d fence_solid_yellow_faces[] = {
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, 0.9593655015712707), 0, 1, 2, 3, fence_yellow_color, fence_yellow_a_shading),
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, -0.9593655015712707), 3, 2, 4, 5, fence_yellow_color, fence_yellow_b_shading),
        face_3d(fence_vertices, vertex_3d(1.0, 0.0, 0.0), 2, 1, 4, fence_yellow_color, fence_yellow_lateral_shading),
        face_3d(fence_vertices, vertex_3d(-1.0, 0.0, 0.0), 3, 5, 0, fence_yellow_color, fence_yellow_lateral_shading),
    };

    constexpr inline model_3d_item fence_solid_yellow(fence_vertices, fence_solid_yellow_faces, fence_collision_face_ptr);

    constexpr inline face_3d fence_yellow_faces[] = {
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, 0.9593655015712707), 0, 1, 2, 3, fence_yellow_color, fence_yellow_a_shading),
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, -0.9593655015712707), 3, 2, 4, 5, fence_yellow_color, fence_yellow_b_shading),
    };

    constexpr inline model_3d_item fence_yellow(fence_vertices, fence_yellow_faces, fence_collision_face_ptr);

    // Black:

    constexpr inline int fence_black_color = 6;
    constexpr inline int fence_black_a_shading = 7;
    constexpr inline int fence_black_b_shading = 6;
    constexpr inline int fence_black_lateral_shading = 1;
    constexpr inline int fence_black_down_shading = 0;

    constexpr inline face_3d fence_full_black_faces[] = {
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, 0.9593655015712707), 0, 1, 2, 3, fence_black_color, fence_black_a_shading),
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, -0.9593655015712707), 3, 2, 4, 5, fence_black_color, fence_black_b_shading),
        face_3d(fence_vertices, vertex_3d(1.0, 0.0, 0.0), 2, 1, 4, fence_black_color, fence_black_lateral_shading),
        face_3d(fence_vertices, vertex_3d(-1.0, 0.0, 0.0), 3, 5, 0, fence_black_color, fence_black_lateral_shading),
        face_3d(fence_vertices, vertex_3d(0.0, -1.0, 0.0), 0, 5, 4, 1, fence_black_color, fence_black_down_shading),
    };

    constexpr inline model_3d_item fence_full_black(fence_vertices, fence_full_black_faces, fence_collision_face_ptr);

    constexpr inline face_3d fence_solid_black_faces[] = {
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, 0.9593655015712707), 0, 1, 2, 3, fence_black_color, fence_black_a_shading),
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, -0.9593655015712707), 3, 2, 4, 5, fence_black_color, fence_black_b_shading),
        face_3d(fence_vertices, vertex_3d(1.0, 0.0, 0.0), 2, 1, 4, fence_black_color, fence_black_lateral_shading),
        face_3d(fence_vertices, vertex_3d(-1.0, 0.0, 0.0), 3, 5, 0, fence_black_color, fence_black_lateral_shading),
    };

    constexpr inline model_3d_item fence_solid_black(fence_vertices, fence_solid_black_faces, fence_collision_face_ptr);

    constexpr inline face_3d fence_black_faces[] = {
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, 0.9593655015712707), 0, 1, 2, 3, fence_black_color, fence_black_a_shading),
        face_3d(fence_vertices, vertex_3d(0.0, 0.28216632399155017, -0.9593655015712707), 3, 2, 4, 5, fence_black_color, fence_black_b_shading),
    };

    constexpr inline model_3d_item fence_black(fence_vertices, fence_black_faces, fence_collision_face_ptr);
}

#endif

