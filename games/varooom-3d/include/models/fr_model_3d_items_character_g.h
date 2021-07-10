/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_CHARACTER_G_H
#define FR_MODEL_3D_ITEMS_CHARACTER_G_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d character_g_vertices[] = {
        vertex_3d(-3.0, 15.0, -9.0),
        vertex_3d(3.0, 9.0, -9.0),
        vertex_3d(3.0, 15.0, -9.0),
        vertex_3d(3.0, 15.0, 9.0),
        vertex_3d(-3.0, 9.0, -9.0),
        vertex_3d(3.0, -3.0, -3.0),
        vertex_3d(3.0, 3.0, -9.0),
        vertex_3d(3.0, -15.0, 9.0),
        vertex_3d(3.0, -15.0, -9.0),
        vertex_3d(3.0, -9.0, -3.0),
        vertex_3d(3.0, -3.0, 0.0),
        vertex_3d(3.0, -9.0, 3.0),
        vertex_3d(3.0, 3.0, 0.0),
        vertex_3d(3.0, 9.0, 3.0),
        vertex_3d(-3.0, 15.0, 9.0),
        vertex_3d(-3.0, -3.0, -3.0),
        vertex_3d(-3.0, 3.0, -9.0),
        vertex_3d(-3.0, -15.0, 9.0),
        vertex_3d(-3.0, -15.0, -9.0),
        vertex_3d(-3.0, -9.0, -3.0),
        vertex_3d(-3.0, -3.0, 0.0),
        vertex_3d(-3.0, -9.0, 3.0),
        vertex_3d(-3.0, 3.0, 0.0),
        vertex_3d(-3.0, 9.0, 3.0),
    };

    constexpr inline int character_g_front_color = 7;
    constexpr inline int character_g_front_shading = 7;

    constexpr inline int character_g_back_color = 7;
    constexpr inline int character_g_back_shading = 0;

    constexpr inline int character_g_up_color = 7;
    constexpr inline int character_g_up_shading = 3;

    constexpr inline int character_g_left_color = 7;
    constexpr inline int character_g_left_shading = 2;

    constexpr inline int character_g_right_color = 7;
    constexpr inline int character_g_right_shading = 2;

    constexpr inline int character_g_down_color = 7;
    constexpr inline int character_g_down_shading = 1;

    constexpr inline face_3d character_g_full_faces[] = {
        face_3d(character_g_vertices, vertex_3d(1.0, -0.0, 0.0), 2, 3, 13, 1, character_g_front_color, character_g_front_shading),
        face_3d(character_g_vertices, vertex_3d(1.0, -0.0, 0.0), 13, 3, 7, 11, character_g_front_color, character_g_front_shading),
        face_3d(character_g_vertices, vertex_3d(1.0, 0.0, 0.0), 12, 10, 5, 6, character_g_front_color, character_g_front_shading),
        face_3d(character_g_vertices, vertex_3d(1.0, -0.0, 0.0), 6, 5, 9, 8, character_g_front_color, character_g_front_shading),
        face_3d(character_g_vertices, vertex_3d(1.0, 0.0, 0.0), 7, 8, 9, 11, character_g_front_color, character_g_front_shading),
        face_3d(character_g_vertices, vertex_3d(-1.0, 0.0, 0.0), 0, 4, 23, 14, character_g_back_color, character_g_back_shading),
        face_3d(character_g_vertices, vertex_3d(-1.0, 0.0, 0.0), 23, 21, 17, 14, character_g_back_color, character_g_back_shading),
        face_3d(character_g_vertices, vertex_3d(-1.0, 0.0, 0.0), 22, 16, 15, 20, character_g_back_color, character_g_back_shading),
        face_3d(character_g_vertices, vertex_3d(-1.0, 0.0, -0.0), 16, 18, 19, 15, character_g_back_color, character_g_back_shading),
        face_3d(character_g_vertices, vertex_3d(-1.0, 0.0, 0.0), 17, 21, 19, 18, character_g_back_color, character_g_back_shading),
        face_3d(character_g_vertices, vertex_3d(0.0, 1.0, 0.0), 2, 0, 14, 3, character_g_up_color, character_g_up_shading),
        face_3d(character_g_vertices, vertex_3d(0.0, 1.0, 0.0), 12, 6, 16, 22, character_g_up_color, character_g_up_shading),
        face_3d(character_g_vertices, vertex_3d(0.0, 1.0, 0.0), 11, 9, 19, 21, character_g_up_color, character_g_up_shading),
        face_3d(character_g_vertices, vertex_3d(-0.0, 0.0, 1.0), 10, 12, 22, 20, character_g_left_color, character_g_left_shading),
        face_3d(character_g_vertices, vertex_3d(0.0, 0.0, 1.0), 3, 14, 17, 7, character_g_left_color, character_g_left_shading),
        face_3d(character_g_vertices, vertex_3d(-0.0, 0.0, 1.0), 9, 5, 15, 19, character_g_left_color, character_g_left_shading),
        face_3d(character_g_vertices, vertex_3d(0.0, 0.0, -1.0), 11, 21, 23, 13, character_g_right_color, character_g_right_shading),
        face_3d(character_g_vertices, vertex_3d(0.0, 0.0, -1.0), 1, 4, 0, 2, character_g_right_color, character_g_right_shading),
        face_3d(character_g_vertices, vertex_3d(0.0, 0.0, -1.0), 6, 8, 18, 16, character_g_right_color, character_g_right_shading),
        face_3d(character_g_vertices, vertex_3d(0.0, -1.0, 0.0), 8, 7, 17, 18, character_g_down_color, character_g_down_shading),
        face_3d(character_g_vertices, vertex_3d(0.0, -1.0, 0.0), 13, 23, 4, 1, character_g_down_color, character_g_down_shading),
        face_3d(character_g_vertices, vertex_3d(0.0, -1.0, 0.0), 5, 10, 20, 15, character_g_down_color, character_g_down_shading),
    };

    constexpr inline model_3d_item character_g_full(character_g_vertices, character_g_full_faces);

    constexpr inline face_3d character_g_faces[] = {
        face_3d(character_g_vertices, vertex_3d(1.0, -0.0, 0.0), 2, 3, 13, 1, character_g_front_color, character_g_front_shading),
        face_3d(character_g_vertices, vertex_3d(1.0, -0.0, 0.0), 13, 3, 7, 11, character_g_front_color, character_g_front_shading),
        face_3d(character_g_vertices, vertex_3d(1.0, 0.0, 0.0), 12, 10, 5, 6, character_g_front_color, character_g_front_shading),
        face_3d(character_g_vertices, vertex_3d(1.0, -0.0, 0.0), 6, 5, 9, 8, character_g_front_color, character_g_front_shading),
        face_3d(character_g_vertices, vertex_3d(1.0, 0.0, 0.0), 7, 8, 9, 11, character_g_front_color, character_g_front_shading),
        face_3d(character_g_vertices, vertex_3d(0.0, 1.0, 0.0), 2, 0, 14, 3, character_g_up_color, character_g_up_shading),
        face_3d(character_g_vertices, vertex_3d(0.0, 1.0, 0.0), 12, 6, 16, 22, character_g_up_color, character_g_up_shading),
        face_3d(character_g_vertices, vertex_3d(0.0, 1.0, 0.0), 11, 9, 19, 21, character_g_up_color, character_g_up_shading),
        face_3d(character_g_vertices, vertex_3d(-0.0, 0.0, 1.0), 10, 12, 22, 20, character_g_left_color, character_g_left_shading),
        face_3d(character_g_vertices, vertex_3d(0.0, 0.0, 1.0), 3, 14, 17, 7, character_g_left_color, character_g_left_shading),
        face_3d(character_g_vertices, vertex_3d(-0.0, 0.0, 1.0), 9, 5, 15, 19, character_g_left_color, character_g_left_shading),
        face_3d(character_g_vertices, vertex_3d(0.0, 0.0, -1.0), 11, 21, 23, 13, character_g_right_color, character_g_right_shading),
        face_3d(character_g_vertices, vertex_3d(0.0, 0.0, -1.0), 1, 4, 0, 2, character_g_right_color, character_g_right_shading),
        face_3d(character_g_vertices, vertex_3d(0.0, 0.0, -1.0), 6, 8, 18, 16, character_g_right_color, character_g_right_shading),
        face_3d(character_g_vertices, vertex_3d(0.0, -1.0, 0.0), 8, 7, 17, 18, character_g_down_color, character_g_down_shading),
        face_3d(character_g_vertices, vertex_3d(0.0, -1.0, 0.0), 13, 23, 4, 1, character_g_down_color, character_g_down_shading),
        face_3d(character_g_vertices, vertex_3d(0.0, -1.0, 0.0), 5, 10, 20, 15, character_g_down_color, character_g_down_shading),
    };

    constexpr inline model_3d_item character_g(character_g_vertices, character_g_faces);
}

#endif

