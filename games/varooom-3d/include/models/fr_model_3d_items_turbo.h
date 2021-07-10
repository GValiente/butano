/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_TURBO_H
#define FR_MODEL_3D_ITEMS_TURBO_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d turbo_vertices[] = {
        vertex_3d(0.0, 14.14, -14.14),
        vertex_3d(0.0, 0.0, -20.0),
        vertex_3d(0.0, -14.14, -14.14),
        vertex_3d(12.25, 14.14, -7.07),
        vertex_3d(17.32, 0.0, -10.0),
        vertex_3d(12.25, -14.14, -7.07),
        vertex_3d(12.25, 14.14, 7.07),
        vertex_3d(17.32, 0.0, 10.0),
        vertex_3d(12.25, -14.14, 7.07),
        vertex_3d(0.0, 14.14, 14.14),
        vertex_3d(0.0, 0.0, 20.0),
        vertex_3d(0.0, -14.14, 14.14),
        vertex_3d(0.0, -20.0, 0.0),
        vertex_3d(0.0, 20.0, 0.0),
        vertex_3d(-12.25, 14.14, 7.07),
        vertex_3d(-17.32, 0.0, 10.0),
        vertex_3d(-12.25, -14.14, 7.07),
        vertex_3d(-12.25, 14.14, -7.07),
        vertex_3d(-17.32, 0.0, -10.0),
        vertex_3d(-12.25, -14.14, -7.07),
    };

    constexpr inline int turbo_top_a_color = 7;
    constexpr inline int turbo_top_a_shading = 7;

    constexpr inline int turbo_top_b_color = 7;
    constexpr inline int turbo_top_b_shading = 5;

    constexpr inline int turbo_middle_a_color = 7;
    constexpr inline int turbo_middle_a_shading = 6;

    constexpr inline int turbo_middle_b_color = 7;
    constexpr inline int turbo_middle_b_shading = 4;

    constexpr inline face_3d turbo_faces[] = {
        face_3d(turbo_vertices, vertex_3d(0.2157560029157307, 0.9020515480265191, -0.37383465851735515), 0, 13, 3, turbo_top_a_color, turbo_top_a_shading),
        face_3d(turbo_vertices, vertex_3d(0.43153366796847525, -0.9020968315040653, 0.0), 12, 5, 8, turbo_top_a_color, turbo_top_a_shading),
        face_3d(turbo_vertices, vertex_3d(0.2157560029157307, 0.9020515480265191, 0.37383465851735515), 6, 13, 9, turbo_top_a_color, turbo_top_a_shading),
        face_3d(turbo_vertices, vertex_3d(-0.2157560029157307, -0.9020515480265191, 0.37383465851735515), 12, 11, 16, turbo_top_a_color, turbo_top_a_shading),
        face_3d(turbo_vertices, vertex_3d(-0.43153366796847525, 0.9020968315040653, 0.0), 14, 13, 17, turbo_top_a_color, turbo_top_a_shading),
        face_3d(turbo_vertices, vertex_3d(-0.2157560029157307, -0.9020515480265191, -0.37383465851735515), 12, 19, 2, turbo_top_a_color, turbo_top_a_shading),
        face_3d(turbo_vertices, vertex_3d(0.2157560029157307, -0.9020515480265191, -0.37383465851735515), 12, 2, 5, turbo_top_b_color, turbo_top_b_shading),
        face_3d(turbo_vertices, vertex_3d(0.43153366796847525, 0.9020968315040653, 0.0), 3, 13, 6, turbo_top_b_color, turbo_top_b_shading),
        face_3d(turbo_vertices, vertex_3d(0.2157560029157307, -0.9020515480265191, 0.37383465851735515), 12, 8, 11, turbo_top_b_color, turbo_top_b_shading),
        face_3d(turbo_vertices, vertex_3d(-0.2157560029157307, 0.9020515480265191, 0.37383465851735515), 9, 13, 14, turbo_top_b_color, turbo_top_b_shading),
        face_3d(turbo_vertices, vertex_3d(-0.43153366796847525, -0.9020968315040653, 0.0), 12, 16, 19, turbo_top_b_color, turbo_top_b_shading),
        face_3d(turbo_vertices, vertex_3d(-0.2157560029157307, 0.9020515480265191, -0.37383465851735515), 17, 13, 0, turbo_top_b_color, turbo_top_b_shading),
        face_3d(turbo_vertices, vertex_3d(0.4704761594718717, 0.3378333367445799, -0.8151814644314609), 1, 0, 3, 4, turbo_middle_a_color, turbo_middle_a_shading),
        face_3d(turbo_vertices, vertex_3d(0.9413194520946065, -0.33751694640167296, 0.0), 5, 4, 7, 8, turbo_middle_a_color, turbo_middle_a_shading),
        face_3d(turbo_vertices, vertex_3d(0.4705117571072387, 0.3376348669728741, 0.8152431435026414), 7, 6, 9, 10, turbo_middle_a_color, turbo_middle_a_shading),
        face_3d(turbo_vertices, vertex_3d(-0.47061854463050473, -0.3378042666971853, 0.8151113193000342), 11, 10, 15, 16, turbo_middle_a_color, turbo_middle_a_shading),
        face_3d(turbo_vertices, vertex_3d(-0.9413194520946065, 0.33751694640167296, 0.0), 15, 14, 17, 18, turbo_middle_a_color, turbo_middle_a_shading),
        face_3d(turbo_vertices, vertex_3d(-0.4706437268353451, -0.33766390765559656, -0.8151549348788177), 19, 18, 1, 2, turbo_middle_a_color, turbo_middle_a_shading),
        face_3d(turbo_vertices, vertex_3d(0.47061854463050473, -0.3378042666971853, -0.8151113193000342), 2, 1, 4, 5, turbo_middle_b_color, turbo_middle_b_shading),
        face_3d(turbo_vertices, vertex_3d(0.9413194520946065, 0.33751694640167296, 0.0), 4, 3, 6, 7, turbo_middle_b_color, turbo_middle_b_shading),
        face_3d(turbo_vertices, vertex_3d(0.4706437268353451, -0.33766390765559656, 0.8151549348788177), 8, 7, 10, 11, turbo_middle_b_color, turbo_middle_b_shading),
        face_3d(turbo_vertices, vertex_3d(-0.4704761594718717, 0.3378333367445799, 0.8151814644314609), 10, 9, 14, 15, turbo_middle_b_color, turbo_middle_b_shading),
        face_3d(turbo_vertices, vertex_3d(-0.9413194520946065, -0.33751694640167296, 0.0), 16, 15, 18, 19, turbo_middle_b_color, turbo_middle_b_shading),
        face_3d(turbo_vertices, vertex_3d(-0.4705117571072387, 0.3376348669728741, -0.8152431435026414), 18, 17, 0, 1, turbo_middle_b_color, turbo_middle_b_shading),
    };

    constexpr inline model_3d_item turbo(turbo_vertices, turbo_faces);
}

#endif

