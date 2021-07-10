/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * CA3
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_HOUSE_H
#define FR_MODEL_3D_ITEMS_HOUSE_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d house_vertices[] = {
        vertex_3d(-70.0, -8.0 - 10.0, 22.0),
        vertex_3d(-70.0, 6.0 - 10.0, 22.0),
        vertex_3d(-70.0, 6.0 - 10.0, -22.0),
        vertex_3d(-70.0, -8.0 - 10.0, -22.0),
        vertex_3d(-70.0, -15.0 - 10.0, -40.0),
        vertex_3d(-70.0, 20.0 - 10.0, -40.0),
        vertex_3d(70.0, 20.0 - 10.0, -40.0),
        vertex_3d(70.0, -15.0 - 10.0, -40.0),
        vertex_3d(70.0, -15.0 - 10.0, -15.0),
        vertex_3d(70.0, 5.0 - 10.0, -15.0),
        vertex_3d(70.0, 5.0 - 10.0, 11.0),
        vertex_3d(70.0, -15.0 - 10.0, 11.0),
        vertex_3d(42.0, 8.0 - 10.0, 40.0),
        vertex_3d(-42.0, 8.0 - 10.0, 40.0),
        vertex_3d(-42.0, -15.0 - 10.0, 40.0),
        vertex_3d(42.0, -15.0 - 10.0, 40.0),
        vertex_3d(-40.0, 40.0 - 10.0, 0.0),
        vertex_3d(-73.0, 20.0 - 10.0, -43.0),
        vertex_3d(72.0, 20.0 - 10.0, -43.0),
        vertex_3d(40.0, 40.0 - 10.0, 0.0),
        vertex_3d(72.0, 20.0 - 10.0, 43.0),
        vertex_3d(-73.0, 20.0 - 10.0, 43.0),
        vertex_3d(70.0, 20.0 - 10.0, 40.0),
        vertex_3d(-70.0, 20.0 - 10.0, 40.0),
        vertex_3d(70.0, -15.0 - 10.0, 40.0),
        vertex_3d(-70.0, -15.0 - 10.0, 40.0),
    };

    constexpr inline int house_roof_front_color = 0;
    constexpr inline int house_roof_front_shading = 7;

    constexpr inline int house_roof_left_color = 0;
    constexpr inline int house_roof_left_shading = 6;

    constexpr inline int house_roof_right_color = 0;
    constexpr inline int house_roof_right_shading = 5;

    constexpr inline int house_roof_back_color = 0;
    constexpr inline int house_roof_back_shading = 3;

    constexpr inline int house_roof_down_color = 0;
    constexpr inline int house_roof_down_shading = 2;

    constexpr inline int house_window_color = 1;
    constexpr inline int house_window_shading = -1;

    constexpr inline int house_small_door_color = 4;
    constexpr inline int house_small_door_shading = -1;

    constexpr inline int house_big_door_color = 5;
    constexpr inline int house_big_door_shading = -1;

    constexpr inline int house_wall_front_color = 2;
    constexpr inline int house_wall_front_shading = 5;

    constexpr inline int house_wall_left_color = 2;
    constexpr inline int house_wall_left_shading = 4;

    constexpr inline int house_wall_right_color = 2;
    constexpr inline int house_wall_right_shading = 3;

    constexpr inline int house_wall_back_color = 2;
    constexpr inline int house_wall_back_shading = 2;

    constexpr inline int house_wall_down_color = 2;
    constexpr inline int house_wall_down_shading = 0;

    constexpr inline face_3d house_full_faces[] = {
        face_3d(house_vertices, vertex_3d(-0.5183017160933442, 0.8551978315540179, 0.0), 16, 17, 21, house_roof_front_color, house_roof_front_shading),
        face_3d(house_vertices, vertex_3d(0.0, 0.9067211118885072, -0.4217307497155848), 16, 19, 18, 17, house_roof_left_color, house_roof_left_shading),
        face_3d(house_vertices, vertex_3d(0.0, 0.9067211118885072, 0.4217307497155848), 19, 16, 21, 20, house_roof_right_color, house_roof_right_shading),
        face_3d(house_vertices, vertex_3d(0.5299989400031799, 0.8479983040050879, 0.0), 20, 18, 19, house_roof_back_color, house_roof_back_shading),
        face_3d(house_vertices, vertex_3d(-0.0, -1.0, 0.0), 21, 17, 5, 23, house_roof_down_color, house_roof_down_shading),
        face_3d(house_vertices, vertex_3d(0.0, -1.0, 0.0), 21, 23, 22, 20, house_roof_down_color, house_roof_down_shading),
        face_3d(house_vertices, vertex_3d(0.0, -1.0, 0.0), 20, 22, 6, 18, house_roof_down_color, house_roof_down_shading),
        face_3d(house_vertices, vertex_3d(0.0, -1.0, 0.0), 6, 5, 17, 18, house_roof_down_color, house_roof_down_shading),
        face_3d(house_vertices, vertex_3d(-1.0, 0.0, 0.0), 0, 1, 2, 3, house_window_color, house_window_shading),
        face_3d(house_vertices, vertex_3d(1.0, 0.0, 0.0), 8, 9, 10, 11, house_small_door_color, house_small_door_shading),
        face_3d(house_vertices, vertex_3d(0.0, 0.0, 1.0), 12, 13, 14, 15, house_big_door_color, house_big_door_shading),
        face_3d(house_vertices, vertex_3d(-1.0, 0.0, 0.0), 0, 3, 4, 25, house_wall_front_color, house_wall_front_shading),
        face_3d(house_vertices, vertex_3d(-1.0, 0.0, 0.0), 1, 0, 25, 23, house_wall_front_color, house_wall_front_shading),
        face_3d(house_vertices, vertex_3d(-1.0, 0.0, 0.0), 2, 1, 23, 5, house_wall_front_color, house_wall_front_shading),
        face_3d(house_vertices, vertex_3d(-1.0, 0.0, 0.0), 3, 2, 5, 4, house_wall_front_color, house_wall_front_shading),
        face_3d(house_vertices, vertex_3d(-0.0, 0.0, -1.0), 4, 5, 6, 7, house_wall_left_color, house_wall_left_shading),
        face_3d(house_vertices, vertex_3d(0.0, 0.0, 1.0), 13, 12, 22, 23, house_wall_right_color, house_wall_right_shading),
        face_3d(house_vertices, vertex_3d(0.0, 0.0, 1.0), 22, 12, 15, 24, house_wall_right_color, house_wall_right_shading),
        face_3d(house_vertices, vertex_3d(0.0, 0.0, 1.0), 25, 14, 13, 23, house_wall_right_color, house_wall_right_shading),
        face_3d(house_vertices, vertex_3d(1.0, 0.0, 0.0), 24, 11, 10, 22, house_wall_back_color, house_wall_back_shading),
        face_3d(house_vertices, vertex_3d(1.0, 0.0, 0.0), 22, 10, 9, 6, house_wall_back_color, house_wall_back_shading),
        face_3d(house_vertices, vertex_3d(1.0, 0.0, 0.0), 7, 6, 9, 8, house_wall_back_color, house_wall_back_shading),
        face_3d(house_vertices, vertex_3d(0.0, -1.0, -0.0), 4, 7, 24, 25, house_wall_down_color, house_wall_down_shading),
    };

    constexpr inline const face_3d* house_collision_face_ptr = &house_full_faces[22];

    constexpr inline model_3d_item house_full(house_vertices, house_full_faces, house_collision_face_ptr);

    constexpr inline face_3d house_faces[] = {
        face_3d(house_vertices, vertex_3d(-0.5183017160933442, 0.8551978315540179, 0.0), 16, 17, 21, house_roof_front_color, house_roof_front_shading),
        face_3d(house_vertices, vertex_3d(0.0, 0.9067211118885072, -0.4217307497155848), 16, 19, 18, 17, house_roof_left_color, house_roof_left_shading),
        face_3d(house_vertices, vertex_3d(0.0, 0.9067211118885072, 0.4217307497155848), 19, 16, 21, 20, house_roof_right_color, house_roof_right_shading),
        face_3d(house_vertices, vertex_3d(0.5299989400031799, 0.8479983040050879, 0.0), 20, 18, 19, house_roof_back_color, house_roof_back_shading),
        face_3d(house_vertices, vertex_3d(-1.0, 0.0, 0.0), 0, 1, 2, 3, house_window_color, house_window_shading),
        face_3d(house_vertices, vertex_3d(1.0, 0.0, 0.0), 8, 9, 10, 11, house_small_door_color, house_small_door_shading),
        face_3d(house_vertices, vertex_3d(0.0, 0.0, 1.0), 12, 13, 14, 15, house_big_door_color, house_big_door_shading),
        face_3d(house_vertices, vertex_3d(-1.0, 0.0, 0.0), 0, 3, 4, 25, house_wall_front_color, house_wall_front_shading),
        face_3d(house_vertices, vertex_3d(-1.0, 0.0, 0.0), 1, 0, 25, 23, house_wall_front_color, house_wall_front_shading),
        face_3d(house_vertices, vertex_3d(-1.0, 0.0, 0.0), 2, 1, 23, 5, house_wall_front_color, house_wall_front_shading),
        face_3d(house_vertices, vertex_3d(-1.0, 0.0, 0.0), 3, 2, 5, 4, house_wall_front_color, house_wall_front_shading),
        face_3d(house_vertices, vertex_3d(-0.0, 0.0, -1.0), 4, 5, 6, 7, house_wall_left_color, house_wall_left_shading),
        face_3d(house_vertices, vertex_3d(0.0, 0.0, 1.0), 13, 12, 22, 23, house_wall_right_color, house_wall_right_shading),
        face_3d(house_vertices, vertex_3d(0.0, 0.0, 1.0), 22, 12, 15, 24, house_wall_right_color, house_wall_right_shading),
        face_3d(house_vertices, vertex_3d(0.0, 0.0, 1.0), 25, 14, 13, 23, house_wall_right_color, house_wall_right_shading),
        face_3d(house_vertices, vertex_3d(1.0, 0.0, 0.0), 24, 11, 10, 22, house_wall_back_color, house_wall_back_shading),
        face_3d(house_vertices, vertex_3d(1.0, 0.0, 0.0), 22, 10, 9, 6, house_wall_back_color, house_wall_back_shading),
        face_3d(house_vertices, vertex_3d(1.0, 0.0, 0.0), 7, 6, 9, 8, house_wall_back_color, house_wall_back_shading),
    };

    constexpr inline model_3d_item house(house_vertices, house_faces, house_collision_face_ptr);
}

#endif

