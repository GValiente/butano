/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_CHINESE_HOUSE_FULL_H
#define FR_MODEL_3D_ITEMS_CHINESE_HOUSE_FULL_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d chinese_house_full_vertices[] = {
        vertex_3d(-9.83, 29.84, -9.83),
        vertex_3d(-9.83, 29.84, 9.83),
        vertex_3d(0.0, 46.57, 0.0),
        vertex_3d(9.83, 29.84, 9.83),
        vertex_3d(9.83, 29.84, -9.83),
        vertex_3d(-9.83, 10.05, -9.83),
        vertex_3d(-31.0, 4.0, 0.0),
        vertex_3d(-9.83, 10.05, 9.83),
        vertex_3d(0.0, 4.0, 31.0),
        vertex_3d(9.83, 10.05, 9.83),
        vertex_3d(9.83, 10.05, -9.83),
        vertex_3d(0.0, 4.0, -31.0),
        vertex_3d(31.0, 4.0, 0.0),
        vertex_3d(37.0, -10.0, 40.0),
        vertex_3d(-37.0, -10.0, -40.0),
        vertex_3d(-37.0, -10.0, 40.0),
        vertex_3d(37.0, -10.0, -40.0),
        vertex_3d(-22.0, -39.26, -5.84),
        vertex_3d(-22.0, -39.26, 5.84),
        vertex_3d(-22.0, -11.4, 5.84),
        vertex_3d(-22.0, -11.4, -5.84),
        vertex_3d(22.0, -39.26, -22.0),
        vertex_3d(22.0, 1.0, -22.0),
        vertex_3d(22.0, 1.0, 22.0),
        vertex_3d(22.0, -39.26, 22.0),
        vertex_3d(-22.0, -39.26, -22.0),
        vertex_3d(-22.0, 1.0, -22.0),
        vertex_3d(-22.0, 1.0, 22.0),
        vertex_3d(-22.0, -39.26, 22.0),
        vertex_3d(-22.0, 1.0, 0.0),
        vertex_3d(22.0, 1.0, 0.0),
        vertex_3d(0.0, 1.0, 22.0),
        vertex_3d(0.0, 1.0, -22.0),
        vertex_3d(9.83, 23.24, -3.28),
        vertex_3d(9.83, 16.65, -3.28),
        vertex_3d(9.83, 23.0, 3.28),
        vertex_3d(9.83, 16.65, 3.28),
    };

    constexpr inline int chinese_house_full_roof_top_a_color = 5;
    constexpr inline int chinese_house_full_roof_top_a_shading = 7;

    constexpr inline int chinese_house_full_roof_top_b_color = 5;
    constexpr inline int chinese_house_full_roof_top_b_shading = 5;

    constexpr inline int chinese_house_full_roof_bottom_a_color = 5;
    constexpr inline int chinese_house_full_roof_bottom_a_shading = 5;

    constexpr inline int chinese_house_full_roof_bottom_b_color = 5;
    constexpr inline int chinese_house_full_roof_bottom_b_shading = 2;

    constexpr inline int chinese_house_full_roof_bottom_c_color = 5;
    constexpr inline int chinese_house_full_roof_bottom_c_shading = 0;

    constexpr inline int chinese_house_full_door_color = 0;
    constexpr inline int chinese_house_full_door_shading = -1;

    constexpr inline int chinese_house_full_wall_front_color = 2;
    constexpr inline int chinese_house_full_wall_front_shading = 7;

    constexpr inline int chinese_house_full_wall_side_color = 2;
    constexpr inline int chinese_house_full_wall_side_shading = 5;

    constexpr inline int chinese_house_full_wall_back_color = 2;
    constexpr inline int chinese_house_full_wall_back_shading = 3;

    constexpr inline int chinese_house_full_wall_down_color = 2;
    constexpr inline int chinese_house_full_wall_down_shading = 1;

    constexpr inline int chinese_house_full_window_color = 1;
    constexpr inline int chinese_house_full_window_shading = -1;

    constexpr inline face_3d chinese_house_full_faces[] = {
        face_3d(chinese_house_full_vertices, vertex_3d(-0.8621858238437726, 0.5065921487378532, 0.0), 0, 1, 2, chinese_house_full_roof_top_a_color, chinese_house_full_roof_top_a_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.8621858238437726, 0.5065921487378532, 0.0), 4, 2, 3, chinese_house_full_roof_top_a_color, chinese_house_full_roof_top_a_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.0, 0.5065921487378532, 0.8621858238437726), 1, 3, 2, chinese_house_full_roof_top_b_color, chinese_house_full_roof_top_b_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.0, 0.5065921487378532, -0.8621858238437726), 4, 0, 2, chinese_house_full_roof_top_b_color, chinese_house_full_roof_top_b_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(-0.27478111388379434, 0.9615068067636242, 0.0), 5, 6, 7, chinese_house_full_roof_bottom_a_color, chinese_house_full_roof_bottom_a_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.0, 0.9615068067636242, 0.27478111388379434), 7, 8, 9, chinese_house_full_roof_bottom_a_color, chinese_house_full_roof_bottom_a_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.0, 0.9615068067636242, -0.27478111388379434), 10, 11, 5, chinese_house_full_roof_bottom_a_color, chinese_house_full_roof_bottom_a_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(-0.2956561979945413, -0.8869685939836239, -0.35478743759344955), 15, 6, 29, chinese_house_full_roof_bottom_a_color, chinese_house_full_roof_bottom_a_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.2956561979945413, -0.8869685939836239, 0.35478743759344955), 16, 12, 30, chinese_house_full_roof_bottom_a_color, chinese_house_full_roof_bottom_a_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(-0.3995731563327024, -0.8696592226064699, 0.28988640753549), 13, 8, 31, chinese_house_full_roof_bottom_a_color, chinese_house_full_roof_bottom_a_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.3995731563327024, -0.8696592226064699, -0.28988640753549), 14, 11, 32, chinese_house_full_roof_bottom_a_color, chinese_house_full_roof_bottom_a_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.27478111388379434, 0.9615068067636242, -0.0), 9, 12, 10, chinese_house_full_roof_bottom_a_color, chinese_house_full_roof_bottom_a_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.37365361272397013, 0.8915440949715971, 0.2559923913314634), 9, 13, 12, chinese_house_full_roof_bottom_b_color, chinese_house_full_roof_bottom_b_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(-0.37365361272397013, 0.8915440949715971, -0.2559923913314634), 5, 14, 6, chinese_house_full_roof_bottom_b_color, chinese_house_full_roof_bottom_b_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(-0.24832525194127839, 0.89480437164605, 0.37102520902415537), 7, 15, 8, chinese_house_full_roof_bottom_b_color, chinese_house_full_roof_bottom_b_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.24832525194127839, 0.89480437164605, -0.37102520902415537), 10, 16, 11, chinese_house_full_roof_bottom_b_color, chinese_house_full_roof_bottom_b_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.2956561979945413, -0.8869685939836239, -0.35478743759344955), 13, 30, 12, chinese_house_full_roof_bottom_b_color, chinese_house_full_roof_bottom_b_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.3995731563327024, -0.8696592226064699, 0.28988640753549), 15, 31, 8, chinese_house_full_roof_bottom_b_color, chinese_house_full_roof_bottom_b_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(-0.3995731563327024, -0.8696592226064699, -0.28988640753549), 16, 32, 11, chinese_house_full_roof_bottom_b_color, chinese_house_full_roof_bottom_b_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(-0.2956561979945413, -0.8869685939836239, 0.35478743759344955), 14, 29, 6, chinese_house_full_roof_bottom_b_color, chinese_house_full_roof_bottom_b_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.37365361272397013, 0.8915440949715971, -0.2559923913314634), 10, 12, 16, chinese_house_full_roof_bottom_c_color, chinese_house_full_roof_bottom_c_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(-0.37365361272397013, 0.8915440949715971, 0.2559923913314634), 7, 6, 15, chinese_house_full_roof_bottom_c_color, chinese_house_full_roof_bottom_c_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.24832525194127839, 0.89480437164605, 0.37102520902415537), 13, 9, 8, chinese_house_full_roof_bottom_c_color, chinese_house_full_roof_bottom_c_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(-0.24832525194127839, 0.89480437164605, -0.37102520902415537), 14, 5, 11, chinese_house_full_roof_bottom_c_color, chinese_house_full_roof_bottom_c_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.5913636636275174, -0.8064049958557055, 0.0), 27, 15, 29, chinese_house_full_roof_bottom_c_color, chinese_house_full_roof_bottom_c_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(-0.5913636636275174, -0.8064049958557055, 0.0), 13, 23, 30, chinese_house_full_roof_bottom_c_color, chinese_house_full_roof_bottom_c_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(-0.5913636636275174, -0.8064049958557055, -0.0), 22, 16, 30, chinese_house_full_roof_bottom_c_color, chinese_house_full_roof_bottom_c_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(-0.0, -0.8532818336519591, -0.521450009453975), 15, 27, 31, chinese_house_full_roof_bottom_c_color, chinese_house_full_roof_bottom_c_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(-0.0, -0.8532818336519591, -0.521450009453975), 13, 31, 23, chinese_house_full_roof_bottom_c_color, chinese_house_full_roof_bottom_c_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.0, -0.8532818336519591, 0.521450009453975), 16, 22, 32, chinese_house_full_roof_bottom_c_color, chinese_house_full_roof_bottom_c_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.0, -0.8532818336519591, 0.521450009453975), 26, 14, 32, chinese_house_full_roof_bottom_c_color, chinese_house_full_roof_bottom_c_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.5913636636275174, -0.8064049958557055, 0.0), 14, 26, 29, chinese_house_full_roof_bottom_c_color, chinese_house_full_roof_bottom_c_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(-1.0, -0.0, 0.0), 17, 18, 19, 20, chinese_house_full_door_color, chinese_house_full_door_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(1.0, 0.0, 0.0), 21, 22, 23, 24, chinese_house_full_wall_front_color, chinese_house_full_wall_front_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(1.0, 0.0, 0.0), 33, 4, 3, 35, chinese_house_full_wall_front_color, chinese_house_full_wall_front_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(1.0, 0.0, 0.0), 9, 36, 35, 3, chinese_house_full_wall_front_color, chinese_house_full_wall_front_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(1.0, 0.0, 0.0), 34, 10, 4, 33, chinese_house_full_wall_front_color, chinese_house_full_wall_front_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(1.0, 0.0, 0.0), 34, 36, 9, 10, chinese_house_full_wall_front_color, chinese_house_full_wall_front_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.0, 0.0, 1.0), 3, 1, 7, 9, chinese_house_full_wall_side_color, chinese_house_full_wall_side_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.0, 0.0, -1.0), 4, 10, 5, 0, chinese_house_full_wall_side_color, chinese_house_full_wall_side_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.0, 0.0, -1.0), 21, 25, 26, 22, chinese_house_full_wall_side_color, chinese_house_full_wall_side_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.0, 0.0, 1.0), 23, 27, 28, 24, chinese_house_full_wall_side_color, chinese_house_full_wall_side_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(-1.0, 0.0, 0.0), 7, 1, 0, 5, chinese_house_full_wall_back_color, chinese_house_full_wall_back_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(-1.0, 0.0, 0.0), 27, 19, 18, 28, chinese_house_full_wall_back_color, chinese_house_full_wall_back_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(-1.0, 0.0, 0.0), 27, 26, 20, 19, chinese_house_full_wall_back_color, chinese_house_full_wall_back_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(-1.0, -0.0, 0.0), 25, 17, 20, 26, chinese_house_full_wall_back_color, chinese_house_full_wall_back_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(0.0, -1.0, 0.0), 24, 28, 25, 21, chinese_house_full_wall_down_color, chinese_house_full_wall_down_shading),
        face_3d(chinese_house_full_vertices, vertex_3d(1.0, -0.0, 0.0), 33, 35, 36, 34, chinese_house_full_window_color, chinese_house_full_window_shading),
    };

    constexpr inline model_3d_item chinese_house_full(chinese_house_full_vertices, chinese_house_full_faces);
}

#endif

