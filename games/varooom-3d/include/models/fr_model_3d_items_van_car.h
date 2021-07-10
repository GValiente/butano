/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_VAN_CAR_H
#define FR_MODEL_3D_ITEMS_VAN_CAR_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d van_car_vertices[] = {
        vertex_3d(4.5, 5.5, 7.5),
        vertex_3d(4.5, 5.5, -12.5),
        vertex_3d(-4.5, 5.5, -12.5),
        vertex_3d(-4.5, 5.5, 7.5),
        vertex_3d(6.0, 1.0, 5.0),
        vertex_3d(4.5, 5.5, 5.0),
        vertex_3d(6.0, 1.0, 12.0),
        vertex_3d(-6.0, 1.0, 12.0),
        vertex_3d(-6.0, 1.0, 5.0),
        vertex_3d(-4.5, 5.5, 5.0),
        vertex_3d(6.0, 1.0, -14.0),
        vertex_3d(-6.0, 1.0, -14.0),
        vertex_3d(5.0, 0.0, -14.0),
        vertex_3d(5.0, -2.0, -14.0),
        vertex_3d(3.0, -2.0, -14.0),
        vertex_3d(3.0, 0.0, -14.0),
        vertex_3d(-3.0, 0.0, -14.0),
        vertex_3d(-3.0, -2.0, -14.0),
        vertex_3d(-5.0, -2.0, -14.0),
        vertex_3d(-5.0, 0.0, -14.0),
        vertex_3d(6.0, -6.0, -14.0),
        vertex_3d(-6.0, -6.0, -14.0),
        vertex_3d(-6.0, -0.5, 14.0),
        vertex_3d(6.0, -0.5, 14.0),
        vertex_3d(2.0, -1.0, 14.0),
        vertex_3d(-2.0, -1.0, 14.0),
        vertex_3d(-2.0, -3.0, 14.0),
        vertex_3d(2.0, -3.0, 14.0),
        vertex_3d(-3.0, -2.0, 14.0),
        vertex_3d(-4.0, -1.0, 14.0),
        vertex_3d(-5.0, -2.0, 14.0),
        vertex_3d(-4.0, -3.0, 14.0),
        vertex_3d(5.0, -2.0, 14.0),
        vertex_3d(4.0, -1.0, 14.0),
        vertex_3d(3.0, -2.0, 14.0),
        vertex_3d(4.0, -3.0, 14.0),
        vertex_3d(-6.0, -6.0, 14.0),
        vertex_3d(6.0, -6.0, 14.0),
        vertex_3d(-6.0, -6.0, -8.0),
        vertex_3d(-6.0, -2.0, -8.0),
        vertex_3d(-6.0, -2.0, -12.0),
        vertex_3d(-6.0, -6.0, -12.0),
        vertex_3d(-6.0, -2.0, 12.0),
        vertex_3d(-6.0, -2.0, 8.0),
        vertex_3d(-6.0, -6.0, 8.0),
        vertex_3d(-6.0, -6.0, 12.0),
        vertex_3d(6.0, -2.0, 12.0),
        vertex_3d(6.0, -6.0, 12.0),
        vertex_3d(6.0, -6.0, 8.0),
        vertex_3d(6.0, -2.0, 8.0),
        vertex_3d(6.0, -6.0, -8.0),
        vertex_3d(6.0, -6.0, -12.0),
        vertex_3d(6.0, -2.0, -12.0),
        vertex_3d(6.0, -2.0, -8.0),
    };

    constexpr inline int van_car_chassis_top_1_color = 2;
    constexpr inline int van_car_chassis_top_1_shading = 4;

    constexpr inline int van_car_chassis_top_2_color = 2;
    constexpr inline int van_car_chassis_top_2_shading = 2;

    constexpr inline int van_car_chassis_side_1_color = 2;
    constexpr inline int van_car_chassis_side_1_shading = 1;

    constexpr inline int van_car_chassis_side_2_color = 2;
    constexpr inline int van_car_chassis_side_2_shading = 0;

    constexpr inline int van_car_chassis_bottom_color = 6;
    constexpr inline int van_car_chassis_bottom_shading = 0;

    constexpr inline int van_car_crystal_front_color = 1;
    constexpr inline int van_car_crystal_front_shading = 7;

    constexpr inline int van_car_crystal_back_color = 1;
    constexpr inline int van_car_crystal_back_shading = 6;

    constexpr inline int van_car_crystal_side_color = 1;
    constexpr inline int van_car_crystal_side_shading = -1;

    constexpr inline int van_car_light_front_color = 7;
    constexpr inline int van_car_light_front_shading = -1;

    constexpr inline int van_car_light_back_color = 0;
    constexpr inline int van_car_light_back_shading = 0;

    constexpr inline int van_car_ventilation_color = 6;
    constexpr inline int van_car_ventilation_shading = -1;

    constexpr inline int van_car_wheel_color = 6;
    constexpr inline int van_car_wheel_shading = 7;

    constexpr inline face_3d van_car_faces[] = {
        face_3d(van_car_vertices, vertex_3d(0.0, 1.0, 0.0), 0, 1, 2, 3, van_car_chassis_top_1_color, van_car_chassis_top_1_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, 0.8, 0.6), 22, 23, 6, 7, van_car_chassis_top_2_color, van_car_chassis_top_2_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, 0.0, -1.0), 10, 20, 13, 12, van_car_chassis_side_1_color, van_car_chassis_side_1_shading),
        face_3d(van_car_vertices, vertex_3d(-0.0, 0.0, -1.0), 21, 11, 19, 18, van_car_chassis_side_1_color, van_car_chassis_side_1_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, 0.0, -1.0), 11, 10, 12, 19, van_car_chassis_side_1_color, van_car_chassis_side_1_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, 0.0, -1.0), 13, 20, 21, 18, van_car_chassis_side_1_color, van_car_chassis_side_1_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, 0.0, -1.0), 14, 17, 16, 15, van_car_chassis_side_1_color, van_car_chassis_side_1_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 36, 37, 35, 31, van_car_chassis_side_1_color, van_car_chassis_side_1_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, -0.0, 1.0), 23, 22, 29, 33, van_car_chassis_side_1_color, van_car_chassis_side_1_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 26, 28, 31, van_car_chassis_side_1_color, van_car_chassis_side_1_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 24, 27, 33, van_car_chassis_side_1_color, van_car_chassis_side_1_shading),
        face_3d(van_car_vertices, vertex_3d(-0.0, 0.0, 1.0), 26, 25, 29, van_car_chassis_side_1_color, van_car_chassis_side_1_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 34, 27, 35, van_car_chassis_side_1_color, van_car_chassis_side_1_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, -0.0, 1.0), 29, 22, 30, van_car_chassis_side_1_color, van_car_chassis_side_1_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 30, 36, 31, van_car_chassis_side_1_color, van_car_chassis_side_1_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 22, 36, 30, van_car_chassis_side_1_color, van_car_chassis_side_1_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 33, 32, 23, van_car_chassis_side_1_color, van_car_chassis_side_1_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 35, 37, 32, van_car_chassis_side_1_color, van_car_chassis_side_1_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, -0.0, 1.0), 23, 32, 37, van_car_chassis_side_1_color, van_car_chassis_side_1_shading),
        face_3d(van_car_vertices, vertex_3d(0.9486832980505138, 0.3162277660168379, -0.0), 5, 4, 10, 1, van_car_chassis_side_1_color, van_car_chassis_side_1_shading),
        face_3d(van_car_vertices, vertex_3d(-0.9486832980505138, 0.31622776601683794, 0.0), 8, 9, 2, 11, van_car_chassis_side_1_color, van_car_chassis_side_1_shading),
        face_3d(van_car_vertices, vertex_3d(1.0, 0.0, 0.0), 20, 10, 52, 51, van_car_chassis_side_2_color, van_car_chassis_side_2_shading),
        face_3d(van_car_vertices, vertex_3d(1.0, 0.0, 0.0), 37, 47, 46, 23, van_car_chassis_side_2_color, van_car_chassis_side_2_shading),
        face_3d(van_car_vertices, vertex_3d(1.0, 0.0, 0.0), 23, 46, 49, 6, van_car_chassis_side_2_color, van_car_chassis_side_2_shading),
        face_3d(van_car_vertices, vertex_3d(1.0, 0.0, 0.0), 52, 10, 6, 49, van_car_chassis_side_2_color, van_car_chassis_side_2_shading),
        face_3d(van_car_vertices, vertex_3d(1.0, 0.0, 0.0), 49, 48, 50, 53, van_car_chassis_side_2_color, van_car_chassis_side_2_shading),
        face_3d(van_car_vertices, vertex_3d(-1.0, -0.0, 0.0), 21, 41, 40, 11, van_car_chassis_side_2_color, van_car_chassis_side_2_shading),
        face_3d(van_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 36, 22, 42, 45, van_car_chassis_side_2_color, van_car_chassis_side_2_shading),
        face_3d(van_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 22, 7, 43, 42, van_car_chassis_side_2_color, van_car_chassis_side_2_shading),
        face_3d(van_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 40, 43, 7, 11, van_car_chassis_side_2_color, van_car_chassis_side_2_shading),
        face_3d(van_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 39, 38, 44, 43, van_car_chassis_side_2_color, van_car_chassis_side_2_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, -1.0, 0.0), 20, 37, 36, 21, van_car_chassis_bottom_color, van_car_chassis_bottom_shading),
        face_3d(van_car_vertices, vertex_3d(-0.0, 0.7071067811865475, 0.7071067811865475), 6, 0, 3, 7, van_car_crystal_front_color, van_car_crystal_front_shading),
        face_3d(van_car_vertices, vertex_3d(0.9486832980505139, 0.31622776601683794, 0.0), 4, 5, 0, 6, van_car_crystal_side_color, van_car_crystal_side_shading),
        face_3d(van_car_vertices, vertex_3d(-0.9486832980505139, 0.31622776601683794, 0.0), 8, 7, 3, 9, van_car_crystal_side_color, van_car_crystal_side_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, 0.31622776601683794, -0.9486832980505138), 10, 11, 2, 1, van_car_crystal_back_color, van_car_crystal_back_shading),
        face_3d(van_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 38, 39, 40, 41, van_car_wheel_color, van_car_wheel_shading),
        face_3d(van_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 42, 43, 44, 45, van_car_wheel_color, van_car_wheel_shading),
        face_3d(van_car_vertices, vertex_3d(1.0, 0.0, 0.0), 46, 47, 48, 49, van_car_wheel_color, van_car_wheel_shading),
        face_3d(van_car_vertices, vertex_3d(1.0, 0.0, 0.0), 50, 51, 52, 53, van_car_wheel_color, van_car_wheel_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 28, 29, 30, 31, van_car_light_front_color, van_car_light_front_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 32, 33, 34, 35, van_car_light_front_color, van_car_light_front_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, 0.0, -1.0), 12, 13, 14, 15, van_car_light_back_color, van_car_light_back_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, 0.0, -1.0), 16, 17, 18, 19, van_car_light_back_color, van_car_light_back_shading),
        face_3d(van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 24, 25, 26, 27, van_car_ventilation_color, van_car_ventilation_shading),
    };

    constexpr inline model_3d_item van_car(van_car_vertices, van_car_faces);
}

#endif

