/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * CA3
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_BUTANO_HOUSE_FULL_H
#define FR_MODEL_3D_ITEMS_BUTANO_HOUSE_FULL_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d butano_house_full_vertices[] = {
        vertex_3d(4.5, 44.0, 4.5),
        vertex_3d(4.5, 44.0, -4.5),
        vertex_3d(-4.5, 44.0, -4.5),
        vertex_3d(-4.5, 44.0, 4.5),
        vertex_3d(4.5, 36.5, 4.5),
        vertex_3d(4.5, 36.5, -4.5),
        vertex_3d(-4.5, 36.5, -4.5),
        vertex_3d(-4.5, 36.5, 4.5),
        vertex_3d(-12.0, 33.5, -7.5),
        vertex_3d(-7.5, 35.0, -7.5),
        vertex_3d(-5.0, 37.0, -15.0),
        vertex_3d(-9.0, 38.0, -18.0),
        vertex_3d(5.0, 37.0, -15.0),
        vertex_3d(7.5, 35.0, -7.5),
        vertex_3d(12.0, 33.5, -7.5),
        vertex_3d(9.0, 38.0, -18.0),
        vertex_3d(-12.0, 33.5, 7.5),
        vertex_3d(-9.0, 38.0, 18.0),
        vertex_3d(-5.0, 37.0, 15.0),
        vertex_3d(-7.5, 35.0, 7.5),
        vertex_3d(5.0, 37.0, 15.0),
        vertex_3d(9.0, 38.0, 18.0),
        vertex_3d(12.0, 33.5, 7.5),
        vertex_3d(7.5, 35.0, 7.5),
        vertex_3d(9.0, 36.5, -22.5),
        vertex_3d(-9.0, 36.5, -22.5),
        vertex_3d(-9.0, 36.5, 22.5),
        vertex_3d(9.0, 36.5, 22.5),
        vertex_3d(-12.0, 32.0, -12.0),
        vertex_3d(12.0, 32.0, -12.0),
        vertex_3d(-12.0, 32.0, 12.0),
        vertex_3d(12.0, 32.0, 12.0),
        vertex_3d(24.0, 30.5, 0.0),
        vertex_3d(18.0, 26.0, -18.0),
        vertex_3d(0.0, 30.5, -24.0),
        vertex_3d(-18.0, 26.0, 18.0),
        vertex_3d(0.0, 30.5, 24.0),
        vertex_3d(-24.0, 30.5, 0.0),
        vertex_3d(18.0, 26.0, 18.0),
        vertex_3d(-18.0, 26.0, -18.0),
        vertex_3d(-24.0, -35.0, 0.0),
        vertex_3d(-18.0, -35.0, 18.0),
        vertex_3d(24.0, -35.0, 0.0),
        vertex_3d(18.0, -35.0, -18.0),
        vertex_3d(0.0, -35.0, -24.0),
        vertex_3d(-18.0, -35.0, -18.0),
        vertex_3d(18.0, -35.0, 18.0),
        vertex_3d(0.0, -35.0, 24.0),
        vertex_3d(4.5, 36.5, 4.5),
        vertex_3d(-4.5, 36.5, 4.5),
        vertex_3d(0.0, 30.5, 24.0),
        vertex_3d(-2.25, 33.5, 14.25),
        vertex_3d(2.25, 33.5, 14.25),
        vertex_3d(2.25, 33.5, -14.25),
        vertex_3d(-2.25, 33.5, -14.25),
        vertex_3d(0.0, 38.0, -18.0),
        vertex_3d(0.0, 36.5, -22.5),
        vertex_3d(0.0, 37.0, -15.0),
        vertex_3d(0.0, 38.0, 18.0),
        vertex_3d(0.0, 36.5, 22.5),
        vertex_3d(0.0, 37.0, 15.0),
    };

    constexpr inline int butano_house_full_cap_top_color = 6;
    constexpr inline int butano_house_full_cap_top_shading = 7;

    constexpr inline int butano_house_full_cap_a_color = 6;
    constexpr inline int butano_house_full_cap_a_shading = 6;

    constexpr inline int butano_house_full_cap_b_color = 6;
    constexpr inline int butano_house_full_cap_b_shading = 0;

    constexpr inline int butano_house_full_handle_up_color = 6;
    constexpr inline int butano_house_full_handle_up_shading = 6;

    constexpr inline int butano_house_full_handle_ext_a_color = 6;
    constexpr inline int butano_house_full_handle_ext_a_shading = 6;

    constexpr inline int butano_house_full_handle_ext_b_color = 6;
    constexpr inline int butano_house_full_handle_ext_b_shading = 0;

    constexpr inline int butano_house_full_handle_bottom_color = 6;
    constexpr inline int butano_house_full_handle_bottom_shading = 0;

    constexpr inline int butano_house_full_cylinder_top_a_color = 0;
    constexpr inline int butano_house_full_cylinder_top_a_shading = 6;

    constexpr inline int butano_house_full_cylinder_top_b_color = 0;
    constexpr inline int butano_house_full_cylinder_top_b_shading = 7;

    constexpr inline int butano_house_full_cylinder_a_color = 0;
    constexpr inline int butano_house_full_cylinder_a_shading = 1;

    constexpr inline int butano_house_full_cylinder_b_color = 0;
    constexpr inline int butano_house_full_cylinder_b_shading = 0;

    constexpr inline int butano_house_full_cylinder_bottom_color = 6;
    constexpr inline int butano_house_full_cylinder_bottom_shading = 0;

    constexpr inline face_3d butano_house_full_faces[] = {
        face_3d(butano_house_full_vertices, vertex_3d(-0.0, 1.0, 0.0), 0, 1, 2, 3, butano_house_full_cap_top_color, butano_house_full_cap_top_shading),
        face_3d(butano_house_full_vertices, vertex_3d(1.0, 0.0, -0.0), 0, 4, 5, 1, butano_house_full_cap_a_color, butano_house_full_cap_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-1.0, 0.0, 0.0), 3, 2, 6, 7, butano_house_full_cap_a_color, butano_house_full_cap_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.0, 0.0, 1.0), 3, 7, 4, 0, butano_house_full_cap_b_color, butano_house_full_cap_b_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.0, 0.0, -1.0), 2, 1, 5, 6, butano_house_full_cap_b_color, butano_house_full_cap_b_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-0.054312544659356844, 0.9233132592090663, 0.3801878126154979), 8, 9, 10, 11, butano_house_full_handle_up_color, butano_house_full_handle_up_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.054312544659356844, 0.9233132592090663, 0.3801878126154979), 12, 13, 14, 15, butano_house_full_handle_up_color, butano_house_full_handle_up_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.0, 0.9486832980505138, 0.31622776601683794), 55, 57, 12, 15, butano_house_full_handle_up_color, butano_house_full_handle_up_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-0.054312544659356844, 0.9233132592090663, -0.3801878126154979), 16, 17, 18, 19, butano_house_full_handle_up_color, butano_house_full_handle_up_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.054312544659356844, 0.9233132592090663, -0.3801878126154979), 20, 21, 22, 23, butano_house_full_handle_up_color, butano_house_full_handle_up_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.0, 0.9486832980505138, -0.31622776601683794), 60, 18, 17, 58, butano_house_full_handle_up_color, butano_house_full_handle_up_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.0, 0.9486832980505138, 0.31622776601683794), 11, 10, 57, 55, butano_house_full_handle_up_color, butano_house_full_handle_up_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.0, 0.9486832980505138, -0.31622776601683794), 21, 20, 60, 58, butano_house_full_handle_up_color, butano_house_full_handle_up_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.0, 0.9486832980505138, 0.31622776601683794), 17, 26, 59, 58, butano_house_full_handle_ext_a_color, butano_house_full_handle_ext_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.0, 0.9486832980505138, -0.31622776601683794), 55, 15, 24, 56, butano_house_full_handle_ext_a_color, butano_house_full_handle_ext_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.0, 0.9486832980505138, -0.31622776601683794), 56, 25, 11, 55, butano_house_full_handle_ext_a_color, butano_house_full_handle_ext_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-0.0, 0.9486832980505138, 0.31622776601683794), 27, 21, 58, 59, butano_house_full_handle_ext_a_color, butano_house_full_handle_ext_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.9299811099505542, 0.3487429162314578, -0.11624763874381927), 14, 29, 24, 15, butano_house_full_handle_ext_b_color, butano_house_full_handle_ext_b_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-0.9299811099505542, 0.3487429162314578, 0.11624763874381927), 16, 30, 26, 17, butano_house_full_handle_ext_b_color, butano_house_full_handle_ext_b_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.9299811099505542, 0.3487429162314578, 0.11624763874381927), 31, 22, 21, 27, butano_house_full_handle_ext_b_color, butano_house_full_handle_ext_b_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-0.9299811099505542, 0.3487429162314578, -0.11624763874381927), 8, 11, 25, 28, butano_house_full_handle_ext_b_color, butano_house_full_handle_ext_b_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-0.6421175024741438, -0.7537901116000818, -0.13959076140742258), 13, 12, 24, 29, butano_house_full_handle_bottom_color, butano_house_full_handle_bottom_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.6421175024741438, -0.7537901116000818, -0.13959076140742258), 9, 28, 25, 10, butano_house_full_handle_bottom_color, butano_house_full_handle_bottom_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-0.0, -0.997785157856609, -0.06651901052377393), 20, 27, 26, 18, butano_house_full_handle_bottom_color, butano_house_full_handle_bottom_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.6421175024741438, -0.7537901116000818, 0.13959076140742258), 19, 18, 26, 30, butano_house_full_handle_bottom_color, butano_house_full_handle_bottom_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-0.6421175024741438, -0.7537901116000818, 0.13959076140742258), 23, 31, 27, 20, butano_house_full_handle_bottom_color, butano_house_full_handle_bottom_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.0, -0.997785157856609, 0.06651901052377393), 10, 25, 24, 12, butano_house_full_handle_bottom_color, butano_house_full_handle_bottom_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.4275400916690467, 0.8331550504319886, -0.3508021264976794), 29, 14, 32, 33, butano_house_full_cylinder_top_a_color, butano_house_full_cylinder_top_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.32008885332023684, 0.9374030704378364, 0.13718093713724436), 22, 32, 4, 23, butano_house_full_cylinder_top_a_color, butano_house_full_cylinder_top_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.32008885332023684, 0.9374030704378364, -0.13718093713724436), 14, 13, 5, 32, butano_house_full_cylinder_top_a_color, butano_house_full_cylinder_top_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.4275400916690467, 0.8331550504319886, 0.3508021264976794), 31, 38, 32, 22, butano_house_full_cylinder_top_a_color, butano_house_full_cylinder_top_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.35629061899150133, 0.8143785576948601, 0.45808793870335884), 38, 31, 36, butano_house_full_cylinder_top_a_color, butano_house_full_cylinder_top_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.21791627586898657, 0.9175422141852065, -0.33260905264213736), 13, 29, 34, 5, butano_house_full_cylinder_top_a_color, butano_house_full_cylinder_top_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.35629061899150133, 0.8143785576948601, -0.45808793870335884), 33, 34, 29, butano_house_full_cylinder_top_a_color, butano_house_full_cylinder_top_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-0.32008885332023684, 0.9374030704378364, -0.13718093713724436), 8, 37, 6, 9, butano_house_full_cylinder_top_a_color, butano_house_full_cylinder_top_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-0.4275400916690467, 0.8331550504319886, -0.3508021264976794), 28, 39, 37, 8, butano_house_full_cylinder_top_a_color, butano_house_full_cylinder_top_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-0.35629061899150133, 0.8143785576948601, -0.45808793870335884), 39, 28, 34, butano_house_full_cylinder_top_a_color, butano_house_full_cylinder_top_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-0.32008885332023684, 0.9374030704378364, 0.13718093713724436), 16, 19, 7, 37, butano_house_full_cylinder_top_a_color, butano_house_full_cylinder_top_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-0.4275400916690467, 0.8331550504319886, 0.3508021264976794), 16, 37, 35, 30, butano_house_full_cylinder_top_a_color, butano_house_full_cylinder_top_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-0.21791627586898657, 0.9175422141852065, -0.33260905264213736), 9, 6, 34, 28, butano_house_full_cylinder_top_a_color, butano_house_full_cylinder_top_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-0.35629061899150133, 0.8143785576948601, 0.45808793870335884), 35, 36, 30, butano_house_full_cylinder_top_a_color, butano_house_full_cylinder_top_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-0.21791627586898657, 0.9175422141852065, 0.33260905264213736), 30, 50, 49, 19, butano_house_full_cylinder_top_a_color, butano_house_full_cylinder_top_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.21791627586898657, 0.9175422141852065, 0.33260905264213736), 50, 31, 23, 48, butano_house_full_cylinder_top_a_color, butano_house_full_cylinder_top_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.2940858488375231, 0.9557790087219501, -0.0), 4, 32, 5, butano_house_full_cylinder_top_b_color, butano_house_full_cylinder_top_b_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.0, 0.9557790087219501, -0.2940858488375231), 34, 54, 53, butano_house_full_cylinder_top_b_color, butano_house_full_cylinder_top_b_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-0.2940858488375231, 0.9557790087219501, 0.0), 37, 7, 6, butano_house_full_cylinder_top_b_color, butano_house_full_cylinder_top_b_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.0, 0.9557790087219501, 0.2940858488375231), 51, 50, 52, butano_house_full_cylinder_top_b_color, butano_house_full_cylinder_top_b_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.0, 0.9557790087219501, -0.2940858488375231), 5, 53, 54, 6, butano_house_full_cylinder_top_b_color, butano_house_full_cylinder_top_b_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.0, 0.9557790087219501, 0.2940858488375231), 49, 51, 52, 48, butano_house_full_cylinder_top_b_color, butano_house_full_cylinder_top_b_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-0.9486832980505138, 0.0, 0.31622776601683794), 35, 37, 40, 41, butano_house_full_cylinder_a_color, butano_house_full_cylinder_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.9486832980505138, 0.0, -0.31622776601683794), 42, 43, 33, 32, butano_house_full_cylinder_a_color, butano_house_full_cylinder_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-0.31622776601683794, 0.0, -0.9486832980505138), 34, 44, 45, 39, butano_house_full_cylinder_a_color, butano_house_full_cylinder_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.31622776601683794, 0.0, 0.9486832980505138), 46, 38, 36, 47, butano_house_full_cylinder_a_color, butano_house_full_cylinder_a_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-0.9486832980505138, 0.0, -0.31622776601683794), 37, 39, 45, 40, butano_house_full_cylinder_b_color, butano_house_full_cylinder_b_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.31622776601683794, 0.0, -0.9486832980505138), 43, 44, 34, 33, butano_house_full_cylinder_b_color, butano_house_full_cylinder_b_shading),
        face_3d(butano_house_full_vertices, vertex_3d(-0.31622776601683794, 0.0, 0.9486832980505138), 36, 35, 41, 47, butano_house_full_cylinder_b_color, butano_house_full_cylinder_b_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.9486832980505138, -0.0, 0.31622776601683794), 38, 46, 42, 32, butano_house_full_cylinder_b_color, butano_house_full_cylinder_b_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.0, -1.0, -0.0), 42, 46, 44, 43, butano_house_full_cylinder_bottom_color, butano_house_full_cylinder_bottom_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.0, -1.0, 0.0), 46, 47, 41, 44, butano_house_full_cylinder_bottom_color, butano_house_full_cylinder_bottom_shading),
        face_3d(butano_house_full_vertices, vertex_3d(0.0, -1.0, 0.0), 40, 45, 44, 41, butano_house_full_cylinder_bottom_color, butano_house_full_cylinder_bottom_shading),
    };

    constexpr inline model_3d_item butano_house_full(butano_house_full_vertices, butano_house_full_faces);
}

#endif

