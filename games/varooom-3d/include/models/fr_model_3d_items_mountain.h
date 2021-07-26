/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_MOUNTAIN_H
#define FR_MODEL_3D_ITEMS_MOUNTAIN_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d mountain_vertices[] = {
        vertex_3d(35.0, -40.0, -40.0),
        vertex_3d(-3.0, -40.0, -30.0),
        vertex_3d(10.0, 22.0, -18.0),
        vertex_3d(35.0, -16.0, -23.0),
        vertex_3d(-35.0, -40.0, -35.0),
        vertex_3d(-35.0, 2.0, -27.0),
        vertex_3d(35.0, 7.0, 3.8),
        vertex_3d(-14.0, 18.0, 14.0),
        vertex_3d(-35.0, -6.0, 2.0),
        vertex_3d(35.0, 27.0, 66.0),
        vertex_3d(3.0, 27.0, 66.0),
        vertex_3d(-35.0, 27.0, 66.0),
    };

    constexpr inline int mountain_a1_color = 4;
    constexpr inline int mountain_a1_shading = 7;

    constexpr inline int mountain_a2_color = 4;
    constexpr inline int mountain_a2_shading = 6;

    constexpr inline int mountain_b1_color = 4;
    constexpr inline int mountain_b1_shading = 5;

    constexpr inline int mountain_b2_color = 4;
    constexpr inline int mountain_b2_shading = 3;

    constexpr inline int mountain_c1_color = 4;
    constexpr inline int mountain_c1_shading = 1;

    constexpr inline int mountain_c2_color = 4;
    constexpr inline int mountain_c2_shading = 0;

    constexpr inline face_3d mountain_faces[] = {
        face_3d(mountain_vertices, vertex_3d(0.0, 0.3344098451707649, -0.9424277454812464), 0, 1, 2, 3, mountain_a1_color, mountain_a1_shading),
        face_3d(mountain_vertices, vertex_3d(0.05953639074203382, 0.2951592252888965, -0.9535913432410501), 4, 5, 2, 1, mountain_a2_color, mountain_a2_shading),
        face_3d(mountain_vertices, vertex_3d(0.715171789785949, 0.5304019251235177, -0.455195682008989), 6, 3, 2, 7, mountain_b1_color, mountain_b1_shading),
        face_3d(mountain_vertices, vertex_3d(-0.3727298385775959, 0.9130778294149391, -0.16541265025632954), 7, 2, 5, 8, mountain_b2_color, mountain_b2_shading),
        face_3d(mountain_vertices, vertex_3d(0.14833373129990318, 0.9414651109034671, -0.3027219006120473), 9, 6, 7, 10, mountain_c1_color, mountain_c1_shading),
        face_3d(mountain_vertices, vertex_3d(-0.7799981148267789, 0.6075774789177015, 0.14984174311146017), 10, 7, 8, 11, mountain_c2_color, mountain_c2_shading),
    };

    constexpr inline face_3d mountain_collision_face(mountain_vertices, vertex_3d(0, 0, 0), 0, 1, 4, 11, 0, -1);

    constexpr inline model_3d_item mountain(mountain_vertices, mountain_faces, &mountain_collision_face);
}

#endif

