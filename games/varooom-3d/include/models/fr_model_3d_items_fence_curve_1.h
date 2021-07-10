/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_FENCE_CURVE_1_H
#define FR_MODEL_3D_ITEMS_FENCE_CURVE_1_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d fence_curve_1_vertices[] = {
        vertex_3d(-98.75, -5.67, 5.0), // 0
        vertex_3d(-28.75, -5.67, 5.0),
        vertex_3d(-28.75, 11.33, 0.0),
        vertex_3d(-98.75, 11.33, 0.0),
        vertex_3d(41.25, 11.33, 0.0),
        vertex_3d(41.25, -5.67, 5.0),
        vertex_3d(86.25, -5.67, 5.0), // 6
        vertex_3d(86.25, 11.33, 0.0),
        vertex_3d(-28.75, -5.67, -5.0),
        vertex_3d(-98.75, -5.67, -5.0), // 9
        vertex_3d(41.25, -5.67, -5.0),
        vertex_3d(86.25, -5.67, -5.0), // 11
    };

    constexpr inline face_3d fence_curve_1_collision_face(fence_curve_1_vertices, vertex_3d(0, 0, 0), 11, 9, 0, 6, 0, -1);

    constexpr inline int fence_curve_1_red_a_color = 0;
    constexpr inline int fence_curve_1_red_a_shading = 6;

    constexpr inline int fence_curve_1_red_b_color = 0;
    constexpr inline int fence_curve_1_red_b_shading = 3;

    constexpr inline int fence_curve_1_red_lateral_color = 0;
    constexpr inline int fence_curve_1_red_lateral_shading = 1;

    constexpr inline int fence_curve_1_red_down_color = 0;
    constexpr inline int fence_curve_1_red_down_shading = 0;

    constexpr inline int fence_curve_1_white_a_color = 2;
    constexpr inline int fence_curve_1_white_a_shading = 6;

    constexpr inline int fence_curve_1_white_b_color = 2;
    constexpr inline int fence_curve_1_white_b_shading = 3;

    constexpr inline int fence_curve_1_white_down_color = 2;
    constexpr inline int fence_curve_1_white_down_shading = 0;

    constexpr inline face_3d fence_curve_1_full_faces[] = {
        face_3d(fence_curve_1_vertices, vertex_3d(0.0, 0.28216632399155017, 0.9593655015712707), 0, 1, 2, 3, fence_curve_1_red_a_color, fence_curve_1_red_a_shading),
        face_3d(fence_curve_1_vertices, vertex_3d(0.0, 0.28216632399155017, 0.9593655015712707), 4, 5, 6, 7, fence_curve_1_red_a_color, fence_curve_1_red_a_shading),
        face_3d(fence_curve_1_vertices, vertex_3d(0.0, 0.28216632399155017, -0.9593655015712707), 3, 2, 8, 9, fence_curve_1_red_b_color, fence_curve_1_red_b_shading),
        face_3d(fence_curve_1_vertices, vertex_3d(0.0, 0.28216632399155017, -0.9593655015712707), 10, 4, 7, 11, fence_curve_1_red_b_color, fence_curve_1_red_b_shading),
        face_3d(fence_curve_1_vertices, vertex_3d(-1.0, 0.0, 0.0), 3, 9, 0, fence_curve_1_red_lateral_color, fence_curve_1_red_lateral_shading),
        face_3d(fence_curve_1_vertices, vertex_3d(1.0, 0.0, 0.0), 7, 6, 11, fence_curve_1_red_lateral_color, fence_curve_1_red_lateral_shading),
        face_3d(fence_curve_1_vertices, vertex_3d(0.0, -1.0, 0.0), 0, 9, 8, 1, fence_curve_1_red_down_color, fence_curve_1_red_down_shading),
        face_3d(fence_curve_1_vertices, vertex_3d(0.0, -1.0, 0.0), 6, 5, 10, 11, fence_curve_1_red_down_color, fence_curve_1_red_down_shading),
        face_3d(fence_curve_1_vertices, vertex_3d(0.0, 0.28216632399155017, 0.9593655015712707), 2, 1, 5, 4, fence_curve_1_white_a_color, fence_curve_1_white_a_shading),
        face_3d(fence_curve_1_vertices, vertex_3d(0.0, 0.28216632399155017, -0.9593655015712707), 10, 8, 2, 4, fence_curve_1_white_b_color, fence_curve_1_white_b_shading),
        face_3d(fence_curve_1_vertices, vertex_3d(0.0, -1.0, 0.0), 5, 1, 8, 10, fence_curve_1_white_down_color, fence_curve_1_white_down_shading),
    };

    constexpr inline model_3d_item fence_curve_1_full(fence_curve_1_vertices, fence_curve_1_full_faces, &fence_curve_1_collision_face);

    constexpr inline face_3d fence_curve_1_faces[] = {
        face_3d(fence_curve_1_vertices, vertex_3d(0.0, 0.28216632399155017, 0.9593655015712707), 0, 1, 2, 3, fence_curve_1_red_a_color, fence_curve_1_red_a_shading),
        face_3d(fence_curve_1_vertices, vertex_3d(0.0, 0.28216632399155017, 0.9593655015712707), 4, 5, 6, 7, fence_curve_1_red_a_color, fence_curve_1_red_a_shading),
        face_3d(fence_curve_1_vertices, vertex_3d(0.0, 0.28216632399155017, -0.9593655015712707), 3, 2, 8, 9, fence_curve_1_red_b_color, fence_curve_1_red_b_shading),
        face_3d(fence_curve_1_vertices, vertex_3d(0.0, 0.28216632399155017, -0.9593655015712707), 10, 4, 7, 11, fence_curve_1_red_b_color, fence_curve_1_red_b_shading),
        face_3d(fence_curve_1_vertices, vertex_3d(-1.0, 0.0, 0.0), 3, 9, 0, fence_curve_1_red_lateral_color, fence_curve_1_red_lateral_shading),
        face_3d(fence_curve_1_vertices, vertex_3d(1.0, 0.0, 0.0), 7, 6, 11, fence_curve_1_red_lateral_color, fence_curve_1_red_lateral_shading),
        face_3d(fence_curve_1_vertices, vertex_3d(0.0, 0.28216632399155017, 0.9593655015712707), 2, 1, 5, 4, fence_curve_1_white_a_color, fence_curve_1_white_a_shading),
        face_3d(fence_curve_1_vertices, vertex_3d(0.0, 0.28216632399155017, -0.9593655015712707), 10, 8, 2, 4, fence_curve_1_white_b_color, fence_curve_1_white_b_shading),
    };

    constexpr inline model_3d_item fence_curve_1(fence_curve_1_vertices, fence_curve_1_faces, &fence_curve_1_collision_face);
}

#endif

