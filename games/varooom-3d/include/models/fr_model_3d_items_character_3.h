/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_CHARACTER_3_H
#define FR_MODEL_3D_ITEMS_CHARACTER_3_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d character_3_vertices[] = {
        vertex_3d(3.0, 9.0, 7.0),
        vertex_3d(3.0, 9.0, -5.0),
        vertex_3d(3.0, 15.0, -11.0),
        vertex_3d(3.0, 15.0, 7.0),
        vertex_3d(3.0, 3.0, 1.0),
        vertex_3d(3.0, -3.0, 1.0),
        vertex_3d(3.0, -3.0, -5.0),
        vertex_3d(3.0, 3.0, -5.0),
        vertex_3d(3.0, -9.0, 7.0),
        vertex_3d(3.0, -15.0, 7.0),
        vertex_3d(3.0, -15.0, -11.0),
        vertex_3d(3.0, -9.0, -5.0),
        vertex_3d(-3.0, 9.0, 7.0),
        vertex_3d(-3.0, 15.0, 7.0),
        vertex_3d(-3.0, 15.0, -11.0),
        vertex_3d(-3.0, 9.0, -5.0),
        vertex_3d(-3.0, 3.0, 1.0),
        vertex_3d(-3.0, 3.0, -5.0),
        vertex_3d(-3.0, -3.0, -5.0),
        vertex_3d(-3.0, -3.0, 1.0),
        vertex_3d(-3.0, -9.0, 7.0),
        vertex_3d(-3.0, -9.0, -5.0),
        vertex_3d(-3.0, -15.0, -11.0),
        vertex_3d(-3.0, -15.0, 7.0),
    };

    constexpr inline int character_3_front_color = 7;
    constexpr inline int character_3_front_shading = 7;

    constexpr inline int character_3_back_color = 7;
    constexpr inline int character_3_back_shading = 0;

    constexpr inline int character_3_up_color = 7;
    constexpr inline int character_3_up_shading = 3;

    constexpr inline int character_3_left_color = 7;
    constexpr inline int character_3_left_shading = 2;

    constexpr inline int character_3_right_color = 7;
    constexpr inline int character_3_right_shading = 2;

    constexpr inline int character_3_down_color = 7;
    constexpr inline int character_3_down_shading = 1;

    constexpr inline face_3d character_3_faces[] = {
        face_3d(character_3_vertices, vertex_3d(1.0, 0.0, 0.0), 0, 1, 2, 3, character_3_front_color, character_3_front_shading),
        face_3d(character_3_vertices, vertex_3d(1.0, 0.0, 0.0), 4, 5, 6, 7, character_3_front_color, character_3_front_shading),
        face_3d(character_3_vertices, vertex_3d(1.0, 0.0, 0.0), 8, 9, 10, 11, character_3_front_color, character_3_front_shading),
        face_3d(character_3_vertices, vertex_3d(1.0, 0.0, 0.0), 10, 2, 1, 11, character_3_front_color, character_3_front_shading),
        face_3d(character_3_vertices, vertex_3d(-1.0, 0.0, 0.0), 12, 13, 14, 15, character_3_back_color, character_3_back_shading),
        face_3d(character_3_vertices, vertex_3d(-1.0, 0.0, 0.0), 16, 17, 18, 19, character_3_back_color, character_3_back_shading),
        face_3d(character_3_vertices, vertex_3d(-1.0, 0.0, 0.0), 20, 21, 22, 23, character_3_back_color, character_3_back_shading),
        face_3d(character_3_vertices, vertex_3d(-1.0, -0.0, 0.0), 22, 21, 15, 14, character_3_back_color, character_3_back_shading),
        face_3d(character_3_vertices, vertex_3d(0.0, 1.0, 0.0), 3, 2, 14, 13, character_3_up_color, character_3_up_shading),
        face_3d(character_3_vertices, vertex_3d(0.0, 1.0, 0.0), 11, 21, 20, 8, character_3_up_color, character_3_up_shading),
        face_3d(character_3_vertices, vertex_3d(0.0, 1.0, 0.0), 7, 17, 16, 4, character_3_up_color, character_3_up_shading),
        face_3d(character_3_vertices, vertex_3d(0.0, 0.0, 1.0), 8, 20, 23, 9, character_3_left_color, character_3_left_shading),
        face_3d(character_3_vertices, vertex_3d(-0.0, 0.0, 1.0), 11, 6, 18, 21, character_3_left_color, character_3_left_shading),
        face_3d(character_3_vertices, vertex_3d(-0.0, 0.0, 1.0), 7, 1, 15, 17, character_3_left_color, character_3_left_shading),
        face_3d(character_3_vertices, vertex_3d(-0.0, 0.0, 1.0), 0, 3, 13, 12, character_3_left_color, character_3_left_shading),
        face_3d(character_3_vertices, vertex_3d(0.0, 0.0, 1.0), 4, 16, 19, 5, character_3_left_color, character_3_left_shading),
        face_3d(character_3_vertices, vertex_3d(0.0, 0.0, -1.0), 2, 10, 22, 14, character_3_right_color, character_3_right_shading),
        face_3d(character_3_vertices, vertex_3d(0.0, -1.0, 0.0), 9, 23, 22, 10, character_3_down_color, character_3_down_shading),
        face_3d(character_3_vertices, vertex_3d(0.0, -1.0, 0.0), 1, 0, 12, 15, character_3_down_color, character_3_down_shading),
        face_3d(character_3_vertices, vertex_3d(0.0, -1.0, 0.0), 5, 19, 18, 6, character_3_down_color, character_3_down_shading),
    };

    constexpr inline model_3d_item character_3(character_3_vertices, character_3_faces);

    constexpr inline int character_3_red_front_color = 0;
    constexpr inline int character_3_red_front_shading = 7;

    constexpr inline int character_3_red_back_color = 0;
    constexpr inline int character_3_red_back_shading = 0;

    constexpr inline int character_3_red_up_color = 0;
    constexpr inline int character_3_red_up_shading = 3;

    constexpr inline int character_3_red_left_color = 0;
    constexpr inline int character_3_red_left_shading = 2;

    constexpr inline int character_3_red_right_color = 0;
    constexpr inline int character_3_red_right_shading = 2;

    constexpr inline int character_3_red_down_color = 0;
    constexpr inline int character_3_red_down_shading = 1;

    constexpr inline face_3d character_3_red_faces[] = {
        face_3d(character_3_vertices, vertex_3d(1.0, 0.0, 0.0), 0, 1, 2, 3, character_3_red_front_color, character_3_red_front_shading),
        face_3d(character_3_vertices, vertex_3d(1.0, 0.0, 0.0), 4, 5, 6, 7, character_3_red_front_color, character_3_red_front_shading),
        face_3d(character_3_vertices, vertex_3d(1.0, 0.0, 0.0), 8, 9, 10, 11, character_3_red_front_color, character_3_red_front_shading),
        face_3d(character_3_vertices, vertex_3d(1.0, 0.0, 0.0), 10, 2, 1, 11, character_3_red_front_color, character_3_red_front_shading),
        face_3d(character_3_vertices, vertex_3d(-1.0, 0.0, 0.0), 12, 13, 14, 15, character_3_red_back_color, character_3_red_back_shading),
        face_3d(character_3_vertices, vertex_3d(-1.0, 0.0, 0.0), 16, 17, 18, 19, character_3_red_back_color, character_3_red_back_shading),
        face_3d(character_3_vertices, vertex_3d(-1.0, 0.0, 0.0), 20, 21, 22, 23, character_3_red_back_color, character_3_red_back_shading),
        face_3d(character_3_vertices, vertex_3d(-1.0, -0.0, 0.0), 22, 21, 15, 14, character_3_red_back_color, character_3_red_back_shading),
        face_3d(character_3_vertices, vertex_3d(0.0, 1.0, 0.0), 3, 2, 14, 13, character_3_red_up_color, character_3_red_up_shading),
        face_3d(character_3_vertices, vertex_3d(0.0, 1.0, 0.0), 11, 21, 20, 8, character_3_red_up_color, character_3_red_up_shading),
        face_3d(character_3_vertices, vertex_3d(0.0, 1.0, 0.0), 7, 17, 16, 4, character_3_red_up_color, character_3_red_up_shading),
        face_3d(character_3_vertices, vertex_3d(0.0, 0.0, 1.0), 8, 20, 23, 9, character_3_red_left_color, character_3_red_left_shading),
        face_3d(character_3_vertices, vertex_3d(-0.0, 0.0, 1.0), 11, 6, 18, 21, character_3_red_left_color, character_3_red_left_shading),
        face_3d(character_3_vertices, vertex_3d(-0.0, 0.0, 1.0), 7, 1, 15, 17, character_3_red_left_color, character_3_red_left_shading),
        face_3d(character_3_vertices, vertex_3d(-0.0, 0.0, 1.0), 0, 3, 13, 12, character_3_red_left_color, character_3_red_left_shading),
        face_3d(character_3_vertices, vertex_3d(0.0, 0.0, 1.0), 4, 16, 19, 5, character_3_red_left_color, character_3_red_left_shading),
        face_3d(character_3_vertices, vertex_3d(0.0, 0.0, -1.0), 2, 10, 22, 14, character_3_red_right_color, character_3_red_right_shading),
        face_3d(character_3_vertices, vertex_3d(0.0, -1.0, 0.0), 9, 23, 22, 10, character_3_red_down_color, character_3_red_down_shading),
        face_3d(character_3_vertices, vertex_3d(0.0, -1.0, 0.0), 1, 0, 12, 15, character_3_red_down_color, character_3_red_down_shading),
        face_3d(character_3_vertices, vertex_3d(0.0, -1.0, 0.0), 5, 19, 18, 6, character_3_red_down_color, character_3_red_down_shading),
    };

    constexpr inline model_3d_item character_3_red(character_3_vertices, character_3_red_faces);
}

#endif

