/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_CHARACTER_A_H
#define FR_MODEL_3D_ITEMS_CHARACTER_A_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d character_a_vertices[] = {
        vertex_3d(3.0, 9.0, 3.0),
        vertex_3d(3.0, 15.0, 9.0),
        vertex_3d(3.0, -15.0, 9.0),
        vertex_3d(3.0, 3.0, 3.0),
        vertex_3d(3.0, -15.0, -9.0),
        vertex_3d(3.0, 3.0, -3.0),
        vertex_3d(3.0, 9.0, -3.0),
        vertex_3d(3.0, 15.0, -9.0),
        vertex_3d(-3.0, 9.0, -3.0),
        vertex_3d(-3.0, 9.0, 3.0),
        vertex_3d(-3.0, 15.0, 9.0),
        vertex_3d(-3.0, 15.0, -9.0),
        vertex_3d(-3.0, -15.0, -9.0),
        vertex_3d(-3.0, 3.0, -3.0),
        vertex_3d(-3.0, 3.0, 3.0),
        vertex_3d(-3.0, -15.0, 9.0),
        vertex_3d(3.0, -3.0, 3.0),
        vertex_3d(3.0, -3.0, -3.0),
        vertex_3d(-3.0, -3.0, -3.0),
        vertex_3d(-3.0, -3.0, 3.0),
        vertex_3d(3.0, -15.0, 3.0),
        vertex_3d(3.0, -15.0, -3.0),
        vertex_3d(-3.0, -15.0, -3.0),
        vertex_3d(-3.0, -15.0, 3.0),
    };

    constexpr inline int character_a_front_color = 7;
    constexpr inline int character_a_front_shading = 7;

    constexpr inline int character_a_back_color = 7;
    constexpr inline int character_a_back_shading = 0;

    constexpr inline int character_a_up_color = 7;
    constexpr inline int character_a_up_shading = 3;

    constexpr inline int character_a_left_color = 7;
    constexpr inline int character_a_left_shading = 2;

    constexpr inline int character_a_right_color = 7;
    constexpr inline int character_a_right_shading = 2;

    constexpr inline int character_a_down_color = 7;
    constexpr inline int character_a_down_shading = 1;

    constexpr inline face_3d character_a_faces[] = {
        face_3d(character_a_vertices, vertex_3d(1.0, 0.0, 0.0), 1, 0, 6, 7, character_a_front_color, character_a_front_shading),
        face_3d(character_a_vertices, vertex_3d(1.0, -0.0, 0.0), 7, 6, 21, 4, character_a_front_color, character_a_front_shading),
        face_3d(character_a_vertices, vertex_3d(1.0, 0.0, 0.0), 3, 16, 17, 5, character_a_front_color, character_a_front_shading),
        face_3d(character_a_vertices, vertex_3d(1.0, 0.0, 0.0), 1, 2, 20, 0, character_a_front_color, character_a_front_shading),
        face_3d(character_a_vertices, vertex_3d(-1.0, 0.0, 0.0), 8, 9, 10, 11, character_a_back_color, character_a_back_shading),
        face_3d(character_a_vertices, vertex_3d(-1.0, 0.0, 0.0), 14, 13, 18, 19, character_a_back_color, character_a_back_shading),
        face_3d(character_a_vertices, vertex_3d(-1.0, 0.0, 0.0), 15, 10, 9, 23, character_a_back_color, character_a_back_shading),
        face_3d(character_a_vertices, vertex_3d(-1.0, -0.0, 0.0), 12, 22, 8, 11, character_a_back_color, character_a_back_shading),
        face_3d(character_a_vertices, vertex_3d(0.0, 1.0, 0.0), 5, 13, 14, 3, character_a_up_color, character_a_up_shading),
        face_3d(character_a_vertices, vertex_3d(0.0, 1.0, 0.0), 7, 11, 10, 1, character_a_up_color, character_a_up_shading),
        face_3d(character_a_vertices, vertex_3d(-0.0, 0.0, 1.0), 5, 6, 8, 13, character_a_left_color, character_a_left_shading),
        face_3d(character_a_vertices, vertex_3d(0.0, 0.0, 1.0), 17, 18, 22, 21, character_a_left_color, character_a_left_shading),
        face_3d(character_a_vertices, vertex_3d(-0.0, 0.0, 1.0), 2, 1, 10, 15, character_a_left_color, character_a_left_shading),
        face_3d(character_a_vertices, vertex_3d(0.0, 0.0, -1.0), 3, 14, 9, 0, character_a_right_color, character_a_right_shading),
        face_3d(character_a_vertices, vertex_3d(0.0, 0.0, -1.0), 16, 20, 23, 19, character_a_right_color, character_a_right_shading),
        face_3d(character_a_vertices, vertex_3d(0.0, 0.0, -1.0), 4, 12, 11, 7, character_a_right_color, character_a_right_shading),
        face_3d(character_a_vertices, vertex_3d(0.0, -1.0, -0.0), 12, 4, 21, 22, character_a_down_color, character_a_down_shading),
        face_3d(character_a_vertices, vertex_3d(0.0, -1.0, 0.0), 16, 19, 18, 17, character_a_down_color, character_a_down_shading),
        face_3d(character_a_vertices, vertex_3d(0.0, -1.0, 0.0), 9, 8, 6, 0, character_a_down_color, character_a_down_shading),
        face_3d(character_a_vertices, vertex_3d(0.0, -1.0, 0.0), 15, 23, 20, 2, character_a_down_color, character_a_down_shading),
    };

    constexpr inline model_3d_item character_a(character_a_vertices, character_a_faces);
}

#endif

