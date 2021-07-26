/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_MOUNTAIN_FLIPPED_H
#define FR_MODEL_3D_ITEMS_MOUNTAIN_FLIPPED_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d mountain_flipped_vertices[] = {
        vertex_3d(35.0, -40.0, -35.0),
        vertex_3d(2.0, -40.0, -30.0),
        vertex_3d(-11.0, 22.0, -18.0),
        vertex_3d(35.0, 2.0, -27.0),
        vertex_3d(-35.0, -40.0, -40.0),
        vertex_3d(-35.0, -16.0, -23.0),
        vertex_3d(13.0, 18.0, 14.0),
        vertex_3d(35.0, -6.0, 2.0),
        vertex_3d(-35.0, 7.0, 3.8),
        vertex_3d(-3.0, 27.0, 66.0),
        vertex_3d(35.0, 27.0, 66.0),
        vertex_3d(-35.0, 27.0, 66.0),
    };

    constexpr inline int mountain_flipped_a1_color = 4;
    constexpr inline int mountain_flipped_a1_shading = 7;

    constexpr inline int mountain_flipped_a2_color = 4;
    constexpr inline int mountain_flipped_a2_shading = 6;

    constexpr inline int mountain_flipped_b1_color = 4;
    constexpr inline int mountain_flipped_b1_shading = 5;

    constexpr inline int mountain_flipped_b2_color = 4;
    constexpr inline int mountain_flipped_b2_shading = 3;

    constexpr inline int mountain_flipped_c1_color = 4;
    constexpr inline int mountain_flipped_c1_shading = 1;

    constexpr inline int mountain_flipped_c2_color = 4;
    constexpr inline int mountain_flipped_c2_shading = 0;

    constexpr inline face_3d mountain_flipped_faces[] = {
        face_3d(mountain_flipped_vertices, vertex_3d(0.0, 0.3344098451707649, -0.9424277454812465), 0, 1, 2, 3, mountain_flipped_a1_color, mountain_flipped_a1_shading),
        face_3d(mountain_flipped_vertices, vertex_3d(-0.5975282326806027, 0.46348197515909867, -0.6543274943422569), 4, 5, 2, 1, mountain_flipped_a2_color, mountain_flipped_a2_shading),
        face_3d(mountain_flipped_vertices, vertex_3d(0.7673482013750775, 0.6181416066632569, 0.170521822527795), 6, 7, 3, 2, mountain_flipped_b1_color, mountain_flipped_b1_shading),
        face_3d(mountain_flipped_vertices, vertex_3d(-0.275730587473073, 0.9063730512862318, 0.3200945720155837), 8, 6, 2, 5, mountain_flipped_b2_color, mountain_flipped_b2_shading),
        face_3d(mountain_flipped_vertices, vertex_3d(0.0, 0.8888031674084939, -0.45828913319500464), 9, 10, 7, 6, mountain_flipped_c1_color, mountain_flipped_c1_shading),
        face_3d(mountain_flipped_vertices, vertex_3d(0.0, 0.9853505058554479, -0.1705414337057506), 11, 9, 6, 8, mountain_flipped_c2_color, mountain_flipped_c2_shading),
    };

    constexpr inline face_3d mountain_flipped_collision_face(mountain_flipped_vertices, vertex_3d(0, 0, 0), 0, 1, 4, 11, 0, -1);

    constexpr inline model_3d_item mountain_flipped(mountain_flipped_vertices, mountain_flipped_faces, &mountain_flipped_collision_face);
}

#endif

