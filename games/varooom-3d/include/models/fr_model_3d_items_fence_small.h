/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_FENCE_SMALL_H
#define FR_MODEL_3D_ITEMS_FENCE_SMALL_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d fence_small_vertices[] = {
        vertex_3d(-20.0, -10.0, 5.0),
        vertex_3d(20.0, -10.0, 5.0),
        vertex_3d(20.0, 7.0, 0.0),
        vertex_3d(-20.0, 7.0, 0.0),
        vertex_3d(20.0, -10.0, -5.0),
        vertex_3d(-20.0, -10.0, -5.0),
    };

    constexpr inline int fence_small_color = 0;
    constexpr inline int fence_small_a_shading = 3;
    constexpr inline int fence_small_b_shading = 6;
    constexpr inline int fence_small_lateral_shading = 1;
    constexpr inline int fence_small_down_shading = 0;

    constexpr inline face_3d fence_small_full_faces[] = {
        face_3d(fence_small_vertices, vertex_3d(0.0, 0.28216632399155017, 0.9593655015712707), 0, 1, 2, 3, fence_small_color, fence_small_a_shading),
        face_3d(fence_small_vertices, vertex_3d(0.0, 0.28216632399155017, -0.9593655015712707), 3, 2, 4, 5, fence_small_color, fence_small_b_shading),
        face_3d(fence_small_vertices, vertex_3d(1.0, 0.0, 0.0), 2, 1, 4, fence_small_color, fence_small_lateral_shading),
        face_3d(fence_small_vertices, vertex_3d(-1.0, 0.0, 0.0), 3, 5, 0, fence_small_color, fence_small_lateral_shading),
        face_3d(fence_small_vertices, vertex_3d(0.0, -1.0, 0.0), 0, 5, 4, 1, fence_small_color, fence_small_down_shading),
    };

    constexpr inline const face_3d* fence_small_collision_face_ptr = &fence_small_full_faces[4];

    constexpr inline model_3d_item fence_small_full(fence_small_vertices, fence_small_full_faces, fence_small_collision_face_ptr);

    constexpr inline face_3d fence_small_solid_faces[] = {
        face_3d(fence_small_vertices, vertex_3d(0.0, 0.28216632399155017, 0.9593655015712707), 0, 1, 2, 3, fence_small_color, fence_small_a_shading),
        face_3d(fence_small_vertices, vertex_3d(0.0, 0.28216632399155017, -0.9593655015712707), 3, 2, 4, 5, fence_small_color, fence_small_b_shading),
        face_3d(fence_small_vertices, vertex_3d(1.0, 0.0, 0.0), 2, 1, 4, fence_small_color, fence_small_lateral_shading),
        face_3d(fence_small_vertices, vertex_3d(-1.0, 0.0, 0.0), 3, 5, 0, fence_small_color, fence_small_lateral_shading),
    };

    constexpr inline model_3d_item fence_small_solid(fence_small_vertices, fence_small_solid_faces, fence_small_collision_face_ptr);

    constexpr inline face_3d fence_small_faces[] = {
        face_3d(fence_small_vertices, vertex_3d(0.0, 0.28216632399155017, 0.9593655015712707), 0, 1, 2, 3, fence_small_color, fence_small_a_shading),
        face_3d(fence_small_vertices, vertex_3d(0.0, 0.28216632399155017, -0.9593655015712707), 3, 2, 4, 5, fence_small_color, fence_small_b_shading),
    };

    constexpr inline model_3d_item fence_small(fence_small_vertices, fence_small_faces, fence_small_collision_face_ptr);
}

#endif

