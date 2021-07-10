/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_CHARACTER_W_H
#define FR_MODEL_3D_ITEMS_CHARACTER_W_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d character_w_vertices[] = {
        vertex_3d(-3.0, -9.0, -3.0),
        vertex_3d(3.0, -12.0, 0.0),
        vertex_3d(3.0, -9.0, -3.0),
        vertex_3d(3.0, 15.0, 3.0),
        vertex_3d(3.0, -15.0, 9.0),
        vertex_3d(3.0, -15.0, 3.0),
        vertex_3d(3.0, 15.0, 9.0),
        vertex_3d(3.0, 15.0, -9.0),
        vertex_3d(3.0, 15.0, -3.0),
        vertex_3d(3.0, -15.0, -3.0),
        vertex_3d(3.0, -15.0, -9.0),
        vertex_3d(-3.0, -9.0, 3.0),
        vertex_3d(-3.0, -12.0, 0.0),
        vertex_3d(3.0, -9.0, 3.0),
        vertex_3d(-3.0, 15.0, 3.0),
        vertex_3d(-3.0, -15.0, 9.0),
        vertex_3d(-3.0, 15.0, 9.0),
        vertex_3d(-3.0, -15.0, 3.0),
        vertex_3d(-3.0, -15.0, -3.0),
        vertex_3d(-3.0, 15.0, -3.0),
        vertex_3d(-3.0, 15.0, -9.0),
        vertex_3d(-3.0, -15.0, -9.0),
        vertex_3d(3.0, -6.0, 0.0),
        vertex_3d(-3.0, -6.0, 0.0),
    };

    constexpr inline int character_w_front_color = 0;
    constexpr inline int character_w_front_shading = 7;

    constexpr inline int character_w_back_color = 0;
    constexpr inline int character_w_back_shading = 0;

    constexpr inline int character_w_up_color = 0;
    constexpr inline int character_w_up_shading = 3;

    constexpr inline int character_w_left_color = 0;
    constexpr inline int character_w_left_shading = 2;

    constexpr inline int character_w_right_color = 0;
    constexpr inline int character_w_right_shading = 2;

    constexpr inline int character_w_down_color = 0;
    constexpr inline int character_w_down_shading = 1;

    constexpr inline face_3d character_w_faces[] = {
        face_3d(character_w_vertices, vertex_3d(1.0, 0.0, 0.0), 22, 1, 9, 2, character_w_front_color, character_w_front_shading),
        face_3d(character_w_vertices, vertex_3d(1.0, 0.0, 0.0), 4, 5, 3, 6, character_w_front_color, character_w_front_shading),
        face_3d(character_w_vertices, vertex_3d(1.0, -0.0, 0.0), 7, 8, 9, 10, character_w_front_color, character_w_front_shading),
        face_3d(character_w_vertices, vertex_3d(1.0, 0.0, -0.0), 13, 5, 1, 22, character_w_front_color, character_w_front_shading),
        face_3d(character_w_vertices, vertex_3d(-1.0, 0.0, 0.0), 17, 11, 23, 12, character_w_back_color, character_w_back_shading),
        face_3d(character_w_vertices, vertex_3d(-1.0, 0.0, 0.0), 15, 16, 14, 17, character_w_back_color, character_w_back_shading),
        face_3d(character_w_vertices, vertex_3d(-1.0, 0.0, 0.0), 18, 19, 20, 21, character_w_back_color, character_w_back_shading),
        face_3d(character_w_vertices, vertex_3d(-1.0, 0.0, 0.0), 12, 23, 0, 18, character_w_back_color, character_w_back_shading),
        face_3d(character_w_vertices, vertex_3d(-0.0, 0.7071067811865476, 0.7071067811865476), 13, 22, 23, 11, character_w_up_color, character_w_up_shading),
        face_3d(character_w_vertices, vertex_3d(0.0, 1.0, 0.0), 20, 19, 8, 7, character_w_up_color, character_w_up_shading),
        face_3d(character_w_vertices, vertex_3d(0.0, 0.7071067811865476, -0.7071067811865476), 0, 23, 22, 2, character_w_up_color, character_w_up_shading),
        face_3d(character_w_vertices, vertex_3d(0.0, 1.0, 0.0), 6, 3, 14, 16, character_w_up_color, character_w_up_shading),
        face_3d(character_w_vertices, vertex_3d(-0.0, 0.0, 1.0), 4, 6, 16, 15, character_w_left_color, character_w_left_shading),
        face_3d(character_w_vertices, vertex_3d(0.0, 0.0, 1.0), 0, 2, 8, 19, character_w_left_color, character_w_left_shading),
        face_3d(character_w_vertices, vertex_3d(-0.0, 0.0, -1.0), 11, 14, 3, 13, character_w_right_color, character_w_right_shading),
        face_3d(character_w_vertices, vertex_3d(0.0, 0.0, -1.0), 10, 21, 20, 7, character_w_right_color, character_w_right_shading),
        face_3d(character_w_vertices, vertex_3d(0.0, -1.0, 0.0), 10, 9, 18, 21, character_w_down_color, character_w_down_shading),
        face_3d(character_w_vertices, vertex_3d(0.0, -1.0, 0.0), 5, 4, 15, 17, character_w_down_color, character_w_down_shading),
        face_3d(character_w_vertices, vertex_3d(0.0, -0.7071067811865476, 0.7071067811865476), 12, 18, 9, 1, character_w_down_color, character_w_down_shading),
        face_3d(character_w_vertices, vertex_3d(0.0, -0.7071067811865476, -0.7071067811865476), 12, 1, 5, 17, character_w_down_color, character_w_down_shading),
    };

    constexpr inline model_3d_item character_w(character_w_vertices, character_w_faces);
}

#endif

