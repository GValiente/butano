/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_CHARACTER_INTERROGATION_H
#define FR_MODEL_3D_ITEMS_CHARACTER_INTERROGATION_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d character_interrogation_vertices[] = {
        vertex_3d(3.0, 9.0, 7.0),
        vertex_3d(3.0, 9.0, -5.0),
        vertex_3d(3.0, 15.0, -11.0),
        vertex_3d(3.0, 15.0, 7.0),
        vertex_3d(3.0, 3.0, 1.0),
        vertex_3d(3.0, -3.0, -5.0),
        vertex_3d(3.0, -3.0, -11.0),
        vertex_3d(3.0, 3.0, -5.0),
        vertex_3d(3.0, -6.0, 1.0),
        vertex_3d(3.0, -6.0, -5.0),
        vertex_3d(3.0, -15.0, -5.0),
        vertex_3d(3.0, -9.0, -5.0),
        vertex_3d(-3.0, 9.0, 7.0),
        vertex_3d(-3.0, 9.0, -5.0),
        vertex_3d(-3.0, 15.0, -11.0),
        vertex_3d(-3.0, 15.0, 7.0),
        vertex_3d(-3.0, 3.0, 1.0),
        vertex_3d(-3.0, -3.0, -5.0),
        vertex_3d(-3.0, -3.0, -11.0),
        vertex_3d(-3.0, 3.0, -5.0),
        vertex_3d(-3.0, -6.0, 1.0),
        vertex_3d(-3.0, -6.0, -5.0),
        vertex_3d(-3.0, -15.0, -5.0),
        vertex_3d(-3.0, -9.0, -5.0),
        vertex_3d(3.0, -15.0, 1.0),
        vertex_3d(3.0, -9.0, 1.0),
        vertex_3d(-3.0, -15.0, 1.0),
        vertex_3d(-3.0, -9.0, 1.0),
    };

    constexpr inline int character_interrogation_front_color = 7;
    constexpr inline int character_interrogation_front_shading = 7;

    constexpr inline int character_interrogation_back_color = 7;
    constexpr inline int character_interrogation_back_shading = 0;

    constexpr inline int character_interrogation_up_color = 7;
    constexpr inline int character_interrogation_up_shading = 3;

    constexpr inline int character_interrogation_left_color = 7;
    constexpr inline int character_interrogation_left_shading = 2;

    constexpr inline int character_interrogation_right_color = 7;
    constexpr inline int character_interrogation_right_shading = 2;

    constexpr inline int character_interrogation_down_color = 7;
    constexpr inline int character_interrogation_down_shading = 1;

    constexpr inline face_3d character_interrogation_faces[] = {
        face_3d(character_interrogation_vertices, vertex_3d(1.0, 0.0, 0.0), 0, 1, 2, 3, character_interrogation_front_color, character_interrogation_front_shading),
        face_3d(character_interrogation_vertices, vertex_3d(1.0, 0.0, 0.0), 4, 5, 6, 7, character_interrogation_front_color, character_interrogation_front_shading),
        face_3d(character_interrogation_vertices, vertex_3d(1.0, -0.0, 0.0), 2, 1, 7, 6, character_interrogation_front_color, character_interrogation_front_shading),
        face_3d(character_interrogation_vertices, vertex_3d(1.0, -0.0, 0.0), 5, 4, 8, 9, character_interrogation_front_color, character_interrogation_front_shading),
        face_3d(character_interrogation_vertices, vertex_3d(1.0, -0.0, 0.0), 11, 25, 24, 10, character_interrogation_front_color, character_interrogation_front_shading),
        face_3d(character_interrogation_vertices, vertex_3d(-1.0, 0.0, 0.0), 27, 23, 22, 26, character_interrogation_back_color, character_interrogation_back_shading),
        face_3d(character_interrogation_vertices, vertex_3d(-1.0, 0.0, 0.0), 12, 15, 14, 13, character_interrogation_back_color, character_interrogation_back_shading),
        face_3d(character_interrogation_vertices, vertex_3d(-1.0, 0.0, 0.0), 16, 19, 18, 17, character_interrogation_back_color, character_interrogation_back_shading),
        face_3d(character_interrogation_vertices, vertex_3d(-1.0, 0.0, -0.0), 14, 18, 19, 13, character_interrogation_back_color, character_interrogation_back_shading),
        face_3d(character_interrogation_vertices, vertex_3d(-1.0, 0.0, 0.0), 17, 21, 20, 16, character_interrogation_back_color, character_interrogation_back_shading),
        face_3d(character_interrogation_vertices, vertex_3d(0.0, 1.0, 0.0), 2, 14, 15, 3, character_interrogation_up_color, character_interrogation_up_shading),
        face_3d(character_interrogation_vertices, vertex_3d(0.0, 1.0, 0.0), 7, 19, 16, 4, character_interrogation_up_color, character_interrogation_up_shading),
        face_3d(character_interrogation_vertices, vertex_3d(0.0, 1.0, -0.0), 27, 25, 11, 23, character_interrogation_up_color, character_interrogation_up_shading),
        face_3d(character_interrogation_vertices, vertex_3d(-0.0, 0.0, 1.0), 24, 25, 27, 26, character_interrogation_left_color, character_interrogation_left_shading),
        face_3d(character_interrogation_vertices, vertex_3d(0.0, 0.0, 1.0), 4, 16, 20, 8, character_interrogation_left_color, character_interrogation_left_shading),
        face_3d(character_interrogation_vertices, vertex_3d(0.0, 0.0, 1.0), 1, 13, 19, 7, character_interrogation_left_color, character_interrogation_left_shading),
        face_3d(character_interrogation_vertices, vertex_3d(0.0, 0.0, 1.0), 3, 15, 12, 0, character_interrogation_left_color, character_interrogation_left_shading),
        face_3d(character_interrogation_vertices, vertex_3d(0.0, 0.0, -1.0), 6, 18, 14, 2, character_interrogation_right_color, character_interrogation_right_shading),
        face_3d(character_interrogation_vertices, vertex_3d(0.0, 0.0, -1.0), 9, 21, 17, 5, character_interrogation_right_color, character_interrogation_right_shading),
        face_3d(character_interrogation_vertices, vertex_3d(0.0, 0.0, -1.0), 10, 22, 23, 11, character_interrogation_right_color, character_interrogation_right_shading),
        face_3d(character_interrogation_vertices, vertex_3d(0.0, -1.0, 0.0), 5, 17, 18, 6, character_interrogation_down_color, character_interrogation_down_shading),
        face_3d(character_interrogation_vertices, vertex_3d(0.0, -1.0, 0.0), 9, 8, 20, 21, character_interrogation_down_color, character_interrogation_down_shading),
        face_3d(character_interrogation_vertices, vertex_3d(0.0, -1.0, 0.0), 0, 12, 13, 1, character_interrogation_down_color, character_interrogation_down_shading),
        face_3d(character_interrogation_vertices, vertex_3d(0.0, -1.0, 0.0), 26, 22, 10, 24, character_interrogation_down_color, character_interrogation_down_shading),
    };

    constexpr inline model_3d_item character_interrogation(character_interrogation_vertices, character_interrogation_faces);
}

#endif

