/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_COMMON_CAR_H
#define FR_MODEL_3D_ITEMS_COMMON_CAR_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d common_car_vertices[] = {
        vertex_3d(4.5, 4.0, 7.0),
        vertex_3d(4.5, 4.0, 1.0),
        vertex_3d(-4.5, 4.0, 1.0),
        vertex_3d(-4.5, 4.0, 7.0),
        vertex_3d(6.0, 1.0, 9.0),
        vertex_3d(-6.0, 1.0, 9.0),
        vertex_3d(-6.0, 1.0, 13.0),
        vertex_3d(6.0, 1.0, 13.0),
        vertex_3d(-6.0, 1.0, -4.0),
        vertex_3d(6.0, 1.0, -4.0),
        vertex_3d(6.0, 0.0, -13.0),
        vertex_3d(-6.0, 0.0, -13.0),
        vertex_3d(-6.0, -5.0, -13.0),
        vertex_3d(-5.0, -0.5, -13.0),
        vertex_3d(-5.0, -2.0, -13.0),
        vertex_3d(6.0, -5.0, -13.0),
        vertex_3d(5.0, -2.0, -13.0),
        vertex_3d(5.0, -0.5, -13.0),
        vertex_3d(6.0, -5.0, 13.0),
        vertex_3d(5.0, -1.5, 13.0),
        vertex_3d(-5.0, -1.5, 13.0),
        vertex_3d(-6.0, -5.0, 13.0),
        vertex_3d(5.0, 0.0, 13.0),
        vertex_3d(3.0, 0.0, 13.0),
        vertex_3d(-3.0, 0.0, 13.0),
        vertex_3d(-3.0, -1.5, 13.0),
        vertex_3d(3.0, -1.5, 13.0),
        vertex_3d(-5.0, 0.0, 13.0),
        vertex_3d(6.0, -5.0, 11.0),
        vertex_3d(6.0, -1.0, 11.0),
        vertex_3d(6.0, -1.0, -6.0),
        vertex_3d(6.0, -1.0, -10.0),
        vertex_3d(6.0, -5.0, -10.0),
        vertex_3d(6.0, -1.0, 7.0),
        vertex_3d(6.0, -5.0, 7.0),
        vertex_3d(6.0, -5.0, -6.0),
        vertex_3d(-6.0, -1.0, 7.0),
        vertex_3d(-6.0, -1.0, -6.0),
        vertex_3d(-6.0, -5.0, -6.0),
        vertex_3d(-6.0, -5.0, 7.0),
        vertex_3d(-6.0, -5.0, -10.0),
        vertex_3d(-6.0, -1.0, -10.0),
        vertex_3d(-6.0, -1.0, 11.0),
        vertex_3d(-6.0, -5.0, 11.0),
        vertex_3d(3.0, -0.5, -13.0),
        vertex_3d(3.0, -2.0, -13.0),
        vertex_3d(-3.0, -0.5, -13.0),
        vertex_3d(-3.0, -2.0, -13.0),
    };

    constexpr inline int common_car_red_top_a_color = 7;
    constexpr inline int common_car_red_top_a_shading = 3;

    constexpr inline int common_car_red_top_b_color = 7;
    constexpr inline int common_car_red_top_b_shading = 2;

    constexpr inline int common_car_red_side_a_color = 7;
    constexpr inline int common_car_red_side_a_shading = 0;

    constexpr inline int common_car_red_side_b_color = 7;
    constexpr inline int common_car_red_side_b_shading = 1;

    constexpr inline int common_car_red_bottom_color = 6;
    constexpr inline int common_car_red_bottom_shading = 0;

    constexpr inline int common_car_glass_front_color = 1;
    constexpr inline int common_car_glass_front_shading = 7;

    constexpr inline int common_car_glass_back_color = 1;
    constexpr inline int common_car_glass_back_shading = 6;

    constexpr inline int common_car_glass_sides_color = 1;
    constexpr inline int common_car_glass_sides_shading = -1;

    constexpr inline int common_car_front_light_color = 2;
    constexpr inline int common_car_front_light_shading = -1;

    constexpr inline int common_car_back_light_color = 0;
    constexpr inline int common_car_back_light_shading = 0;

    constexpr inline int common_car_ventilation_color = 6;
    constexpr inline int common_car_ventilation_shading = -1;

    constexpr inline int common_car_wheel_color = 6;
    constexpr inline int common_car_wheel_shading = 7;

    constexpr inline face_3d common_car_faces[] = {
        face_3d(common_car_vertices, vertex_3d(0.0, 1.0, 0.0), 0, 1, 2, 3, common_car_red_top_a_color, common_car_red_top_a_shading),
        face_3d(common_car_vertices, vertex_3d(0.0, 1.0, 0.0), 4, 5, 6, 7, common_car_red_top_a_color, common_car_red_top_a_shading),
        face_3d(common_car_vertices, vertex_3d(0.0, 0.9938837346736189, -0.11043152607484655), 8, 9, 10, 11, common_car_red_top_b_color, common_car_red_top_b_shading),
        face_3d(common_car_vertices, vertex_3d(-0.0, 0.0, -1.0), 12, 11, 13, 14, common_car_red_side_a_color, common_car_red_side_a_shading),
        face_3d(common_car_vertices, vertex_3d(0.0, 0.0, -1.0), 15, 16, 17, 10, common_car_red_side_a_color, common_car_red_side_a_shading),
        face_3d(common_car_vertices, vertex_3d(0.0, 0.0, -1.0), 11, 10, 17, 13, common_car_red_side_a_color, common_car_red_side_a_shading),
        face_3d(common_car_vertices, vertex_3d(0.0, 0.0, -1.0), 16, 15, 12, 14, common_car_red_side_a_color, common_car_red_side_a_shading),
        face_3d(common_car_vertices, vertex_3d(-0.0, 0.0, 1.0), 18, 19, 20, 21, common_car_red_side_a_color, common_car_red_side_a_shading),
        face_3d(common_car_vertices, vertex_3d(0.0, 0.0, 1.0), 22, 19, 18, 7, common_car_red_side_a_color, common_car_red_side_a_shading),
        face_3d(common_car_vertices, vertex_3d(0.0, 0.0, 1.0), 23, 24, 25, 26, common_car_red_side_a_color, common_car_red_side_a_shading),
        face_3d(common_car_vertices, vertex_3d(0.0, 0.0, 1.0), 20, 27, 6, 21, common_car_red_side_a_color, common_car_red_side_a_shading),
        face_3d(common_car_vertices, vertex_3d(0.0, -0.0, 1.0), 7, 6, 27, 22, common_car_red_side_a_color, common_car_red_side_a_shading),
        face_3d(common_car_vertices, vertex_3d(1.0, 0.0, 0.0), 18, 28, 29, 7, common_car_red_side_b_color, common_car_red_side_b_shading),
        face_3d(common_car_vertices, vertex_3d(1.0, 0.0, 0.0), 29, 30, 9, 7, common_car_red_side_b_color, common_car_red_side_b_shading),
        face_3d(common_car_vertices, vertex_3d(1.0, 0.0, 0.0), 15, 10, 31, 32, common_car_red_side_b_color, common_car_red_side_b_shading),
        face_3d(common_car_vertices, vertex_3d(1.0, -0.0, 0.0), 10, 9, 30, 31, common_car_red_side_b_color, common_car_red_side_b_shading),
        face_3d(common_car_vertices, vertex_3d(1.0, 0.0, 0.0), 33, 34, 35, 30, common_car_red_side_b_color, common_car_red_side_b_shading),
        face_3d(common_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 36, 37, 38, 39, common_car_red_side_b_color, common_car_red_side_b_shading),
        face_3d(common_car_vertices, vertex_3d(-1.0, -0.0, 0.0), 12, 40, 41, 11, common_car_red_side_b_color, common_car_red_side_b_shading),
        face_3d(common_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 21, 6, 42, 43, common_car_red_side_b_color, common_car_red_side_b_shading),
        face_3d(common_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 11, 41, 37, 8, common_car_red_side_b_color, common_car_red_side_b_shading),
        face_3d(common_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 37, 42, 6, 8, common_car_red_side_b_color, common_car_red_side_b_shading),
        face_3d(common_car_vertices, vertex_3d(0.0, -1.0, 0.0), 15, 18, 21, 12, common_car_red_bottom_color, common_car_red_bottom_shading),
        face_3d(common_car_vertices, vertex_3d(0.0, 0.8574929257125442, -0.5144957554275266), 1, 9, 8, 2, common_car_glass_front_color, common_car_glass_front_shading),
        face_3d(common_car_vertices, vertex_3d(0.0, 0.554700196225229, 0.8320502943378436), 5, 4, 0, 3, common_car_glass_back_color, common_car_glass_back_shading),
        face_3d(common_car_vertices, vertex_3d(-0.8944271909999159, 0.4472135954999579, 0.0), 5, 3, 2, 8, common_car_glass_sides_color, common_car_glass_sides_shading),
        face_3d(common_car_vertices, vertex_3d(0.894427190999916, 0.447213595499958, 0.0), 4, 9, 1, 0, common_car_glass_sides_color, common_car_glass_sides_shading),
        face_3d(common_car_vertices, vertex_3d(0.0, 0.0, -1.0), 44, 17, 16, 45, common_car_front_light_color, common_car_front_light_shading),
        face_3d(common_car_vertices, vertex_3d(0.0, 0.0, -1.0), 13, 46, 47, 14, common_car_front_light_color, common_car_front_light_shading),
        face_3d(common_car_vertices, vertex_3d(0.0, 0.0, 1.0), 22, 23, 26, 19, common_car_back_light_color, common_car_back_light_shading),
        face_3d(common_car_vertices, vertex_3d(0.0, 0.0, 1.0), 24, 27, 20, 25, common_car_back_light_color, common_car_back_light_shading),
        face_3d(common_car_vertices, vertex_3d(-0.0, 0.0, -1.0), 47, 46, 44, 45, common_car_ventilation_color, common_car_ventilation_shading),
        face_3d(common_car_vertices, vertex_3d(1.0, 0.0, 0.0), 32, 31, 30, 35, common_car_wheel_color, common_car_wheel_shading),
        face_3d(common_car_vertices, vertex_3d(1.0, 0.0, 0.0), 34, 33, 29, 28, common_car_wheel_color, common_car_wheel_shading),
        face_3d(common_car_vertices, vertex_3d(-1.0, -0.0, 0.0), 40, 38, 37, 41, common_car_wheel_color, common_car_wheel_shading),
        face_3d(common_car_vertices, vertex_3d(-1.0, -0.0, 0.0), 39, 43, 42, 36, common_car_wheel_color, common_car_wheel_shading),
    };

    constexpr inline model_3d_item common_car(common_car_vertices, common_car_faces);
}

#endif

