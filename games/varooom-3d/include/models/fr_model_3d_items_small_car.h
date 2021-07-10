/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_SMALL_CAR_H
#define FR_MODEL_3D_ITEMS_SMALL_CAR_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d small_car_vertices[] = {
        vertex_3d(-3.5, 3.0, 0.37),
        vertex_3d(-3.5, 3.0, 7.37),
        vertex_3d(3.5, 3.0, 7.37),
        vertex_3d(3.5, 3.0, 0.37),
        vertex_3d(-5.0, -1.0, -7.63),
        vertex_3d(5.0, -1.0, -7.63),
        vertex_3d(5.0, -4.0, -7.63),
        vertex_3d(-5.0, -4.0, -7.63),
        vertex_3d(-3.5, 3.0, 4.87),
        vertex_3d(-5.0, 0.0, -4.63),
        vertex_3d(-5.0, 0.0, 5.37),
        vertex_3d(5.0, 0.0, -4.63),
        vertex_3d(5.0, -4.0, 9.37),
        vertex_3d(5.0, 0.0, 9.37),
        vertex_3d(-5.0, 0.0, 9.37),
        vertex_3d(-5.0, -4.0, 9.37),
        vertex_3d(3.5, 3.0, 4.87),
        vertex_3d(5.0, 0.0, 5.37),
        vertex_3d(5.0, -4.0, 8.37),
        vertex_3d(5.0, -4.0, 5.87),
        vertex_3d(5.0, -1.5, 8.37),
        vertex_3d(5.0, -1.5, 5.87),
        vertex_3d(5.0, -4.0, -3.38),
        vertex_3d(5.0, -4.0, -5.88),
        vertex_3d(5.0, -1.5, -3.38),
        vertex_3d(5.0, -1.5, -5.88),
        vertex_3d(-5.0, -4.0, 8.37),
        vertex_3d(-5.0, -4.0, 5.87),
        vertex_3d(-5.0, -1.5, 8.37),
        vertex_3d(-5.0, -1.5, 5.87),
        vertex_3d(-5.0, -4.0, -3.38),
        vertex_3d(-5.0, -4.0, -5.88),
        vertex_3d(-5.0, -1.5, -3.38),
        vertex_3d(-5.0, -1.5, -5.88),
        vertex_3d(2.5, -4.0, -7.63),
        vertex_3d(-2.5, -4.0, -7.63),
        vertex_3d(2.5, -2.0, -7.63),
        vertex_3d(-2.5, -2.0, -7.63),
        vertex_3d(4.5, -1.5, -7.63),
        vertex_3d(4.5, -2.0, -7.63),
        vertex_3d(3.0, -1.5, -7.63),
        vertex_3d(3.0, -2.0, -7.63),
        vertex_3d(-3.0, -1.5, -7.63),
        vertex_3d(-3.0, -2.0, -7.63),
        vertex_3d(-4.5, -1.5, -7.63),
        vertex_3d(-4.5, -2.0, -7.63),
        vertex_3d(-5.0, -0.5, 9.37),
        vertex_3d(-5.0, -1.5, 9.37),
        vertex_3d(-3.0, -0.5, 9.37),
        vertex_3d(-3.0, -1.5, 9.37),
        vertex_3d(3.0, -0.5, 9.37),
        vertex_3d(3.0, -1.5, 9.37),
        vertex_3d(5.0, -0.5, 9.37),
        vertex_3d(5.0, -1.5, 9.37),
    };

    constexpr inline int small_car_crystal_front_color = 1;
    constexpr inline int small_car_crystal_front_shading = 7;

    constexpr inline int small_car_crystal_side_color = 1;
    constexpr inline int small_car_crystal_side_shading = -1;

    constexpr inline int small_car_crystal_back_color = 1;
    constexpr inline int small_car_crystal_back_shading = 5;

    constexpr inline int small_car_chassis_top_color = 7;
    constexpr inline int small_car_chassis_top_shading = 7;

    constexpr inline int small_car_chassis_front_color = 7;
    constexpr inline int small_car_chassis_front_shading = 6;

    constexpr inline int small_car_chassis_side_1_color = 7;
    constexpr inline int small_car_chassis_side_1_shading = 2;

    constexpr inline int small_car_chassis_side_2_color = 7;
    constexpr inline int small_car_chassis_side_2_shading = 3;

    constexpr inline int small_car_chassis_bottom_color = 6;
    constexpr inline int small_car_chassis_bottom_shading = 0;

    constexpr inline int small_car_wheel_color = 6;
    constexpr inline int small_car_wheel_shading = 7;

    constexpr inline int small_car_ventilation_color = 6;
    constexpr inline int small_car_ventilation_shading = -1;

    constexpr inline int small_car_front_lights_color = 2;
    constexpr inline int small_car_front_lights_shading = -1;

    constexpr inline int small_car_back_lights_color = 0;
    constexpr inline int small_car_back_lights_shading = 0;

    constexpr inline face_3d small_car_faces[] = {
        face_3d(small_car_vertices, vertex_3d(0.0, 0.8574929257125442, -0.5144957554275266), 3, 11, 9, 0, small_car_crystal_front_color, small_car_crystal_front_shading),
        face_3d(small_car_vertices, vertex_3d(-0.8944271909999159, 0.4472135954999579, 0.0), 8, 0, 9, 10, small_car_crystal_side_color, small_car_crystal_side_shading),
        face_3d(small_car_vertices, vertex_3d(0.894427190999916, 0.447213595499958, 0.0), 17, 11, 3, 16, small_car_crystal_side_color, small_car_crystal_side_shading),
        face_3d(small_car_vertices, vertex_3d(0.0, 0.5547001962252289, 0.8320502943378437), 1, 14, 13, 2, small_car_crystal_back_color, small_car_crystal_back_shading),
        face_3d(small_car_vertices, vertex_3d(0.0, 1.0, 0.0), 0, 1, 2, 3, small_car_chassis_top_color, small_car_chassis_top_shading),
        face_3d(small_car_vertices, vertex_3d(0.0, 0.9486832980505138, -0.31622776601683794), 11, 5, 4, 9, small_car_chassis_front_color, small_car_chassis_front_shading),
        face_3d(small_car_vertices, vertex_3d(1.0, 0.0, 0.0), 13, 12, 18, 20, small_car_chassis_side_1_color, small_car_chassis_side_1_shading),
        face_3d(small_car_vertices, vertex_3d(1.0, 0.0, 0.0), 21, 17, 13, 20, small_car_chassis_side_1_color, small_car_chassis_side_1_shading),
        face_3d(small_car_vertices, vertex_3d(1.0, -0.0, 0.0), 5, 25, 23, 6, small_car_chassis_side_1_color, small_car_chassis_side_1_shading),
        face_3d(small_car_vertices, vertex_3d(1.0, 0.0, 0.0), 11, 24, 25, 5, small_car_chassis_side_1_color, small_car_chassis_side_1_shading),
        face_3d(small_car_vertices, vertex_3d(1.0, 0.0, 0.0), 11, 17, 21, 24, small_car_chassis_side_1_color, small_car_chassis_side_1_shading),
        face_3d(small_car_vertices, vertex_3d(1.0, 0.0, 0.0), 19, 22, 24, 21, small_car_chassis_side_1_color, small_car_chassis_side_1_shading),
        face_3d(small_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 15, 14, 28, 26, small_car_chassis_side_1_color, small_car_chassis_side_1_shading),
        face_3d(small_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 14, 10, 29, 28, small_car_chassis_side_1_color, small_car_chassis_side_1_shading),
        face_3d(small_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 4, 7, 31, 33, small_car_chassis_side_1_color, small_car_chassis_side_1_shading),
        face_3d(small_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 9, 4, 33, 32, small_car_chassis_side_1_color, small_car_chassis_side_1_shading),
        face_3d(small_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 9, 32, 29, 10, small_car_chassis_side_1_color, small_car_chassis_side_1_shading),
        face_3d(small_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 27, 29, 32, 30, small_car_chassis_side_1_color, small_car_chassis_side_1_shading),
        face_3d(small_car_vertices, vertex_3d(0.894427190999916, 0.447213595499958, 0.0), 16, 2, 13, 17, small_car_chassis_side_2_color, small_car_chassis_side_2_shading),
        face_3d(small_car_vertices, vertex_3d(-0.894427190999916, 0.447213595499958, 0.0), 1, 8, 10, 14, small_car_chassis_side_2_color, small_car_chassis_side_2_shading),
        face_3d(small_car_vertices, vertex_3d(0.0, 0.0, -1.0), 5, 6, 39, 38, small_car_chassis_side_2_color, small_car_chassis_side_2_shading),
        face_3d(small_car_vertices, vertex_3d(0.0, 0.0, -1.0), 36, 39, 6, 34, small_car_chassis_side_2_color, small_car_chassis_side_2_shading),
        face_3d(small_car_vertices, vertex_3d(-0.0, 0.0, -1.0), 7, 4, 44, 45, small_car_chassis_side_2_color, small_car_chassis_side_2_shading),
        face_3d(small_car_vertices, vertex_3d(0.0, 0.0, -1.0), 37, 35, 7, 45, small_car_chassis_side_2_color, small_car_chassis_side_2_shading),
        face_3d(small_car_vertices, vertex_3d(0.0, 0.0, -1.0), 4, 5, 38, 44, small_car_chassis_side_2_color, small_car_chassis_side_2_shading),
        face_3d(small_car_vertices, vertex_3d(0.0, 0.0, -1.0), 40, 41, 43, 42, small_car_chassis_side_2_color, small_car_chassis_side_2_shading),
        face_3d(small_car_vertices, vertex_3d(0.0, 0.0, 1.0), 13, 14, 46, 52, small_car_chassis_side_2_color, small_car_chassis_side_2_shading),
        face_3d(small_car_vertices, vertex_3d(0.0, 0.0, 1.0), 48, 49, 51, 50, small_car_chassis_side_2_color, small_car_chassis_side_2_shading),
        face_3d(small_car_vertices, vertex_3d(0.0, 0.0, 1.0), 15, 12, 53, 47, small_car_chassis_side_2_color, small_car_chassis_side_2_shading),
        face_3d(small_car_vertices, vertex_3d(0.0, -1.0, -0.0), 7, 6, 12, 15, small_car_chassis_bottom_color, small_car_chassis_bottom_shading),
        face_3d(small_car_vertices, vertex_3d(1.0, 0.0, 0.0), 19, 21, 20, 18, small_car_wheel_color, small_car_wheel_shading),
        face_3d(small_car_vertices, vertex_3d(1.0, 0.0, 0.0), 24, 22, 23, 25, small_car_wheel_color, small_car_wheel_shading),
        face_3d(small_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 28, 29, 27, 26, small_car_wheel_color, small_car_wheel_shading),
        face_3d(small_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 32, 33, 31, 30, small_car_wheel_color, small_car_wheel_shading),
        face_3d(small_car_vertices, vertex_3d(-0.0, 0.0, -1.0), 35, 37, 36, 34, small_car_ventilation_color, small_car_ventilation_shading),
        face_3d(small_car_vertices, vertex_3d(0.0, 0.0, -1.0), 39, 41, 40, 38, small_car_front_lights_color, small_car_front_lights_shading),
        face_3d(small_car_vertices, vertex_3d(0.0, 0.0, -1.0), 43, 45, 44, 42, small_car_front_lights_color, small_car_front_lights_shading),
        face_3d(small_car_vertices, vertex_3d(0.0, 0.0, 1.0), 47, 49, 48, 46, small_car_back_lights_color, small_car_back_lights_shading),
        face_3d(small_car_vertices, vertex_3d(0.0, 0.0, 1.0), 51, 53, 52, 50, small_car_back_lights_color, small_car_back_lights_shading),
    };

    constexpr inline model_3d_item small_car(small_car_vertices, small_car_faces);
}

#endif

