/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_PICKUP_CAR_H
#define FR_MODEL_3D_ITEMS_PICKUP_CAR_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d pickup_car_vertices[] = {
        vertex_3d(5.5, 1.5, -5.24),
        vertex_3d(-5.5, 1.5, -5.24),
        vertex_3d(-4.5, 5.5, -3.24),
        vertex_3d(4.5, 5.5, -3.24),
        vertex_3d(-5.5, 1.5, 4.76),
        vertex_3d(-4.5, 5.5, 3.26),
        vertex_3d(4.5, 5.5, 3.26),
        vertex_3d(5.5, 1.5, 4.76),
        vertex_3d(5.5, 1.5, 13.76),
        vertex_3d(4.0, 1.5, 4.76),
        vertex_3d(4.0, 1.5, 12.26),
        vertex_3d(-4.0, 1.5, 12.26),
        vertex_3d(-5.5, 1.5, 13.76),
        vertex_3d(-4.0, 1.5, 4.76),
        vertex_3d(5.5, 0.5, -13.24),
        vertex_3d(-5.5, 0.5, -13.24),
        vertex_3d(-4.0, -3.5, 12.26),
        vertex_3d(4.0, -3.5, 12.26),
        vertex_3d(4.0, -3.5, 4.76),
        vertex_3d(-4.0, -3.5, 4.76),
        vertex_3d(5.5, -0.5, -13.24),
        vertex_3d(-5.5, -0.5, -13.24),
        vertex_3d(-5.5, -1.5, -13.24),
        vertex_3d(-3.19, -1.5, -13.24),
        vertex_3d(-3.19, -2.5, -13.24),
        vertex_3d(-5.5, -5.5, -13.24),
        vertex_3d(5.5, -1.5, -13.24),
        vertex_3d(5.5, -5.5, -13.24),
        vertex_3d(3.19, -2.5, -13.24),
        vertex_3d(3.19, -1.5, -13.24),
        vertex_3d(-5.5, 0.5, 13.76),
        vertex_3d(5.5, 0.5, 13.76),
        vertex_3d(3.0, -1.5, 13.76),
        vertex_3d(3.0, 0.5, 13.76),
        vertex_3d(-3.0, 0.5, 13.76),
        vertex_3d(-3.0, -1.5, 13.76),
        vertex_3d(-5.5, -5.5, 13.76),
        vertex_3d(5.5, -5.5, 13.76),
        vertex_3d(5.5, -1.5, 13.76),
        vertex_3d(-5.5, -1.5, 13.76),
        vertex_3d(5.5, -1.0, -6.74),
        vertex_3d(5.5, -1.0, -11.24),
        vertex_3d(5.5, -5.5, -11.24),
        vertex_3d(5.5, -5.5, 11.76),
        vertex_3d(5.5, -1.0, 11.76),
        vertex_3d(5.5, -1.0, 7.26),
        vertex_3d(5.5, -5.5, 7.26),
        vertex_3d(5.5, -5.5, -6.74),
        vertex_3d(-5.5, -5.5, -11.24),
        vertex_3d(-5.5, -1.0, -11.24),
        vertex_3d(-5.5, -1.0, -6.74),
        vertex_3d(-5.5, -1.0, 11.76),
        vertex_3d(-5.5, -5.5, 11.76),
        vertex_3d(-5.5, -1.0, 7.26),
        vertex_3d(-5.5, -5.5, -6.74),
        vertex_3d(-5.5, -5.5, 7.26),
        vertex_3d(-3.19, -0.5, -13.24),
        vertex_3d(3.19, -0.5, -13.24),
        vertex_3d(5.5, -5.5, 4.76),
        vertex_3d(-5.5, -5.5, 4.76),
    };

    constexpr inline int pickup_car_crystal_front_color = 1;
    constexpr inline int pickup_car_crystal_front_shading = 7;

    constexpr inline int pickup_car_crystal_side_color = 1;
    constexpr inline int pickup_car_crystal_side_shading = 4;

    constexpr inline int pickup_car_crystal_back_color = 1;
    constexpr inline int pickup_car_crystal_back_shading = 6;

    constexpr inline int pickup_car_chassis_top_1_color = 5;
    constexpr inline int pickup_car_chassis_top_1_shading = 7;

    constexpr inline int pickup_car_chassis_top_2_color = 5;
    constexpr inline int pickup_car_chassis_top_2_shading = 7;

    constexpr inline int pickup_car_chassis_top_3_color = 5;
    constexpr inline int pickup_car_chassis_top_3_shading = 5;

    constexpr inline int pickup_car_chassis_side_1_color = 5;
    constexpr inline int pickup_car_chassis_side_1_shading = 2;

    constexpr inline int pickup_car_chassis_side_2_color = 5;
    constexpr inline int pickup_car_chassis_side_2_shading = 0;

    constexpr inline int pickup_car_chassis_bottom_color = 6;
    constexpr inline int pickup_car_chassis_bottom_shading = 0;

    constexpr inline int pickup_car_light_front_color = 7;
    constexpr inline int pickup_car_light_front_shading = -1;

    constexpr inline int pickup_car_light_back_color = 0;
    constexpr inline int pickup_car_light_back_shading = -1;

    constexpr inline int pickup_car_ventilation_color = 6;
    constexpr inline int pickup_car_ventilation_shading = -1;

    constexpr inline int pickup_car_wheel_color = 6;
    constexpr inline int pickup_car_wheel_shading = 7;

    constexpr inline face_3d pickup_car_faces[] = {
        face_3d(pickup_car_vertices, vertex_3d(0.0, 0.4472135954999579, -0.8944271909999159), 0, 1, 2, 3, pickup_car_crystal_front_color, pickup_car_crystal_front_shading),
        face_3d(pickup_car_vertices, vertex_3d(-0.9701425001453319, 0.24253562503633297, 0.0), 2, 1, 4, 5, pickup_car_crystal_side_color, pickup_car_crystal_side_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.9701425001453319, 0.24253562503633297, 0.0), 3, 6, 7, 0, pickup_car_crystal_side_color, pickup_car_crystal_side_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.0, 0.3511234415883917, 0.9363291775690444), 4, 7, 6, 5, pickup_car_crystal_back_color, pickup_car_crystal_back_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.0, 1.0, 0.0), 2, 5, 6, 3, pickup_car_chassis_top_1_color, pickup_car_chassis_top_1_shading),
        face_3d(pickup_car_vertices, vertex_3d(-0.0, 1.0, 0.0), 8, 7, 9, 10, pickup_car_chassis_top_1_color, pickup_car_chassis_top_1_shading),
        face_3d(pickup_car_vertices, vertex_3d(-0.0, 1.0, 0.0), 8, 10, 11, 12, pickup_car_chassis_top_1_color, pickup_car_chassis_top_1_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.0, 1.0, 0.0), 11, 13, 4, 12, pickup_car_chassis_top_1_color, pickup_car_chassis_top_1_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.0, 0.9922778767136677, -0.12403473458920847), 0, 14, 15, 1, pickup_car_chassis_top_2_color, pickup_car_chassis_top_2_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.0, 1.0, 0.0), 16, 17, 18, 19, pickup_car_chassis_top_3_color, pickup_car_chassis_top_3_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.0, -0.0, -1.0), 20, 21, 15, 14, pickup_car_chassis_side_1_color, pickup_car_chassis_side_1_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.0, 0.0, -1.0), 22, 23, 24, 25, pickup_car_chassis_side_1_color, pickup_car_chassis_side_1_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.0, 0.0, -1.0), 26, 27, 28, 29, pickup_car_chassis_side_1_color, pickup_car_chassis_side_1_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.0, 0.0, -1.0), 28, 27, 25, 24, pickup_car_chassis_side_1_color, pickup_car_chassis_side_1_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.0, 0.0, 1.0), 30, 31, 8, 12, pickup_car_chassis_side_1_color, pickup_car_chassis_side_1_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.0, 0.0, 1.0), 32, 33, 34, 35, pickup_car_chassis_side_1_color, pickup_car_chassis_side_1_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.0, 0.0, 1.0), 36, 37, 38, 39, pickup_car_chassis_side_1_color, pickup_car_chassis_side_1_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.0, -0.0, 1.0), 9, 13, 19, 18, pickup_car_chassis_side_1_color, pickup_car_chassis_side_1_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.0, 0.0, -1.0), 10, 17, 16, 11, pickup_car_chassis_side_1_color, pickup_car_chassis_side_1_shading),
        face_3d(pickup_car_vertices, vertex_3d(1.0, 0.0, -0.0), 14, 0, 40, 41, pickup_car_chassis_side_2_color, pickup_car_chassis_side_2_shading),
        face_3d(pickup_car_vertices, vertex_3d(1.0, 0.0, 0.0), 41, 42, 27, 14, pickup_car_chassis_side_2_color, pickup_car_chassis_side_2_shading),
        face_3d(pickup_car_vertices, vertex_3d(1.0, 0.0, -0.0), 8, 37, 43, 44, pickup_car_chassis_side_2_color, pickup_car_chassis_side_2_shading),
        face_3d(pickup_car_vertices, vertex_3d(1.0, 0.0, 0.0), 40, 45, 46, 47, pickup_car_chassis_side_2_color, pickup_car_chassis_side_2_shading),
        face_3d(pickup_car_vertices, vertex_3d(1.0, 0.0, -0.0), 40, 0, 8, 44, pickup_car_chassis_side_2_color, pickup_car_chassis_side_2_shading),
        face_3d(pickup_car_vertices, vertex_3d(-1.0, 0.0, -0.0), 15, 25, 48, 49, pickup_car_chassis_side_2_color, pickup_car_chassis_side_2_shading),
        face_3d(pickup_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 15, 49, 50, 1, pickup_car_chassis_side_2_color, pickup_car_chassis_side_2_shading),
        face_3d(pickup_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 12, 51, 52, 36, pickup_car_chassis_side_2_color, pickup_car_chassis_side_2_shading),
        face_3d(pickup_car_vertices, vertex_3d(-1.0, 0.0, -0.0), 50, 51, 12, 1, pickup_car_chassis_side_2_color, pickup_car_chassis_side_2_shading),
        face_3d(pickup_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 53, 50, 54, 55, pickup_car_chassis_side_2_color, pickup_car_chassis_side_2_shading),
        face_3d(pickup_car_vertices, vertex_3d(-1.0, 0.0, -0.0), 9, 18, 17, 10, pickup_car_chassis_side_2_color, pickup_car_chassis_side_2_shading),
        face_3d(pickup_car_vertices, vertex_3d(1.0, 0.0, 0.0), 16, 19, 13, 11, pickup_car_chassis_side_2_color, pickup_car_chassis_side_2_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.0, -1.0, 0.0), 37, 36, 59, 58, pickup_car_chassis_bottom_color, pickup_car_chassis_bottom_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.0, -1.0, 0.0), 25, 27, 58, 59, pickup_car_chassis_bottom_color, pickup_car_chassis_bottom_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.0, -0.0, -1.0), 23, 22, 21, 56, pickup_car_light_front_color, pickup_car_light_front_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.0, 0.0, -1.0), 29, 57, 20, 26, pickup_car_light_front_color, pickup_car_light_front_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.0, 0.0, 1.0), 38, 31, 33, 32, pickup_car_light_back_color, pickup_car_light_back_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.0, -0.0, 1.0), 34, 30, 39, 35, pickup_car_light_back_color, pickup_car_light_back_shading),
        face_3d(pickup_car_vertices, vertex_3d(0.0, 0.0, -1.0), 56, 57, 28, 24, pickup_car_ventilation_color, pickup_car_ventilation_shading),
        face_3d(pickup_car_vertices, vertex_3d(1.0, 0.0, 0.0), 47, 42, 41, 40, pickup_car_wheel_color, pickup_car_wheel_shading),
        face_3d(pickup_car_vertices, vertex_3d(1.0, 0.0, 0.0), 46, 45, 44, 43, pickup_car_wheel_color, pickup_car_wheel_shading),
        face_3d(pickup_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 55, 52, 51, 53, pickup_car_wheel_color, pickup_car_wheel_shading),
        face_3d(pickup_car_vertices, vertex_3d(-1.0, 0.0, -0.0), 49, 48, 54, 50, pickup_car_wheel_color, pickup_car_wheel_shading),
    };

    constexpr inline model_3d_item pickup_car(pickup_car_vertices, pickup_car_faces);
}

#endif

