/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_CHARACTER_O_H
#define FR_MODEL_3D_ITEMS_CHARACTER_O_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d character_o_vertices[] = {
        vertex_3d(-3.0, 15.0, -9.0),
        vertex_3d(3.0, 15.0, -9.0),
        vertex_3d(3.0, 15.0, 9.0),
        vertex_3d(3.0, 9.0, -3.0),
        vertex_3d(3.0, -15.0, 9.0),
        vertex_3d(3.0, -15.0, -9.0),
        vertex_3d(3.0, -9.0, -3.0),
        vertex_3d(3.0, -9.0, 3.0),
        vertex_3d(3.0, 9.0, 3.0),
        vertex_3d(-3.0, 15.0, 9.0),
        vertex_3d(-3.0, 9.0, -3.0),
        vertex_3d(-3.0, -15.0, 9.0),
        vertex_3d(-3.0, -15.0, -9.0),
        vertex_3d(-3.0, -9.0, -3.0),
        vertex_3d(-3.0, -9.0, 3.0),
        vertex_3d(-3.0, 9.0, 3.0),
    };

    constexpr inline int character_o_front_color = 7;
    constexpr inline int character_o_front_shading = 7;

    constexpr inline int character_o_back_color = 7;
    constexpr inline int character_o_back_shading = 0;

    constexpr inline int character_o_up_color = 7;
    constexpr inline int character_o_up_shading = 3;

    constexpr inline int character_o_left_color = 7;
    constexpr inline int character_o_left_shading = 2;

    constexpr inline int character_o_right_color = 7;
    constexpr inline int character_o_right_shading = 2;

    constexpr inline int character_o_down_color = 7;
    constexpr inline int character_o_down_shading = 1;

    constexpr inline face_3d character_o_full_faces[] = {
        face_3d(character_o_vertices, vertex_3d(1.0, -0.0, 0.0), 8, 2, 4, 7, character_o_front_color, character_o_front_shading),
        face_3d(character_o_vertices, vertex_3d(1.0, 0.0, 0.0), 4, 5, 6, 7, character_o_front_color, character_o_front_shading),
        face_3d(character_o_vertices, vertex_3d(1.0, 0.0, 0.0), 2, 8, 3, 1, character_o_front_color, character_o_front_shading),
        face_3d(character_o_vertices, vertex_3d(1.0, 0.0, 0.0), 3, 6, 5, 1, character_o_front_color, character_o_front_shading),
        face_3d(character_o_vertices, vertex_3d(-1.0, 0.0, 0.0), 10, 15, 9, 0, character_o_back_color, character_o_back_shading),
        face_3d(character_o_vertices, vertex_3d(-1.0, 0.0, -0.0), 0, 12, 13, 10, character_o_back_color, character_o_back_shading),
        face_3d(character_o_vertices, vertex_3d(-1.0, 0.0, 0.0), 15, 14, 11, 9, character_o_back_color, character_o_back_shading),
        face_3d(character_o_vertices, vertex_3d(-1.0, 0.0, 0.0), 11, 14, 13, 12, character_o_back_color, character_o_back_shading),
        face_3d(character_o_vertices, vertex_3d(0.0, 1.0, 0.0), 1, 0, 9, 2, character_o_up_color, character_o_up_shading),
        face_3d(character_o_vertices, vertex_3d(0.0, 1.0, 0.0), 7, 6, 13, 14, character_o_up_color, character_o_up_shading),
        face_3d(character_o_vertices, vertex_3d(0.0, 0.0, 1.0), 2, 9, 11, 4, character_o_left_color, character_o_left_shading),
        face_3d(character_o_vertices, vertex_3d(-0.0, 0.0, 1.0), 6, 3, 10, 13, character_o_left_color, character_o_left_shading),
        face_3d(character_o_vertices, vertex_3d(0.0, 0.0, -1.0), 7, 14, 15, 8, character_o_right_color, character_o_right_shading),
        face_3d(character_o_vertices, vertex_3d(0.0, 0.0, -1.0), 0, 1, 5, 12, character_o_right_color, character_o_right_shading),
        face_3d(character_o_vertices, vertex_3d(0.0, -1.0, 0.0), 15, 10, 3, 8, character_o_down_color, character_o_down_shading),
        face_3d(character_o_vertices, vertex_3d(0.0, -1.0, 0.0), 5, 4, 11, 12, character_o_down_color, character_o_down_shading),
    };

    constexpr inline model_3d_item character_o_full(character_o_vertices, character_o_full_faces);

    constexpr inline face_3d character_o_faces[] = {
        face_3d(character_o_vertices, vertex_3d(1.0, -0.0, 0.0), 8, 2, 4, 7, character_o_front_color, character_o_front_shading),
        face_3d(character_o_vertices, vertex_3d(1.0, 0.0, 0.0), 4, 5, 6, 7, character_o_front_color, character_o_front_shading),
        face_3d(character_o_vertices, vertex_3d(1.0, 0.0, 0.0), 2, 8, 3, 1, character_o_front_color, character_o_front_shading),
        face_3d(character_o_vertices, vertex_3d(1.0, 0.0, 0.0), 3, 6, 5, 1, character_o_front_color, character_o_front_shading),
        face_3d(character_o_vertices, vertex_3d(0.0, 1.0, 0.0), 1, 0, 9, 2, character_o_up_color, character_o_up_shading),
        face_3d(character_o_vertices, vertex_3d(0.0, 1.0, 0.0), 7, 6, 13, 14, character_o_up_color, character_o_up_shading),
        face_3d(character_o_vertices, vertex_3d(0.0, 0.0, 1.0), 2, 9, 11, 4, character_o_left_color, character_o_left_shading),
        face_3d(character_o_vertices, vertex_3d(-0.0, 0.0, 1.0), 6, 3, 10, 13, character_o_left_color, character_o_left_shading),
        face_3d(character_o_vertices, vertex_3d(0.0, 0.0, -1.0), 7, 14, 15, 8, character_o_right_color, character_o_right_shading),
        face_3d(character_o_vertices, vertex_3d(0.0, 0.0, -1.0), 0, 1, 5, 12, character_o_right_color, character_o_right_shading),
        face_3d(character_o_vertices, vertex_3d(0.0, -1.0, 0.0), 15, 10, 3, 8, character_o_down_color, character_o_down_shading),
        face_3d(character_o_vertices, vertex_3d(0.0, -1.0, 0.0), 5, 4, 11, 12, character_o_down_color, character_o_down_shading),
    };

    constexpr inline model_3d_item character_o(character_o_vertices, character_o_faces);

    constexpr inline int character_o_first_color_front_color = 0;
    constexpr inline int character_o_first_color_front_shading = 7;

    constexpr inline int character_o_first_color_back_color = 0;
    constexpr inline int character_o_first_color_back_shading = 0;

    constexpr inline int character_o_first_color_up_color = 0;
    constexpr inline int character_o_first_color_up_shading = 3;

    constexpr inline int character_o_first_color_left_color = 0;
    constexpr inline int character_o_first_color_left_shading = 2;

    constexpr inline int character_o_first_color_right_color = 0;
    constexpr inline int character_o_first_color_right_shading = 2;

    constexpr inline int character_o_first_color_down_color = 0;
    constexpr inline int character_o_first_color_down_shading = 1;

    constexpr inline face_3d character_o_first_color_faces[] = {
        face_3d(character_o_vertices, vertex_3d(1.0, -0.0, 0.0), 8, 2, 4, 7, character_o_first_color_front_color, character_o_first_color_front_shading),
        face_3d(character_o_vertices, vertex_3d(1.0, 0.0, 0.0), 4, 5, 6, 7, character_o_first_color_front_color, character_o_first_color_front_shading),
        face_3d(character_o_vertices, vertex_3d(1.0, 0.0, 0.0), 2, 8, 3, 1, character_o_first_color_front_color, character_o_first_color_front_shading),
        face_3d(character_o_vertices, vertex_3d(1.0, 0.0, 0.0), 3, 6, 5, 1, character_o_first_color_front_color, character_o_first_color_front_shading),
        face_3d(character_o_vertices, vertex_3d(-1.0, 0.0, 0.0), 10, 15, 9, 0, character_o_first_color_back_color, character_o_first_color_back_shading),
        face_3d(character_o_vertices, vertex_3d(-1.0, 0.0, -0.0), 0, 12, 13, 10, character_o_first_color_back_color, character_o_first_color_back_shading),
        face_3d(character_o_vertices, vertex_3d(-1.0, 0.0, 0.0), 15, 14, 11, 9, character_o_first_color_back_color, character_o_first_color_back_shading),
        face_3d(character_o_vertices, vertex_3d(-1.0, 0.0, 0.0), 11, 14, 13, 12, character_o_first_color_back_color, character_o_first_color_back_shading),
        face_3d(character_o_vertices, vertex_3d(0.0, 1.0, 0.0), 1, 0, 9, 2, character_o_first_color_up_color, character_o_first_color_up_shading),
        face_3d(character_o_vertices, vertex_3d(0.0, 1.0, 0.0), 7, 6, 13, 14, character_o_first_color_up_color, character_o_first_color_up_shading),
        face_3d(character_o_vertices, vertex_3d(0.0, 0.0, 1.0), 2, 9, 11, 4, character_o_first_color_left_color, character_o_first_color_left_shading),
        face_3d(character_o_vertices, vertex_3d(-0.0, 0.0, 1.0), 6, 3, 10, 13, character_o_first_color_left_color, character_o_first_color_left_shading),
        face_3d(character_o_vertices, vertex_3d(0.0, 0.0, -1.0), 7, 14, 15, 8, character_o_first_color_right_color, character_o_first_color_right_shading),
        face_3d(character_o_vertices, vertex_3d(0.0, 0.0, -1.0), 0, 1, 5, 12, character_o_first_color_right_color, character_o_first_color_right_shading),
        face_3d(character_o_vertices, vertex_3d(0.0, -1.0, 0.0), 15, 10, 3, 8, character_o_first_color_down_color, character_o_first_color_down_shading),
        face_3d(character_o_vertices, vertex_3d(0.0, -1.0, 0.0), 5, 4, 11, 12, character_o_first_color_down_color, character_o_first_color_down_shading),
    };

    constexpr inline model_3d_item character_o_first_color(character_o_vertices, character_o_first_color_faces);
}

#endif

