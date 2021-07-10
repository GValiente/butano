/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_ANTENNA_H
#define FR_MODEL_3D_ITEMS_ANTENNA_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d antenna_vertices[] = {
        vertex_3d(20.0, -32.0, 20.0),
        vertex_3d(-20.0, -32.0, 20.0),
        vertex_3d(-20.0, -52.0, 20.0),
        vertex_3d(20.0, -52.0, 20.0),
        vertex_3d(-20.0, -32.0, -20.0),
        vertex_3d(20.0, -32.0, -20.0),
        vertex_3d(20.0, -52.0, -20.0),
        vertex_3d(-20.0, -52.0, -20.0),
        vertex_3d(0.0, 52.0, 0.0),
        vertex_3d(16.0, -32.0, -16.0),
        vertex_3d(16.0, -32.0, 16.0),
        vertex_3d(-16.0, -32.0, 16.0),
        vertex_3d(-16.0, -32.0, -16.0),
        vertex_3d(16.0, -32.0, 6.0),
        vertex_3d(6.0, -32.0, 16.0),
        vertex_3d(16.0, -32.0, -6.0),
        vertex_3d(6.0, -32.0, -16.0),
        vertex_3d(-6.0, -32.0, -16.0),
        vertex_3d(-16.0, -32.0, -6.0),
        vertex_3d(-16.0, -32.0, 6.0),
        vertex_3d(-6.0, -32.0, 16.0),
    };

    constexpr inline int antenna_base_top_color = 7;
    constexpr inline int antenna_base_top_shading = 6;

    constexpr inline int antenna_base_side_a_color = 7;
    constexpr inline int antenna_base_side_a_shading = 4;

    constexpr inline int antenna_base_side_b_color = 7;
    constexpr inline int antenna_base_side_b_shading = 2;

    constexpr inline int antenna_base_down_color = 7;
    constexpr inline int antenna_base_down_shading = 0;

    constexpr inline int antenna_middle_a_color = 3;
    constexpr inline int antenna_middle_a_shading = 3;

    constexpr inline int antenna_middle_b_color = 3;
    constexpr inline int antenna_middle_b_shading = 1;

    constexpr inline int antenna_middle_interior_color = 3;
    constexpr inline int antenna_middle_interior_shading = 0;

    constexpr inline face_3d antenna_full_faces[] = {
        face_3d(antenna_vertices, vertex_3d(0.0, 1.0, 0.0), 4, 1, 0, 5, antenna_base_top_color, antenna_base_top_shading),
        face_3d(antenna_vertices, vertex_3d(0.0, 0.0, 1.0), 2, 3, 0, 1, antenna_base_side_a_color, antenna_base_side_a_shading),
        face_3d(antenna_vertices, vertex_3d(-0.0, 0.0, -1.0), 7, 4, 5, 6, antenna_base_side_a_color, antenna_base_side_a_shading),
        face_3d(antenna_vertices, vertex_3d(-1.0, -0.0, 0.0), 7, 2, 1, 4, antenna_base_side_b_color, antenna_base_side_b_shading),
        face_3d(antenna_vertices, vertex_3d(1.0, 0.0, 0.0), 3, 6, 5, 0, antenna_base_side_b_color, antenna_base_side_b_shading),
        face_3d(antenna_vertices, vertex_3d(0.0, -1.0, -0.0), 7, 6, 3, 2, antenna_base_down_color, antenna_base_down_shading),
        face_3d(antenna_vertices, vertex_3d(0.0, 0.1871121078899952, 0.9823385664224747), 14, 10, 8, antenna_middle_a_color, antenna_middle_a_shading),
        face_3d(antenna_vertices, vertex_3d(0.0, 0.1871121078899952, -0.9823385664224747), 16, 8, 9, antenna_middle_a_color, antenna_middle_a_shading),
        face_3d(antenna_vertices, vertex_3d(0.0, 0.1871121078899952, -0.9823385664224747), 17, 12, 8, antenna_middle_a_color, antenna_middle_a_shading),
        face_3d(antenna_vertices, vertex_3d(0.0, 0.1871121078899952, 0.9823385664224747), 8, 11, 20, antenna_middle_a_color, antenna_middle_a_shading),
        face_3d(antenna_vertices, vertex_3d(0.9823385664224747, 0.1871121078899952, 0.0), 10, 13, 8, antenna_middle_b_color, antenna_middle_b_shading),
        face_3d(antenna_vertices, vertex_3d(0.9823385664224747, 0.1871121078899952, 0.0), 9, 8, 15, antenna_middle_b_color, antenna_middle_b_shading),
        face_3d(antenna_vertices, vertex_3d(-0.9823385664224747, 0.1871121078899952, 0.0), 12, 18, 8, antenna_middle_b_color, antenna_middle_b_shading),
        face_3d(antenna_vertices, vertex_3d(-0.9823385664224747, 0.1871121078899952, 0.0), 19, 11, 8, antenna_middle_b_color, antenna_middle_b_shading),
        face_3d(antenna_vertices, vertex_3d(-0.69528419736107, -0.18209824216599452, -0.69528419736107), 8, 13, 14, antenna_middle_interior_color, antenna_middle_interior_shading),
        face_3d(antenna_vertices, vertex_3d(-0.69528419736107, -0.18209824216599452, 0.69528419736107), 15, 8, 16, antenna_middle_interior_color, antenna_middle_interior_shading),
        face_3d(antenna_vertices, vertex_3d(0.69528419736107, -0.18209824216599452, 0.69528419736107), 8, 18, 17, antenna_middle_interior_color, antenna_middle_interior_shading),
        face_3d(antenna_vertices, vertex_3d(0.69528419736107, -0.18209824216599452, -0.69528419736107), 8, 20, 19, antenna_middle_interior_color, antenna_middle_interior_shading),
    };

    constexpr inline const face_3d* antenna_collision_face_ptr = &antenna_full_faces[5];

    constexpr inline model_3d_item antenna_full(antenna_vertices, antenna_full_faces, antenna_collision_face_ptr);

    constexpr inline face_3d antenna_faces[] = {
        face_3d(antenna_vertices, vertex_3d(0.0, 1.0, 0.0), 4, 1, 0, 5, antenna_base_top_color, antenna_base_top_shading),
        face_3d(antenna_vertices, vertex_3d(0.0, 0.0, 1.0), 2, 3, 0, 1, antenna_base_side_a_color, antenna_base_side_a_shading),
        face_3d(antenna_vertices, vertex_3d(-0.0, 0.0, -1.0), 7, 4, 5, 6, antenna_base_side_a_color, antenna_base_side_a_shading),
        face_3d(antenna_vertices, vertex_3d(-1.0, -0.0, 0.0), 7, 2, 1, 4, antenna_base_side_b_color, antenna_base_side_b_shading),
        face_3d(antenna_vertices, vertex_3d(1.0, 0.0, 0.0), 3, 6, 5, 0, antenna_base_side_b_color, antenna_base_side_b_shading),
        face_3d(antenna_vertices, vertex_3d(0.0, 0.1871121078899952, 0.9823385664224747), 14, 10, 8, antenna_middle_a_color, antenna_middle_a_shading),
        face_3d(antenna_vertices, vertex_3d(0.0, 0.1871121078899952, -0.9823385664224747), 16, 8, 9, antenna_middle_a_color, antenna_middle_a_shading),
        face_3d(antenna_vertices, vertex_3d(0.0, 0.1871121078899952, -0.9823385664224747), 17, 12, 8, antenna_middle_a_color, antenna_middle_a_shading),
        face_3d(antenna_vertices, vertex_3d(0.0, 0.1871121078899952, 0.9823385664224747), 8, 11, 20, antenna_middle_a_color, antenna_middle_a_shading),
        face_3d(antenna_vertices, vertex_3d(0.9823385664224747, 0.1871121078899952, 0.0), 10, 13, 8, antenna_middle_b_color, antenna_middle_b_shading),
        face_3d(antenna_vertices, vertex_3d(0.9823385664224747, 0.1871121078899952, 0.0), 9, 8, 15, antenna_middle_b_color, antenna_middle_b_shading),
        face_3d(antenna_vertices, vertex_3d(-0.9823385664224747, 0.1871121078899952, 0.0), 12, 18, 8, antenna_middle_b_color, antenna_middle_b_shading),
        face_3d(antenna_vertices, vertex_3d(-0.9823385664224747, 0.1871121078899952, 0.0), 19, 11, 8, antenna_middle_b_color, antenna_middle_b_shading),
        face_3d(antenna_vertices, vertex_3d(-0.69528419736107, -0.18209824216599452, -0.69528419736107), 8, 13, 14, antenna_middle_interior_color, antenna_middle_interior_shading),
        face_3d(antenna_vertices, vertex_3d(-0.69528419736107, -0.18209824216599452, 0.69528419736107), 15, 8, 16, antenna_middle_interior_color, antenna_middle_interior_shading),
        face_3d(antenna_vertices, vertex_3d(0.69528419736107, -0.18209824216599452, 0.69528419736107), 8, 18, 17, antenna_middle_interior_color, antenna_middle_interior_shading),
        face_3d(antenna_vertices, vertex_3d(0.69528419736107, -0.18209824216599452, -0.69528419736107), 8, 20, 19, antenna_middle_interior_color, antenna_middle_interior_shading),
    };

    constexpr inline model_3d_item antenna(antenna_vertices, antenna_faces, antenna_collision_face_ptr);
}

#endif

