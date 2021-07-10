/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_EXPLOSION_3_H
#define FR_MODEL_3D_ITEMS_EXPLOSION_3_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d explosion_3_vertices[] = {
        vertex_3d(34.0, 0.0, 0.0),
        vertex_3d(20.0, 14.0, 0.0),
        vertex_3d(6.0, 0.0, 0.0),
        vertex_3d(20.0, -14.0, 0.0),
        vertex_3d(17.0, 0.0, -29.44),
        vertex_3d(10.0, 14.0, -17.32),
        vertex_3d(3.0, 0.0, -5.2),
        vertex_3d(10.0, -14.0, -17.32),
        vertex_3d(-17.0, 0.0, -29.44),
        vertex_3d(-10.0, 14.0, -17.32),
        vertex_3d(-3.0, 0.0, -5.2),
        vertex_3d(-10.0, -14.0, -17.32),
        vertex_3d(-34.0, 0.0, 0.0),
        vertex_3d(-20.0, 14.0, 0.0),
        vertex_3d(-6.0, 0.0, 0.0),
        vertex_3d(-20.0, -14.0, 0.0),
        vertex_3d(-17.0, 0.0, 29.44),
        vertex_3d(-10.0, 14.0, 17.32),
        vertex_3d(-3.0, 0.0, 5.2),
        vertex_3d(-10.0, -14.0, 17.32),
        vertex_3d(17.0, 0.0, 29.44),
        vertex_3d(10.0, 14.0, 17.32),
        vertex_3d(3.0, 0.0, 5.2),
        vertex_3d(10.0, -14.0, 17.32),
    };

    constexpr inline int explosion_3_top_a_color = 7;
    constexpr inline int explosion_3_top_a_shading = 7;

    constexpr inline int explosion_3_top_b_color = 7;
    constexpr inline int explosion_3_top_b_shading = 5;

    constexpr inline int explosion_3_middle_a_color = 7;
    constexpr inline int explosion_3_middle_a_shading = 6;

    constexpr inline int explosion_3_middle_b_color = 7;
    constexpr inline int explosion_3_middle_b_shading = 4;

    constexpr inline face_3d explosion_3_faces[] = {
        face_3d(explosion_3_vertices, vertex_3d(0.6546972104838855, -0.6545892102842016, -0.3780006988936983), 3, 7, 4, 0, explosion_3_top_a_color, explosion_3_top_a_shading),
        face_3d(explosion_3_vertices, vertex_3d(0.0, 0.6545192492927033, -0.7560453374668189), 4, 8, 9, 5, explosion_3_top_a_color, explosion_3_top_a_shading),
        face_3d(explosion_3_vertices, vertex_3d(-0.6546509272536524, -0.6546509272536524, -0.37797397647439507), 11, 15, 12, 8, explosion_3_top_a_color, explosion_3_top_a_shading),
        face_3d(explosion_3_vertices, vertex_3d(-0.6546654466664574, 0.6546019115881707, 0.37803371580603856), 12, 16, 17, 13, explosion_3_top_a_color, explosion_3_top_a_shading),
        face_3d(explosion_3_vertices, vertex_3d(0.0, -0.6545192492927032, 0.7560453374668189), 19, 23, 20, 16, explosion_3_top_a_color, explosion_3_top_a_shading),
        face_3d(explosion_3_vertices, vertex_3d(0.6546382193875748, 0.6546382193875748, 0.3780179935322273), 20, 0, 1, 21, explosion_3_top_a_color, explosion_3_top_a_shading),
        face_3d(explosion_3_vertices, vertex_3d(0.6546654466664574, 0.6546019115881707, -0.37803371580603856), 0, 4, 5, 1, explosion_3_top_b_color, explosion_3_top_b_shading),
        face_3d(explosion_3_vertices, vertex_3d(0.0, -0.6545192492927032, -0.7560453374668189), 7, 11, 8, 4, explosion_3_top_b_color, explosion_3_top_b_shading),
        face_3d(explosion_3_vertices, vertex_3d(-0.6546382193875748, 0.6546382193875748, -0.3780179935322273), 8, 12, 13, 9, explosion_3_top_b_color, explosion_3_top_b_shading),
        face_3d(explosion_3_vertices, vertex_3d(-0.6546972104838855, -0.6545892102842016, 0.3780006988936983), 15, 19, 16, 12, explosion_3_top_b_color, explosion_3_top_b_shading),
        face_3d(explosion_3_vertices, vertex_3d(0.0, 0.6545192492927033, 0.7560453374668189), 16, 20, 21, 17, explosion_3_top_b_color, explosion_3_top_b_shading),
        face_3d(explosion_3_vertices, vertex_3d(0.6546509272536524, -0.6546509272536524, 0.37797397647439507), 23, 3, 0, 20, explosion_3_top_b_color, explosion_3_top_b_shading),
        face_3d(explosion_3_vertices, vertex_3d(-0.6548770727672643, -0.6545172501998097, 0.37781369582726776), 2, 6, 7, 3, explosion_3_middle_a_color, explosion_3_middle_a_shading),
        face_3d(explosion_3_vertices, vertex_3d(0.0, 0.6545192492927032, 0.7560453374668189), 5, 9, 10, 6, explosion_3_middle_a_color, explosion_3_middle_a_shading),
        face_3d(explosion_3_vertices, vertex_3d(0.6547228547156427, -0.6547228547156428, 0.3777247238744093), 10, 14, 15, 11, explosion_3_middle_a_color, explosion_3_middle_a_shading),
        face_3d(explosion_3_vertices, vertex_3d(0.6546972104838855, 0.6545892102842016, -0.3780006988936983), 13, 17, 18, 14, explosion_3_middle_a_color, explosion_3_middle_a_shading),
        face_3d(explosion_3_vertices, vertex_3d(0.0, -0.6545192492927032, -0.756045337466819), 18, 22, 23, 19, explosion_3_middle_a_color, explosion_3_middle_a_shading),
        face_3d(explosion_3_vertices, vertex_3d(-0.6546509272536524, 0.6546509272536524, -0.37797397647439507), 21, 1, 2, 22, explosion_3_middle_a_color, explosion_3_middle_a_shading),
        face_3d(explosion_3_vertices, vertex_3d(-0.6546972104838855, 0.6545892102842016, 0.3780006988936983), 1, 5, 6, 2, explosion_3_middle_b_color, explosion_3_middle_b_shading),
        face_3d(explosion_3_vertices, vertex_3d(0.0, -0.6545192492927032, 0.756045337466819), 6, 10, 11, 7, explosion_3_middle_b_color, explosion_3_middle_b_shading),
        face_3d(explosion_3_vertices, vertex_3d(0.6546509272536524, 0.6546509272536524, 0.37797397647439507), 9, 13, 14, 10, explosion_3_middle_b_color, explosion_3_middle_b_shading),
        face_3d(explosion_3_vertices, vertex_3d(0.6548770727672643, -0.6545172501998097, -0.37781369582726776), 14, 18, 19, 15, explosion_3_middle_b_color, explosion_3_middle_b_shading),
        face_3d(explosion_3_vertices, vertex_3d(0.0, 0.6545192492927032, -0.7560453374668189), 17, 21, 22, 18, explosion_3_middle_b_color, explosion_3_middle_b_shading),
        face_3d(explosion_3_vertices, vertex_3d(-0.6547228547156427, -0.6547228547156428, -0.3777247238744093), 22, 2, 3, 23, explosion_3_middle_b_color, explosion_3_middle_b_shading),
    };

    constexpr inline model_3d_item explosion_3(explosion_3_vertices, explosion_3_faces);
}

#endif

