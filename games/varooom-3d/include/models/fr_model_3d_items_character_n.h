/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_CHARACTER_N_H
#define FR_MODEL_3D_ITEMS_CHARACTER_N_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d character_n_vertices[] = {
        vertex_3d(3.0, 9.0, 3.0),
        vertex_3d(3.0, 15.0, 9.0),
        vertex_3d(3.0, -15.0, 9.0),
        vertex_3d(3.0, -15.0, 3.0),
        vertex_3d(3.0, -15.0, -9.0),
        vertex_3d(3.0, -15.0, -3.0),
        vertex_3d(3.0, 9.0, -3.0),
        vertex_3d(3.0, 15.0, -9.0),
        vertex_3d(-3.0, 9.0, -3.0),
        vertex_3d(-3.0, 9.0, 3.0),
        vertex_3d(-3.0, 15.0, 9.0),
        vertex_3d(-3.0, 15.0, -9.0),
        vertex_3d(-3.0, -15.0, -9.0),
        vertex_3d(-3.0, -15.0, -3.0),
        vertex_3d(-3.0, -15.0, 3.0),
        vertex_3d(-3.0, -15.0, 9.0),
        vertex_3d(3.0, 3.0, -3.0),
        vertex_3d(-3.0, 3.0, -3.0),
        vertex_3d(3.0, 15.0, 3.0),
        vertex_3d(-3.0, 15.0, 3.0),
        vertex_3d(3.0, 15.0, -3.0),
        vertex_3d(-3.0, 15.0, -3.0),
    };

    constexpr inline int character_n_front_color = 0;
    constexpr inline int character_n_front_shading = 7;

    constexpr inline int character_n_back_color = 0;
    constexpr inline int character_n_back_shading = 0;

    constexpr inline int character_n_up_color = 0;
    constexpr inline int character_n_up_shading = 3;

    constexpr inline int character_n_left_color = 0;
    constexpr inline int character_n_left_shading = 2;

    constexpr inline int character_n_right_color = 0;
    constexpr inline int character_n_right_shading = 2;

    constexpr inline int character_n_down_color = 0;
    constexpr inline int character_n_down_shading = 1;

    constexpr inline face_3d character_n_faces[] = {
        face_3d(character_n_vertices, vertex_3d(1.0, 0.0, -0.0), 0, 16, 6, 18, character_n_front_color, character_n_front_shading),
        face_3d(character_n_vertices, vertex_3d(1.0, 0.0, 0.0), 2, 3, 18, 1, character_n_front_color, character_n_front_shading),
        face_3d(character_n_vertices, vertex_3d(1.0, -0.0, 0.0), 7, 20, 5, 4, character_n_front_color, character_n_front_shading),
        face_3d(character_n_vertices, vertex_3d(-1.0, 0.0, -0.0), 8, 17, 9, 19, character_n_back_color, character_n_back_shading),
        face_3d(character_n_vertices, vertex_3d(-1.0, 0.0, 0.0), 15, 10, 19, 14, character_n_back_color, character_n_back_shading),
        face_3d(character_n_vertices, vertex_3d(-1.0, 0.0, 0.0), 13, 21, 11, 12, character_n_back_color, character_n_back_shading),
        face_3d(character_n_vertices, vertex_3d(0.0, 1.0, -0.0), 10, 1, 18, 19, character_n_up_color, character_n_up_shading),
        face_3d(character_n_vertices, vertex_3d(0.0, 1.0, 0.0), 11, 21, 20, 7, character_n_up_color, character_n_up_shading),
        face_3d(character_n_vertices, vertex_3d(0.0, 0.7071067811865476, -0.7071067811865476), 6, 8, 19, 18, character_n_up_color, character_n_up_shading),
        face_3d(character_n_vertices, vertex_3d(0.0, 0.0, 1.0), 21, 8, 6, 20, character_n_left_color, character_n_left_shading),
        face_3d(character_n_vertices, vertex_3d(0.0, 0.0, 1.0), 13, 5, 16, 17, character_n_left_color, character_n_left_shading),
        face_3d(character_n_vertices, vertex_3d(-0.0, 0.0, 1.0), 2, 1, 10, 15, character_n_left_color, character_n_left_shading),
        face_3d(character_n_vertices, vertex_3d(0.0, 0.0, -1.0), 4, 12, 11, 7, character_n_right_color, character_n_right_shading),
        face_3d(character_n_vertices, vertex_3d(0.0, 0.0, -1.0), 3, 14, 9, 0, character_n_right_color, character_n_right_shading),
        face_3d(character_n_vertices, vertex_3d(0.0, -1.0, 0.0), 4, 5, 13, 12, character_n_down_color, character_n_down_shading),
        face_3d(character_n_vertices, vertex_3d(0.0, -1.0, 0.0), 3, 2, 15, 14, character_n_down_color, character_n_down_shading),
        face_3d(character_n_vertices, vertex_3d(0.0, -0.7071067811865476, 0.7071067811865476), 17, 16, 0, 9, character_n_down_color, character_n_down_shading),
    };

    constexpr inline model_3d_item character_n(character_n_vertices, character_n_faces);
}

#endif

