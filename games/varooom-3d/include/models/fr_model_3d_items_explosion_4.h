/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_EXPLOSION_4_H
#define FR_MODEL_3D_ITEMS_EXPLOSION_4_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d explosion_4_vertices[] = {
        vertex_3d(32.0, 0.0, 0.0),
        vertex_3d(20.0, 12.0, 0.0),
        vertex_3d(8.0, 0.0, 0.0),
        vertex_3d(20.0, -12.0, 0.0),
        vertex_3d(16.0, 0.0, -27.71),
        vertex_3d(10.0, 12.0, -17.32),
        vertex_3d(4.0, 0.0, -6.93),
        vertex_3d(10.0, -12.0, -17.32),
        vertex_3d(-16.0, 0.0, -27.71),
        vertex_3d(-10.0, 12.0, -17.32),
        vertex_3d(-4.0, 0.0, -6.93),
        vertex_3d(-10.0, -12.0, -17.32),
        vertex_3d(-32.0, 0.0, 0.0),
        vertex_3d(-20.0, 12.0, 0.0),
        vertex_3d(-8.0, 0.0, 0.0),
        vertex_3d(-20.0, -12.0, 0.0),
        vertex_3d(-16.0, 0.0, 27.71),
        vertex_3d(-10.0, 12.0, 17.32),
        vertex_3d(-4.0, 0.0, 6.93),
        vertex_3d(-10.0, -12.0, 17.32),
        vertex_3d(16.0, 0.0, 27.71),
        vertex_3d(10.0, 12.0, 17.32),
        vertex_3d(4.0, 0.0, 6.93),
        vertex_3d(10.0, -12.0, 17.32),
    };

    constexpr inline int explosion_4_top_a_color = 7;
    constexpr inline int explosion_4_top_a_shading = 7;

    constexpr inline int explosion_4_top_b_color = 7;
    constexpr inline int explosion_4_top_b_shading = 5;

    constexpr inline int explosion_4_middle_a_color = 7;
    constexpr inline int explosion_4_middle_a_shading = 6;

    constexpr inline int explosion_4_middle_b_color = 7;
    constexpr inline int explosion_4_middle_b_shading = 4;

    constexpr inline face_3d explosion_4_faces[] = {
        face_3d(explosion_4_vertices, vertex_3d(0.6546779255396236, -0.6546149272788904, -0.37798956439932074), 3, 7, 4, 0, explosion_4_top_a_color, explosion_4_top_a_shading),
        face_3d(explosion_4_vertices, vertex_3d(0.0, 0.6545706922416017, -0.7560007995090683), 4, 8, 9, 5, explosion_4_top_a_color, explosion_4_top_a_shading),
        face_3d(explosion_4_vertices, vertex_3d(-0.6546509272536523, -0.6546509272536524, -0.37797397647439507), 11, 15, 12, 8, explosion_4_top_a_color, explosion_4_top_a_shading),
        face_3d(explosion_4_vertices, vertex_3d(-0.6546610512901945, 0.6546216755503118, 0.378007102874165), 12, 16, 17, 13, explosion_4_top_a_color, explosion_4_top_a_shading),
        face_3d(explosion_4_vertices, vertex_3d(0.0, -0.6545706922416016, 0.7560007995090683), 19, 23, 20, 16, explosion_4_top_a_color, explosion_4_top_a_shading),
        face_3d(explosion_4_vertices, vertex_3d(0.6546441767525629, 0.6546441767525629, 0.37799735936633005), 20, 0, 1, 21, explosion_4_top_a_color, explosion_4_top_a_shading),
        face_3d(explosion_4_vertices, vertex_3d(0.6546610512901945, 0.6546216755503118, -0.378007102874165), 0, 4, 5, 1, explosion_4_top_b_color, explosion_4_top_b_shading),
        face_3d(explosion_4_vertices, vertex_3d(0.0, -0.6545706922416016, -0.7560007995090683), 7, 11, 8, 4, explosion_4_top_b_color, explosion_4_top_b_shading),
        face_3d(explosion_4_vertices, vertex_3d(-0.6546441767525629, 0.6546441767525629, -0.37799735936633005), 8, 12, 13, 9, explosion_4_top_b_color, explosion_4_top_b_shading),
        face_3d(explosion_4_vertices, vertex_3d(-0.6546779255396236, -0.6546149272788904, 0.37798956439932074), 15, 19, 16, 12, explosion_4_top_b_color, explosion_4_top_b_shading),
        face_3d(explosion_4_vertices, vertex_3d(0.0, 0.6545706922416017, 0.7560007995090683), 16, 20, 21, 17, explosion_4_top_b_color, explosion_4_top_b_shading),
        face_3d(explosion_4_vertices, vertex_3d(0.6546509272536523, -0.6546509272536524, 0.37797397647439507), 23, 3, 0, 20, explosion_4_top_b_color, explosion_4_top_b_shading),
        face_3d(explosion_4_vertices, vertex_3d(-0.6547454027071528, -0.6545879362803353, 0.37791942436199294), 2, 6, 7, 3, explosion_4_middle_a_color, explosion_4_middle_a_shading),
        face_3d(explosion_4_vertices, vertex_3d(0.0, 0.6545706922416016, 0.7560007995090683), 5, 9, 10, 6, explosion_4_middle_a_color, explosion_4_middle_a_shading),
        face_3d(explosion_4_vertices, vertex_3d(0.6546779167345106, -0.6546779167345106, 0.37788047141963094), 10, 14, 15, 11, explosion_4_middle_a_color, explosion_4_middle_a_shading),
        face_3d(explosion_4_vertices, vertex_3d(0.6546779255396236, 0.6546149272788903, -0.37798956439932074), 13, 17, 18, 14, explosion_4_middle_a_color, explosion_4_middle_a_shading),
        face_3d(explosion_4_vertices, vertex_3d(0.0, -0.6545706922416017, -0.7560007995090683), 18, 22, 23, 19, explosion_4_middle_a_color, explosion_4_middle_a_shading),
        face_3d(explosion_4_vertices, vertex_3d(-0.6546509272536523, 0.6546509272536523, -0.37797397647439507), 21, 1, 2, 22, explosion_4_middle_a_color, explosion_4_middle_a_shading),
        face_3d(explosion_4_vertices, vertex_3d(-0.6546779255396236, 0.6546149272788903, 0.37798956439932074), 1, 5, 6, 2, explosion_4_middle_b_color, explosion_4_middle_b_shading),
        face_3d(explosion_4_vertices, vertex_3d(0.0, -0.6545706922416017, 0.7560007995090683), 6, 10, 11, 7, explosion_4_middle_b_color, explosion_4_middle_b_shading),
        face_3d(explosion_4_vertices, vertex_3d(0.6546509272536523, 0.6546509272536523, 0.37797397647439507), 9, 13, 14, 10, explosion_4_middle_b_color, explosion_4_middle_b_shading),
        face_3d(explosion_4_vertices, vertex_3d(0.6547454027071528, -0.6545879362803353, -0.37791942436199294), 14, 18, 19, 15, explosion_4_middle_b_color, explosion_4_middle_b_shading),
        face_3d(explosion_4_vertices, vertex_3d(0.0, 0.6545706922416016, -0.7560007995090683), 17, 21, 22, 18, explosion_4_middle_b_color, explosion_4_middle_b_shading),
        face_3d(explosion_4_vertices, vertex_3d(-0.6546779167345106, -0.6546779167345106, -0.37788047141963094), 22, 2, 3, 23, explosion_4_middle_b_color, explosion_4_middle_b_shading),
    };

    constexpr inline model_3d_item explosion_4(explosion_4_vertices, explosion_4_faces);
}

#endif

