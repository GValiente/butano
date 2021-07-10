/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_CHARACTER_E_H
#define FR_MODEL_3D_ITEMS_CHARACTER_E_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d character_e_vertices[] = {
        vertex_3d(3.0, 9.0, -9.0),
        vertex_3d(3.0, 9.0, 3.0),
        vertex_3d(3.0, 15.0, 9.0),
        vertex_3d(3.0, 15.0, -9.0),
        vertex_3d(3.0, 3.0, -3.0),
        vertex_3d(3.0, -3.0, -3.0),
        vertex_3d(3.0, -3.0, 3.0),
        vertex_3d(3.0, 3.0, 3.0),
        vertex_3d(3.0, -9.0, -9.0),
        vertex_3d(3.0, -15.0, -9.0),
        vertex_3d(3.0, -15.0, 9.0),
        vertex_3d(3.0, -9.0, 3.0),
        vertex_3d(-3.0, 9.0, -9.0),
        vertex_3d(-3.0, 15.0, -9.0),
        vertex_3d(-3.0, 15.0, 9.0),
        vertex_3d(-3.0, 9.0, 3.0),
        vertex_3d(-3.0, 3.0, -3.0),
        vertex_3d(-3.0, 3.0, 3.0),
        vertex_3d(-3.0, -3.0, 3.0),
        vertex_3d(-3.0, -3.0, -3.0),
        vertex_3d(-3.0, -9.0, -9.0),
        vertex_3d(-3.0, -9.0, 3.0),
        vertex_3d(-3.0, -15.0, 9.0),
        vertex_3d(-3.0, -15.0, -9.0),
    };

    constexpr inline int character_e_front_color = 0;
    constexpr inline int character_e_front_shading = 7;

    constexpr inline int character_e_back_color = 0;
    constexpr inline int character_e_back_shading = 0;

    constexpr inline int character_e_up_color = 0;
    constexpr inline int character_e_up_shading = 3;

    constexpr inline int character_e_left_color = 0;
    constexpr inline int character_e_left_shading = 2;

    constexpr inline int character_e_right_color = 0;
    constexpr inline int character_e_right_shading = 2;

    constexpr inline int character_e_down_color = 0;
    constexpr inline int character_e_down_shading = 1;

    constexpr inline face_3d character_e_faces[] = {
        face_3d(character_e_vertices, vertex_3d(1.0, 0.0, 0.0), 0, 3, 2, 1, character_e_front_color, character_e_front_shading),
        face_3d(character_e_vertices, vertex_3d(1.0, -0.0, 0.0), 4, 7, 6, 5, character_e_front_color, character_e_front_shading),
        face_3d(character_e_vertices, vertex_3d(1.0, 0.0, 0.0), 8, 11, 10, 9, character_e_front_color, character_e_front_shading),
        face_3d(character_e_vertices, vertex_3d(1.0, 0.0, 0.0), 10, 11, 1, 2, character_e_front_color, character_e_front_shading),
        face_3d(character_e_vertices, vertex_3d(-1.0, 0.0, 0.0), 12, 15, 14, 13, character_e_back_color, character_e_back_shading),
        face_3d(character_e_vertices, vertex_3d(-1.0, 0.0, 0.0), 16, 19, 18, 17, character_e_back_color, character_e_back_shading),
        face_3d(character_e_vertices, vertex_3d(-1.0, 0.0, 0.0), 20, 23, 22, 21, character_e_back_color, character_e_back_shading),
        face_3d(character_e_vertices, vertex_3d(-1.0, 0.0, 0.0), 22, 14, 15, 21, character_e_back_color, character_e_back_shading),
        face_3d(character_e_vertices, vertex_3d(0.0, 1.0, 0.0), 3, 13, 14, 2, character_e_up_color, character_e_up_shading),
        face_3d(character_e_vertices, vertex_3d(0.0, 1.0, 0.0), 11, 8, 20, 21, character_e_up_color, character_e_up_shading),
        face_3d(character_e_vertices, vertex_3d(0.0, 1.0, 0.0), 7, 4, 16, 17, character_e_up_color, character_e_up_shading),
        face_3d(character_e_vertices, vertex_3d(0.0, 0.0, 1.0), 2, 14, 22, 10, character_e_left_color, character_e_left_shading),
        face_3d(character_e_vertices, vertex_3d(0.0, 0.0, -1.0), 8, 9, 23, 20, character_e_right_color, character_e_right_shading),
        face_3d(character_e_vertices, vertex_3d(0.0, 0.0, -1.0), 11, 21, 18, 6, character_e_right_color, character_e_right_shading),
        face_3d(character_e_vertices, vertex_3d(0.0, 0.0, -1.0), 7, 17, 15, 1, character_e_right_color, character_e_right_shading),
        face_3d(character_e_vertices, vertex_3d(0.0, 0.0, -1.0), 0, 12, 13, 3, character_e_right_color, character_e_right_shading),
        face_3d(character_e_vertices, vertex_3d(0.0, 0.0, -1.0), 4, 5, 19, 16, character_e_right_color, character_e_right_shading),
        face_3d(character_e_vertices, vertex_3d(0.0, -1.0, 0.0), 9, 10, 22, 23, character_e_down_color, character_e_down_shading),
        face_3d(character_e_vertices, vertex_3d(0.0, -1.0, 0.0), 1, 15, 12, 0, character_e_down_color, character_e_down_shading),
        face_3d(character_e_vertices, vertex_3d(0.0, -1.0, 0.0), 5, 6, 18, 19, character_e_down_color, character_e_down_shading),
    };

    constexpr inline model_3d_item character_e(character_e_vertices, character_e_faces);
}

#endif

