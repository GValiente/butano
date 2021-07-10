/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_CHARACTER_I_H
#define FR_MODEL_3D_ITEMS_CHARACTER_I_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d character_i_vertices[] = {
        vertex_3d(3.0, 9.0, 3.0),
        vertex_3d(3.0, 15.0, 9.0),
        vertex_3d(3.0, -15.0, 9.0),
        vertex_3d(3.0, -9.0, 3.0),
        vertex_3d(3.0, -15.0, -9.0),
        vertex_3d(3.0, -9.0, -3.0),
        vertex_3d(3.0, 9.0, -3.0),
        vertex_3d(3.0, 15.0, -9.0),
        vertex_3d(-3.0, 9.0, -3.0),
        vertex_3d(-3.0, 9.0, 3.0),
        vertex_3d(-3.0, 15.0, 9.0),
        vertex_3d(-3.0, 15.0, -9.0),
        vertex_3d(-3.0, -15.0, -9.0),
        vertex_3d(-3.0, -9.0, -3.0),
        vertex_3d(-3.0, -9.0, 3.0),
        vertex_3d(-3.0, -15.0, 9.0),
        vertex_3d(3.0, 9.0, 9.0),
        vertex_3d(-3.0, 9.0, 9.0),
        vertex_3d(3.0, -9.0, 9.0),
        vertex_3d(-3.0, -9.0, 9.0),
        vertex_3d(3.0, -9.0, -9.0),
        vertex_3d(-3.0, -9.0, -9.0),
        vertex_3d(3.0, 9.0, -9.0),
        vertex_3d(-3.0, 9.0, -9.0),
    };

    constexpr inline int character_i_front_color = 0;
    constexpr inline int character_i_front_shading = 7;

    constexpr inline int character_i_back_color = 0;
    constexpr inline int character_i_back_shading = 0;

    constexpr inline int character_i_up_color = 0;
    constexpr inline int character_i_up_shading = 3;

    constexpr inline int character_i_left_color = 0;
    constexpr inline int character_i_left_shading = 2;

    constexpr inline int character_i_right_color = 0;
    constexpr inline int character_i_right_shading = 2;

    constexpr inline int character_i_down_color = 0;
    constexpr inline int character_i_down_shading = 1;

    constexpr inline face_3d character_i_faces[] = {
        face_3d(character_i_vertices, vertex_3d(1.0, 0.0, 0.0), 2, 4, 20, 18, character_i_front_color, character_i_front_shading),
        face_3d(character_i_vertices, vertex_3d(1.0, -0.0, 0.0), 7, 1, 16, 22, character_i_front_color, character_i_front_shading),
        face_3d(character_i_vertices, vertex_3d(1.0, 0.0, 0.0), 0, 3, 5, 6, character_i_front_color, character_i_front_shading),
        face_3d(character_i_vertices, vertex_3d(-1.0, 0.0, 0.0), 11, 23, 17, 10, character_i_back_color, character_i_back_shading),
        face_3d(character_i_vertices, vertex_3d(-1.0, 0.0, 0.0), 9, 8, 13, 14, character_i_back_color, character_i_back_shading),
        face_3d(character_i_vertices, vertex_3d(-1.0, 0.0, 0.0), 15, 19, 21, 12, character_i_back_color, character_i_back_shading),
        face_3d(character_i_vertices, vertex_3d(0.0, 1.0, -0.0), 13, 5, 20, 21, character_i_up_color, character_i_up_shading),
        face_3d(character_i_vertices, vertex_3d(0.0, 1.0, 0.0), 11, 10, 1, 7, character_i_up_color, character_i_up_shading),
        face_3d(character_i_vertices, vertex_3d(0.0, 1.0, 0.0), 14, 19, 18, 3, character_i_up_color, character_i_up_shading),
        face_3d(character_i_vertices, vertex_3d(0.0, 0.0, 1.0), 18, 19, 15, 2, character_i_left_color, character_i_left_shading),
        face_3d(character_i_vertices, vertex_3d(-0.0, 0.0, 1.0), 3, 0, 9, 14, character_i_left_color, character_i_left_shading),
        face_3d(character_i_vertices, vertex_3d(0.0, 0.0, 1.0), 10, 17, 16, 1, character_i_left_color, character_i_left_shading),
        face_3d(character_i_vertices, vertex_3d(-0.0, 0.0, -1.0), 23, 11, 7, 22, character_i_right_color, character_i_right_shading),
        face_3d(character_i_vertices, vertex_3d(0.0, 0.0, -1.0), 20, 4, 12, 21, character_i_right_color, character_i_right_shading),
        face_3d(character_i_vertices, vertex_3d(0.0, 0.0, -1.0), 5, 13, 8, 6, character_i_right_color, character_i_right_shading),
        face_3d(character_i_vertices, vertex_3d(0.0, -1.0, 0.0), 15, 12, 4, 2, character_i_down_color, character_i_down_shading),
        face_3d(character_i_vertices, vertex_3d(0.0, -1.0, 0.0), 8, 23, 22, 6, character_i_down_color, character_i_down_shading),
        face_3d(character_i_vertices, vertex_3d(0.0, -1.0, -0.0), 9, 0, 16, 17, character_i_down_color, character_i_down_shading),
    };

    constexpr inline model_3d_item character_i(character_i_vertices, character_i_faces);
}

#endif

