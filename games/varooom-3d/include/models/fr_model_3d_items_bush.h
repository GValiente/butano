/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_BUSH_H
#define FR_MODEL_3D_ITEMS_BUSH_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d bush_vertices[] = {
        vertex_3d(10.0, -0.74, -10.0),
        vertex_3d(0.0, 2.96, 0.0),
        vertex_3d(10.0, -0.74, 10.0),
        vertex_3d(-10.0, -0.74, 10.0),
        vertex_3d(-10.0, -0.74, -10.0),
    };

    constexpr inline face_3d bush_full_faces[] = {
        face_3d(bush_vertices, vertex_3d(0.34700888179205425, 0.9378618426812277, 0.0), 0, 1, 2, 5, 7),
        face_3d(bush_vertices, vertex_3d(-0.0, 0.9378618426812277, 0.34700888179205425), 2, 1, 3, 5, 6),
        face_3d(bush_vertices, vertex_3d(0.0, 0.9378618426812277, -0.34700888179205425), 1, 0, 4, 5, 4),
        face_3d(bush_vertices, vertex_3d(-0.34700888179205425, 0.9378618426812277, 0.0), 4, 3, 1, 5, 1),
        face_3d(bush_vertices, vertex_3d(0.0, -1.0, 0.0), 0, 2, 3, 4, 5, 0),
    };

    constexpr inline model_3d_item bush_full(bush_vertices, bush_full_faces);

    constexpr inline face_3d bush_faces[] = {
        face_3d(bush_vertices, vertex_3d(0.34700888179205425, 0.9378618426812277, 0.0), 0, 1, 2, 5, 7),
        face_3d(bush_vertices, vertex_3d(-0.0, 0.9378618426812277, 0.34700888179205425), 2, 1, 3, 5, 6),
        face_3d(bush_vertices, vertex_3d(0.0, 0.9378618426812277, -0.34700888179205425), 1, 0, 4, 5, 4),
        face_3d(bush_vertices, vertex_3d(-0.34700888179205425, 0.9378618426812277, 0.0), 4, 3, 1, 5, 1),
    };

    constexpr inline model_3d_item bush(bush_vertices, bush_faces);
}

#endif

