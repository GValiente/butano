/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_STATION_CAR_H
#define FR_MODEL_3D_ITEMS_STATION_CAR_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d station_car_vertices[] = {
        vertex_3d(-5.0, -1.5, -14.5),
        vertex_3d(-3.5, -1.5, -14.5),
        vertex_3d(-6.0, -5.0, -14.5),
        vertex_3d(-6.0, 1.0, -14.5),
        vertex_3d(-4.0, 5.0, 3.0),
        vertex_3d(-6.0, 0.0, 14.5),
        vertex_3d(-6.0, 1.0, 7.0),
        vertex_3d(-4.0, 5.0, -12.0),
        vertex_3d(-6.0, -5.0, 8.0),
        vertex_3d(-6.0, -5.0, -8.0),
        vertex_3d(-6.0, -5.0, 14.5),
        vertex_3d(-6.0, -5.0, -12.0),
        vertex_3d(-6.0, -1.0, -12.0),
        vertex_3d(-6.0, -1.0, 8.0),
        vertex_3d(-6.0, -5.0, 12.0),
        vertex_3d(-4.0, -0.5, 14.5),
        vertex_3d(-5.0, -1.5, 14.5),
        vertex_3d(-4.0, -2.5, 14.5),
        vertex_3d(-3.0, -1.5, 14.5),
        vertex_3d(-2.0, -0.5, 14.5),
        vertex_3d(-2.0, -2.5, 14.5),
        vertex_3d(-5.0, 0.0, -14.5),
        vertex_3d(-3.5, 0.0, -14.5),
        vertex_3d(5.0, -1.5, -14.5),
        vertex_3d(3.5, -1.5, -14.5),
        vertex_3d(6.0, -5.0, -14.5),
        vertex_3d(6.0, 1.0, -14.5),
        vertex_3d(4.0, 5.0, 3.0),
        vertex_3d(6.0, 0.0, 14.5),
        vertex_3d(6.0, 1.0, 7.0),
        vertex_3d(4.0, 5.0, -12.0),
        vertex_3d(6.0, -1.0, 8.0),
        vertex_3d(6.0, -1.0, -12.0),
        vertex_3d(6.0, -5.0, 14.5),
        vertex_3d(6.0, -5.0, -12.0),
        vertex_3d(6.0, -5.0, -8.0),
        vertex_3d(6.0, -5.0, 8.0),
        vertex_3d(-6.0, -1.0, 12.0),
        vertex_3d(4.0, -0.5, 14.5),
        vertex_3d(5.0, -1.5, 14.5),
        vertex_3d(4.0, -2.5, 14.5),
        vertex_3d(3.0, -1.5, 14.5),
        vertex_3d(2.0, -0.5, 14.5),
        vertex_3d(2.0, -2.5, 14.5),
        vertex_3d(5.0, 0.0, -14.5),
        vertex_3d(3.5, 0.0, -14.5),
        vertex_3d(-6.0, -1.0, -8.0),
        vertex_3d(6.0, -1.0, -8.0),
        vertex_3d(6.0, -5.0, 12.0),
        vertex_3d(6.0, -1.0, 12.0),
    };

    constexpr inline int station_car_crystal_front_color = 6;
    constexpr inline int station_car_crystal_front_shading = 7;

    constexpr inline int station_car_crystal_side_color = 6;
    constexpr inline int station_car_crystal_side_shading = 4;

    constexpr inline int station_car_crystal_back_color = 6;
    constexpr inline int station_car_crystal_back_shading = 0;

    constexpr inline int station_car_chassis_top_1_color = 1;
    constexpr inline int station_car_chassis_top_1_shading = 4;

    constexpr inline int station_car_chassis_top_2_color = 1;
    constexpr inline int station_car_chassis_top_2_shading = 3;

    constexpr inline int station_car_chassis_side_1_color = 1;
    constexpr inline int station_car_chassis_side_1_shading = 0;

    constexpr inline int station_car_chassis_side_2_color = 1;
    constexpr inline int station_car_chassis_side_2_shading = 2;

    constexpr inline int station_car_chassis_bottom_color = 6;
    constexpr inline int station_car_chassis_bottom_shading = 0;

    constexpr inline int station_car_light_front_color = 7;
    constexpr inline int station_car_light_front_shading = -1;

    constexpr inline int station_car_light_back_color = 0;
    constexpr inline int station_car_light_back_shading = -1;

    constexpr inline int station_car_ventilation_color = 6;
    constexpr inline int station_car_ventilation_shading = -1;

    constexpr inline int station_car_wheel_color = 6;
    constexpr inline int station_car_wheel_shading = 7;

    constexpr inline face_3d station_car_faces[] = {
        face_3d(station_car_vertices, vertex_3d(0.0, 1.0, 0.0), 30, 7, 4, 27, station_car_chassis_top_1_color, station_car_chassis_top_1_shading),
        face_3d(station_car_vertices, vertex_3d(0.0, 0.9912279006826347, 0.13216372009101796), 5, 28, 29, 6, station_car_chassis_top_2_color, station_car_chassis_top_2_shading),
        face_3d(station_car_vertices, vertex_3d(0.0, 0.0, -1.0), 25, 23, 44, 26, station_car_chassis_side_1_color, station_car_chassis_side_1_shading),
        face_3d(station_car_vertices, vertex_3d(-0.0, 0.0, -1.0), 2, 3, 21, 0, station_car_chassis_side_1_color, station_car_chassis_side_1_shading),
        face_3d(station_car_vertices, vertex_3d(0.0, 0.0, -1.0), 3, 26, 44, 21, station_car_chassis_side_1_color, station_car_chassis_side_1_shading),
        face_3d(station_car_vertices, vertex_3d(0.0, 0.0, -1.0), 24, 1, 22, 45, station_car_chassis_side_1_color, station_car_chassis_side_1_shading),
        face_3d(station_car_vertices, vertex_3d(0.0, 0.0, -1.0), 23, 25, 2, 0, station_car_chassis_side_1_color, station_car_chassis_side_1_shading),
        face_3d(station_car_vertices, vertex_3d(0.0, -0.0, 1.0), 19, 15, 20, station_car_chassis_side_1_color, station_car_chassis_side_1_shading),
        face_3d(station_car_vertices, vertex_3d(0.0, 0.0, 1.0), 42, 43, 38, station_car_chassis_side_1_color, station_car_chassis_side_1_shading),
        face_3d(station_car_vertices, vertex_3d(-0.0, 0.0, 1.0), 33, 40, 17, 10, station_car_chassis_side_1_color, station_car_chassis_side_1_shading),
        face_3d(station_car_vertices, vertex_3d(0.0, 0.0, 1.0), 18, 17, 20, station_car_chassis_side_1_color, station_car_chassis_side_1_shading),
        face_3d(station_car_vertices, vertex_3d(0.0, 0.0, 1.0), 41, 43, 40, station_car_chassis_side_1_color, station_car_chassis_side_1_shading),
        face_3d(station_car_vertices, vertex_3d(0.0, 0.0, 1.0), 17, 16, 10, station_car_chassis_side_1_color, station_car_chassis_side_1_shading),
        face_3d(station_car_vertices, vertex_3d(0.0, 0.0, 1.0), 10, 16, 5, station_car_chassis_side_1_color, station_car_chassis_side_1_shading),
        face_3d(station_car_vertices, vertex_3d(0.0, 0.0, 1.0), 16, 15, 5, station_car_chassis_side_1_color, station_car_chassis_side_1_shading),
        face_3d(station_car_vertices, vertex_3d(0.0, 0.0, 1.0), 39, 40, 33, station_car_chassis_side_1_color, station_car_chassis_side_1_shading),
        face_3d(station_car_vertices, vertex_3d(-0.0, 0.0, 1.0), 33, 28, 39, station_car_chassis_side_1_color, station_car_chassis_side_1_shading),
        face_3d(station_car_vertices, vertex_3d(-0.0, 0.0, 1.0), 39, 28, 38, station_car_chassis_side_1_color, station_car_chassis_side_1_shading),
        face_3d(station_car_vertices, vertex_3d(0.0, -0.0, 1.0), 28, 5, 15, 38, station_car_chassis_side_1_color, station_car_chassis_side_1_shading),
        face_3d(station_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 5, 37, 14, 10, station_car_chassis_side_2_color, station_car_chassis_side_2_shading),
        face_3d(station_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 5, 6, 13, 37, station_car_chassis_side_2_color, station_car_chassis_side_2_shading),
        face_3d(station_car_vertices, vertex_3d(-1.0, -0.0, 0.0), 2, 11, 12, 3, station_car_chassis_side_2_color, station_car_chassis_side_2_shading),
        face_3d(station_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 3, 12, 13, 6, station_car_chassis_side_2_color, station_car_chassis_side_2_shading),
        face_3d(station_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 46, 9, 8, 13, station_car_chassis_side_2_color, station_car_chassis_side_2_shading),
        face_3d(station_car_vertices, vertex_3d(1.0, 0.0, 0.0), 28, 33, 48, 49, station_car_chassis_side_2_color, station_car_chassis_side_2_shading),
        face_3d(station_car_vertices, vertex_3d(1.0, 0.0, 0.0), 28, 49, 31, 29, station_car_chassis_side_2_color, station_car_chassis_side_2_shading),
        face_3d(station_car_vertices, vertex_3d(1.0, 0.0, 0.0), 25, 26, 32, 34, station_car_chassis_side_2_color, station_car_chassis_side_2_shading),
        face_3d(station_car_vertices, vertex_3d(1.0, 0.0, 0.0), 26, 29, 31, 32, station_car_chassis_side_2_color, station_car_chassis_side_2_shading),
        face_3d(station_car_vertices, vertex_3d(1.0, 0.0, 0.0), 31, 36, 35, 47, station_car_chassis_side_2_color, station_car_chassis_side_2_shading),
        face_3d(station_car_vertices, vertex_3d(0.0, -1.0, -0.0), 2, 25, 33, 10, station_car_chassis_bottom_color, station_car_chassis_bottom_shading),
        face_3d(station_car_vertices, vertex_3d(0.0, 0.7071067811865476, 0.7071067811865476), 6, 29, 27, 4, station_car_crystal_front_color, station_car_crystal_front_shading),
        face_3d(station_car_vertices, vertex_3d(0.894427190999916, 0.447213595499958, -0.0), 27, 29, 26, 30, station_car_crystal_side_color, station_car_crystal_side_shading),
        face_3d(station_car_vertices, vertex_3d(-0.894427190999916, 0.447213595499958, 0.0), 4, 7, 3, 6, station_car_crystal_side_color, station_car_crystal_side_shading),
        face_3d(station_car_vertices, vertex_3d(0.0, 0.52999894000318, -0.847998304005088), 26, 3, 7, 30, station_car_crystal_back_color, station_car_crystal_back_shading),
        face_3d(station_car_vertices, vertex_3d(0.0, 0.0, 1.0), 16, 17, 18, 15, station_car_light_front_color, station_car_light_front_shading),
        face_3d(station_car_vertices, vertex_3d(0.0, 0.0, 1.0), 39, 38, 41, 40, station_car_light_front_color, station_car_light_front_shading),
        face_3d(station_car_vertices, vertex_3d(-0.0, 0.0, -1.0), 24, 45, 44, 23, station_car_light_back_color, station_car_light_back_shading),
        face_3d(station_car_vertices, vertex_3d(0.0, 0.0, -1.0), 1, 0, 21, 22, station_car_light_back_color, station_car_light_back_shading),
        face_3d(station_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 9, 46, 12, 11, station_car_wheel_color, station_car_wheel_shading),
        face_3d(station_car_vertices, vertex_3d(-1.0, 0.0, 0.0), 37, 13, 8, 14, station_car_wheel_color, station_car_wheel_shading),
        face_3d(station_car_vertices, vertex_3d(1.0, 0.0, 0.0), 49, 48, 36, 31, station_car_wheel_color, station_car_wheel_shading),
        face_3d(station_car_vertices, vertex_3d(1.0, 0.0, 0.0), 35, 34, 32, 47, station_car_wheel_color, station_car_wheel_shading),
        face_3d(station_car_vertices, vertex_3d(-0.0, 0.0, 1.0), 43, 42, 19, 20, station_car_ventilation_color, station_car_ventilation_shading),
    };

    constexpr inline model_3d_item station_car(station_car_vertices, station_car_faces);
}

#endif

