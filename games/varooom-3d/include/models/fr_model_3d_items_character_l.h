/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_CHARACTER_L_H
#define FR_MODEL_3D_ITEMS_CHARACTER_L_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d character_l_vertices[] = {
        vertex_3d(3.0, 15.0, 3.0),
        vertex_3d(3.0, 15.0, 9.0),
        vertex_3d(3.0, -15.0, 9.0),
        vertex_3d(3.0, -9.0, 3.0),
        vertex_3d(3.0, -15.0, -9.0),
        vertex_3d(3.0, -9.0, -9.0),
        vertex_3d(-3.0, 15.0, 3.0),
        vertex_3d(-3.0, 15.0, 9.0),
        vertex_3d(-3.0, -15.0, -9.0),
        vertex_3d(-3.0, -9.0, -9.0),
        vertex_3d(-3.0, -9.0, 3.0),
        vertex_3d(-3.0, -15.0, 9.0),
    };

    constexpr inline int character_l_front_color = 0;
    constexpr inline int character_l_front_shading = 7;

    constexpr inline int character_l_back_color = 0;
    constexpr inline int character_l_back_shading = 0;

    constexpr inline int character_l_up_color = 0;
    constexpr inline int character_l_up_shading = 3;

    constexpr inline int character_l_left_color = 0;
    constexpr inline int character_l_left_shading = 2;

    constexpr inline int character_l_right_color = 0;
    constexpr inline int character_l_right_shading = 2;

    constexpr inline int character_l_down_color = 0;
    constexpr inline int character_l_down_shading = 1;

    constexpr inline face_3d character_l_faces[] = {
        face_3d(character_l_vertices, vertex_3d(1.0, -0.0, 0.0), 0, 1, 2, 3, character_l_front_color, character_l_front_shading),
        face_3d(character_l_vertices, vertex_3d(1.0, 0.0, 0.0), 2, 4, 5, 3, character_l_front_color, character_l_front_shading),
        face_3d(character_l_vertices, vertex_3d(-1.0, 0.0, 0.0), 6, 10, 11, 7, character_l_back_color, character_l_back_shading),
        face_3d(character_l_vertices, vertex_3d(-1.0, 0.0, 0.0), 11, 10, 9, 8, character_l_back_color, character_l_back_shading),
        face_3d(character_l_vertices, vertex_3d(0.0, 1.0, 0.0), 3, 5, 9, 10, character_l_up_color, character_l_up_shading),
        face_3d(character_l_vertices, vertex_3d(0.0, 1.0, 0.0), 1, 0, 6, 7, character_l_up_color, character_l_up_shading),
        face_3d(character_l_vertices, vertex_3d(0.0, 0.0, 1.0), 1, 7, 11, 2, character_l_left_color, character_l_left_shading),
        face_3d(character_l_vertices, vertex_3d(0.0, 0.0, -1.0), 3, 10, 6, 0, character_l_right_color, character_l_right_shading),
        face_3d(character_l_vertices, vertex_3d(0.0, 0.0, -1.0), 5, 4, 8, 9, character_l_right_color, character_l_right_shading),
        face_3d(character_l_vertices, vertex_3d(0.0, -1.0, 0.0), 4, 2, 11, 8, character_l_down_color, character_l_down_shading),
    };

    constexpr inline model_3d_item character_l(character_l_vertices, character_l_faces);
}

#endif

