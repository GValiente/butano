/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * CA3
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_GABLE_HOUSE_H
#define FR_MODEL_3D_ITEMS_GABLE_HOUSE_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d gable_house_vertices[] = {
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
        vertex_3d(-43.72, -47.2, -32.2),
        vertex_3d(-43.72, -47.2, 32.2),
        vertex_3d(43.72, -47.2, -32.2),
        vertex_3d(43.72, -47.2, 32.2),
    };

    constexpr inline int gable_house_roof_top_a_color = 0;
    constexpr inline int gable_house_roof_top_a_shading = 7;

    constexpr inline int gable_house_roof_top_b_color = 0;
    constexpr inline int gable_house_roof_top_b_shading = 6;

    constexpr inline int gable_house_roof_side_a_color = 0;
    constexpr inline int gable_house_roof_side_a_shading = 5;

    constexpr inline int gable_house_roof_side_b_color = 0;
    constexpr inline int gable_house_roof_side_b_shading = 2;

    constexpr inline int gable_house_roof_bottom_a_color = 0;
    constexpr inline int gable_house_roof_bottom_a_shading = 1;

    constexpr inline int gable_house_roof_bottom_b_color = 0;
    constexpr inline int gable_house_roof_bottom_b_shading = 0;

    constexpr inline int gable_house_chimney_top_color = 6;
    constexpr inline int gable_house_chimney_top_shading = -1;

    constexpr inline int gable_house_wall_front_color = 2;
    constexpr inline int gable_house_wall_front_shading = 4;

    constexpr inline int gable_house_wall_side_color = 2;
    constexpr inline int gable_house_wall_side_shading = 2;

    constexpr inline int gable_house_wall_back_color = 2;
    constexpr inline int gable_house_wall_back_shading = 0;

    constexpr inline int gable_house_chimney_back_color = 2;
    constexpr inline int gable_house_chimney_back_shading = 0;

    constexpr inline int gable_house_window_color = 1;
    constexpr inline int gable_house_window_shading = -1;

    constexpr inline int gable_house_foot_front_color = 5;
    constexpr inline int gable_house_foot_front_shading = 7;

    constexpr inline int gable_house_foot_side_color = 5;
    constexpr inline int gable_house_foot_side_shading = 5;

    constexpr inline int gable_house_foot_back_color = 5;
    constexpr inline int gable_house_foot_back_shading = 2;

    constexpr inline face_3d gable_house_faces[] = {
        face_3d(gable_house_vertices, vertex_3d(0.0, 0.7366688108814069, -0.6762536972723875), 0, 1, 2, 3, gable_house_roof_top_a_color, gable_house_roof_top_a_shading),
        face_3d(gable_house_vertices, vertex_3d(0.0, 0.7366688108814069, 0.6762536972723875), 0, 3, 4, 5, gable_house_roof_top_b_color, gable_house_roof_top_b_shading),
        face_3d(gable_house_vertices, vertex_3d(1.0, -0.0, 0.0), 1, 0, 6, 7, gable_house_roof_side_a_color, gable_house_roof_side_a_shading),
        face_3d(gable_house_vertices, vertex_3d(-1.0, 0.0, 0.0), 3, 2, 8, 9, gable_house_roof_side_a_color, gable_house_roof_side_a_shading),
        face_3d(gable_house_vertices, vertex_3d(1.0, 0.0, 0.0), 6, 0, 5, 10, gable_house_roof_side_a_color, gable_house_roof_side_a_shading),
        face_3d(gable_house_vertices, vertex_3d(-1.0, 0.0, 0.0), 3, 9, 11, 4, gable_house_roof_side_a_color, gable_house_roof_side_a_shading),
        face_3d(gable_house_vertices, vertex_3d(0.0, 0.0, 1.0), 11, 10, 5, 4, gable_house_roof_side_b_color, gable_house_roof_side_b_shading),
        face_3d(gable_house_vertices, vertex_3d(0.0, 0.0, -1.0), 7, 8, 2, 1, gable_house_roof_side_b_color, gable_house_roof_side_b_shading),
        face_3d(gable_house_vertices, vertex_3d(0.0, 1.0, 0.0), 12, 13, 14, 15, gable_house_chimney_top_color, gable_house_chimney_top_shading),
        face_3d(gable_house_vertices, vertex_3d(-1.0, 0.0, 0.0), 16, 17, 18, 19, gable_house_wall_front_color, gable_house_wall_front_shading),
        face_3d(gable_house_vertices, vertex_3d(-1.0, 0.0, 0.0), 20, 18, 21, gable_house_wall_front_color, gable_house_wall_front_shading),
        face_3d(gable_house_vertices, vertex_3d(-1.0, -0.0, 0.0), 22, 16, 19, 23, gable_house_wall_front_color, gable_house_wall_front_shading),
        face_3d(gable_house_vertices, vertex_3d(-1.0, 0.0, 0.0), 17, 24, 21, 18, gable_house_wall_front_color, gable_house_wall_front_shading),
        face_3d(gable_house_vertices, vertex_3d(-1.0, 0.0, 0.0), 21, 24, 22, 23, gable_house_wall_front_color, gable_house_wall_front_shading),
        face_3d(gable_house_vertices, vertex_3d(-1.0, -0.0, 0.0), 25, 26, 15, 14, gable_house_wall_front_color, gable_house_wall_front_shading),
        face_3d(gable_house_vertices, vertex_3d(0.0, 0.0, 1.0), 27, 21, 23, 28, gable_house_wall_side_color, gable_house_wall_side_shading),
        face_3d(gable_house_vertices, vertex_3d(0.0, 0.0, -1.0), 29, 19, 18, 30, gable_house_wall_side_color, gable_house_wall_side_shading),
        face_3d(gable_house_vertices, vertex_3d(0.0, 0.0, 1.0), 26, 31, 12, 15, gable_house_wall_side_color, gable_house_wall_side_shading),
        face_3d(gable_house_vertices, vertex_3d(0.0, 0.0, -1.0), 32, 25, 14, 13, gable_house_wall_side_color, gable_house_wall_side_shading),
        face_3d(gable_house_vertices, vertex_3d(1.0, 0.0, 0.0), 31, 32, 13, 12, gable_house_chimney_back_color, gable_house_chimney_back_shading),
        face_3d(gable_house_vertices, vertex_3d(-1.0, 0.0, 0.0), 22, 24, 17, 16, gable_house_window_color, gable_house_window_shading),
        face_3d(gable_house_vertices, vertex_3d(-1.0, -0.0, 0.0), 33, 34, 23, 19, gable_house_foot_front_color, gable_house_foot_front_shading),
        face_3d(gable_house_vertices, vertex_3d(0.0, 0.0, -1.0), 35, 33, 19, 29, gable_house_foot_side_color, gable_house_foot_side_shading),
        face_3d(gable_house_vertices, vertex_3d(-0.0, 0.0, 1.0), 36, 28, 23, 34, gable_house_foot_side_color, gable_house_foot_side_shading),
    };

    constexpr inline face_3d gable_house_collision_face(gable_house_vertices, vertex_3d(0.0, -1.0, 0.0), 34, 33, 35, 36, 0, -1);

    constexpr inline model_3d_item gable_house(gable_house_vertices, gable_house_faces, &gable_house_collision_face);
}

#endif

