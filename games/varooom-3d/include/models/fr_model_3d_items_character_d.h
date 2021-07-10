/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_CHARACTER_D_H
#define FR_MODEL_3D_ITEMS_CHARACTER_D_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d character_d_vertices[] = {
        vertex_3d(3.0, -9.0, 0.0),
        vertex_3d(3.0, -15.0, -3.0),
        vertex_3d(3.0, 9.0, 0.0),
        vertex_3d(3.0, 6.0, -3.0),
        vertex_3d(3.0, -9.0, -9.0),
        vertex_3d(3.0, 9.0, -9.0),
        vertex_3d(3.0, -9.0, 3.0),
        vertex_3d(3.0, 15.0, 9.0),
        vertex_3d(3.0, -15.0, 9.0),
        vertex_3d(3.0, 9.0, 3.0),
        vertex_3d(3.0, 15.0, -3.0),
        vertex_3d(-3.0, -9.0, 3.0),
        vertex_3d(-3.0, -9.0, 0.0),
        vertex_3d(-3.0, -6.0, -3.0),
        vertex_3d(3.0, -6.0, -3.0),
        vertex_3d(-3.0, 15.0, 9.0),
        vertex_3d(-3.0, 9.0, 3.0),
        vertex_3d(-3.0, -15.0, 9.0),
        vertex_3d(-3.0, 15.0, -3.0),
        vertex_3d(-3.0, 9.0, -9.0),
        vertex_3d(-3.0, -9.0, -9.0),
        vertex_3d(-3.0, 6.0, -3.0),
        vertex_3d(-3.0, 9.0, 0.0),
        vertex_3d(-3.0, -15.0, -3.0),
    };

    constexpr inline int character_d_front_color = 7;
    constexpr inline int character_d_front_shading = 7;

    constexpr inline int character_d_back_color = 7;
    constexpr inline int character_d_back_shading = 0;

    constexpr inline int character_d_up_color = 7;
    constexpr inline int character_d_up_shading = 3;

    constexpr inline int character_d_left_color = 7;
    constexpr inline int character_d_left_shading = 2;

    constexpr inline int character_d_right_color = 7;
    constexpr inline int character_d_right_shading = 2;

    constexpr inline int character_d_down_color = 7;
    constexpr inline int character_d_down_shading = 1;

    constexpr inline face_3d character_d_faces[] = {
        face_3d(character_d_vertices, vertex_3d(1.0, 0.0, -0.0), 2, 3, 5, 10, character_d_front_color, character_d_front_shading),
        face_3d(character_d_vertices, vertex_3d(1.0, -0.0, 0.0), 5, 3, 14, 4, character_d_front_color, character_d_front_shading),
        face_3d(character_d_vertices, vertex_3d(1.0, 0.0, 0.0), 7, 9, 2, 10, character_d_front_color, character_d_front_shading),
        face_3d(character_d_vertices, vertex_3d(1.0, 0.0, 0.0), 1, 4, 14, 0, character_d_front_color, character_d_front_shading),
        face_3d(character_d_vertices, vertex_3d(1.0, 0.0, 0.0), 8, 1, 0, 6, character_d_front_color, character_d_front_shading),
        face_3d(character_d_vertices, vertex_3d(1.0, 0.0, -0.0), 7, 8, 6, 9, character_d_front_color, character_d_front_shading),
        face_3d(character_d_vertices, vertex_3d(-1.0, -0.0, 0.0), 23, 17, 11, 12, character_d_back_color, character_d_back_shading),
        face_3d(character_d_vertices, vertex_3d(-1.0, 0.0, 0.0), 18, 19, 21, 22, character_d_back_color, character_d_back_shading),
        face_3d(character_d_vertices, vertex_3d(-1.0, 0.0, 0.0), 15, 16, 11, 17, character_d_back_color, character_d_back_shading),
        face_3d(character_d_vertices, vertex_3d(-1.0, 0.0, -0.0), 19, 20, 13, 21, character_d_back_color, character_d_back_shading),
        face_3d(character_d_vertices, vertex_3d(-1.0, 0.0, 0.0), 15, 18, 22, 16, character_d_back_color, character_d_back_shading),
        face_3d(character_d_vertices, vertex_3d(-1.0, 0.0, -0.0), 20, 23, 12, 13, character_d_back_color, character_d_back_shading),
        face_3d(character_d_vertices, vertex_3d(0.0, 1.0, 0.0), 7, 10, 18, 15, character_d_up_color, character_d_up_shading),
        face_3d(character_d_vertices, vertex_3d(0.0, 0.7071067811865476, -0.7071067811865476), 18, 10, 5, 19, character_d_up_color, character_d_up_shading),
        face_3d(character_d_vertices, vertex_3d(0.0, 1.0, 0.0), 12, 11, 6, 0, character_d_up_color, character_d_up_shading),
        face_3d(character_d_vertices, vertex_3d(0.0, 0.7071067811865476, 0.7071067811865476), 13, 12, 0, 14, character_d_up_color, character_d_up_shading),
        face_3d(character_d_vertices, vertex_3d(0.0, 0.0, 1.0), 15, 17, 8, 7, character_d_left_color, character_d_left_shading),
        face_3d(character_d_vertices, vertex_3d(-0.0, 0.0, 1.0), 14, 3, 21, 13, character_d_left_color, character_d_left_shading),
        face_3d(character_d_vertices, vertex_3d(0.0, 0.0, -1.0), 6, 11, 16, 9, character_d_right_color, character_d_right_shading),
        face_3d(character_d_vertices, vertex_3d(0.0, 0.0, -1.0), 4, 20, 19, 5, character_d_right_color, character_d_right_shading),
        face_3d(character_d_vertices, vertex_3d(0.0, -1.0, 0.0), 2, 9, 16, 22, character_d_down_color, character_d_down_shading),
        face_3d(character_d_vertices, vertex_3d(0.0, -0.7071067811865476, 0.7071067811865476), 22, 21, 3, 2, character_d_down_color, character_d_down_shading),
        face_3d(character_d_vertices, vertex_3d(0.0, -1.0, 0.0), 17, 23, 1, 8, character_d_down_color, character_d_down_shading),
        face_3d(character_d_vertices, vertex_3d(-0.0, -0.7071067811865476, -0.7071067811865476), 23, 20, 4, 1, character_d_down_color, character_d_down_shading),
    };

    constexpr inline model_3d_item character_d(character_d_vertices, character_d_faces);

    constexpr inline int character_d_red_front_color = 0;
    constexpr inline int character_d_red_front_shading = 7;

    constexpr inline int character_d_red_back_color = 0;
    constexpr inline int character_d_red_back_shading = 0;

    constexpr inline int character_d_red_up_color = 0;
    constexpr inline int character_d_red_up_shading = 3;

    constexpr inline int character_d_red_left_color = 0;
    constexpr inline int character_d_red_left_shading = 2;

    constexpr inline int character_d_red_right_color = 0;
    constexpr inline int character_d_red_right_shading = 2;

    constexpr inline int character_d_red_down_color = 0;
    constexpr inline int character_d_red_down_shading = 1;

    constexpr inline face_3d character_d_red_faces[] = {
        face_3d(character_d_vertices, vertex_3d(1.0, 0.0, -0.0), 2, 3, 5, 10, character_d_red_front_color, character_d_red_front_shading),
        face_3d(character_d_vertices, vertex_3d(1.0, -0.0, 0.0), 5, 3, 14, 4, character_d_red_front_color, character_d_red_front_shading),
        face_3d(character_d_vertices, vertex_3d(1.0, 0.0, 0.0), 7, 9, 2, 10, character_d_red_front_color, character_d_red_front_shading),
        face_3d(character_d_vertices, vertex_3d(1.0, 0.0, 0.0), 1, 4, 14, 0, character_d_red_front_color, character_d_red_front_shading),
        face_3d(character_d_vertices, vertex_3d(1.0, 0.0, 0.0), 8, 1, 0, 6, character_d_red_front_color, character_d_red_front_shading),
        face_3d(character_d_vertices, vertex_3d(1.0, 0.0, -0.0), 7, 8, 6, 9, character_d_red_front_color, character_d_red_front_shading),
        face_3d(character_d_vertices, vertex_3d(-1.0, -0.0, 0.0), 23, 17, 11, 12, character_d_red_back_color, character_d_red_back_shading),
        face_3d(character_d_vertices, vertex_3d(-1.0, 0.0, 0.0), 18, 19, 21, 22, character_d_red_back_color, character_d_red_back_shading),
        face_3d(character_d_vertices, vertex_3d(-1.0, 0.0, 0.0), 15, 16, 11, 17, character_d_red_back_color, character_d_red_back_shading),
        face_3d(character_d_vertices, vertex_3d(-1.0, 0.0, -0.0), 19, 20, 13, 21, character_d_red_back_color, character_d_red_back_shading),
        face_3d(character_d_vertices, vertex_3d(-1.0, 0.0, 0.0), 15, 18, 22, 16, character_d_red_back_color, character_d_red_back_shading),
        face_3d(character_d_vertices, vertex_3d(-1.0, 0.0, -0.0), 20, 23, 12, 13, character_d_red_back_color, character_d_red_back_shading),
        face_3d(character_d_vertices, vertex_3d(0.0, 1.0, 0.0), 7, 10, 18, 15, character_d_red_up_color, character_d_red_up_shading),
        face_3d(character_d_vertices, vertex_3d(0.0, 0.7071067811865476, -0.7071067811865476), 18, 10, 5, 19, character_d_red_up_color, character_d_red_up_shading),
        face_3d(character_d_vertices, vertex_3d(0.0, 1.0, 0.0), 12, 11, 6, 0, character_d_red_up_color, character_d_red_up_shading),
        face_3d(character_d_vertices, vertex_3d(0.0, 0.7071067811865476, 0.7071067811865476), 13, 12, 0, 14, character_d_red_up_color, character_d_red_up_shading),
        face_3d(character_d_vertices, vertex_3d(0.0, 0.0, 1.0), 15, 17, 8, 7, character_d_red_left_color, character_d_red_left_shading),
        face_3d(character_d_vertices, vertex_3d(-0.0, 0.0, 1.0), 14, 3, 21, 13, character_d_red_left_color, character_d_red_left_shading),
        face_3d(character_d_vertices, vertex_3d(0.0, 0.0, -1.0), 6, 11, 16, 9, character_d_red_right_color, character_d_red_right_shading),
        face_3d(character_d_vertices, vertex_3d(0.0, 0.0, -1.0), 4, 20, 19, 5, character_d_red_right_color, character_d_red_right_shading),
        face_3d(character_d_vertices, vertex_3d(0.0, -1.0, 0.0), 2, 9, 16, 22, character_d_red_down_color, character_d_red_down_shading),
        face_3d(character_d_vertices, vertex_3d(0.0, -0.7071067811865476, 0.7071067811865476), 22, 21, 3, 2, character_d_red_down_color, character_d_red_down_shading),
        face_3d(character_d_vertices, vertex_3d(0.0, -1.0, 0.0), 17, 23, 1, 8, character_d_red_down_color, character_d_red_down_shading),
        face_3d(character_d_vertices, vertex_3d(-0.0, -0.7071067811865476, -0.7071067811865476), 23, 20, 4, 1, character_d_red_down_color, character_d_red_down_shading),
    };

    constexpr inline model_3d_item character_d_red(character_d_vertices, character_d_red_faces);
}

#endif

