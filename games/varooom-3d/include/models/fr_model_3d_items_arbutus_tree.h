/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * CA3
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_ARBUTUS_TREE_H
#define FR_MODEL_3D_ITEMS_ARBUTUS_TREE_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d arbutus_tree_vertices[] = {
        vertex_3d(6.14, -28.0, 4.89),
        vertex_3d(-3.59, -28.0, 4.77),
        vertex_3d(-3.64, -28.0, -4.93),
        vertex_3d(6.09, -28.0, -4.81),
        vertex_3d(-1.9, 11.08, -3.07),
        vertex_3d(4.26, 11.08, -3.07),
        vertex_3d(-10.22, 16.0, -11.42),
        vertex_3d(12.58, 16.0, -11.42),
        vertex_3d(4.26, 11.08, 3.1),
        vertex_3d(-1.9, 11.08, 3.1),
        vertex_3d(-10.22, 16.0, 11.38),
        vertex_3d(-9.34, 21.0, 10.5),
        vertex_3d(-9.34, 21.0, -10.54),
        vertex_3d(12.58, 16.0, 11.38),
        vertex_3d(11.7, 21.0, 10.5),
        vertex_3d(11.7, 21.0, -10.54),
        vertex_3d(0.0, 28.0, 0.0),
    };

    constexpr inline int arbutus_tree_green_up_a_color = 5;
    constexpr inline int arbutus_tree_green_up_a_shading = 7;

    constexpr inline int arbutus_tree_green_up_b_color = 5;
    constexpr inline int arbutus_tree_green_up_b_shading = 5;

    constexpr inline int arbutus_tree_green_up_c_color = 5;
    constexpr inline int arbutus_tree_green_up_c_shading = 2;

    constexpr inline int arbutus_tree_green_side_a_color = 5;
    constexpr inline int arbutus_tree_green_side_a_shading = 5;

    constexpr inline int arbutus_tree_green_side_b_color = 5;
    constexpr inline int arbutus_tree_green_side_b_shading = 2;

    constexpr inline int arbutus_tree_green_side_c_color = 5;
    constexpr inline int arbutus_tree_green_side_c_shading = 0;

    constexpr inline int arbutus_tree_green_down_a_color = 5;
    constexpr inline int arbutus_tree_green_down_a_shading = 2;

    constexpr inline int arbutus_tree_green_down_b_color = 5;
    constexpr inline int arbutus_tree_green_down_b_shading = 0;

    constexpr inline int arbutus_tree_green_down_c_color = 5;
    constexpr inline int arbutus_tree_green_down_c_shading = 0;

    constexpr inline int arbutus_tree_brown_side_a_color = 4;
    constexpr inline int arbutus_tree_brown_side_a_shading = 7;

    constexpr inline int arbutus_tree_brown_side_b_color = 4;
    constexpr inline int arbutus_tree_brown_side_b_shading = 5;

    constexpr inline int arbutus_tree_brown_side_c_color = 4;
    constexpr inline int arbutus_tree_brown_side_c_shading = 3;

    constexpr inline int arbutus_tree_brown_down_color = 4;
    constexpr inline int arbutus_tree_brown_down_shading = 1;

    constexpr inline face_3d arbutus_tree_full_faces[] = {
        face_3d(arbutus_tree_vertices, vertex_3d(-0.5997258043991921, 0.8002055732983505, 0.0), 12, 11, 16, arbutus_tree_green_up_a_color, arbutus_tree_green_up_a_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(0.0, 0.8320502943378436, 0.5547001962252291), 11, 14, 16, arbutus_tree_green_up_b_color, arbutus_tree_green_up_b_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(0.0, 0.8330217450985782, -0.553240248167936), 15, 12, 16, arbutus_tree_green_up_b_color, arbutus_tree_green_up_b_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(0.5134167418058964, 0.858139411304141, 0.0), 14, 15, 16, arbutus_tree_green_up_c_color, arbutus_tree_green_up_c_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(-0.984862774053917, 0.17333584823348952, 0.0), 6, 10, 11, 12, arbutus_tree_green_side_a_color, arbutus_tree_green_side_a_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(0.0, 0.17333584823348952, 0.984862774053917), 10, 13, 14, 11, arbutus_tree_green_side_b_color, arbutus_tree_green_side_b_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(0.0, 0.17333584823348952, -0.984862774053917), 7, 6, 12, 15, arbutus_tree_green_side_b_color, arbutus_tree_green_side_b_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(0.984862774053917, 0.17333584823348952, 0.0), 13, 7, 15, 14, arbutus_tree_green_side_c_color, arbutus_tree_green_side_c_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(-0.5090081280287463, -0.8607617124388556, 0.0), 4, 9, 10, 6, arbutus_tree_green_down_a_color, arbutus_tree_green_down_a_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(0.0, -0.8615626379207554, -0.5076512788706725), 5, 4, 6, 7, arbutus_tree_green_down_b_color, arbutus_tree_green_down_b_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(0.0, -0.8596837754248103, 0.5108265911944525), 9, 8, 13, 10, arbutus_tree_green_down_b_color, arbutus_tree_green_down_b_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(0.5090081280287463, -0.8607617124388556, 0.0), 8, 5, 7, 13, arbutus_tree_green_down_c_color, arbutus_tree_green_down_c_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(-0.9990662605775071, 0.043204247194881966, 0.0), 1, 9, 4, 2, arbutus_tree_brown_side_a_color, arbutus_tree_brown_side_a_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(-0.01231947099926069, 0.045160623058553014, 0.9989037735233869), 1, 0, 8, 9, arbutus_tree_brown_side_b_color, arbutus_tree_brown_side_b_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(0.012318430423813236, 0.04699002086565907, -0.9988194001975225), 3, 2, 4, 5, arbutus_tree_brown_side_b_color, arbutus_tree_brown_side_b_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(0.9988814360653361, 0.047003891189454, -0.005148873381780065), 0, 3, 5, 8, arbutus_tree_brown_side_c_color, arbutus_tree_brown_side_c_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(-0.0, -1.0, 0.0), 2, 3, 0, 1, arbutus_tree_brown_down_color, arbutus_tree_brown_down_shading),
    };

    constexpr inline const face_3d* arbutus_tree_collision_face_ptr = &arbutus_tree_full_faces[16];

    constexpr inline model_3d_item arbutus_tree_full(arbutus_tree_vertices, arbutus_tree_full_faces, arbutus_tree_collision_face_ptr);

    constexpr inline face_3d arbutus_tree_faces[] = {
        face_3d(arbutus_tree_vertices, vertex_3d(-0.5997258043991921, 0.8002055732983505, 0.0), 12, 11, 16, arbutus_tree_green_up_a_color, arbutus_tree_green_up_a_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(0.0, 0.8320502943378436, 0.5547001962252291), 11, 14, 16, arbutus_tree_green_up_b_color, arbutus_tree_green_up_b_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(0.0, 0.8330217450985782, -0.553240248167936), 15, 12, 16, arbutus_tree_green_up_b_color, arbutus_tree_green_up_b_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(0.5134167418058964, 0.858139411304141, 0.0), 14, 15, 16, arbutus_tree_green_up_c_color, arbutus_tree_green_up_c_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(-0.984862774053917, 0.17333584823348952, 0.0), 6, 10, 11, 12, arbutus_tree_green_side_a_color, arbutus_tree_green_side_a_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(0.0, 0.17333584823348952, 0.984862774053917), 10, 13, 14, 11, arbutus_tree_green_side_b_color, arbutus_tree_green_side_b_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(0.0, 0.17333584823348952, -0.984862774053917), 7, 6, 12, 15, arbutus_tree_green_side_b_color, arbutus_tree_green_side_b_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(0.984862774053917, 0.17333584823348952, 0.0), 13, 7, 15, 14, arbutus_tree_green_side_c_color, arbutus_tree_green_side_c_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(-0.9990662605775071, 0.043204247194881966, 0.0), 1, 9, 4, 2, arbutus_tree_brown_side_a_color, arbutus_tree_brown_side_a_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(-0.01231947099926069, 0.045160623058553014, 0.9989037735233869), 1, 0, 8, 9, arbutus_tree_brown_side_b_color, arbutus_tree_brown_side_b_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(0.012318430423813236, 0.04699002086565907, -0.9988194001975225), 3, 2, 4, 5, arbutus_tree_brown_side_b_color, arbutus_tree_brown_side_b_shading),
        face_3d(arbutus_tree_vertices, vertex_3d(0.9988814360653361, 0.047003891189454, -0.005148873381780065), 0, 3, 5, 8, arbutus_tree_brown_side_c_color, arbutus_tree_brown_side_c_shading),
    };

    constexpr inline model_3d_item arbutus_tree(arbutus_tree_vertices, arbutus_tree_faces, arbutus_tree_collision_face_ptr);
}

#endif

