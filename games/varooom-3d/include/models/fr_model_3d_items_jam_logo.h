/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_JAM_LOGO_H
#define FR_MODEL_3D_ITEMS_JAM_LOGO_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d jam_logo_vertices[] = {
        vertex_3d(-9.5, 2.0, -6.0),
        vertex_3d(-20.0, 2.0, -6.0),
        vertex_3d(-18.5, 2.0, -3.5),
        vertex_3d(-9.5, 2.0, -3.5),
        vertex_3d(-20.0, 2.0, 6.0),
        vertex_3d(-18.5, 2.0, 3.0),
        vertex_3d(-22.0, 2.0, -4.5),
        vertex_3d(-22.0, 2.0, 4.0),
        vertex_3d(-11.0, 2.0, 6.0),
        vertex_3d(-12.5, 2.0, 3.0),
        vertex_3d(-9.5, 2.0, 4.0),
        vertex_3d(-12.5, 2.0, 1.0),
        vertex_3d(-9.5, 2.0, -1.5),
        vertex_3d(-17.0, 2.0, -1.5),
        vertex_3d(-17.0, 2.0, 1.0),
        vertex_3d(-8.5, 2.0, -6.0),
        vertex_3d(-8.5, 2.0, 6.0),
        vertex_3d(-5.5, 2.0, 3.0),
        vertex_3d(-5.5, 2.0, -3.5),
        vertex_3d(2.5, 2.0, -6.0),
        vertex_3d(0.5, 2.0, -3.5),
        vertex_3d(2.5, 2.0, 6.0),
        vertex_3d(0.5, 2.0, 3.0),
        vertex_3d(0.5, 2.0, -1.5),
        vertex_3d(-4.0, 2.0, -1.5),
        vertex_3d(-4.0, 2.0, 1.0),
        vertex_3d(0.5, 2.0, 1.0),
        vertex_3d(2.5, 2.0, 0.0),
        vertex_3d(3.5, 2.0, -0.5),
        vertex_3d(3.5, 2.0, -5.0),
        vertex_3d(3.5, 2.0, 4.5),
        vertex_3d(3.5, 2.0, 0.5),
        vertex_3d(11.75, 2.0, 6.0),
        vertex_3d(11.75, 2.0, 3.0),
        vertex_3d(8.75, 2.0, 3.0),
        vertex_3d(3.5, 2.0, 6.0),
        vertex_3d(11.75, 2.0, -2.0),
        vertex_3d(10.5, 2.0, -6.0),
        vertex_3d(13.0, 2.0, -6.0),
        vertex_3d(16.25, 2.0, 6.0),
        vertex_3d(20.0, 2.0, 6.0),
        vertex_3d(-8.5, 2.0, 4.0),
        vertex_3d(4.38, 2.0, 4.5),
        vertex_3d(10.5, 1.0, -6.5),
        vertex_3d(2.5, 1.0, -6.5),
        vertex_3d(2.5, 1.0, -6.0),
        vertex_3d(10.5, 1.0, -6.0),
        vertex_3d(-21.5, 1.0, -8.0),
        vertex_3d(-24.5, 1.0, -5.0),
        vertex_3d(-24.5, 1.0, 5.0),
        vertex_3d(-21.5, 1.0, 8.0),
        vertex_3d(14.0, 1.0, -8.0),
        vertex_3d(24.0, 1.0, 8.0),
        vertex_3d(15.5, 1.0, -7.0),
        vertex_3d(-21.5, -1.0, 8.0),
        vertex_3d(24.0, -1.0, 8.0),
        vertex_3d(-24.5, -1.0, 5.0),
        vertex_3d(-24.5, -1.0, -5.0),
        vertex_3d(-21.5, -1.0, -8.0),
        vertex_3d(14.0, -1.0, -8.0),
        vertex_3d(15.5, -1.0, -7.0),
        vertex_3d(-20.0, -2.0, -6.0),
        vertex_3d(13.0, -2.0, -6.0),
        vertex_3d(-22.0, -2.0, -4.5),
        vertex_3d(-22.0, -2.0, 4.0),
        vertex_3d(-20.0, -2.0, 6.0),
        vertex_3d(20.0, -2.0, 6.0),
    };

    constexpr inline int jam_logo_white_color = 0;
    constexpr inline int jam_logo_white_shading = -1;

    constexpr inline int jam_logo_purple_front_light_color = 1;
    constexpr inline int jam_logo_purple_front_light_shading = 7;

    constexpr inline int jam_logo_purple_front_dark_color = 1;
    constexpr inline int jam_logo_purple_front_dark_shading = 6;

    constexpr inline int jam_logo_purple_middle_color = 1;
    constexpr inline int jam_logo_purple_middle_shading = 4;

    constexpr inline int jam_logo_purple_back_light_color = 1;
    constexpr inline int jam_logo_purple_back_light_shading = 2;

    constexpr inline int jam_logo_purple_back_dark_color = 1;
    constexpr inline int jam_logo_purple_back_dark_shading = 0;

    constexpr inline face_3d jam_logo_faces[] = {
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, 0.0), 0, 1, 2, 3, jam_logo_white_color, jam_logo_white_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, 0.0), 1, 4, 5, 2, jam_logo_white_color, jam_logo_white_shading),
        face_3d(jam_logo_vertices, vertex_3d(-0.0, 1.0, 0.0), 4, 1, 6, 7, jam_logo_white_color, jam_logo_white_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, -0.0), 4, 8, 9, 5, jam_logo_white_color, jam_logo_white_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, 0.0), 10, 11, 9, 8, jam_logo_white_color, jam_logo_white_shading),
        face_3d(jam_logo_vertices, vertex_3d(-0.0, 1.0, 0.0), 10, 12, 11, jam_logo_white_color, jam_logo_white_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, 0.0), 12, 13, 14, 11, jam_logo_white_color, jam_logo_white_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, 0.0), 15, 16, 17, 18, jam_logo_white_color, jam_logo_white_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, 0.0), 19, 15, 18, 20, jam_logo_white_color, jam_logo_white_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, -0.0), 16, 21, 22, 17, jam_logo_white_color, jam_logo_white_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, 0.0), 19, 20, 22, 21, jam_logo_white_color, jam_logo_white_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, 0.0), 23, 24, 25, 26, jam_logo_white_color, jam_logo_white_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, 0.0), 27, 28, 29, 19, jam_logo_white_color, jam_logo_white_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, 0.0), 27, 21, 30, 31, jam_logo_white_color, jam_logo_white_shading),
        face_3d(jam_logo_vertices, vertex_3d(-0.0, 1.0, 0.0), 32, 33, 34, 35, jam_logo_white_color, jam_logo_white_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, 0.0), 35, 34, 36, 37, jam_logo_white_color, jam_logo_white_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, 0.0), 36, 38, 37, jam_logo_white_color, jam_logo_white_shading),
        face_3d(jam_logo_vertices, vertex_3d(-0.0, 1.0, 0.0), 38, 36, 39, 40, jam_logo_white_color, jam_logo_white_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, 0.0), 32, 39, 36, jam_logo_purple_front_light_color, jam_logo_purple_front_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(-0.0, 1.0, 0.0), 33, 36, 34, jam_logo_purple_front_light_color, jam_logo_purple_front_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, -0.0), 8, 16, 41, 10, jam_logo_purple_front_light_color, jam_logo_purple_front_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, 0.0), 0, 10, 41, 15, jam_logo_purple_front_light_color, jam_logo_purple_front_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, 0.0), 12, 3, 2, 13, jam_logo_purple_front_light_color, jam_logo_purple_front_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, 0.0), 14, 13, 2, 5, jam_logo_purple_front_light_color, jam_logo_purple_front_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(-0.0, 1.0, 0.0), 11, 14, 5, 9, jam_logo_purple_front_light_color, jam_logo_purple_front_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, 0.0), 18, 24, 23, 20, jam_logo_purple_front_light_color, jam_logo_purple_front_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(-0.0, 1.0, 0.0), 22, 26, 25, 17, jam_logo_purple_front_light_color, jam_logo_purple_front_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, -0.0), 21, 35, 42, 30, jam_logo_purple_front_light_color, jam_logo_purple_front_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, 0.0), 37, 29, 30, 42, jam_logo_purple_front_light_color, jam_logo_purple_front_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, 0.0), 29, 37, 19, jam_logo_purple_front_light_color, jam_logo_purple_front_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, -0.0), 27, 31, 28, jam_logo_purple_front_light_color, jam_logo_purple_front_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, 0.0), 43, 44, 45, 46, jam_logo_purple_front_light_color, jam_logo_purple_front_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 1.0, 0.0), 17, 25, 24, 18, jam_logo_purple_front_light_color, jam_logo_purple_front_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(-0.26490647141300877, 0.9271726499455306, -0.26490647141300877), 47, 48, 6, 1, jam_logo_purple_front_dark_color, jam_logo_purple_front_dark_shading),
        face_3d(jam_logo_vertices, vertex_3d(-0.3713906763541037, 0.9284766908852593, 0.0), 48, 49, 7, 6, jam_logo_purple_front_dark_color, jam_logo_purple_front_dark_shading),
        face_3d(jam_logo_vertices, vertex_3d(-0.26490647141300877, 0.9271726499455306, 0.26490647141300877), 49, 50, 4, 7, jam_logo_purple_front_dark_color, jam_logo_purple_front_dark_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 0.8944271909999159, -0.4472135954999579), 51, 47, 1, 38, jam_logo_purple_front_dark_color, jam_logo_purple_front_dark_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 0.8944271909999159, 0.4472135954999579), 50, 52, 40, 4, jam_logo_purple_front_dark_color, jam_logo_purple_front_dark_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.2279211529192759, 0.9116846116771036, -0.3418817293789138), 51, 38, 53, jam_logo_purple_front_dark_color, jam_logo_purple_front_dark_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.3053444999481842, 0.9363897998410982, -0.1730285499706377), 38, 40, 52, 53, jam_logo_purple_front_dark_color, jam_logo_purple_front_dark_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 0.0, 1.0), 50, 54, 55, 52, jam_logo_purple_middle_color, jam_logo_purple_middle_shading),
        face_3d(jam_logo_vertices, vertex_3d(-0.7071067811865476, 0.0, 0.7071067811865476), 49, 56, 54, 50, jam_logo_purple_middle_color, jam_logo_purple_middle_shading),
        face_3d(jam_logo_vertices, vertex_3d(-1.0, 0.0, -0.0), 48, 57, 56, 49, jam_logo_purple_middle_color, jam_logo_purple_middle_shading),
        face_3d(jam_logo_vertices, vertex_3d(-0.7071067811865476, 0.0, -0.7071067811865476), 47, 58, 57, 48, jam_logo_purple_middle_color, jam_logo_purple_middle_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, 0.0, -1.0), 51, 59, 58, 47, jam_logo_purple_middle_color, jam_logo_purple_middle_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.5547001962252291, 0.0, -0.8320502943378437), 53, 60, 59, 51, jam_logo_purple_middle_color, jam_logo_purple_middle_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.8700221858486124, 0.0, -0.49301257198088033), 53, 52, 55, 60, jam_logo_purple_middle_color, jam_logo_purple_middle_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, -0.8944271909999159, -0.4472135954999579), 61, 58, 59, 62, jam_logo_purple_back_light_color, jam_logo_purple_back_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(-0.26490647141300877, -0.9271726499455306, -0.26490647141300877), 61, 63, 57, 58, jam_logo_purple_back_light_color, jam_logo_purple_back_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(-0.3713906763541037, -0.9284766908852593, -0.0), 63, 64, 56, 57, jam_logo_purple_back_light_color, jam_logo_purple_back_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(-0.26490647141300877, -0.9271726499455306, 0.26490647141300877), 56, 64, 65, 54, jam_logo_purple_back_light_color, jam_logo_purple_back_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, -0.8944271909999159, 0.4472135954999579), 55, 54, 65, 66, jam_logo_purple_back_light_color, jam_logo_purple_back_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.3053444999481842, -0.9363897998410982, -0.1730285499706377), 60, 55, 66, 62, jam_logo_purple_back_light_color, jam_logo_purple_back_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.2279211529192759, -0.9116846116771036, -0.3418817293789138), 60, 62, 59, jam_logo_purple_back_light_color, jam_logo_purple_back_light_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, -1.0, 0.0), 63, 61, 65, 64, jam_logo_purple_back_dark_color, jam_logo_purple_back_dark_shading),
        face_3d(jam_logo_vertices, vertex_3d(0.0, -1.0, 0.0), 66, 65, 61, 62, jam_logo_purple_back_dark_color, jam_logo_purple_back_dark_shading),
    };

    constexpr inline model_3d_item jam_logo(jam_logo_vertices, jam_logo_faces);
}

#endif

