/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_CHARACTER_R_H
#define FR_MODEL_3D_ITEMS_CHARACTER_R_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d character_r_vertices[] = {
        vertex_3d(3.0, 9.0, 3.0),
        vertex_3d(3.0, 15.0, 9.0),
        vertex_3d(3.0, -15.0, 9.0),
        vertex_3d(3.0, 3.0, 3.0),
        vertex_3d(3.0, -15.0, -9.0),
        vertex_3d(3.0, 3.0, 0.0),
        vertex_3d(3.0, 9.0, 0.0),
        vertex_3d(3.0, 9.0, -9.0),
        vertex_3d(-3.0, 9.0, 0.0),
        vertex_3d(-3.0, 9.0, 3.0),
        vertex_3d(-3.0, 15.0, 9.0),
        vertex_3d(-3.0, 9.0, -9.0),
        vertex_3d(-3.0, -15.0, -9.0),
        vertex_3d(-3.0, 3.0, 0.0),
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
        vertex_3d(3.0, 15.0, -3.0),
        vertex_3d(-3.0, 15.0, -3.0),
        vertex_3d(3.0, 0.0, -6.0),
        vertex_3d(-3.0, 0.0, -6.0),
        vertex_3d(3.0, 3.0, -9.0),
        vertex_3d(-3.0, 3.0, -9.0),
        vertex_3d(3.0, -3.0, -9.0),
        vertex_3d(-3.0, -3.0, -9.0),
        vertex_3d(3.0, 6.0, -3.0),
        vertex_3d(-3.0, 6.0, -3.0),
    };

    constexpr inline int character_r_front_color = 7;
    constexpr inline int character_r_front_shading = 7;

    constexpr inline int character_r_back_color = 7;
    constexpr inline int character_r_back_shading = 0;

    constexpr inline int character_r_up_color = 7;
    constexpr inline int character_r_up_shading = 3;

    constexpr inline int character_r_left_color = 7;
    constexpr inline int character_r_left_shading = 2;

    constexpr inline int character_r_right_color = 7;
    constexpr inline int character_r_right_shading = 2;

    constexpr inline int character_r_down_color = 7;
    constexpr inline int character_r_down_shading = 1;

    constexpr inline face_3d character_r_faces[] = {
        face_3d(character_r_vertices, vertex_3d(1.0, -0.0, 0.0), 26, 5, 17, 30, character_r_front_color, character_r_front_shading),
        face_3d(character_r_vertices, vertex_3d(1.0, 0.0, 0.0), 6, 32, 28, 7, character_r_front_color, character_r_front_shading),
        face_3d(character_r_vertices, vertex_3d(1.0, -0.0, 0.0), 5, 3, 16, 17, character_r_front_color, character_r_front_shading),
        face_3d(character_r_vertices, vertex_3d(1.0, 0.0, 0.0), 1, 2, 20, 0, character_r_front_color, character_r_front_shading),
        face_3d(character_r_vertices, vertex_3d(1.0, -0.0, 0.0), 24, 1, 0, 7, character_r_front_color, character_r_front_shading),
        face_3d(character_r_vertices, vertex_3d(1.0, 0.0, 0.0), 4, 30, 17, 21, character_r_front_color, character_r_front_shading),
        face_3d(character_r_vertices, vertex_3d(1.0, 0.0, -0.0), 5, 26, 28, 32, character_r_front_color, character_r_front_shading),
        face_3d(character_r_vertices, vertex_3d(-1.0, 0.0, 0.0), 14, 13, 18, 19, character_r_back_color, character_r_back_shading),
        face_3d(character_r_vertices, vertex_3d(-1.0, 0.0, 0.0), 10, 9, 23, 15, character_r_back_color, character_r_back_shading),
        face_3d(character_r_vertices, vertex_3d(-1.0, 0.0, 0.0), 10, 25, 11, 9, character_r_back_color, character_r_back_shading),
        face_3d(character_r_vertices, vertex_3d(-1.0, 0.0, 0.0), 22, 18, 31, 12, character_r_back_color, character_r_back_shading),
        face_3d(character_r_vertices, vertex_3d(-1.0, 0.0, -0.0), 11, 29, 33, 8, character_r_back_color, character_r_back_shading),
        face_3d(character_r_vertices, vertex_3d(-1.0, 0.0, 0.0), 31, 18, 13, 27, character_r_back_color, character_r_back_shading),
        face_3d(character_r_vertices, vertex_3d(-1.0, 0.0, 0.0), 33, 29, 27, 13, character_r_back_color, character_r_back_shading),
        face_3d(character_r_vertices, vertex_3d(0.0, 1.0, 0.0), 5, 13, 14, 3, character_r_up_color, character_r_up_shading),
        face_3d(character_r_vertices, vertex_3d(0.0, 1.0, 0.0), 1, 24, 25, 10, character_r_up_color, character_r_up_shading),
        face_3d(character_r_vertices, vertex_3d(0.0, 0.0, 1.0), 17, 18, 22, 21, character_r_left_color, character_r_left_shading),
        face_3d(character_r_vertices, vertex_3d(0.0, -0.7071067811865476, 0.7071067811865476), 32, 6, 8, 33, character_r_left_color, character_r_left_shading),
        face_3d(character_r_vertices, vertex_3d(-0.0, 0.0, 1.0), 2, 1, 10, 15, character_r_left_color, character_r_left_shading),
        face_3d(character_r_vertices, vertex_3d(0.0, 0.7071067811865476, 0.7071067811865476), 33, 13, 5, 32, character_r_left_color, character_r_left_shading),
        face_3d(character_r_vertices, vertex_3d(0.0, 0.0, -1.0), 3, 14, 9, 0, character_r_right_color, character_r_right_shading),
        face_3d(character_r_vertices, vertex_3d(0.0, 0.0, -1.0), 16, 20, 23, 19, character_r_right_color, character_r_right_shading),
        face_3d(character_r_vertices, vertex_3d(0.0, 0.0, -1.0), 30, 4, 12, 31, character_r_right_color, character_r_right_shading),
        face_3d(character_r_vertices, vertex_3d(0.0, 0.7071067811865476, -0.7071067811865476), 31, 27, 26, 30, character_r_right_color, character_r_right_shading),
        face_3d(character_r_vertices, vertex_3d(0.0, -0.7071067811865476, -0.7071067811865476), 29, 28, 26, 27, character_r_right_color, character_r_right_shading),
        face_3d(character_r_vertices, vertex_3d(0.0, 0.0, -1.0), 7, 28, 29, 11, character_r_right_color, character_r_right_shading),
        face_3d(character_r_vertices, vertex_3d(0.0, 0.7071067811865476, -0.7071067811865476), 24, 7, 11, 25, character_r_right_color, character_r_right_shading),
        face_3d(character_r_vertices, vertex_3d(0.0, -1.0, -0.0), 12, 4, 21, 22, character_r_down_color, character_r_down_shading),
        face_3d(character_r_vertices, vertex_3d(0.0, -1.0, 0.0), 16, 19, 18, 17, character_r_down_color, character_r_down_shading),
        face_3d(character_r_vertices, vertex_3d(0.0, -1.0, 0.0), 15, 23, 20, 2, character_r_down_color, character_r_down_shading),
        face_3d(character_r_vertices, vertex_3d(0.0, -1.0, 0.0), 6, 0, 9, 8, character_r_down_color, character_r_down_shading),
    };

    constexpr inline model_3d_item character_r(character_r_vertices, character_r_faces);
}

#endif

