/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * CA3
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_FAT_TREE_H
#define FR_MODEL_3D_ITEMS_FAT_TREE_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d fat_tree_vertices[] = {
        vertex_3d(-25.88, 17.28, -25.88),
        vertex_3d(-25.88, 17.28, 25.88),
        vertex_3d(0.0, 51.09, 0.0),
        vertex_3d(25.88, 17.28, 25.88),
        vertex_3d(25.88, 17.28, -25.88),
        vertex_3d(-6.84, -0.75, -6.84),
        vertex_3d(-6.84, -0.75, 6.84),
        vertex_3d(6.84, -0.75, -6.84),
        vertex_3d(6.84, -0.75, 6.84),
        vertex_3d(-6.84, -39.07, -6.84),
        vertex_3d(-6.84, -39.07, 6.84),
        vertex_3d(6.84, -39.07, -6.84),
        vertex_3d(6.84, -39.07, 6.84),
    };

    constexpr inline int fat_tree_green_up_front_color = 5;
    constexpr inline int fat_tree_green_up_front_shading = 7;

    constexpr inline int fat_tree_green_up_side_color = 5;
    constexpr inline int fat_tree_green_up_side_shading = 6;

    constexpr inline int fat_tree_green_up_back_color = 5;
    constexpr inline int fat_tree_green_up_back_shading = 4;

    constexpr inline int fat_tree_green_down_front_color = 5;
    constexpr inline int fat_tree_green_down_front_shading = 5;

    constexpr inline int fat_tree_green_down_side_color = 5;
    constexpr inline int fat_tree_green_down_side_shading = 2;

    constexpr inline int fat_tree_green_down_back_color = 5;
    constexpr inline int fat_tree_green_down_back_shading = 0;

    constexpr inline int fat_tree_brown_front_color = 4;
    constexpr inline int fat_tree_brown_front_shading = 7;

    constexpr inline int fat_tree_brown_side_color = 4;
    constexpr inline int fat_tree_brown_side_shading = 6;

    constexpr inline int fat_tree_brown_back_color = 4;
    constexpr inline int fat_tree_brown_back_shading = 5;

    constexpr inline int fat_tree_brown_down_color = 4;
    constexpr inline int fat_tree_brown_down_shading = 3;

    constexpr inline face_3d fat_tree_full_faces[] = {
        face_3d(fat_tree_vertices, vertex_3d(-0.7940710888550809, 0.6078248973549095, 0.0), 0, 1, 2, fat_tree_green_up_front_color, fat_tree_green_up_front_shading),
        face_3d(fat_tree_vertices, vertex_3d(-0.0, 0.6078248973549095, 0.7940710888550809), 3, 2, 1, fat_tree_green_up_side_color, fat_tree_green_up_side_shading),
        face_3d(fat_tree_vertices, vertex_3d(0.0, 0.6078248973549095, -0.7940710888550809), 4, 0, 2, fat_tree_green_up_side_color, fat_tree_green_up_side_shading),
        face_3d(fat_tree_vertices, vertex_3d(0.7940710888550809, 0.6078248973549095, -0.0), 2, 3, 4, fat_tree_green_up_back_color, fat_tree_green_up_back_shading),
        face_3d(fat_tree_vertices, vertex_3d(-0.687585964865955, -0.7261029823099158, -0.0), 0, 5, 6, 1, fat_tree_green_down_front_color, fat_tree_green_down_front_shading),
        face_3d(fat_tree_vertices, vertex_3d(-0.0, -0.7261029823099157, -0.6875859648659549), 5, 0, 4, 7, fat_tree_green_down_side_color, fat_tree_green_down_side_shading),
        face_3d(fat_tree_vertices, vertex_3d(0.0, -0.7261029823099157, 0.6875859648659549), 3, 1, 6, 8, fat_tree_green_down_side_color, fat_tree_green_down_side_shading),
        face_3d(fat_tree_vertices, vertex_3d(0.6875859648659549, -0.7261029823099157, 0.0), 7, 4, 3, 8, fat_tree_green_down_back_color, fat_tree_green_down_back_shading),
        face_3d(fat_tree_vertices, vertex_3d(-1.0, 0.0, 0.0), 6, 5, 9, 10, fat_tree_brown_front_color, fat_tree_brown_front_shading),
        face_3d(fat_tree_vertices, vertex_3d(0.0, 0.0, -1.0), 5, 7, 11, 9, fat_tree_brown_side_color, fat_tree_brown_side_shading),
        face_3d(fat_tree_vertices, vertex_3d(0.0, 0.0, 1.0), 8, 6, 10, 12, fat_tree_brown_side_color, fat_tree_brown_side_shading),
        face_3d(fat_tree_vertices, vertex_3d(1.0, -0.0, 0.0), 7, 8, 12, 11, fat_tree_brown_back_color, fat_tree_brown_back_shading),
        face_3d(fat_tree_vertices, vertex_3d(0.0, -1.0, -0.0), 9, 11, 12, 10, fat_tree_brown_down_color, fat_tree_brown_down_shading),
    };

    constexpr inline const face_3d* fat_tree_collision_face_ptr = &fat_tree_full_faces[12];

    constexpr inline model_3d_item fat_tree_full(fat_tree_vertices, fat_tree_full_faces, fat_tree_collision_face_ptr);

    constexpr inline face_3d fat_tree_faces[] = {
        face_3d(fat_tree_vertices, vertex_3d(-0.7940710888550809, 0.6078248973549095, 0.0), 0, 1, 2, fat_tree_green_up_front_color, fat_tree_green_up_front_shading),
        face_3d(fat_tree_vertices, vertex_3d(-0.0, 0.6078248973549095, 0.7940710888550809), 3, 2, 1, fat_tree_green_up_side_color, fat_tree_green_up_side_shading),
        face_3d(fat_tree_vertices, vertex_3d(0.0, 0.6078248973549095, -0.7940710888550809), 4, 0, 2, fat_tree_green_up_side_color, fat_tree_green_up_side_shading),
        face_3d(fat_tree_vertices, vertex_3d(0.7940710888550809, 0.6078248973549095, -0.0), 2, 3, 4, fat_tree_green_up_back_color, fat_tree_green_up_back_shading),
        face_3d(fat_tree_vertices, vertex_3d(-1.0, 0.0, 0.0), 6, 5, 9, 10, fat_tree_brown_front_color, fat_tree_brown_front_shading),
        face_3d(fat_tree_vertices, vertex_3d(0.0, 0.0, -1.0), 5, 7, 11, 9, fat_tree_brown_side_color, fat_tree_brown_side_shading),
        face_3d(fat_tree_vertices, vertex_3d(0.0, 0.0, 1.0), 8, 6, 10, 12, fat_tree_brown_side_color, fat_tree_brown_side_shading),
        face_3d(fat_tree_vertices, vertex_3d(1.0, -0.0, 0.0), 7, 8, 12, 11, fat_tree_brown_back_color, fat_tree_brown_back_shading),
    };

    constexpr inline model_3d_item fat_tree(fat_tree_vertices, fat_tree_faces, fat_tree_collision_face_ptr);
}

#endif

