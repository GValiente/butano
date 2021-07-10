/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * CA3
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_BIG_TREE_H
#define FR_MODEL_3D_ITEMS_BIG_TREE_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d big_tree_vertices[] = {
        vertex_3d(0.61, 55.29, -37.44),
        vertex_3d(8.11, 62.71, 26.09),
        vertex_3d(-22.27, 23.07, 44.23),
        vertex_3d(27.58, 21.84, 40.82),
        vertex_3d(-27.79, 12.23, -40.9),
        vertex_3d(-24.93, 31.56, 4.36),
        vertex_3d(24.94, 31.41, 1.02),
        vertex_3d(22.08, 12.08, -44.24),
        vertex_3d(-6.19, -7.0, -6.78),
        vertex_3d(-7.57, -5.0, 14.12),
        vertex_3d(6.99, -7.0, -7.67),
        vertex_3d(5.61, -5.0, 13.22),
        vertex_3d(-6.4, -63.99, -0.67),
        vertex_3d(-6.4, -63.99, 12.55),
        vertex_3d(6.82, -63.99, 12.55),
        vertex_3d(6.82, -63.99, -0.67),
    };

    constexpr inline int big_tree_green_a_color = 5;
    constexpr inline int big_tree_green_a_shading = 6;

    constexpr inline int big_tree_green_b_color = 5;
    constexpr inline int big_tree_green_b_shading = 4;

    constexpr inline int big_tree_green_down_a_color = 5;
    constexpr inline int big_tree_green_down_a_shading = 4;

    constexpr inline int big_tree_green_down_b_color = 5;
    constexpr inline int big_tree_green_down_b_shading = 1;

    constexpr inline int big_tree_brown_a_color = 4;
    constexpr inline int big_tree_brown_a_shading = 7;

    constexpr inline int big_tree_brown_b_color = 4;
    constexpr inline int big_tree_brown_b_shading = 3;

    constexpr inline int big_tree_brown_down_color = 4;
    constexpr inline int big_tree_brown_down_shading = 0;

    constexpr inline face_3d big_tree_full_faces[] = {
        face_3d(big_tree_vertices, vertex_3d(0.03675813551110298, 0.8592389202795455, 0.5102522095499327), 0, 5, 6, big_tree_green_a_color, big_tree_green_a_shading),
        face_3d(big_tree_vertices, vertex_3d(-0.06594799935732604, 0.12306163028236283, -0.9902053809856889), 7, 4, 0, big_tree_green_a_color, big_tree_green_a_shading),
        face_3d(big_tree_vertices, vertex_3d(-0.7363042139445517, 0.6501349770188464, 0.1875649652365817), 1, 5, 2, big_tree_green_a_color, big_tree_green_a_shading),
        face_3d(big_tree_vertices, vertex_3d(0.8950634749705064, 0.4434282333201712, 0.04725227685808811), 3, 6, 1, big_tree_green_a_color, big_tree_green_a_shading),
        face_3d(big_tree_vertices, vertex_3d(-0.051266609527768375, 0.6073912685150437, -0.7927468585110973), 5, 1, 6, big_tree_green_b_color, big_tree_green_b_shading),
        face_3d(big_tree_vertices, vertex_3d(0.07249000019560511, 0.36855365746630286, 0.9267758096971738), 1, 2, 3, big_tree_green_b_color, big_tree_green_b_shading),
        face_3d(big_tree_vertices, vertex_3d(0.8505216870391799, 0.4621212003854271, -0.2511112423416341), 7, 0, 6, big_tree_green_b_color, big_tree_green_b_shading),
        face_3d(big_tree_vertices, vertex_3d(-0.8136526220455438, 0.551432981548103, -0.1840952946149929), 5, 0, 4, big_tree_green_b_color, big_tree_green_b_shading),
        face_3d(big_tree_vertices, vertex_3d(-0.8926426474468082, -0.4504869317607085, -0.01583124353852525), 8, 9, 2, 5, big_tree_green_down_a_color, big_tree_green_down_a_shading),
        face_3d(big_tree_vertices, vertex_3d(-0.032091343101259696, -0.8792705871890301, -0.47524033940966703), 10, 8, 4, 7, big_tree_green_down_a_color, big_tree_green_down_a_shading),
        face_3d(big_tree_vertices, vertex_3d(0.8432199663503047, -0.5072759533193301, -0.17790782875454247), 6, 3, 11, big_tree_green_down_a_color, big_tree_green_down_a_shading),
        face_3d(big_tree_vertices, vertex_3d(-0.8373313631096689, -0.4816551209552917, 0.2586204415943314), 5, 4, 8, big_tree_green_down_b_color, big_tree_green_down_b_shading),
        face_3d(big_tree_vertices, vertex_3d(0.8930997914626998, -0.4313293294208452, 0.12778017088205065), 7, 6, 11, 10, big_tree_green_down_b_color, big_tree_green_down_b_shading),
        face_3d(big_tree_vertices, vertex_3d(0.028892080208766935, -0.7280063649948687, 0.6849612983433211), 11, 3, 2, 9, big_tree_green_down_b_color, big_tree_green_down_b_shading),
        face_3d(big_tree_vertices, vertex_3d(-0.9978426392961529, -0.0033524817957672516, -0.06556544873861991), 9, 8, 12, 13, big_tree_brown_a_color, big_tree_brown_a_shading),
        face_3d(big_tree_vertices, vertex_3d(0.9999955509464318, -0.002982966198647014, 0.0), 14, 15, 10, 11, big_tree_brown_a_color, big_tree_brown_a_shading),
        face_3d(big_tree_vertices, vertex_3d(0.0, -0.026605259321497387, 0.9996460174363903), 9, 13, 14, 11, big_tree_brown_b_color, big_tree_brown_b_shading),
        face_3d(big_tree_vertices, vertex_3d(0.0, -0.10660088709100785, -0.9943018912138357), 15, 12, 8, 10, big_tree_brown_b_color, big_tree_brown_b_shading),
        face_3d(big_tree_vertices, vertex_3d(0.0, -1.0, 0.0), 13, 12, 15, 14, big_tree_brown_down_color, big_tree_brown_down_shading),
    };

    constexpr inline const face_3d* big_tree_collision_face_ptr = &big_tree_full_faces[18];

    constexpr inline model_3d_item big_tree_full(big_tree_vertices, big_tree_full_faces, big_tree_collision_face_ptr);

    constexpr inline face_3d big_tree_faces[] = {
        face_3d(big_tree_vertices, vertex_3d(0.03675813551110298, 0.8592389202795455, 0.5102522095499327), 0, 5, 6, big_tree_green_a_color, big_tree_green_a_shading),
        face_3d(big_tree_vertices, vertex_3d(-0.06594799935732604, 0.12306163028236283, -0.9902053809856889), 7, 4, 0, big_tree_green_a_color, big_tree_green_a_shading),
        face_3d(big_tree_vertices, vertex_3d(-0.7363042139445517, 0.6501349770188464, 0.1875649652365817), 1, 5, 2, big_tree_green_a_color, big_tree_green_a_shading),
        face_3d(big_tree_vertices, vertex_3d(0.8950634749705064, 0.4434282333201712, 0.04725227685808811), 3, 6, 1, big_tree_green_a_color, big_tree_green_a_shading),
        face_3d(big_tree_vertices, vertex_3d(-0.051266609527768375, 0.6073912685150437, -0.7927468585110973), 5, 1, 6, big_tree_green_b_color, big_tree_green_b_shading),
        face_3d(big_tree_vertices, vertex_3d(0.07249000019560511, 0.36855365746630286, 0.9267758096971738), 1, 2, 3, big_tree_green_b_color, big_tree_green_b_shading),
        face_3d(big_tree_vertices, vertex_3d(0.8505216870391799, 0.4621212003854271, -0.2511112423416341), 7, 0, 6, big_tree_green_b_color, big_tree_green_b_shading),
        face_3d(big_tree_vertices, vertex_3d(-0.8136526220455438, 0.551432981548103, -0.1840952946149929), 5, 0, 4, big_tree_green_b_color, big_tree_green_b_shading),
        face_3d(big_tree_vertices, vertex_3d(-0.9978426392961529, -0.0033524817957672516, -0.06556544873861991), 9, 8, 12, 13, big_tree_brown_a_color, big_tree_brown_a_shading),
        face_3d(big_tree_vertices, vertex_3d(0.9999955509464318, -0.002982966198647014, 0.0), 14, 15, 10, 11, big_tree_brown_a_color, big_tree_brown_a_shading),
        face_3d(big_tree_vertices, vertex_3d(0.0, -0.026605259321497387, 0.9996460174363903), 9, 13, 14, 11, big_tree_brown_b_color, big_tree_brown_b_shading),
        face_3d(big_tree_vertices, vertex_3d(0.0, -0.10660088709100785, -0.9943018912138357), 15, 12, 8, 10, big_tree_brown_b_color, big_tree_brown_b_shading),
    };

    constexpr inline model_3d_item big_tree(big_tree_vertices, big_tree_faces, big_tree_collision_face_ptr);
}

#endif

