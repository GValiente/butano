/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_EXPLOSION_7_H
#define FR_MODEL_3D_ITEMS_EXPLOSION_7_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d explosion_7_vertices[] = {
        vertex_3d(26.0, 0.0, 0.0),
        vertex_3d(20.0, 6.0, 0.0),
        vertex_3d(14.0, 0.0, 0.0),
        vertex_3d(20.0, -6.0, 0.0),
        vertex_3d(13.0, 0.0, -22.52),
        vertex_3d(10.0, 6.0, -17.32),
        vertex_3d(7.0, 0.0, -12.12),
        vertex_3d(10.0, -6.0, -17.32),
        vertex_3d(-13.0, 0.0, -22.52),
        vertex_3d(-10.0, 6.0, -17.32),
        vertex_3d(-7.0, 0.0, -12.12),
        vertex_3d(-10.0, -6.0, -17.32),
        vertex_3d(-26.0, 0.0, 0.0),
        vertex_3d(-20.0, 6.0, 0.0),
        vertex_3d(-14.0, 0.0, 0.0),
        vertex_3d(-20.0, -6.0, 0.0),
        vertex_3d(-13.0, 0.0, 22.52),
        vertex_3d(-10.0, 6.0, 17.32),
        vertex_3d(-7.0, 0.0, 12.12),
        vertex_3d(-10.0, -6.0, 17.32),
        vertex_3d(13.0, 0.0, 22.52),
        vertex_3d(10.0, 6.0, 17.32),
        vertex_3d(7.0, 0.0, 12.12),
        vertex_3d(10.0, -6.0, 17.32),
    };

    constexpr inline int explosion_7_top_a_color = 7;
    constexpr inline int explosion_7_top_a_shading = 7;

    constexpr inline int explosion_7_top_b_color = 7;
    constexpr inline int explosion_7_top_b_shading = 5;

    constexpr inline int explosion_7_middle_a_color = 7;
    constexpr inline int explosion_7_middle_a_shading = 6;

    constexpr inline int explosion_7_middle_b_color = 7;
    constexpr inline int explosion_7_middle_b_shading = 4;

    constexpr inline face_3d explosion_7_faces[] = {
        face_3d(explosion_7_vertices, vertex_3d(0.6545429415349606, -0.6547948826210016, -0.3779116290617555), 3, 7, 4, 0, explosion_7_top_a_color, explosion_7_top_a_shading),
        face_3d(explosion_7_vertices, vertex_3d(0.0, 0.6549305384178419, -0.7556890827898176), 4, 8, 9, 5, explosion_7_top_a_color, explosion_7_top_a_shading),
        face_3d(explosion_7_vertices, vertex_3d(-0.6546509272536523, -0.6546509272536524, -0.37797397647439507), 11, 15, 12, 8, explosion_7_top_a_color, explosion_7_top_a_shading),
        face_3d(explosion_7_vertices, vertex_3d(-0.6545844799753632, 0.6547782586846512, 0.37786848311188814), 12, 16, 17, 13, explosion_7_top_a_color, explosion_7_top_a_shading),
        face_3d(explosion_7_vertices, vertex_3d(0.0, -0.6549305384178418, 0.7556890827898176), 19, 23, 20, 16, explosion_7_top_a_color, explosion_7_top_a_shading),
        face_3d(explosion_7_vertices, vertex_3d(0.6546675385356144, 0.6546675385356144, 0.3779164298829035), 20, 0, 1, 21, explosion_7_top_a_color, explosion_7_top_a_shading),
        face_3d(explosion_7_vertices, vertex_3d(0.6545844799753632, 0.6547782586846512, -0.37786848311188814), 0, 4, 5, 1, explosion_7_top_b_color, explosion_7_top_b_shading),
        face_3d(explosion_7_vertices, vertex_3d(0.0, -0.6549305384178418, -0.7556890827898176), 7, 11, 8, 4, explosion_7_top_b_color, explosion_7_top_b_shading),
        face_3d(explosion_7_vertices, vertex_3d(-0.6546675385356144, 0.6546675385356144, -0.3779164298829035), 8, 12, 13, 9, explosion_7_top_b_color, explosion_7_top_b_shading),
        face_3d(explosion_7_vertices, vertex_3d(-0.6545429415349606, -0.6547948826210016, 0.3779116290617555), 15, 19, 16, 12, explosion_7_top_b_color, explosion_7_top_b_shading),
        face_3d(explosion_7_vertices, vertex_3d(0.0, 0.6549305384178419, 0.7556890827898176), 16, 20, 21, 17, explosion_7_top_b_color, explosion_7_top_b_shading),
        face_3d(explosion_7_vertices, vertex_3d(0.6546509272536523, -0.6546509272536524, 0.37797397647439507), 23, 3, 0, 20, explosion_7_top_b_color, explosion_7_top_b_shading),
        face_3d(explosion_7_vertices, vertex_3d(-0.6544657668332726, -0.6548257590042482, 0.37799177952416735), 2, 6, 7, 3, explosion_7_middle_a_color, explosion_7_middle_a_shading),
        face_3d(explosion_7_vertices, vertex_3d(0.0, 0.654930538417842, 0.7556890827898174), 5, 9, 10, 6, explosion_7_middle_a_color, explosion_7_middle_a_shading),
        face_3d(explosion_7_vertices, vertex_3d(0.6546200575899069, -0.6546200575899072, 0.378080891347306), 10, 14, 15, 11, explosion_7_middle_a_color, explosion_7_middle_a_shading),
        face_3d(explosion_7_vertices, vertex_3d(0.6545429415349605, 0.6547948826210017, -0.3779116290617555), 13, 17, 18, 14, explosion_7_middle_a_color, explosion_7_middle_a_shading),
        face_3d(explosion_7_vertices, vertex_3d(0.0, -0.654930538417842, -0.7556890827898175), 18, 22, 23, 19, explosion_7_middle_a_color, explosion_7_middle_a_shading),
        face_3d(explosion_7_vertices, vertex_3d(-0.6546509272536523, 0.6546509272536523, -0.37797397647439507), 21, 1, 2, 22, explosion_7_middle_a_color, explosion_7_middle_a_shading),
        face_3d(explosion_7_vertices, vertex_3d(-0.6545429415349605, 0.6547948826210017, 0.3779116290617555), 1, 5, 6, 2, explosion_7_middle_b_color, explosion_7_middle_b_shading),
        face_3d(explosion_7_vertices, vertex_3d(0.0, -0.654930538417842, 0.7556890827898175), 6, 10, 11, 7, explosion_7_middle_b_color, explosion_7_middle_b_shading),
        face_3d(explosion_7_vertices, vertex_3d(0.6546509272536523, 0.6546509272536523, 0.37797397647439507), 9, 13, 14, 10, explosion_7_middle_b_color, explosion_7_middle_b_shading),
        face_3d(explosion_7_vertices, vertex_3d(0.6544657668332726, -0.6548257590042482, -0.37799177952416735), 14, 18, 19, 15, explosion_7_middle_b_color, explosion_7_middle_b_shading),
        face_3d(explosion_7_vertices, vertex_3d(0.0, 0.654930538417842, -0.7556890827898174), 17, 21, 22, 18, explosion_7_middle_b_color, explosion_7_middle_b_shading),
        face_3d(explosion_7_vertices, vertex_3d(-0.6546200575899069, -0.6546200575899072, -0.378080891347306), 22, 2, 3, 23, explosion_7_middle_b_color, explosion_7_middle_b_shading),
    };

    constexpr inline model_3d_item explosion_7(explosion_7_vertices, explosion_7_faces);
}

#endif

