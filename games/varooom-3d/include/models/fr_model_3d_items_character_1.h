/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_CHARACTER_1_H
#define FR_MODEL_3D_ITEMS_CHARACTER_1_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d character_1_vertices[] = {
        vertex_3d(3.0, 9.0, 6.0),
        vertex_3d(3.0, 9.0, 0.0),
        vertex_3d(3.0, -15.0, 0.0),
        vertex_3d(3.0, -15.0, -6.0),
        vertex_3d(3.0, 15.0, -6.0),
        vertex_3d(3.0, 15.0, 6.0),
        vertex_3d(-3.0, 9.0, 6.0),
        vertex_3d(-3.0, 9.0, 0.0),
        vertex_3d(-3.0, -15.0, 0.0),
        vertex_3d(-3.0, -15.0, -6.0),
        vertex_3d(-3.0, 15.0, -6.0),
        vertex_3d(-3.0, 15.0, 6.0),
    };

    constexpr inline int character_1_front_color = 7;
    constexpr inline int character_1_front_shading = 7;

    constexpr inline int character_1_back_color = 7;
    constexpr inline int character_1_back_shading = 0;

    constexpr inline int character_1_up_color = 7;
    constexpr inline int character_1_up_shading = 3;

    constexpr inline int character_1_left_color = 7;
    constexpr inline int character_1_left_shading = 2;

    constexpr inline int character_1_right_color = 7;
    constexpr inline int character_1_right_shading = 2;

    constexpr inline int character_1_down_color = 7;
    constexpr inline int character_1_down_shading = 1;

    constexpr inline face_3d character_1_faces[] = {
        face_3d(character_1_vertices, vertex_3d(1.0, 0.0, 0.0), 0, 1, 4, 5, character_1_front_color, character_1_front_shading),
        face_3d(character_1_vertices, vertex_3d(1.0, -0.0, 0.0), 4, 1, 2, 3, character_1_front_color, character_1_front_shading),
        face_3d(character_1_vertices, vertex_3d(-1.0, -0.0, 0.0), 9, 8, 7, 10, character_1_back_color, character_1_back_shading),
        face_3d(character_1_vertices, vertex_3d(1.0, 0.0, 0.0), 6, 7, 10, 11, character_1_back_color, character_1_back_shading),
        face_3d(character_1_vertices, vertex_3d(0.0, 1.0, 0.0), 4, 10, 11, 5, character_1_up_color, character_1_up_shading),
        face_3d(character_1_vertices, vertex_3d(0.0, -1.0, -0.0), 9, 3, 2, 8, character_1_left_color, character_1_left_shading),
        face_3d(character_1_vertices, vertex_3d(0.0, 0.0, 1.0), 1, 7, 8, 2, character_1_left_color, character_1_left_shading),
        face_3d(character_1_vertices, vertex_3d(0.0, 0.0, 1.0), 5, 11, 6, 0, character_1_left_color, character_1_left_shading),
        face_3d(character_1_vertices, vertex_3d(0.0, 0.0, -1.0), 10, 4, 3, 9, character_1_right_color, character_1_right_shading),
        face_3d(character_1_vertices, vertex_3d(0.0, -1.0, 0.0), 0, 6, 7, 1, character_1_down_color, character_1_down_shading),
    };

    constexpr inline model_3d_item character_1(character_1_vertices, character_1_faces);
}

#endif

