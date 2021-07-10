/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_CHARACTER_M_H
#define FR_MODEL_3D_ITEMS_CHARACTER_M_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d character_m_vertices[] = {
        vertex_3d(3.0, 9.0, 3.0),
        vertex_3d(3.0, 6.0, 0.0),
        vertex_3d(3.0, 9.0, -3.0),
        vertex_3d(3.0, 15.0, 3.0),
        vertex_3d(3.0, -15.0, 9.0),
        vertex_3d(3.0, -15.0, 3.0),
        vertex_3d(3.0, 15.0, 9.0),
        vertex_3d(3.0, 15.0, -9.0),
        vertex_3d(3.0, 15.0, -3.0),
        vertex_3d(3.0, -15.0, -3.0),
        vertex_3d(3.0, -15.0, -9.0),
        vertex_3d(-3.0, 9.0, -3.0),
        vertex_3d(-3.0, 6.0, 0.0),
        vertex_3d(-3.0, 9.0, 3.0),
        vertex_3d(-3.0, 15.0, 3.0),
        vertex_3d(-3.0, -15.0, 9.0),
        vertex_3d(-3.0, 15.0, 9.0),
        vertex_3d(-3.0, -15.0, 3.0),
        vertex_3d(-3.0, -15.0, -3.0),
        vertex_3d(-3.0, 15.0, -3.0),
        vertex_3d(-3.0, 15.0, -9.0),
        vertex_3d(-3.0, -15.0, -9.0),
        vertex_3d(3.0, 12.0, 0.0),
        vertex_3d(-3.0, 12.0, 0.0),
    };

    constexpr inline int character_m_front_color = 7;
    constexpr inline int character_m_front_shading = 7;

    constexpr inline int character_m_back_color = 7;
    constexpr inline int character_m_back_shading = 0;

    constexpr inline int character_m_up_color = 7;
    constexpr inline int character_m_up_shading = 3;

    constexpr inline int character_m_left_color = 7;
    constexpr inline int character_m_left_shading = 2;

    constexpr inline int character_m_right_color = 7;
    constexpr inline int character_m_right_shading = 2;

    constexpr inline int character_m_down_color = 7;
    constexpr inline int character_m_down_shading = 1;

    constexpr inline face_3d character_m_faces[] = {
        face_3d(character_m_vertices, vertex_3d(1.0, 0.0, 0.0), 4, 5, 3, 6, character_m_front_color, character_m_front_shading),
        face_3d(character_m_vertices, vertex_3d(1.0, -0.0, 0.0), 7, 8, 9, 10, character_m_front_color, character_m_front_shading),
        face_3d(character_m_vertices, vertex_3d(1.0, -0.0, 0.0), 8, 22, 1, 2, character_m_front_color, character_m_front_shading),
        face_3d(character_m_vertices, vertex_3d(1.0, -0.0, 0.0), 22, 3, 0, 1, character_m_front_color, character_m_front_shading),
        face_3d(character_m_vertices, vertex_3d(-1.0, -0.0, 0.0), 11, 12, 23, 19, character_m_back_color, character_m_back_shading),
        face_3d(character_m_vertices, vertex_3d(-1.0, 0.0, 0.0), 15, 16, 14, 17, character_m_back_color, character_m_back_shading),
        face_3d(character_m_vertices, vertex_3d(-1.0, 0.0, 0.0), 18, 19, 20, 21, character_m_back_color, character_m_back_shading),
        face_3d(character_m_vertices, vertex_3d(-1.0, 0.0, 0.0), 14, 23, 12, 13, character_m_back_color, character_m_back_shading),
        face_3d(character_m_vertices, vertex_3d(0.0, 0.7071067811865476, -0.7071067811865476), 14, 3, 22, 23, character_m_up_color, character_m_up_shading),
        face_3d(character_m_vertices, vertex_3d(0.0, 1.0, 0.0), 20, 19, 8, 7, character_m_up_color, character_m_up_shading),
        face_3d(character_m_vertices, vertex_3d(0.0, 1.0, 0.0), 6, 3, 14, 16, character_m_up_color, character_m_up_shading),
        face_3d(character_m_vertices, vertex_3d(0.0, 0.7071067811865476, 0.7071067811865476), 19, 23, 22, 8, character_m_up_color, character_m_up_shading),
        face_3d(character_m_vertices, vertex_3d(-0.0, 0.0, 1.0), 9, 2, 11, 18, character_m_left_color, character_m_left_shading),
        face_3d(character_m_vertices, vertex_3d(-0.0, 0.0, 1.0), 4, 6, 16, 15, character_m_left_color, character_m_left_shading),
        face_3d(character_m_vertices, vertex_3d(0.0, 0.0, -1.0), 10, 21, 20, 7, character_m_right_color, character_m_right_shading),
        face_3d(character_m_vertices, vertex_3d(0.0, 0.0, -1.0), 13, 0, 5, 17, character_m_right_color, character_m_right_shading),
        face_3d(character_m_vertices, vertex_3d(0.0, -1.0, 0.0), 10, 9, 18, 21, character_m_down_color, character_m_down_shading),
        face_3d(character_m_vertices, vertex_3d(0.0, -1.0, 0.0), 5, 4, 15, 17, character_m_down_color, character_m_down_shading),
        face_3d(character_m_vertices, vertex_3d(0.0, -0.7071067811865476, 0.7071067811865476), 1, 0, 13, 12, character_m_down_color, character_m_down_shading),
        face_3d(character_m_vertices, vertex_3d(0.0, -0.7071067811865476, -0.7071067811865476), 11, 2, 1, 12, character_m_down_color, character_m_down_shading),
    };

    constexpr inline model_3d_item character_m(character_m_vertices, character_m_faces);
}

#endif

