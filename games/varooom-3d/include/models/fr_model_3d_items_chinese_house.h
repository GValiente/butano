/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_CHINESE_HOUSE_H
#define FR_MODEL_3D_ITEMS_CHINESE_HOUSE_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d chinese_house_vertices[] = {
        vertex_3d(-9.83, 29.84, -9.83),
        vertex_3d(-9.83, 29.84, 9.83),
        vertex_3d(0.0, 46.57, 0.0),
        vertex_3d(9.83, 29.84, 9.83),
        vertex_3d(9.83, 29.84, -9.83),
        vertex_3d(-9.83, 10.05, -9.83),
        vertex_3d(-30.46, 6.17, -2.04),
        vertex_3d(-9.83, 10.05, 9.83),
        vertex_3d(-0.75, 3.46, 30.91),
        vertex_3d(9.83, 10.05, 9.83),
        vertex_3d(9.83, 10.05, -9.83),
        vertex_3d(0.07, 8.19, -34.35),
        vertex_3d(32.63, 3.2, -2.19),
        vertex_3d(39.27, -10.83, 40.33),
        vertex_3d(-37.09, -9.42, -44.46),
        vertex_3d(-36.81, -10.02, 39.69),
        vertex_3d(38.95, -9.98, -44.73),
        vertex_3d(-22.0, -39.26, -5.84),
        vertex_3d(-22.0, -39.26, 5.84),
        vertex_3d(-22.0, -11.4, 5.84),
        vertex_3d(-22.0, -11.4, -5.84),
        vertex_3d(22.0, -39.26, -22.0),
        vertex_3d(22.0, 1.0, -22.0),
        vertex_3d(22.0, 1.0, 22.0),
        vertex_3d(22.0, -39.26, 22.0),
        vertex_3d(-22.0, -39.26, -22.0),
        vertex_3d(-22.0, 1.0, -22.0),
        vertex_3d(-22.0, 1.0, 22.0),
        vertex_3d(-22.0, -39.26, 22.0),
        vertex_3d(30.58, -39.26, -35.87), // 29
        vertex_3d(30.58, -6.0, -35.87),
        vertex_3d(30.58, -4.5, -29.82),
        vertex_3d(30.58, -39.26, -29.82),
        vertex_3d(30.58, -39.26, 26.84),
        vertex_3d(30.58, -4.5, 26.84),
        vertex_3d(30.58, -6.0, 32.88), // 35
        vertex_3d(30.58, -39.26, 32.88),
        vertex_3d(-23.74, -4.5, 32.88),
        vertex_3d(-23.74, -39.26, 32.88),
        vertex_3d(-23.74, -39.26, -35.53),
        vertex_3d(-23.74, -4.5, -35.53),
        vertex_3d(24.54, -39.26, -35.87),
        vertex_3d(24.54, -4.5, -35.87),
        vertex_3d(24.54, -4.5, 32.88),
        vertex_3d(24.54, -39.26, 32.88),
        vertex_3d(-29.79, -39.26, 26.84),
        vertex_3d(-29.79, -4.5, 26.84),
        vertex_3d(-29.79, -6.0, 32.88),
        vertex_3d(-29.79, -39.26, 32.88), // 48
        vertex_3d(-29.79, -39.26, -35.53), // 49
        vertex_3d(-29.79, -6.0, -35.53),
        vertex_3d(-29.79, -4.5, -29.49),
        vertex_3d(-29.79, -39.26, -29.49),
    };

    constexpr inline face_3d chinese_house_collision_face(chinese_house_vertices, vertex_3d(0, 0, 0), 49, 48, 35, 29, 0, -1);

    constexpr inline int chinese_house_roof_top_a_color = 5;
    constexpr inline int chinese_house_roof_top_a_shading = 7;

    constexpr inline int chinese_house_roof_top_b_color = 5;
    constexpr inline int chinese_house_roof_top_b_shading = 5;

    constexpr inline int chinese_house_roof_bottom_a_color = 5;
    constexpr inline int chinese_house_roof_bottom_a_shading = 5;

    constexpr inline int chinese_house_roof_bottom_b_color = 5;
    constexpr inline int chinese_house_roof_bottom_b_shading = 2;

    constexpr inline int chinese_house_roof_bottom_c_color = 5;
    constexpr inline int chinese_house_roof_bottom_c_shading = 0;

    constexpr inline int chinese_house_door_color = 0;
    constexpr inline int chinese_house_door_shading = -1;

    constexpr inline int chinese_house_wall_front_color = 2;
    constexpr inline int chinese_house_wall_front_shading = 7;

    constexpr inline int chinese_house_wall_side_color = 2;
    constexpr inline int chinese_house_wall_side_shading = 5;

    constexpr inline int chinese_house_wall_back_color = 2;
    constexpr inline int chinese_house_wall_back_shading = 3;

    constexpr inline int chinese_house_pillar_front_color = 4;
    constexpr inline int chinese_house_pillar_front_shading = 3;

    constexpr inline int chinese_house_pillar_side_color = 4;
    constexpr inline int chinese_house_pillar_side_shading = 1;

    constexpr inline int chinese_house_pillar_back_color = 4;
    constexpr inline int chinese_house_pillar_back_shading = 0;

    constexpr inline face_3d chinese_house_faces[] = {
        face_3d(chinese_house_vertices, vertex_3d(-0.8621858238437726, 0.5065921487378532, 0.0), 0, 1, 2, chinese_house_roof_top_a_color, chinese_house_roof_top_a_shading),
        face_3d(chinese_house_vertices, vertex_3d(0.8621858238437726, 0.5065921487378532, -0.0), 2, 3, 4, chinese_house_roof_top_a_color, chinese_house_roof_top_a_shading),
        face_3d(chinese_house_vertices, vertex_3d(0.0, 0.5065921487378532, 0.8621858238437726), 1, 3, 2, chinese_house_roof_top_b_color, chinese_house_roof_top_b_shading),
        face_3d(chinese_house_vertices, vertex_3d(0.0, 0.5065921487378532, -0.8621858238437726), 4, 0, 2, chinese_house_roof_top_b_color, chinese_house_roof_top_b_shading),
        face_3d(chinese_house_vertices, vertex_3d(-0.18483499400004136, 0.982769568613622, 0.0), 5, 6, 7, chinese_house_roof_bottom_a_color, chinese_house_roof_bottom_a_shading),
        face_3d(chinese_house_vertices, vertex_3d(0.0, 0.9544477465515891, 0.2983781143157008), 7, 8, 9, chinese_house_roof_bottom_a_color, chinese_house_roof_bottom_a_shading),
        face_3d(chinese_house_vertices, vertex_3d(0.0, 0.9971352572887411, -0.07563913452516556), 10, 11, 5, chinese_house_roof_bottom_a_color, chinese_house_roof_bottom_a_shading),
        face_3d(chinese_house_vertices, vertex_3d(0.28773322791310063, 0.9577105980173277, -0.0), 9, 12, 10, chinese_house_roof_bottom_a_color, chinese_house_roof_bottom_a_shading),
        face_3d(chinese_house_vertices, vertex_3d(0.39039747123404783, 0.8906926948549191, 0.23292989886689652), 9, 13, 12, chinese_house_roof_bottom_b_color, chinese_house_roof_bottom_b_shading),
        face_3d(chinese_house_vertices, vertex_3d(-0.2820751687617826, 0.913939948496798, -0.29180022225776664), 5, 14, 6, chinese_house_roof_bottom_b_color, chinese_house_roof_bottom_b_shading),
        face_3d(chinese_house_vertices, vertex_3d(-0.24038770732342793, 0.892158005075369, 0.38244979297644244), 7, 15, 8, chinese_house_roof_bottom_b_color, chinese_house_roof_bottom_b_shading),
        face_3d(chinese_house_vertices, vertex_3d(0.36598396437008013, 0.9055938753001567, -0.21437227162766206), 10, 16, 11, chinese_house_roof_bottom_b_color, chinese_house_roof_bottom_b_shading),
        face_3d(chinese_house_vertices, vertex_3d(0.34977465023011883, 0.9083006828089428, -0.22945056911066095), 10, 12, 16, chinese_house_roof_bottom_c_color, chinese_house_roof_bottom_c_shading),
        face_3d(chinese_house_vertices, vertex_3d(-0.3379669202540928, 0.8936384850745905, 0.29527718978538536), 7, 6, 15, chinese_house_roof_bottom_c_color, chinese_house_roof_bottom_c_shading),
        face_3d(chinese_house_vertices, vertex_3d(0.22606733332143458, 0.8916627838032348, 0.39221300435503303), 13, 9, 8, chinese_house_roof_bottom_c_color, chinese_house_roof_bottom_c_shading),
        face_3d(chinese_house_vertices, vertex_3d(-0.3690281262748445, 0.9036009472259658, -0.21753981288585877), 14, 5, 11, chinese_house_roof_bottom_c_color, chinese_house_roof_bottom_c_shading),
        face_3d(chinese_house_vertices, vertex_3d(-1.0, -0.0, 0.0), 17, 18, 19, 20, chinese_house_door_color, chinese_house_door_shading),
        face_3d(chinese_house_vertices, vertex_3d(1.0, 0.0, 0.0), 21, 22, 23, 24, chinese_house_wall_front_color, chinese_house_wall_front_shading),
        face_3d(chinese_house_vertices, vertex_3d(1.0, -0.0, 0.0), 4, 3, 9, 10, chinese_house_wall_front_color, chinese_house_wall_front_shading),
        face_3d(chinese_house_vertices, vertex_3d(0.0, 0.0, -1.0), 4, 10, 5, 0, chinese_house_wall_side_color, chinese_house_wall_side_shading),
        face_3d(chinese_house_vertices, vertex_3d(0.0, 0.0, 1.0), 3, 1, 7, 9, chinese_house_wall_side_color, chinese_house_wall_side_shading),
        face_3d(chinese_house_vertices, vertex_3d(0.0, 0.0, -1.0), 21, 25, 26, 22, chinese_house_wall_side_color, chinese_house_wall_side_shading),
        face_3d(chinese_house_vertices, vertex_3d(0.0, 0.0, 1.0), 23, 27, 28, 24, chinese_house_wall_side_color, chinese_house_wall_side_shading),
        face_3d(chinese_house_vertices, vertex_3d(-1.0, 0.0, 0.0), 7, 1, 0, 5, chinese_house_wall_back_color, chinese_house_wall_back_shading),
        face_3d(chinese_house_vertices, vertex_3d(-1.0, 0.0, 0.0), 27, 19, 18, 28, chinese_house_wall_back_color, chinese_house_wall_back_shading),
        face_3d(chinese_house_vertices, vertex_3d(-1.0, 0.0, 0.0), 27, 26, 20, 19, chinese_house_wall_back_color, chinese_house_wall_back_shading),
        face_3d(chinese_house_vertices, vertex_3d(-1.0, -0.0, 0.0), 25, 17, 20, 26, chinese_house_wall_back_color, chinese_house_wall_back_shading),
        face_3d(chinese_house_vertices, vertex_3d(1.0, 0.0, 0.0), 29, 30, 31, 32, chinese_house_pillar_front_color, chinese_house_pillar_front_shading),
        face_3d(chinese_house_vertices, vertex_3d(1.0, 0.0, 0.0), 33, 34, 35, 36, chinese_house_pillar_front_color, chinese_house_pillar_front_shading),
        face_3d(chinese_house_vertices, vertex_3d(0.7065216703728823, 0.0, -0.7076914082377378), 37, 38, 45, 46, chinese_house_pillar_front_color, chinese_house_pillar_front_shading),
        face_3d(chinese_house_vertices, vertex_3d(0.7065216703728823, 0.0, 0.7076914082377378), 39, 40, 51, 52, chinese_house_pillar_front_color, chinese_house_pillar_front_shading),
        face_3d(chinese_house_vertices, vertex_3d(-0.0, 0.0, -1.0), 41, 42, 30, 29, chinese_house_pillar_side_color, chinese_house_pillar_side_shading),
        face_3d(chinese_house_vertices, vertex_3d(0.0, 0.0, 1.0), 36, 35, 43, 44, chinese_house_pillar_side_color, chinese_house_pillar_side_shading),
        face_3d(chinese_house_vertices, vertex_3d(-0.0, 0.0, 1.0), 38, 37, 47, 48, chinese_house_pillar_side_color, chinese_house_pillar_side_shading),
        face_3d(chinese_house_vertices, vertex_3d(0.0, 0.0, -1.0), 49, 50, 40, 39, chinese_house_pillar_side_color, chinese_house_pillar_side_shading),
        face_3d(chinese_house_vertices, vertex_3d(-0.7076914082377378, 0.0, 0.7065216703728822), 31, 42, 41, 32, chinese_house_pillar_back_color, chinese_house_pillar_back_shading),
        face_3d(chinese_house_vertices, vertex_3d(-1.0, 0.0, 0.0), 48, 47, 46, 45, chinese_house_pillar_back_color, chinese_house_pillar_back_shading),
        face_3d(chinese_house_vertices, vertex_3d(-1.0, 0.0, 0.0), 52, 51, 50, 49, chinese_house_pillar_back_color, chinese_house_pillar_back_shading),
        face_3d(chinese_house_vertices, vertex_3d(-0.7071067811865478, -0.0, -0.7071067811865474), 33, 44, 43, 34, chinese_house_pillar_back_color, chinese_house_pillar_back_shading),
    };

    constexpr inline model_3d_item chinese_house(chinese_house_vertices, chinese_house_faces, &chinese_house_collision_face);
}

#endif

