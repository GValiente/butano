/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */
 
#ifndef FR_MODEL_3D_ITEMS_CAMERA_FULL_H
#define FR_MODEL_3D_ITEMS_CAMERA_FULL_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d camera_full_vertices[] = {
        vertex_3d(-12.5, -32.32, 33.21),
        vertex_3d(-12.5, -32.32, 70.71),
        vertex_3d(12.5, -32.32, 70.71),
        vertex_3d(12.5, -32.32, 33.21),
        vertex_3d(12.5, -51.07, 33.21),
        vertex_3d(12.5, -51.07, 70.71),
        vertex_3d(-12.5, -51.07, 33.21),
        vertex_3d(-12.5, -51.07, 70.71),
        vertex_3d(-7.5, 22.68, -30.54),
        vertex_3d(-7.5, 27.68, -30.54),
        vertex_3d(-7.5, 27.68, -53.04),
        vertex_3d(-7.5, 22.68, -53.04),
        vertex_3d(7.5, 22.68, -53.04),
        vertex_3d(7.5, 27.68, -53.04),
        vertex_3d(7.5, 27.68, -30.54),
        vertex_3d(7.5, 22.68, -30.54),
        vertex_3d(7.5, 37.68, -30.54),
        vertex_3d(-7.5, 37.68, -30.54),
        vertex_3d(2.5, 32.68, -30.54),
        vertex_3d(-2.5, 32.68, -30.54),
        vertex_3d(-6.25, -32.32, 64.46),
        vertex_3d(6.25, -32.32, 64.46),
        vertex_3d(6.25, -32.32, 39.46),
        vertex_3d(2.5, 27.68, -30.54),
        vertex_3d(-2.5, 27.68, -30.54),
        vertex_3d(-6.25, -32.32, 39.46),
        vertex_3d(7.5, 32.68, -53.04),
        vertex_3d(-7.5, 32.68, -53.04),
        vertex_3d(3.44, 16.43, -6.79),
        vertex_3d(4.38, 0.18, 16.96),
        vertex_3d(5.31, -16.07, 40.71),
        vertex_3d(5.31, -17.32, 21.96),
        vertex_3d(4.38, -2.32, 4.46),
        vertex_3d(3.44, 12.68, -13.04),
        vertex_3d(-3.44, 16.43, -6.79),
        vertex_3d(-4.38, 0.18, 16.96),
        vertex_3d(-5.31, -16.07, 40.71),
        vertex_3d(-3.44, 12.68, -13.04),
        vertex_3d(-4.38, -2.32, 4.46),
        vertex_3d(-5.31, -17.32, 21.96),
        vertex_3d(2.97, 20.18, -21.79),
        vertex_3d(2.97, 24.55, -18.66),
        vertex_3d(-2.97, 24.55, -18.66),
        vertex_3d(5.78, -24.82, 30.71),
        vertex_3d(-2.97, 20.18, -21.79),
        vertex_3d(-5.78, -24.82, 30.71),
        vertex_3d(5.78, -24.2, 52.59),
        vertex_3d(-5.78, -24.2, 52.59),
    };

    constexpr inline int camera_full_base_up_color = 0;
    constexpr inline int camera_full_base_up_shading = 7;

    constexpr inline int camera_full_base_side_a_color = 0;
    constexpr inline int camera_full_base_side_a_shading = 4;

    constexpr inline int camera_full_base_side_b_color = 0;
    constexpr inline int camera_full_base_side_b_shading = 2;

    constexpr inline int camera_full_base_down_color = 0;
    constexpr inline int camera_full_base_down_shading = 0;

    constexpr inline int camera_full_elevator_up_color = 6;
    constexpr inline int camera_full_elevator_up_shading = 7;

    constexpr inline int camera_full_elevator_side_color = 6;
    constexpr inline int camera_full_elevator_side_shading = 4;

    constexpr inline int camera_full_elevator_down_color = 6;
    constexpr inline int camera_full_elevator_down_shading = 1;

    constexpr inline int camera_full_crystal_up_color = 1;
    constexpr inline int camera_full_crystal_up_shading = 4;

    constexpr inline int camera_full_crystal_side_a_color = 1;
    constexpr inline int camera_full_crystal_side_a_shading = 2;

    constexpr inline int camera_full_crystal_side_b_color = 1;
    constexpr inline int camera_full_crystal_side_b_shading = 0;

    constexpr inline face_3d camera_full_faces[] = {
        face_3d(camera_full_vertices, vertex_3d(0.0, 1.0, 0.0), 1, 20, 25, 0, camera_full_base_up_color, camera_full_base_up_shading),
        face_3d(camera_full_vertices, vertex_3d(0.0, 1.0, -0.0), 1, 2, 21, 20, camera_full_base_up_color, camera_full_base_up_shading),
        face_3d(camera_full_vertices, vertex_3d(-0.0, 1.0, 0.0), 2, 3, 22, 21, camera_full_base_up_color, camera_full_base_up_shading),
        face_3d(camera_full_vertices, vertex_3d(0.0, 1.0, 0.0), 3, 0, 25, 22, camera_full_base_up_color, camera_full_base_up_shading),
        face_3d(camera_full_vertices, vertex_3d(1.0, 0.0, 0.0), 4, 3, 2, 5, camera_full_base_side_a_color, camera_full_base_side_a_shading),
        face_3d(camera_full_vertices, vertex_3d(-1.0, 0.0, 0.0), 6, 7, 1, 0, camera_full_base_side_a_color, camera_full_base_side_a_shading),
        face_3d(camera_full_vertices, vertex_3d(-1.0, 0.0, 0.0), 8, 9, 10, 11, camera_full_base_side_a_color, camera_full_base_side_a_shading),
        face_3d(camera_full_vertices, vertex_3d(1.0, 0.0, 0.0), 12, 13, 14, 15, camera_full_base_side_a_color, camera_full_base_side_a_shading),
        face_3d(camera_full_vertices, vertex_3d(0.0, -0.0, -1.0), 12, 11, 10, 13, camera_full_base_side_b_color, camera_full_base_side_b_shading),
        face_3d(camera_full_vertices, vertex_3d(0.0, 0.0, 1.0), 7, 5, 2, 1, camera_full_base_side_b_color, camera_full_base_side_b_shading),
        face_3d(camera_full_vertices, vertex_3d(0.0, -0.0, -1.0), 4, 6, 0, 3, camera_full_base_side_b_color, camera_full_base_side_b_shading),
        face_3d(camera_full_vertices, vertex_3d(0.0, 0.0, 1.0), 17, 8, 24, 19, camera_full_base_side_b_color, camera_full_base_side_b_shading),
        face_3d(camera_full_vertices, vertex_3d(0.0, 0.0, 1.0), 17, 19, 18, 16, camera_full_base_side_b_color, camera_full_base_side_b_shading),
        face_3d(camera_full_vertices, vertex_3d(0.0, -0.0, 1.0), 18, 23, 15, 16, camera_full_base_side_b_color, camera_full_base_side_b_shading),
        face_3d(camera_full_vertices, vertex_3d(0.0, 0.0, 1.0), 15, 23, 24, 8, camera_full_base_side_b_color, camera_full_base_side_b_shading),
        face_3d(camera_full_vertices, vertex_3d(0.0, -1.0, 0.0), 6, 4, 5, 7, camera_full_base_down_color, camera_full_base_down_shading),
        face_3d(camera_full_vertices, vertex_3d(0.0, -1.0, 0.0), 11, 12, 15, 8, camera_full_base_down_color, camera_full_base_down_shading),
        face_3d(camera_full_vertices, vertex_3d(0.0, 0.8253072612498318, 0.5646839155919903), 35, 29, 28, 34, camera_full_elevator_up_color, camera_full_elevator_up_shading),
        face_3d(camera_full_vertices, vertex_3d(0.0, 0.8253584237605818, 0.5646091323450657), 46, 47, 20, 21, camera_full_elevator_up_color, camera_full_elevator_up_shading),
        face_3d(camera_full_vertices, vertex_3d(-0.0, 0.825358423760582, 0.5646091323450654), 41, 42, 34, 28, camera_full_elevator_up_color, camera_full_elevator_up_shading),
        face_3d(camera_full_vertices, vertex_3d(0.0, 0.825256141374061, 0.5647586220009357), 42, 41, 18, 19, camera_full_elevator_up_color, camera_full_elevator_up_shading),
        face_3d(camera_full_vertices, vertex_3d(0.0, 0.8252561413740612, 0.5647586220009355), 47, 46, 30, 36, camera_full_elevator_up_color, camera_full_elevator_up_shading),
        face_3d(camera_full_vertices, vertex_3d(-0.0, 0.8253072612498318, 0.5646839155919902), 35, 36, 30, 29, camera_full_elevator_up_color, camera_full_elevator_up_shading),
        face_3d(camera_full_vertices, vertex_3d(0.9983290476272406, 0.05778505571241414, 0.0), 21, 22, 43, 46, camera_full_elevator_side_color, camera_full_elevator_side_shading),
        face_3d(camera_full_vertices, vertex_3d(-0.9993540487310082, 0.030815929858520767, -0.01848955791511246), 37, 34, 42, 44, camera_full_elevator_side_color, camera_full_elevator_side_shading),
        face_3d(camera_full_vertices, vertex_3d(-0.998959738847936, 0.04471534069128857, -0.008943068138257711), 37, 38, 35, 34, camera_full_elevator_side_color, camera_full_elevator_side_shading),
        face_3d(camera_full_vertices, vertex_3d(-0.9983290476272406, 0.057785055712414136, 0.0), 25, 20, 47, 45, camera_full_elevator_side_color, camera_full_elevator_side_shading),
        face_3d(camera_full_vertices, vertex_3d(0.9986369615060559, 0.05207845088975504, -0.0034718967259836694), 30, 31, 32, 29, camera_full_elevator_side_color, camera_full_elevator_side_shading),
        face_3d(camera_full_vertices, vertex_3d(0.998959738847936, 0.04471534069128857, -0.008943068138257718), 33, 28, 29, 32, camera_full_elevator_side_color, camera_full_elevator_side_shading),
        face_3d(camera_full_vertices, vertex_3d(-0.9984615287866587, 0.05542660716561673, -0.0015705894169416098), 45, 47, 36, 39, camera_full_elevator_side_color, camera_full_elevator_side_shading),
        face_3d(camera_full_vertices, vertex_3d(-0.9986369615060559, 0.05207845088975504, -0.0034718967259836694), 38, 39, 36, 35, camera_full_elevator_side_color, camera_full_elevator_side_shading),
        face_3d(camera_full_vertices, vertex_3d(0.9993540487310082, 0.030815929858520767, -0.018489557915112458), 40, 41, 28, 33, camera_full_elevator_side_color, camera_full_elevator_side_shading),
        face_3d(camera_full_vertices, vertex_3d(0.9994672024775384, 0.019005541464723948, -0.026534893354902143), 18, 41, 40, 23, camera_full_elevator_side_color, camera_full_elevator_side_shading),
        face_3d(camera_full_vertices, vertex_3d(-0.9994672024775384, 0.01900554146472395, -0.02653489335490215), 24, 44, 42, 19, camera_full_elevator_side_color, camera_full_elevator_side_shading),
        face_3d(camera_full_vertices, vertex_3d(0.9984615287866587, 0.05542660716561672, -0.0015705894169416117), 43, 31, 30, 46, camera_full_elevator_side_color, camera_full_elevator_side_shading),
        face_3d(camera_full_vertices, vertex_3d(-0.0, -0.7592566023652966, -0.6507913734559685), 31, 43, 45, 39, camera_full_elevator_down_color, camera_full_elevator_down_shading),
        face_3d(camera_full_vertices, vertex_3d(-0.0, -0.7592566023652965, -0.6507913734559685), 37, 33, 32, 38, camera_full_elevator_down_color, camera_full_elevator_down_shading),
        face_3d(camera_full_vertices, vertex_3d(-0.0, -0.7592566023652966, -0.6507913734559685), 39, 38, 32, 31, camera_full_elevator_down_color, camera_full_elevator_down_shading),
        face_3d(camera_full_vertices, vertex_3d(-0.0, -0.7592566023652966, -0.6507913734559685), 37, 44, 40, 33, camera_full_elevator_down_color, camera_full_elevator_down_shading),
        face_3d(camera_full_vertices, vertex_3d(0.0, -0.7592566023652966, -0.6507913734559685), 23, 40, 44, 24, camera_full_elevator_down_color, camera_full_elevator_down_shading),
        face_3d(camera_full_vertices, vertex_3d(0.0, -0.7592566023652966, -0.6507913734559685), 22, 25, 45, 43, camera_full_elevator_down_color, camera_full_elevator_down_shading),
        face_3d(camera_full_vertices, vertex_3d(0.0, 0.9761870601839527, -0.21693045781865616), 17, 16, 26, 27, camera_full_crystal_up_color, camera_full_crystal_up_shading),
        face_3d(camera_full_vertices, vertex_3d(-1.0, 0.0, 0.0), 17, 27, 10, 9, camera_full_crystal_side_a_color, camera_full_crystal_side_a_shading),
        face_3d(camera_full_vertices, vertex_3d(1.0, 0.0, -0.0), 16, 14, 13, 26, camera_full_crystal_side_a_color, camera_full_crystal_side_a_shading),
        face_3d(camera_full_vertices, vertex_3d(0.0, -0.0, -1.0), 13, 10, 27, 26, camera_full_crystal_side_b_color, camera_full_crystal_side_b_shading),
    };

    constexpr inline model_3d_item camera_full(camera_full_vertices, camera_full_faces);
}

#endif

