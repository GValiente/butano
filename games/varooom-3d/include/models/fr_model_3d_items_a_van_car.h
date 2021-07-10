/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_A_VAN_CAR_H
#define FR_MODEL_3D_ITEMS_A_VAN_CAR_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d a_van_car_vertices[] = {
        vertex_3d(4.0, 5.5, 7.5),
        vertex_3d(4.0, 5.5, -12.0),
        vertex_3d(-4.0, 5.5, -12.0),
        vertex_3d(-4.0, 5.5, 7.5),
        vertex_3d(-6.0, -0.5, 14.0),
        vertex_3d(6.0, -0.5, 14.0),
        vertex_3d(6.0, 1.0, 12.0),
        vertex_3d(-6.0, 1.0, 12.0),
        vertex_3d(6.0, 1.0, -14.0),
        vertex_3d(6.0, -6.0, -14.0),
        vertex_3d(5.0, -2.0, -14.0),
        vertex_3d(5.0, 0.0, -14.0),
        vertex_3d(-6.0, -6.0, -14.0),
        vertex_3d(-6.0, 1.0, -14.0),
        vertex_3d(-5.0, 0.0, -14.0),
        vertex_3d(-5.0, -2.0, -14.0),
        vertex_3d(3.0, -2.0, -14.0),
        vertex_3d(-3.0, -2.0, -14.0),
        vertex_3d(-3.0, 0.0, -14.0),
        vertex_3d(3.0, 0.0, -14.0),
        vertex_3d(-6.0, -6.0, 14.0),
        vertex_3d(6.0, -6.0, 14.0),
        vertex_3d(4.0, -3.0, 14.0),
        vertex_3d(-4.0, -3.0, 14.0),
        vertex_3d(-4.0, -1.0, 14.0),
        vertex_3d(4.0, -1.0, 14.0),
        vertex_3d(-2.0, -3.0, 14.0),
        vertex_3d(-3.0, -2.0, 14.0),
        vertex_3d(2.0, -1.0, 14.0),
        vertex_3d(2.0, -3.0, 14.0),
        vertex_3d(-2.0, -1.0, 14.0),
        vertex_3d(3.0, -2.0, 14.0),
        vertex_3d(-5.0, -2.0, 14.0),
        vertex_3d(5.0, -2.0, 14.0),
        vertex_3d(4.0, 5.5, 4.0),
        vertex_3d(6.0, 1.0, 4.0),
        vertex_3d(-6.0, 1.0, 4.0),
        vertex_3d(-4.0, 5.5, 4.0),
        vertex_3d(6.0, -2.0, -12.0),
        vertex_3d(6.0, -6.0, -12.0),
        vertex_3d(6.0, -6.0, 12.0),
        vertex_3d(6.0, -2.0, 12.0),
        vertex_3d(6.0, -2.0, 8.0),
        vertex_3d(6.0, -6.0, 8.0),
        vertex_3d(6.0, -6.0, -8.0),
        vertex_3d(6.0, -2.0, -8.0),
        vertex_3d(-6.0, -6.0, -12.0),
        vertex_3d(-6.0, -2.0, -12.0),
        vertex_3d(-6.0, -2.0, 12.0),
        vertex_3d(-6.0, -6.0, 12.0),
        vertex_3d(-6.0, -2.0, 8.0),
        vertex_3d(-6.0, -2.0, -8.0),
        vertex_3d(-6.0, -6.0, -8.0),
        vertex_3d(-6.0, -6.0, 8.0),
        vertex_3d(4.0, 7.0, -13.0),
        vertex_3d(-4.0, 7.0, -13.0),
        vertex_3d(4.0, 7.0, -10.0),
        vertex_3d(-4.0, 7.0, -10.0),
        vertex_3d(4.0, 5.5, -9.0),
        vertex_3d(-4.0, 5.5, -9.0),
    };

    constexpr inline int a_van_car_chassis_top_1_color = 6;
    constexpr inline int a_van_car_chassis_top_1_shading = 7;

    constexpr inline int a_van_car_chassis_top_2_color = 6;
    constexpr inline int a_van_car_chassis_top_2_shading = 6;

    constexpr inline int a_van_car_chassis_side_1_color = 6;
    constexpr inline int a_van_car_chassis_side_1_shading = 6;

    constexpr inline int a_van_car_chassis_side_2_color = 6;
    constexpr inline int a_van_car_chassis_side_2_shading = 0;

    constexpr inline int a_van_car_chassis_bottom_color = 6;
    constexpr inline int a_van_car_chassis_bottom_shading = 7;

    constexpr inline int a_van_car_crystal_front_color = 3;
    constexpr inline int a_van_car_crystal_front_shading = 3;

    constexpr inline int a_van_car_crystal_back_color = 3;
    constexpr inline int a_van_car_crystal_back_shading = 0;

    constexpr inline int a_van_car_crystal_side_color = 3;
    constexpr inline int a_van_car_crystal_side_shading = 1;

    constexpr inline int a_van_car_light_front_color = 7;
    constexpr inline int a_van_car_light_front_shading = -1;

    constexpr inline int a_van_car_light_back_color = 0;
    constexpr inline int a_van_car_light_back_shading = 0;

    constexpr inline int a_van_car_ventilation_color = 6;
    constexpr inline int a_van_car_ventilation_shading = 0;

    constexpr inline int a_van_car_wheel_color = 6;
    constexpr inline int a_van_car_wheel_shading = 7;

    constexpr inline int a_van_car_red_top_color = 0;
    constexpr inline int a_van_car_red_top_shading = 7;

    constexpr inline int a_van_car_red_side_1_color = 0;
    constexpr inline int a_van_car_red_side_1_shading = 4;

    constexpr inline int a_van_car_red_side_2_color = 0;
    constexpr inline int a_van_car_red_side_2_shading = 2;

    constexpr inline int a_van_car_red_bottom_color = 0;
    constexpr inline int a_van_car_red_bottom_shading = 0;

    constexpr inline face_3d a_van_car_faces[] = {
        face_3d(a_van_car_vertices, vertex_3d(0.0, 1.0, -0.0), 3, 0, 58, 59, a_van_car_chassis_top_1_color, a_van_car_chassis_top_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, 0.8, 0.6), 4, 5, 6, 7, a_van_car_chassis_top_2_color, a_van_car_chassis_top_2_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, 0.0, -1.0), 8, 9, 10, 11, a_van_car_chassis_side_1_color, a_van_car_chassis_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(-0.0, 0.0, -1.0), 12, 13, 14, 15, a_van_car_chassis_side_1_color, a_van_car_chassis_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, 0.0, -1.0), 13, 8, 11, 14, a_van_car_chassis_side_1_color, a_van_car_chassis_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, 0.0, -1.0), 10, 9, 12, 15, a_van_car_chassis_side_1_color, a_van_car_chassis_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, 0.0, -1.0), 16, 17, 18, 19, a_van_car_chassis_side_1_color, a_van_car_chassis_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 20, 21, 22, 23, a_van_car_chassis_side_1_color, a_van_car_chassis_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, -0.0, 1.0), 5, 4, 24, 25, a_van_car_chassis_side_1_color, a_van_car_chassis_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 26, 27, 23, a_van_car_chassis_side_1_color, a_van_car_chassis_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 28, 29, 25, a_van_car_chassis_side_1_color, a_van_car_chassis_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(-0.0, 0.0, 1.0), 26, 30, 24, a_van_car_chassis_side_1_color, a_van_car_chassis_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 31, 29, 22, a_van_car_chassis_side_1_color, a_van_car_chassis_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, -0.0, 1.0), 24, 4, 32, a_van_car_chassis_side_1_color, a_van_car_chassis_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 32, 20, 23, a_van_car_chassis_side_1_color, a_van_car_chassis_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 4, 20, 32, a_van_car_chassis_side_1_color, a_van_car_chassis_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 25, 33, 5, a_van_car_chassis_side_1_color, a_van_car_chassis_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 22, 21, 33, a_van_car_chassis_side_1_color, a_van_car_chassis_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, -0.0, 1.0), 5, 33, 21, a_van_car_chassis_side_1_color, a_van_car_chassis_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.9138115486202572, 0.4061384660534476, 0.0), 35, 58, 34, a_van_car_chassis_side_1_color, a_van_car_chassis_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.9138115486202572, 0.4061384660534476, 0.0), 8, 1, 35, a_van_car_chassis_side_1_color, a_van_car_chassis_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(-0.9138115486202572, 0.4061384660534476, 0.0), 37, 59, 36, a_van_car_chassis_side_1_color, a_van_car_chassis_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(-0.9138115486202572, 0.4061384660534476, 0.0), 13, 36, 2, a_van_car_chassis_side_1_color, a_van_car_chassis_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(1.0, 0.0, 0.0), 9, 8, 38, 39, a_van_car_chassis_side_2_color, a_van_car_chassis_side_2_shading),
        face_3d(a_van_car_vertices, vertex_3d(1.0, 0.0, 0.0), 21, 40, 41, 5, a_van_car_chassis_side_2_color, a_van_car_chassis_side_2_shading),
        face_3d(a_van_car_vertices, vertex_3d(1.0, 0.0, 0.0), 5, 41, 42, 6, a_van_car_chassis_side_2_color, a_van_car_chassis_side_2_shading),
        face_3d(a_van_car_vertices, vertex_3d(1.0, 0.0, 0.0), 38, 8, 6, 42, a_van_car_chassis_side_2_color, a_van_car_chassis_side_2_shading),
        face_3d(a_van_car_vertices, vertex_3d(1.0, 0.0, 0.0), 42, 43, 44, 45, a_van_car_chassis_side_2_color, a_van_car_chassis_side_2_shading),
        face_3d(a_van_car_vertices, vertex_3d(-1.0, -0.0, 0.0), 12, 46, 47, 13, a_van_car_chassis_side_2_color, a_van_car_chassis_side_2_shading),
        face_3d(a_van_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 20, 4, 48, 49, a_van_car_chassis_side_2_color, a_van_car_chassis_side_2_shading),
        face_3d(a_van_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 4, 7, 50, 48, a_van_car_chassis_side_2_color, a_van_car_chassis_side_2_shading),
        face_3d(a_van_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 47, 50, 7, 13, a_van_car_chassis_side_2_color, a_van_car_chassis_side_2_shading),
        face_3d(a_van_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 51, 52, 53, 50, a_van_car_chassis_side_2_color, a_van_car_chassis_side_2_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, -1.0, 0.0), 9, 21, 20, 12, a_van_car_chassis_bottom_color, a_van_car_chassis_bottom_shading),
        face_3d(a_van_car_vertices, vertex_3d(-0.0, 0.7071067811865476, 0.7071067811865476), 6, 0, 3, 7, a_van_car_crystal_front_color, a_van_car_crystal_front_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.9138115486202572, 0.4061384660534476, 0.0), 35, 34, 0, 6, a_van_car_crystal_side_color, a_van_car_crystal_side_shading),
        face_3d(a_van_car_vertices, vertex_3d(-0.9138115486202573, 0.40613846605344767, 0.0), 36, 7, 3, 37, a_van_car_crystal_side_color, a_van_car_crystal_side_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, 0.4061384660534476, -0.9138115486202572), 8, 13, 2, 1, a_van_car_crystal_back_color, a_van_car_crystal_back_shading),
        face_3d(a_van_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 52, 51, 47, 46, a_van_car_wheel_color, a_van_car_wheel_shading),
        face_3d(a_van_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 48, 50, 53, 49, a_van_car_wheel_color, a_van_car_wheel_shading),
        face_3d(a_van_car_vertices, vertex_3d(1.0, 0.0, 0.0), 41, 40, 43, 42, a_van_car_wheel_color, a_van_car_wheel_shading),
        face_3d(a_van_car_vertices, vertex_3d(1.0, 0.0, 0.0), 44, 39, 38, 45, a_van_car_wheel_color, a_van_car_wheel_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 27, 24, 32, 23, a_van_car_light_front_color, a_van_car_light_front_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 33, 25, 31, 22, a_van_car_light_front_color, a_van_car_light_front_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, 0.0, -1.0), 11, 10, 16, 19, a_van_car_light_back_color, a_van_car_light_back_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, 0.0, -1.0), 18, 17, 15, 14, a_van_car_light_back_color, a_van_car_light_back_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, 0.0, 1.0), 28, 30, 26, 29, a_van_car_ventilation_color, a_van_car_ventilation_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, 1.0, 0.0), 54, 55, 57, 56, a_van_car_red_top_color, a_van_car_red_top_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.9138115486202572, 0.4061384660534476, 0.0), 35, 1, 58, a_van_car_red_side_1_color, a_van_car_red_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(0.0, 0.5547001962252291, 0.8320502943378437), 59, 58, 56, 57, a_van_car_red_side_1_color, a_van_car_red_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(-0.9138115486202572, 0.4061384660534476, 0.0), 59, 2, 36, a_van_car_red_side_1_color, a_van_car_red_side_1_shading),
        face_3d(a_van_car_vertices, vertex_3d(1.0, 0.0, 0.0), 1, 54, 56, 58, a_van_car_red_side_2_color, a_van_car_red_side_2_shading),
        face_3d(a_van_car_vertices, vertex_3d(-1.0, -0.0, 0.0), 2, 59, 57, 55, a_van_car_red_side_2_color, a_van_car_red_side_2_shading),
        face_3d(a_van_car_vertices, vertex_3d(-0.0, -0.5547001962252291, -0.8320502943378437), 2, 55, 54, 1, a_van_car_red_bottom_color, a_van_car_red_bottom_shading),
    };

    constexpr inline model_3d_item a_van_car(a_van_car_vertices, a_van_car_faces);
}

#endif

