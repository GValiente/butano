/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_CHARACTER_S_H
#define FR_MODEL_3D_ITEMS_CHARACTER_S_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d character_s_vertices[] = {
        vertex_3d(3.0, 9.0, -11.0),
        vertex_3d(3.0, 9.0, 1.0),
        vertex_3d(3.0, 15.0, 7.0),
        vertex_3d(3.0, 15.0, -11.0),
        vertex_3d(3.0, 3.0, -11.0),
        vertex_3d(3.0, -3.0, -5.0),
        vertex_3d(3.0, -3.0, 7.0),
        vertex_3d(3.0, 3.0, 1.0),
        vertex_3d(3.0, -15.0, -11.0),
        vertex_3d(3.0, -9.0, -5.0),
        vertex_3d(3.0, -15.0, 7.0),
        vertex_3d(3.0, -9.0, 7.0),
        vertex_3d(-3.0, 9.0, -11.0),
        vertex_3d(-3.0, 9.0, 1.0),
        vertex_3d(-3.0, 15.0, 7.0),
        vertex_3d(-3.0, 15.0, -11.0),
        vertex_3d(-3.0, 3.0, -11.0),
        vertex_3d(-3.0, -3.0, -5.0),
        vertex_3d(-3.0, -3.0, 7.0),
        vertex_3d(-3.0, 3.0, 1.0),
        vertex_3d(-3.0, -15.0, -11.0),
        vertex_3d(-3.0, -9.0, -5.0),
        vertex_3d(-3.0, -15.0, 7.0),
        vertex_3d(-3.0, -9.0, 7.0),
    };

    constexpr inline int character_s_front_color = 0;
    constexpr inline int character_s_front_shading = 7;

    constexpr inline int character_s_back_color = 0;
    constexpr inline int character_s_back_shading = 0;

    constexpr inline int character_s_up_color = 0;
    constexpr inline int character_s_up_shading = 3;

    constexpr inline int character_s_left_color = 0;
    constexpr inline int character_s_left_shading = 2;

    constexpr inline int character_s_right_color = 0;
    constexpr inline int character_s_right_shading = 2;

    constexpr inline int character_s_down_color = 0;
    constexpr inline int character_s_down_shading = 1;

    constexpr inline face_3d character_s_faces[] = {
        face_3d(character_s_vertices, vertex_3d(1.0, 0.0, 0.0), 0, 3, 2, 1, character_s_front_color, character_s_front_shading),
        face_3d(character_s_vertices, vertex_3d(1.0, 0.0, 0.0), 4, 7, 6, 5, character_s_front_color, character_s_front_shading),
        face_3d(character_s_vertices, vertex_3d(1.0, 0.0, -0.0), 2, 6, 7, 1, character_s_front_color, character_s_front_shading),
        face_3d(character_s_vertices, vertex_3d(1.0, 0.0, 0.0), 5, 9, 8, 4, character_s_front_color, character_s_front_shading),
        face_3d(character_s_vertices, vertex_3d(1.0, 0.0, 0.0), 10, 8, 9, 11, character_s_front_color, character_s_front_shading),
        face_3d(character_s_vertices, vertex_3d(-1.0, 0.0, 0.0), 12, 13, 14, 15, character_s_back_color, character_s_back_shading),
        face_3d(character_s_vertices, vertex_3d(-1.0, 0.0, 0.0), 16, 17, 18, 19, character_s_back_color, character_s_back_shading),
        face_3d(character_s_vertices, vertex_3d(-1.0, 0.0, 0.0), 14, 13, 19, 18, character_s_back_color, character_s_back_shading),
        face_3d(character_s_vertices, vertex_3d(-1.0, 0.0, 0.0), 17, 16, 20, 21, character_s_back_color, character_s_back_shading),
        face_3d(character_s_vertices, vertex_3d(-1.0, 0.0, 0.0), 22, 23, 21, 20, character_s_back_color, character_s_back_shading),
        face_3d(character_s_vertices, vertex_3d(0.0, 1.0, 0.0), 2, 3, 15, 14, character_s_up_color, character_s_up_shading),
        face_3d(character_s_vertices, vertex_3d(0.0, 1.0, 0.0), 11, 9, 21, 23, character_s_up_color, character_s_up_shading),
        face_3d(character_s_vertices, vertex_3d(0.0, 1.0, 0.0), 7, 4, 16, 19, character_s_up_color, character_s_up_shading),
        face_3d(character_s_vertices, vertex_3d(-0.0, 0.0, 1.0), 6, 2, 14, 18, character_s_left_color, character_s_left_shading),
        face_3d(character_s_vertices, vertex_3d(-0.0, 0.0, 1.0), 9, 5, 17, 21, character_s_left_color, character_s_left_shading),
        face_3d(character_s_vertices, vertex_3d(-0.0, 0.0, 1.0), 10, 11, 23, 22, character_s_left_color, character_s_left_shading),
        face_3d(character_s_vertices, vertex_3d(0.0, 0.0, -1.0), 4, 8, 20, 16, character_s_right_color, character_s_right_shading),
        face_3d(character_s_vertices, vertex_3d(0.0, 0.0, -1.0), 1, 7, 19, 13, character_s_right_color, character_s_right_shading),
        face_3d(character_s_vertices, vertex_3d(0.0, 0.0, -1.0), 3, 0, 12, 15, character_s_right_color, character_s_right_shading),
        face_3d(character_s_vertices, vertex_3d(0.0, -1.0, 0.0), 5, 6, 18, 17, character_s_down_color, character_s_down_shading),
        face_3d(character_s_vertices, vertex_3d(0.0, -1.0, 0.0), 8, 10, 22, 20, character_s_down_color, character_s_down_shading),
        face_3d(character_s_vertices, vertex_3d(0.0, -1.0, 0.0), 0, 1, 13, 12, character_s_down_color, character_s_down_shading),
    };

    constexpr inline model_3d_item character_s(character_s_vertices, character_s_faces);
}

#endif

