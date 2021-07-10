/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_CHARACTER_2_H
#define FR_MODEL_3D_ITEMS_CHARACTER_2_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d character_2_vertices[] = {
        vertex_3d(3.0, 9.0, 7.0),
        vertex_3d(3.0, 9.0, -5.0),
        vertex_3d(3.0, 3.0, -5.0),
        vertex_3d(3.0, 3.0, 7.0),
        vertex_3d(3.0, -3.0, -11.0),
        vertex_3d(3.0, -9.0, -11.0),
        vertex_3d(3.0, -15.0, 7.0),
        vertex_3d(3.0, -15.0, -11.0),
        vertex_3d(3.0, 15.0, -11.0),
        vertex_3d(3.0, 15.0, 7.0),
        vertex_3d(3.0, -9.0, 1.0),
        vertex_3d(3.0, -3.0, 1.0),
        vertex_3d(-3.0, 9.0, 7.0),
        vertex_3d(-3.0, 9.0, -5.0),
        vertex_3d(-3.0, 3.0, -5.0),
        vertex_3d(-3.0, 3.0, 7.0),
        vertex_3d(-3.0, -3.0, -11.0),
        vertex_3d(-3.0, -9.0, -11.0),
        vertex_3d(-3.0, -15.0, 7.0),
        vertex_3d(-3.0, -15.0, -11.0),
        vertex_3d(-3.0, 15.0, -11.0),
        vertex_3d(-3.0, 15.0, 7.0),
        vertex_3d(-3.0, -9.0, 1.0),
        vertex_3d(-3.0, -3.0, 1.0),
    };

    constexpr inline int character_2_front_color = 7;
    constexpr inline int character_2_front_shading = 7;

    constexpr inline int character_2_back_color = 7;
    constexpr inline int character_2_back_shading = 0;

    constexpr inline int character_2_up_color = 7;
    constexpr inline int character_2_up_shading = 3;

    constexpr inline int character_2_left_color = 7;
    constexpr inline int character_2_left_shading = 2;

    constexpr inline int character_2_right_color = 7;
    constexpr inline int character_2_right_shading = 2;

    constexpr inline int character_2_down_color = 7;
    constexpr inline int character_2_down_shading = 1;

    constexpr inline face_3d character_2_faces[] = {
        face_3d(character_2_vertices, vertex_3d(1.0, 0.0, 0.0), 0, 1, 8, 9, character_2_front_color, character_2_front_shading),
        face_3d(character_2_vertices, vertex_3d(1.0, 0.0, 0.0), 3, 11, 4, 2, character_2_front_color, character_2_front_shading),
        face_3d(character_2_vertices, vertex_3d(1.0, -0.0, 0.0), 8, 1, 2, 4, character_2_front_color, character_2_front_shading),
        face_3d(character_2_vertices, vertex_3d(1.0, -0.0, 0.0), 11, 3, 6, 10, character_2_front_color, character_2_front_shading),
        face_3d(character_2_vertices, vertex_3d(1.0, 0.0, 0.0), 7, 5, 10, 6, character_2_front_color, character_2_front_shading),
        face_3d(character_2_vertices, vertex_3d(1.0, 0.0, 0.0), 12, 13, 20, 21, character_2_back_color, character_2_back_shading),
        face_3d(character_2_vertices, vertex_3d(1.0, 0.0, 0.0), 15, 23, 16, 14, character_2_back_color, character_2_back_shading),
        face_3d(character_2_vertices, vertex_3d(1.0, -0.0, 0.0), 20, 13, 14, 16, character_2_back_color, character_2_back_shading),
        face_3d(character_2_vertices, vertex_3d(1.0, -0.0, 0.0), 23, 15, 18, 22, character_2_back_color, character_2_back_shading),
        face_3d(character_2_vertices, vertex_3d(1.0, 0.0, 0.0), 19, 17, 22, 18, character_2_back_color, character_2_back_shading),
        face_3d(character_2_vertices, vertex_3d(0.0, 1.0, 0.0), 8, 20, 21, 9, character_2_up_color, character_2_up_shading),
        face_3d(character_2_vertices, vertex_3d(0.0, 1.0, 0.0), 5, 17, 22, 10, character_2_up_color, character_2_up_shading),
        face_3d(character_2_vertices, vertex_3d(0.0, 1.0, 0.0), 2, 14, 15, 3, character_2_up_color, character_2_up_shading),
        face_3d(character_2_vertices, vertex_3d(0.0, 0.0, 1.0), 3, 15, 18, 6, character_2_left_color, character_2_left_shading),
        face_3d(character_2_vertices, vertex_3d(0.0, 0.0, 1.0), 1, 13, 14, 2, character_2_left_color, character_2_left_shading),
        face_3d(character_2_vertices, vertex_3d(0.0, 0.0, 1.0), 9, 21, 12, 0, character_2_left_color, character_2_left_shading),
        face_3d(character_2_vertices, vertex_3d(0.0, 0.0, -1.0), 4, 16, 20, 8, character_2_right_color, character_2_right_shading),
        face_3d(character_2_vertices, vertex_3d(0.0, 0.0, -1.0), 10, 22, 23, 11, character_2_right_color, character_2_right_shading),
        face_3d(character_2_vertices, vertex_3d(0.0, 0.0, -1.0), 7, 19, 17, 5, character_2_right_color, character_2_right_shading),
        face_3d(character_2_vertices, vertex_3d(0.0, -1.0, 0.0), 11, 23, 16, 4, character_2_down_color, character_2_down_shading),
        face_3d(character_2_vertices, vertex_3d(0.0, -1.0, 0.0), 6, 18, 19, 7, character_2_down_color, character_2_down_shading),
        face_3d(character_2_vertices, vertex_3d(0.0, -1.0, 0.0), 0, 12, 13, 1, character_2_down_color, character_2_down_shading),
    };

    constexpr inline model_3d_item character_2(character_2_vertices, character_2_faces);
}

#endif

