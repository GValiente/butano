/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * CA3
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_GABLE_HOUSE_FULL_H
#define FR_MODEL_3D_ITEMS_GABLE_HOUSE_FULL_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d gable_house_full_vertices[] = {
        vertex_3d(53.0, 47.0, 0.0),
        vertex_3d(53.0, 9.39, -40.97),
        vertex_3d(-53.0, 9.39, -40.97),
        vertex_3d(-53.0, 47.0, 0.0),
        vertex_3d(-53.0, 9.39, 40.97),
        vertex_3d(53.0, 9.39, 40.97),
        vertex_3d(53.0, 40.0, 0.0),
        vertex_3d(53.0, 1.55, -40.97),
        vertex_3d(-53.0, 1.55, -40.97),
        vertex_3d(-53.0, 40.0, 0.0),
        vertex_3d(53.0, 1.55, 40.97),
        vertex_3d(-53.0, 1.55, 40.97),
        vertex_3d(-27.81, 40.0, 23.42),
        vertex_3d(-27.81, 40.0, 17.0),
        vertex_3d(-36.92, 40.0, 17.0),
        vertex_3d(-36.92, 40.0, 23.42),
        vertex_3d(-43.72, -19.0, -15.0),
        vertex_3d(-43.72, -7.0, -15.0),
        vertex_3d(-43.72, 9.5, -32.2),
        vertex_3d(-43.72, -30.23, -32.2),
        vertex_3d(-43.72, 40.0, 0.0),
        vertex_3d(-43.72, 9.5, 32.2),
        vertex_3d(-43.72, -19.0, 15.0),
        vertex_3d(-43.72, -30.23, 32.2),
        vertex_3d(-43.72, -7.0, 15.0),
        vertex_3d(-36.92, 31.39, 17.0),
        vertex_3d(-36.92, 25.49, 23.42),
        vertex_3d(43.72, 9.5, 32.2),
        vertex_3d(43.72, -30.23, 32.2),
        vertex_3d(43.72, -30.23, -32.2),
        vertex_3d(43.72, 9.5, -32.2),
        vertex_3d(-27.81, 25.49, 23.42),
        vertex_3d(-27.81, 31.39, 17.0),
        vertex_3d(43.72, 40.0, 0.0),
        vertex_3d(-43.72, -47.2, -32.2),
        vertex_3d(-43.72, -47.2, 32.2),
        vertex_3d(43.72, -47.2, -32.2),
        vertex_3d(43.72, -47.2, 32.2),
        vertex_3d(43.72, -47.2, 8.0),
        vertex_3d(43.72, -47.2, -8.0),
        vertex_3d(43.72, -13.3, 8.0),
        vertex_3d(43.72, -13.3, -8.0),
        vertex_3d(43.72, -30.23, 8.0),
        vertex_3d(43.72, -30.23, -8.0),
    };

    constexpr inline int gable_house_full_roof_top_a_color = 0;
    constexpr inline int gable_house_full_roof_top_a_shading = 7;

    constexpr inline int gable_house_full_roof_top_b_color = 0;
    constexpr inline int gable_house_full_roof_top_b_shading = 6;

    constexpr inline int gable_house_full_roof_side_a_color = 0;
    constexpr inline int gable_house_full_roof_side_a_shading = 5;

    constexpr inline int gable_house_full_roof_side_b_color = 0;
    constexpr inline int gable_house_full_roof_side_b_shading = 2;

    constexpr inline int gable_house_full_roof_bottom_a_color = 0;
    constexpr inline int gable_house_full_roof_bottom_a_shading = 1;

    constexpr inline int gable_house_full_roof_bottom_b_color = 0;
    constexpr inline int gable_house_full_roof_bottom_b_shading = 0;

    constexpr inline int gable_house_full_chimney_top_color = 6;
    constexpr inline int gable_house_full_chimney_top_shading = -1;

    constexpr inline int gable_house_full_wall_front_color = 2;
    constexpr inline int gable_house_full_wall_front_shading = 4;

    constexpr inline int gable_house_full_wall_side_color = 2;
    constexpr inline int gable_house_full_wall_side_shading = 2;

    constexpr inline int gable_house_full_wall_back_color = 2;
    constexpr inline int gable_house_full_wall_back_shading = 0;

    constexpr inline int gable_house_full_chimney_back_color = 2;
    constexpr inline int gable_house_full_chimney_back_shading = 0;

    constexpr inline int gable_house_full_window_color = 1;
    constexpr inline int gable_house_full_window_shading = -1;

    constexpr inline int gable_house_full_foot_front_color = 5;
    constexpr inline int gable_house_full_foot_front_shading = 7;

    constexpr inline int gable_house_full_foot_side_color = 5;
    constexpr inline int gable_house_full_foot_side_shading = 5;

    constexpr inline int gable_house_full_foot_back_color = 5;
    constexpr inline int gable_house_full_foot_back_shading = 2;

    constexpr inline int gable_house_full_foot_down_color = 5;
    constexpr inline int gable_house_full_foot_down_shading = 0;

    constexpr inline int gable_house_full_door_color = 4;
    constexpr inline int gable_house_full_door_shading = 0;

    constexpr inline face_3d gable_house_full_faces[] = {
        face_3d(gable_house_full_vertices, vertex_3d(0.0, 0.7366688108814069, -0.6762536972723875), 0, 1, 2, 3, gable_house_full_roof_top_a_color, gable_house_full_roof_top_a_shading),
        face_3d(gable_house_full_vertices, vertex_3d(0.0013052162202958168, 0.7362960065548394, 0.6766583237809273), 4, 26, 25, 3, gable_house_full_roof_top_b_color, gable_house_full_roof_top_b_shading),
        face_3d(gable_house_full_vertices, vertex_3d(0.0, 0.7365783443514003, 0.6763522326661975), 0, 3, 25, 32, gable_house_full_roof_top_b_color, gable_house_full_roof_top_b_shading),
        face_3d(gable_house_full_vertices, vertex_3d(0.00011893144503430146, 0.7362966285213266, 0.6766588953700666), 0, 32, 31, 5, gable_house_full_roof_top_b_color, gable_house_full_roof_top_b_shading),
        face_3d(gable_house_full_vertices, vertex_3d(-0.0, 0.7368926491589612, 0.6760097807099302), 5, 31, 26, 4, gable_house_full_roof_top_b_color, gable_house_full_roof_top_b_shading),
        face_3d(gable_house_full_vertices, vertex_3d(1.0, -0.0, 0.0), 1, 0, 6, 7, gable_house_full_roof_side_a_color, gable_house_full_roof_side_a_shading),
        face_3d(gable_house_full_vertices, vertex_3d(-1.0, 0.0, 0.0), 3, 2, 8, 9, gable_house_full_roof_side_a_color, gable_house_full_roof_side_a_shading),
        face_3d(gable_house_full_vertices, vertex_3d(1.0, 0.0, 0.0), 6, 0, 5, 10, gable_house_full_roof_side_a_color, gable_house_full_roof_side_a_shading),
        face_3d(gable_house_full_vertices, vertex_3d(-1.0, 0.0, 0.0), 3, 9, 11, 4, gable_house_full_roof_side_a_color, gable_house_full_roof_side_a_shading),
        face_3d(gable_house_full_vertices, vertex_3d(0.0, 0.0, 1.0), 11, 10, 5, 4, gable_house_full_roof_side_b_color, gable_house_full_roof_side_b_shading),
        face_3d(gable_house_full_vertices, vertex_3d(0.0, 0.0, -1.0), 7, 8, 2, 1, gable_house_full_roof_side_b_color, gable_house_full_roof_side_b_shading),
        face_3d(gable_house_full_vertices, vertex_3d(0.0, -0.7291763361991146, 0.6843258512779097), 6, 9, 8, 7, gable_house_full_roof_bottom_a_color, gable_house_full_roof_bottom_a_shading),
        face_3d(gable_house_full_vertices, vertex_3d(0.0, -0.7291763361991146, -0.6843258512779097), 10, 11, 9, 6, gable_house_full_roof_bottom_b_color, gable_house_full_roof_bottom_b_shading),
        face_3d(gable_house_full_vertices, vertex_3d(0.0, 1.0, 0.0), 12, 13, 14, 15, gable_house_full_chimney_top_color, gable_house_full_chimney_top_shading),
        face_3d(gable_house_full_vertices, vertex_3d(-1.0, 0.0, 0.0), 16, 17, 18, 19, gable_house_full_wall_front_color, gable_house_full_wall_front_shading),
        face_3d(gable_house_full_vertices, vertex_3d(-1.0, 0.0, 0.0), 20, 18, 21, gable_house_full_wall_front_color, gable_house_full_wall_front_shading),
        face_3d(gable_house_full_vertices, vertex_3d(-1.0, -0.0, 0.0), 22, 16, 19, 23, gable_house_full_wall_front_color, gable_house_full_wall_front_shading),
        face_3d(gable_house_full_vertices, vertex_3d(-1.0, 0.0, 0.0), 17, 24, 21, 18, gable_house_full_wall_front_color, gable_house_full_wall_front_shading),
        face_3d(gable_house_full_vertices, vertex_3d(-1.0, 0.0, 0.0), 21, 24, 22, 23, gable_house_full_wall_front_color, gable_house_full_wall_front_shading),
        face_3d(gable_house_full_vertices, vertex_3d(-1.0, -0.0, 0.0), 25, 26, 15, 14, gable_house_full_wall_front_color, gable_house_full_wall_front_shading),
        face_3d(gable_house_full_vertices, vertex_3d(0.0, 0.0, 1.0), 27, 21, 23, 28, gable_house_full_wall_side_color, gable_house_full_wall_side_shading),
        face_3d(gable_house_full_vertices, vertex_3d(0.0, 0.0, -1.0), 29, 19, 18, 30, gable_house_full_wall_side_color, gable_house_full_wall_side_shading),
        face_3d(gable_house_full_vertices, vertex_3d(0.0, 0.0, 1.0), 26, 31, 12, 15, gable_house_full_wall_side_color, gable_house_full_wall_side_shading),
        face_3d(gable_house_full_vertices, vertex_3d(0.0, 0.0, -1.0), 32, 25, 14, 13, gable_house_full_wall_side_color, gable_house_full_wall_side_shading),
        face_3d(gable_house_full_vertices, vertex_3d(1.0, 0.0, 0.0), 30, 33, 27, gable_house_full_wall_back_color, gable_house_full_wall_back_shading),
        face_3d(gable_house_full_vertices, vertex_3d(1.0, 0.0, 0.0), 28, 42, 40, 27, gable_house_full_wall_back_color, gable_house_full_wall_back_shading),
        face_3d(gable_house_full_vertices, vertex_3d(1.0, 0.0, 0.0), 27, 40, 41, 30, gable_house_full_wall_back_color, gable_house_full_wall_back_shading),
        face_3d(gable_house_full_vertices, vertex_3d(1.0, 0.0, 0.0), 30, 41, 43, 29, gable_house_full_wall_back_color, gable_house_full_wall_back_shading),
        face_3d(gable_house_full_vertices, vertex_3d(1.0, 0.0, 0.0), 31, 32, 13, 12, gable_house_full_chimney_back_color, gable_house_full_chimney_back_shading),
        face_3d(gable_house_full_vertices, vertex_3d(-1.0, 0.0, 0.0), 22, 24, 17, 16, gable_house_full_window_color, gable_house_full_window_shading),
        face_3d(gable_house_full_vertices, vertex_3d(-1.0, -0.0, 0.0), 34, 35, 23, 19, gable_house_full_foot_front_color, gable_house_full_foot_front_shading),
        face_3d(gable_house_full_vertices, vertex_3d(0.0, 0.0, -1.0), 36, 34, 19, 29, gable_house_full_foot_side_color, gable_house_full_foot_side_shading),
        face_3d(gable_house_full_vertices, vertex_3d(-0.0, 0.0, 1.0), 37, 28, 23, 35, gable_house_full_foot_side_color, gable_house_full_foot_side_shading),
        face_3d(gable_house_full_vertices, vertex_3d(1.0, 0.0, 0.0), 37, 38, 42, 28, gable_house_full_foot_back_color, gable_house_full_foot_back_shading),
        face_3d(gable_house_full_vertices, vertex_3d(1.0, 0.0, 0.0), 43, 39, 36, 29, gable_house_full_foot_back_color, gable_house_full_foot_back_shading),
        face_3d(gable_house_full_vertices, vertex_3d(0.0, -1.0, 0.0), 35, 34, 36, 37, gable_house_full_foot_down_color, gable_house_full_foot_down_shading),
        face_3d(gable_house_full_vertices, vertex_3d(1.0, 0.0, 0.0), 38, 39, 41, 40, gable_house_full_door_color, gable_house_full_door_shading),
    };

    constexpr inline model_3d_item gable_house_full(gable_house_full_vertices, gable_house_full_faces);
}

#endif

