/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_SIMPLE_CAR_H
#define FR_MODEL_3D_ITEMS_SIMPLE_CAR_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d simple_car_vertices[] = {
        vertex_3d(4.5, 4.0, 6.0),
        vertex_3d(4.5, 4.0, 2.0),
        vertex_3d(-4.5, 4.0, 2.0),
        vertex_3d(-4.5, 4.0, 6.0),
        vertex_3d(-6.0, 1.0, 12.0),
        vertex_3d(6.0, 1.0, 12.0),
        vertex_3d(-6.0, 1.0, -4.0),
        vertex_3d(6.0, 1.0, -4.0),
        vertex_3d(6.0, 0.0, -13.0),
        vertex_3d(-6.0, 0.0, -13.0),
        vertex_3d(-6.0, -5.0, -13.0),
        vertex_3d(-5.0, -0.5, -13.0),
        vertex_3d(-5.0, -1.5, -13.0),
        vertex_3d(6.0, -5.0, -13.0),
        vertex_3d(5.0, -1.5, -13.0),
        vertex_3d(5.0, -0.5, -13.0),
        vertex_3d(6.0, -5.0, 12.0),
        vertex_3d(5.0, -1.0, 12.0),
        vertex_3d(-5.0, -1.0, 12.0),
        vertex_3d(-6.0, -5.0, 12.0),
        vertex_3d(5.0, 0.0, 12.0),
        vertex_3d(1.0, 0.0, 12.0),
        vertex_3d(-1.0, 0.0, 12.0),
        vertex_3d(-1.0, -1.0, 12.0),
        vertex_3d(1.0, -1.0, 12.0),
        vertex_3d(-5.0, 0.0, 12.0),
        vertex_3d(6.0, -5.0, 10.0),
        vertex_3d(6.0, -1.0, 10.0),
        vertex_3d(6.0, -1.0, -6.0),
        vertex_3d(6.0, -1.0, -10.0),
        vertex_3d(6.0, -5.0, -10.0),
        vertex_3d(6.0, -1.0, 6.0),
        vertex_3d(6.0, -5.0, 6.0),
        vertex_3d(6.0, -5.0, -6.0),
        vertex_3d(-6.0, -1.0, 6.0),
        vertex_3d(-6.0, -1.0, -6.0),
        vertex_3d(-6.0, -5.0, -6.0),
        vertex_3d(-6.0, -5.0, 6.0),
        vertex_3d(-6.0, -5.0, -10.0),
        vertex_3d(-6.0, -1.0, -10.0),
        vertex_3d(-6.0, -1.0, 10.0),
        vertex_3d(-6.0, -5.0, 10.0),
        vertex_3d(2.5, -0.5, -13.0),
        vertex_3d(2.5, -1.5, -13.0),
        vertex_3d(-2.5, -0.5, -13.0),
        vertex_3d(-2.5, -1.5, -13.0),
    };

    constexpr inline int simple_car_red_top_a_color = 4;
    constexpr inline int simple_car_red_top_a_shading = 4;

    constexpr inline int simple_car_red_top_b_color = 4;
    constexpr inline int simple_car_red_top_b_shading = 2;

    constexpr inline int simple_car_red_side_a_color = 4;
    constexpr inline int simple_car_red_side_a_shading = 1;

    constexpr inline int simple_car_red_side_b_color = 4;
    constexpr inline int simple_car_red_side_b_shading = 0;

    constexpr inline int simple_car_red_bottom_color = 6;
    constexpr inline int simple_car_red_bottom_shading = 0;

    constexpr inline int simple_car_glass_front_color = 1;
    constexpr inline int simple_car_glass_front_shading = 7;

    constexpr inline int simple_car_glass_back_color = 1;
    constexpr inline int simple_car_glass_back_shading = 6;

    constexpr inline int simple_car_glass_sides_color = 1;
    constexpr inline int simple_car_glass_sides_shading = -1;

    constexpr inline int simple_car_front_light_color = 7;
    constexpr inline int simple_car_front_light_shading = -1;

    constexpr inline int simple_car_back_light_color = 0;
    constexpr inline int simple_car_back_light_shading = 0;

    constexpr inline int simple_car_ventilation_color = 6;
    constexpr inline int simple_car_ventilation_shading = -1;

    constexpr inline int simple_car_wheel_color = 6;
    constexpr inline int simple_car_wheel_shading = 7;

    constexpr inline face_3d simple_car_faces[] = {
        face_3d(simple_car_vertices, vertex_3d(0.0, 1.0, 0.0), 0, 1, 2, 3, simple_car_red_top_a_color, simple_car_red_top_a_shading),
        face_3d(simple_car_vertices, vertex_3d(0.0, 0.9938837346736189, -0.11043152607484655), 6, 7, 8, 9, simple_car_red_top_b_color, simple_car_red_top_b_shading),
        face_3d(simple_car_vertices, vertex_3d(-0.0, 0.0, -1.0), 10, 9, 11, 12, simple_car_red_side_a_color, simple_car_red_side_a_shading),
        face_3d(simple_car_vertices, vertex_3d(0.0, 0.0, -1.0), 13, 14, 15, 8, simple_car_red_side_a_color, simple_car_red_side_a_shading),
        face_3d(simple_car_vertices, vertex_3d(0.0, 0.0, -1.0), 9, 8, 15, 11, simple_car_red_side_a_color, simple_car_red_side_a_shading),
        face_3d(simple_car_vertices, vertex_3d(0.0, 0.0, -1.0), 14, 13, 10, 12, simple_car_red_side_a_color, simple_car_red_side_a_shading),
        face_3d(simple_car_vertices, vertex_3d(-0.0, 0.0, 1.0), 16, 17, 18, 19, simple_car_red_side_a_color, simple_car_red_side_a_shading),
        face_3d(simple_car_vertices, vertex_3d(0.0, 0.0, 1.0), 20, 17, 16, 5, simple_car_red_side_a_color, simple_car_red_side_a_shading),
        face_3d(simple_car_vertices, vertex_3d(0.0, 0.0, 1.0), 21, 22, 23, 24, simple_car_red_side_a_color, simple_car_red_side_a_shading),
        face_3d(simple_car_vertices, vertex_3d(0.0, 0.0, 1.0), 18, 25, 4, 19, simple_car_red_side_a_color, simple_car_red_side_a_shading),
        face_3d(simple_car_vertices, vertex_3d(0.0, -0.0, 1.0), 5, 4, 25, 20, simple_car_red_side_a_color, simple_car_red_side_a_shading),
        face_3d(simple_car_vertices, vertex_3d(1.0, 0.0, 0.0), 16, 26, 27, 5, simple_car_red_side_b_color, simple_car_red_side_b_shading),
        face_3d(simple_car_vertices, vertex_3d(1.0, 0.0, 0.0), 27, 28, 7, 5, simple_car_red_side_b_color, simple_car_red_side_b_shading),
        face_3d(simple_car_vertices, vertex_3d(1.0, 0.0, 0.0), 13, 8, 29, 30, simple_car_red_side_b_color, simple_car_red_side_b_shading),
        face_3d(simple_car_vertices, vertex_3d(1.0, -0.0, 0.0), 8, 7, 28, 29, simple_car_red_side_b_color, simple_car_red_side_b_shading),
        face_3d(simple_car_vertices, vertex_3d(1.0, 0.0, 0.0), 31, 32, 33, 28, simple_car_red_side_b_color, simple_car_red_side_b_shading),
        face_3d(simple_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 34, 35, 36, 37, simple_car_red_side_b_color, simple_car_red_side_b_shading),
        face_3d(simple_car_vertices, vertex_3d(-1.0, -0.0, 0.0), 10, 38, 39, 9, simple_car_red_side_b_color, simple_car_red_side_b_shading),
        face_3d(simple_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 19, 4, 40, 41, simple_car_red_side_b_color, simple_car_red_side_b_shading),
        face_3d(simple_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 9, 39, 35, 6, simple_car_red_side_b_color, simple_car_red_side_b_shading),
        face_3d(simple_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 35, 40, 4, 6, simple_car_red_side_b_color, simple_car_red_side_b_shading),
        face_3d(simple_car_vertices, vertex_3d(0.0, -1.0, 0.0), 13, 16, 19, 10, simple_car_red_bottom_color, simple_car_red_bottom_shading),
        face_3d(simple_car_vertices, vertex_3d(0.0, 0.8944271909999159, -0.4472135954999579), 1, 7, 6, 2, simple_car_glass_front_color, simple_car_glass_front_shading),
        face_3d(simple_car_vertices, vertex_3d(0.0, 0.8944271909999159, 0.4472135954999579), 0, 3, 4, 5, simple_car_glass_back_color, simple_car_glass_back_shading),
        face_3d(simple_car_vertices, vertex_3d(-0.8944271909999159, 0.4472135954999579, 0.0), 2, 6, 4, 3, simple_car_glass_sides_color, simple_car_glass_sides_shading),
        face_3d(simple_car_vertices, vertex_3d(0.8944271909999159, 0.4472135954999579, 0.0), 1, 0, 5, 7, simple_car_glass_sides_color, simple_car_glass_sides_shading),
        face_3d(simple_car_vertices, vertex_3d(0.0, 0.0, -1.0), 42, 15, 14, 43, simple_car_front_light_color, simple_car_front_light_shading),
        face_3d(simple_car_vertices, vertex_3d(0.0, 0.0, -1.0), 11, 44, 45, 12, simple_car_front_light_color, simple_car_front_light_shading),
        face_3d(simple_car_vertices, vertex_3d(0.0, 0.0, 1.0), 20, 21, 24, 17, simple_car_back_light_color, simple_car_back_light_shading),
        face_3d(simple_car_vertices, vertex_3d(0.0, 0.0, 1.0), 22, 25, 18, 23, simple_car_back_light_color, simple_car_back_light_shading),
        face_3d(simple_car_vertices, vertex_3d(-0.0, 0.0, -1.0), 45, 44, 42, 43, simple_car_ventilation_color, simple_car_ventilation_shading),
        face_3d(simple_car_vertices, vertex_3d(1.0, 0.0, 0.0), 30, 29, 28, 33, simple_car_wheel_color, simple_car_wheel_shading),
        face_3d(simple_car_vertices, vertex_3d(1.0, 0.0, 0.0), 32, 31, 27, 26, simple_car_wheel_color, simple_car_wheel_shading),
        face_3d(simple_car_vertices, vertex_3d(-1.0, -0.0, 0.0), 38, 36, 35, 39, simple_car_wheel_color, simple_car_wheel_shading),
        face_3d(simple_car_vertices, vertex_3d(-1.0, -0.0, 0.0), 37, 41, 40, 34, simple_car_wheel_color, simple_car_wheel_shading),
    };

    constexpr inline model_3d_item simple_car(simple_car_vertices, simple_car_faces);
}

#endif

