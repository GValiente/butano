/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_PLAYER_CAR_H
#define FR_MODEL_3D_ITEMS_PLAYER_CAR_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d player_car_vertices[] = {
        vertex_3d(4.5, 4.0, 5.0),
        vertex_3d(4.5, 4.0, 1.0),
        vertex_3d(-4.5, 4.0, 1.0),
        vertex_3d(-4.5, 4.0, 5.0),
        vertex_3d(6.0, 1.0, 10.5),
        vertex_3d(-6.0, 1.0, 10.5),
        vertex_3d(-6.0, 1.0, 14.0),
        vertex_3d(6.0, 1.0, 14.0),
        vertex_3d(-6.0, 1.0, -5.0),
        vertex_3d(6.0, 1.0, -5.0),
        vertex_3d(6.0, 0.0, -14.0),
        vertex_3d(-6.0, 0.0, -14.0),
        vertex_3d(-6.0, -5.0, -14.0),
        vertex_3d(-5.0, -0.5, -14.0),
        vertex_3d(-5.0, -1.5, -14.0),
        vertex_3d(6.0, -5.0, -14.0),
        vertex_3d(5.0, -1.5, -14.0),
        vertex_3d(5.0, -0.5, -14.0),
        vertex_3d(6.0, -5.0, 14.0),
        vertex_3d(5.0, -1.0, 14.0),
        vertex_3d(-5.0, -1.0, 14.0),
        vertex_3d(-6.0, -5.0, 14.0),
        vertex_3d(5.0, 0.0, 14.0),
        vertex_3d(1.0, 0.0, 14.0),
        vertex_3d(-1.0, 0.0, 14.0),
        vertex_3d(-1.0, -1.0, 14.0),
        vertex_3d(1.0, -1.0, 14.0),
        vertex_3d(-5.0, 0.0, 14.0),
        vertex_3d(2.5, -0.5, -14.0),
        vertex_3d(2.5, -1.5, -14.0),
        vertex_3d(-2.5, -0.5, -14.0),
        vertex_3d(-2.5, -1.5, -14.0),
        vertex_3d(6.0, -5.0, 12.0),
        vertex_3d(6.0, -5.0, 8.0),
        vertex_3d(6.0, -1.0, 12.0),
        vertex_3d(6.0, -1.0, 8.0),
        vertex_3d(6.0, -5.0, -7.0),
        vertex_3d(6.0, -5.0, -11.0),
        vertex_3d(6.0, -1.0, -7.0),
        vertex_3d(6.0, -1.0, -11.0),
        vertex_3d(-6.0, -5.0, 12.0),
        vertex_3d(-6.0, -5.0, 8.0),
        vertex_3d(-6.0, -1.0, 12.0),
        vertex_3d(-6.0, -1.0, 8.0),
        vertex_3d(-6.0, -5.0, -7.0),
        vertex_3d(-6.0, -5.0, -11.0),
        vertex_3d(-6.0, -1.0, -7.0),
        vertex_3d(-6.0, -1.0, -11.0),
    };

    constexpr inline int player_car_red_top_a_color = 0;
    constexpr inline int player_car_red_top_a_shading = 7;

    constexpr inline int player_car_red_top_b_color = 0;
    constexpr inline int player_car_red_top_b_shading = 6;

    constexpr inline int player_car_red_side_a_color = 0;
    constexpr inline int player_car_red_side_a_shading = 2;

    constexpr inline int player_car_red_side_b_color = 0;
    constexpr inline int player_car_red_side_b_shading = 3;

    constexpr inline int player_car_red_bottom_color = 6;
    constexpr inline int player_car_red_bottom_shading = 0;

    constexpr inline int player_car_glass_front_color = 1;
    constexpr inline int player_car_glass_front_shading = 7;

    constexpr inline int player_car_glass_back_color = 1;
    constexpr inline int player_car_glass_back_shading = 6;

    constexpr inline int player_car_glass_sides_color = 1;
    constexpr inline int player_car_glass_sides_shading = -1;

    constexpr inline int player_car_front_light_color = 7;
    constexpr inline int player_car_front_light_shading = -1;

    constexpr inline int player_car_back_light_color = 0;
    constexpr inline int player_car_back_light_shading = 0;

    constexpr inline int player_car_ventilation_color = 6;
    constexpr inline int player_car_ventilation_shading = -1;

    constexpr inline int player_car_wheel_color = 6;
    constexpr inline int player_car_wheel_shading = 7;

    constexpr inline face_3d player_car_faces[] = {
        face_3d(player_car_vertices, vertex_3d(0.0, 1.0, 0.0), 0, 1, 2, 3, player_car_red_top_a_color, player_car_red_top_a_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 1.0, 0.0), 4, 5, 6, 7, player_car_red_top_a_color, player_car_red_top_a_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.9938837346736189, -0.11043152607484655), 8, 9, 10, 11, player_car_red_top_b_color, player_car_red_top_b_shading),
        face_3d(player_car_vertices, vertex_3d(-0.0, 0.0, -1.0), 12, 11, 13, 14, player_car_red_side_a_color, player_car_red_side_a_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.0, -1.0), 15, 16, 17, 10, player_car_red_side_a_color, player_car_red_side_a_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.0, -1.0), 11, 10, 17, 13, player_car_red_side_a_color, player_car_red_side_a_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.0, -1.0), 16, 15, 12, 14, player_car_red_side_a_color, player_car_red_side_a_shading),
        face_3d(player_car_vertices, vertex_3d(-0.0, 0.0, 1.0), 18, 19, 20, 21, player_car_red_side_a_color, player_car_red_side_a_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.0, 1.0), 22, 19, 18, 7, player_car_red_side_a_color, player_car_red_side_a_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.0, 1.0), 23, 24, 25, 26, player_car_red_side_a_color, player_car_red_side_a_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.0, 1.0), 20, 27, 6, 21, player_car_red_side_a_color, player_car_red_side_a_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, -0.0, 1.0), 7, 6, 27, 22, player_car_red_side_a_color, player_car_red_side_a_shading),
        face_3d(player_car_vertices, vertex_3d(1.0, 0.0, 0.0), 18, 32, 34, 7, player_car_red_side_b_color, player_car_red_side_b_shading),
        face_3d(player_car_vertices, vertex_3d(1.0, 0.0, 0.0), 34, 38, 9, 7, player_car_red_side_b_color, player_car_red_side_b_shading),
        face_3d(player_car_vertices, vertex_3d(1.0, 0.0, 0.0), 15, 10, 39, 37, player_car_red_side_b_color, player_car_red_side_b_shading),
        face_3d(player_car_vertices, vertex_3d(1.0, -0.0, 0.0), 10, 9, 38, 39, player_car_red_side_b_color, player_car_red_side_b_shading),
        face_3d(player_car_vertices, vertex_3d(1.0, 0.0, 0.0), 35, 33, 36, 38, player_car_red_side_b_color, player_car_red_side_b_shading),
        face_3d(player_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 43, 46, 44, 41, player_car_red_side_b_color, player_car_red_side_b_shading),
        face_3d(player_car_vertices, vertex_3d(-1.0, -0.0, 0.0), 12, 45, 47, 11, player_car_red_side_b_color, player_car_red_side_b_shading),
        face_3d(player_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 21, 6, 42, 40, player_car_red_side_b_color, player_car_red_side_b_shading),
        face_3d(player_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 11, 47, 46, 8, player_car_red_side_b_color, player_car_red_side_b_shading),
        face_3d(player_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 46, 42, 6, 8, player_car_red_side_b_color, player_car_red_side_b_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, -1.0, 0.0), 15, 18, 21, 12, player_car_red_bottom_color, player_car_red_bottom_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.8944271909999159, -0.4472135954999579), 1, 9, 8, 2, player_car_glass_front_color, player_car_glass_front_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.8778955729143844, 0.4788521306805733), 5, 4, 0, 3, player_car_glass_back_color, player_car_glass_back_shading),
        face_3d(player_car_vertices, vertex_3d(-0.8944271909999159, 0.4472135954999579, 0.0), 5, 3, 2, 8, player_car_glass_sides_color, player_car_glass_sides_shading),
        face_3d(player_car_vertices, vertex_3d(0.8944271909999159, 0.4472135954999579, 0.0), 4, 9, 1, 0, player_car_glass_sides_color, player_car_glass_sides_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.0, -1.0), 28, 17, 16, 29, player_car_front_light_color, player_car_front_light_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.0, -1.0), 13, 30, 31, 14, player_car_front_light_color, player_car_front_light_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.0, 1.0), 22, 23, 26, 19, player_car_back_light_color, player_car_back_light_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.0, 1.0), 24, 27, 20, 25, player_car_back_light_color, player_car_back_light_shading),
        face_3d(player_car_vertices, vertex_3d(-0.0, 0.0, -1.0), 31, 30, 28, 29, player_car_ventilation_color, player_car_ventilation_shading),
        face_3d(player_car_vertices, vertex_3d(1.0, 0.0, 0.0), 37, 39, 38, 36, player_car_wheel_color, player_car_wheel_shading),
        face_3d(player_car_vertices, vertex_3d(1.0, 0.0, 0.0), 33, 35, 34, 32, player_car_wheel_color, player_car_wheel_shading),
        face_3d(player_car_vertices, vertex_3d(-1.0, -0.0, 0.0), 45, 44, 46, 47, player_car_wheel_color, player_car_wheel_shading),
        face_3d(player_car_vertices, vertex_3d(-1.0, -0.0, 0.0), 41, 40, 42, 43, player_car_wheel_color, player_car_wheel_shading),
    };

    constexpr inline model_3d_item player_car(player_car_vertices, player_car_faces);

    constexpr inline int rival_car_red_top_a_color = 6;
    constexpr inline int rival_car_red_top_a_shading = 7;

    constexpr inline int rival_car_red_top_b_color = 6;
    constexpr inline int rival_car_red_top_b_shading = 7;

    constexpr inline int rival_car_red_side_a_color = 6;
    constexpr inline int rival_car_red_side_a_shading = 0;

    constexpr inline int rival_car_red_side_b_color = 6;
    constexpr inline int rival_car_red_side_b_shading = 3;

    constexpr inline int rival_car_red_bottom_color = 0;
    constexpr inline int rival_car_red_bottom_shading = 0;

    constexpr inline int rival_car_glass_front_color = 0;
    constexpr inline int rival_car_glass_front_shading = 7;

    constexpr inline int rival_car_glass_back_color = 0;
    constexpr inline int rival_car_glass_back_shading = 6;

    constexpr inline int rival_car_glass_sides_color = 0;
    constexpr inline int rival_car_glass_sides_shading = -1;

    constexpr inline int rival_car_front_light_color = 7;
    constexpr inline int rival_car_front_light_shading = -1;

    constexpr inline int rival_car_back_light_color = 0;
    constexpr inline int rival_car_back_light_shading = 0;

    constexpr inline int rival_car_ventilation_color = 6;
    constexpr inline int rival_car_ventilation_shading = 7;

    constexpr inline int rival_car_wheel_color = 6;
    constexpr inline int rival_car_wheel_shading = 7;

    constexpr inline face_3d rival_car_faces[] = {
        face_3d(player_car_vertices, vertex_3d(0.0, 1.0, 0.0), 0, 1, 2, 3, rival_car_red_top_a_color, rival_car_red_top_a_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 1.0, 0.0), 4, 5, 6, 7, rival_car_red_top_a_color, rival_car_red_top_a_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.9938837346736189, -0.11043152607484655), 8, 9, 10, 11, rival_car_red_top_b_color, rival_car_red_top_b_shading),
        face_3d(player_car_vertices, vertex_3d(-0.0, 0.0, -1.0), 12, 11, 13, 14, rival_car_red_side_a_color, rival_car_red_side_a_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.0, -1.0), 15, 16, 17, 10, rival_car_red_side_a_color, rival_car_red_side_a_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.0, -1.0), 11, 10, 17, 13, rival_car_red_side_a_color, rival_car_red_side_a_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.0, -1.0), 16, 15, 12, 14, rival_car_red_side_a_color, rival_car_red_side_a_shading),
        face_3d(player_car_vertices, vertex_3d(-0.0, 0.0, 1.0), 18, 19, 20, 21, rival_car_red_side_a_color, rival_car_red_side_a_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.0, 1.0), 22, 19, 18, 7, rival_car_red_side_a_color, rival_car_red_side_a_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.0, 1.0), 23, 24, 25, 26, rival_car_red_side_a_color, rival_car_red_side_a_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.0, 1.0), 20, 27, 6, 21, rival_car_red_side_a_color, rival_car_red_side_a_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, -0.0, 1.0), 7, 6, 27, 22, rival_car_red_side_a_color, rival_car_red_side_a_shading),
        face_3d(player_car_vertices, vertex_3d(1.0, 0.0, 0.0), 18, 32, 34, 7, rival_car_red_side_b_color, rival_car_red_side_b_shading),
        face_3d(player_car_vertices, vertex_3d(1.0, 0.0, 0.0), 34, 38, 9, 7, rival_car_red_side_b_color, rival_car_red_side_b_shading),
        face_3d(player_car_vertices, vertex_3d(1.0, 0.0, 0.0), 15, 10, 39, 37, rival_car_red_side_b_color, rival_car_red_side_b_shading),
        face_3d(player_car_vertices, vertex_3d(1.0, -0.0, 0.0), 10, 9, 38, 39, rival_car_red_side_b_color, rival_car_red_side_b_shading),
        face_3d(player_car_vertices, vertex_3d(1.0, 0.0, 0.0), 35, 33, 36, 38, rival_car_red_side_b_color, rival_car_red_side_b_shading),
        face_3d(player_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 43, 46, 44, 41, rival_car_red_side_b_color, rival_car_red_side_b_shading),
        face_3d(player_car_vertices, vertex_3d(-1.0, -0.0, 0.0), 12, 45, 47, 11, rival_car_red_side_b_color, rival_car_red_side_b_shading),
        face_3d(player_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 21, 6, 42, 40, rival_car_red_side_b_color, rival_car_red_side_b_shading),
        face_3d(player_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 11, 47, 46, 8, rival_car_red_side_b_color, rival_car_red_side_b_shading),
        face_3d(player_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 46, 42, 6, 8, rival_car_red_side_b_color, rival_car_red_side_b_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, -1.0, 0.0), 15, 18, 21, 12, rival_car_red_bottom_color, rival_car_red_bottom_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.8944271909999159, -0.4472135954999579), 1, 9, 8, 2, rival_car_glass_front_color, rival_car_glass_front_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.8778955729143844, 0.4788521306805733), 5, 4, 0, 3, rival_car_glass_back_color, rival_car_glass_back_shading),
        face_3d(player_car_vertices, vertex_3d(-0.8944271909999159, 0.4472135954999579, 0.0), 5, 3, 2, 8, rival_car_glass_sides_color, rival_car_glass_sides_shading),
        face_3d(player_car_vertices, vertex_3d(0.8944271909999159, 0.4472135954999579, 0.0), 4, 9, 1, 0, rival_car_glass_sides_color, rival_car_glass_sides_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.0, -1.0), 28, 17, 16, 29, rival_car_front_light_color, rival_car_front_light_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.0, -1.0), 13, 30, 31, 14, rival_car_front_light_color, rival_car_front_light_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.0, 1.0), 22, 23, 26, 19, rival_car_back_light_color, rival_car_back_light_shading),
        face_3d(player_car_vertices, vertex_3d(0.0, 0.0, 1.0), 24, 27, 20, 25, rival_car_back_light_color, rival_car_back_light_shading),
        face_3d(player_car_vertices, vertex_3d(-0.0, 0.0, -1.0), 31, 30, 28, 29, rival_car_ventilation_color, rival_car_ventilation_shading),
        face_3d(player_car_vertices, vertex_3d(1.0, 0.0, 0.0), 37, 39, 38, 36, rival_car_wheel_color, rival_car_wheel_shading),
        face_3d(player_car_vertices, vertex_3d(1.0, 0.0, 0.0), 33, 35, 34, 32, rival_car_wheel_color, rival_car_wheel_shading),
        face_3d(player_car_vertices, vertex_3d(-1.0, -0.0, 0.0), 45, 44, 46, 47, rival_car_wheel_color, rival_car_wheel_shading),
        face_3d(player_car_vertices, vertex_3d(-1.0, -0.0, 0.0), 41, 40, 42, 43, rival_car_wheel_color, rival_car_wheel_shading),
    };

    constexpr inline model_3d_item rival_car(player_car_vertices, rival_car_faces);
}

#endif

