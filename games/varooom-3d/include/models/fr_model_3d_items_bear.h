/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * CA3
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_BEAR_H
#define FR_MODEL_3D_ITEMS_BEAR_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d bear_vertices[] = {
        vertex_3d(17.6, 2.91, -19.29),
        vertex_3d(-5.22, -3.88, -19.29),
        vertex_3d(-2.95, 18.28, -12.47),
        vertex_3d(11.73, 17.68, -12.47),
        vertex_3d(-5.22, -3.88, 16.28),
        vertex_3d(17.6, 2.91, 16.28),
        vertex_3d(11.73, 17.68, 9.47),
        vertex_3d(-2.95, 18.28, 9.47),
        vertex_3d(-2.95, 18.28, -1.11),
        vertex_3d(-5.22, -3.88, -0.86),
        vertex_3d(1.57, -26.71, -19.29),
        vertex_3d(24.4, -19.91, -19.29),
        vertex_3d(24.4, -19.91, 16.28),
        vertex_3d(1.57, -26.71, 16.28),
        vertex_3d(0.01, 31.32, -10.79),
        vertex_3d(14.37, 38.53, -10.79),
        vertex_3d(14.37, 38.53, 7.78),
        vertex_3d(0.01, 31.32, 7.78),
        vertex_3d(-0.85, 44.7, 3.61),
        vertex_3d(-6.56, 40.93, 3.61),
        vertex_3d(-6.56, 40.93, -6.61),
        vertex_3d(-0.85, 44.7, -6.61),
        vertex_3d(24.4, -19.91, -0.86),
        vertex_3d(32.38, -48.0, 19.35), // 23
        vertex_3d(32.18, -48.0, 3.85),
        vertex_3d(1.57, -26.71, -0.86),
        vertex_3d(7.37, -48.0, 4.19),
        vertex_3d(7.56, -48.0, 19.69), // 27
        vertex_3d(33.21, -48.0, -3.67),
        vertex_3d(33.21, -48.0, -22.36), // 29
        vertex_3d(4.26, -48.0, -22.69), // 30
        vertex_3d(4.26, -48.0, -3.99),
        vertex_3d(-26.35, 7.18, 12.42),
        vertex_3d(-26.16, 7.06, 6.32),
        vertex_3d(-22.91, 14.31, 9.96),
        vertex_3d(-22.79, 14.24, 6.2),
        vertex_3d(-30.66, 19.5, -11.62),
        vertex_3d(-28.39, 25.12, -9.93),
        vertex_3d(-28.32, 25.17, -6.86),
        vertex_3d(-30.55, 19.59, -6.64),
    };

    constexpr inline face_3d bear_collision_face(bear_vertices, vertex_3d(0, 0, 0), 30, 27, 23, 29, 0, -1);

    constexpr inline int bear_yellow_color = 7;
    constexpr inline int bear_yellow_shading = -1;

    constexpr inline int bear_yellow_down_color = 7;
    constexpr inline int bear_yellow_down_shading = 0;

    constexpr inline int bear_brown_up_a_color = 4;
    constexpr inline int bear_brown_up_a_shading = 7;

    constexpr inline int bear_brown_up_b_color = 4;
    constexpr inline int bear_brown_up_b_shading = 6;

    constexpr inline int bear_brown_front_a_color = 4;
    constexpr inline int bear_brown_front_a_shading = 6;

    constexpr inline int bear_brown_front_b_color = 4;
    constexpr inline int bear_brown_front_b_shading = 5;

    constexpr inline int bear_brown_side_a_color = 4;
    constexpr inline int bear_brown_side_a_shading = 5;

    constexpr inline int bear_brown_side_b_color = 4;
    constexpr inline int bear_brown_side_b_shading = 3;

    constexpr inline int bear_brown_back_a_color = 4;
    constexpr inline int bear_brown_back_a_shading = 3;

    constexpr inline int bear_brown_back_b_color = 4;
    constexpr inline int bear_brown_back_b_shading = 1;

    constexpr inline int bear_brown_down_a_color = 4;
    constexpr inline int bear_brown_down_a_shading = 1;

    constexpr inline int bear_brown_down_b_color = 4;
    constexpr inline int bear_brown_down_b_shading = 0;

    constexpr inline face_3d bear_full_faces[] = {
        face_3d(bear_vertices, vertex_3d(-0.5509847258718255, 0.8345153275140903, 0.0), 18, 21, 20, 19, bear_yellow_color, bear_yellow_shading),
        face_3d(bear_vertices, vertex_3d(-0.9049270963378457, 0.4239961282885583, -0.03652716126210144), 33, 32, 34, 35, bear_yellow_color, bear_yellow_shading),
        face_3d(bear_vertices, vertex_3d(-0.9283156303436673, 0.3715372314451449, 0.013790435443321264), 36, 39, 38, 37, bear_yellow_color, bear_yellow_shading),
        face_3d(bear_vertices, vertex_3d(0.37569097489345166, 0.9267449980353861, 0.0), 21, 18, 16, 15, bear_brown_up_a_color, bear_brown_up_a_shading),
        face_3d(bear_vertices, vertex_3d(-0.19559643887330508, 0.980378533453213, -0.024494167554926064), 35, 34, 7, 8, bear_brown_up_a_color, bear_brown_up_a_shading),
        face_3d(bear_vertices, vertex_3d(0.2575737745834991, 0.9660170407493316, -0.021606194220948626), 2, 37, 38, 8, bear_brown_up_a_color, bear_brown_up_a_shading),
        face_3d(bear_vertices, vertex_3d(-0.9585053553659841, -0.2850745231246181, 0.0), 13, 4, 1, 10, bear_brown_front_a_color, bear_brown_front_a_shading),
        face_3d(bear_vertices, vertex_3d(-0.9751915913528848, 0.22136250846660577, 0.0), 7, 17, 14, 2, bear_brown_front_a_color, bear_brown_front_a_shading),
        face_3d(bear_vertices, vertex_3d(-0.9648369911680132, -0.26284896894196697, 0.0), 13, 25, 26, 27, bear_brown_front_b_color, bear_brown_front_b_shading),
        face_3d(bear_vertices, vertex_3d(-0.9921121078892451, -0.12535376093105066, 0.0), 25, 10, 30, 31, bear_brown_front_b_color, bear_brown_front_b_shading),
        face_3d(bear_vertices, vertex_3d(-0.1436145110300136, 0.28603389436768306, 0.9473961597423051), 17, 16, 18, 19, bear_brown_side_a_color, bear_brown_side_a_shading),
        face_3d(bear_vertices, vertex_3d(-0.15285768865379268, 0.3044433299678866, -0.9401855060877532), 15, 14, 20, 21, bear_brown_side_a_color, bear_brown_side_a_shading),
        face_3d(bear_vertices, vertex_3d(-0.04350397333881871, 0.14605819284194577, 0.98831898120368), 12, 13, 27, 23, bear_brown_side_a_color, bear_brown_side_a_shading),
        face_3d(bear_vertices, vertex_3d(-0.029611098022830767, 0.09941490703841563, -0.9946053785961673), 10, 11, 29, 30, bear_brown_side_a_color, bear_brown_side_a_shading),
        face_3d(bear_vertices, vertex_3d(-0.18994782253080708, 0.11435630409283604, 0.9751115117923855), 8, 38, 39, 9, bear_brown_side_a_color, bear_brown_side_a_shading),
        face_3d(bear_vertices, vertex_3d(-0.08966263121472891, 0.30134038944331565, -0.9492916212912671), 0, 1, 2, 3, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(-0.1126968877931359, 0.37875448887177643, 0.9186100634334575), 4, 5, 6, 7, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(0.0, 0.0, -1.0), 10, 1, 0, 11, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(-0.0, 0.0, 1.0), 12, 5, 4, 13, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(0.0051752413143885114, 0.1266209041587054, -0.9919376812619681), 3, 2, 14, 15, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(0.0032851434011533574, 0.08037650854821861, 0.9967591608339659), 7, 6, 16, 17, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(0.04554561740158727, -0.15291271254091723, -0.9871895963181981), 25, 22, 24, 26, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(-0.01093312537494722, -0.14679720386412837, 0.9891061862647555), 25, 31, 28, 22, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(-0.04569770940906185, 0.3454448129677285, 0.9373257707694035), 7, 34, 32, 4, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(-0.3142129987806884, 0.021479518729231685, -0.9491094887695557), 8, 9, 33, 35, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(-0.01751313924448241, 0.2944095567998804, -0.9555188657576057), 1, 36, 37, 2, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(0.9583563422094348, 0.2855750712981662, 0.0), 11, 0, 5, 12, bear_brown_back_a_color, bear_brown_back_a_shading),
        face_3d(bear_vertices, vertex_3d(0.9920789697576653, -0.12561575444413597, 0.0), 3, 15, 16, 6, bear_brown_back_a_color, bear_brown_back_a_shading),
        face_3d(bear_vertices, vertex_3d(0.9292988459235874, 0.3693286544056506, -0.0), 6, 5, 0, 3, bear_brown_back_b_color, bear_brown_back_b_shading),
        face_3d(bear_vertices, vertex_3d(0.9619363804414962, 0.2732734893529065, 0.0), 22, 12, 23, 24, bear_brown_back_b_color, bear_brown_back_b_shading),
        face_3d(bear_vertices, vertex_3d(0.9541712145336532, 0.2992612459964929, 0.0), 11, 22, 28, 29, bear_brown_back_b_color, bear_brown_back_b_shading),
        face_3d(bear_vertices, vertex_3d(-0.8255179067353029, -0.5643759258325639, 0.0), 17, 19, 20, 14, bear_brown_down_a_color, bear_brown_down_a_shading),
        face_3d(bear_vertices, vertex_3d(-0.46305775931157367, -0.8863281060314766, -0.0), 33, 9, 4, 32, bear_brown_down_a_color, bear_brown_down_a_shading),
        face_3d(bear_vertices, vertex_3d(-0.6796620750027145, -0.7335253668435773, -0.0), 1, 9, 39, 36, bear_brown_down_a_color, bear_brown_down_a_shading),
        face_3d(bear_vertices, vertex_3d(0.0, -1.0, 0.0), 26, 24, 23, 27, bear_yellow_down_color, bear_yellow_down_shading),
        face_3d(bear_vertices, vertex_3d(0.0, -1.0, -0.0), 30, 29, 28, 31, bear_yellow_down_color, bear_yellow_down_shading),
    };

    constexpr inline model_3d_item bear_full(bear_vertices, bear_full_faces, &bear_collision_face);

    constexpr inline face_3d bear_faces[] = {
        face_3d(bear_vertices, vertex_3d(-0.5509847258718255, 0.8345153275140903, 0.0), 18, 21, 20, 19, bear_yellow_color, bear_yellow_shading),
        face_3d(bear_vertices, vertex_3d(-0.9049270963378457, 0.4239961282885583, -0.03652716126210144), 33, 32, 34, 35, bear_yellow_color, bear_yellow_shading),
        face_3d(bear_vertices, vertex_3d(-0.9283156303436673, 0.3715372314451449, 0.013790435443321264), 36, 39, 38, 37, bear_yellow_color, bear_yellow_shading),
        face_3d(bear_vertices, vertex_3d(0.37569097489345166, 0.9267449980353861, 0.0), 21, 18, 16, 15, bear_brown_up_a_color, bear_brown_up_a_shading),
        face_3d(bear_vertices, vertex_3d(-0.19559643887330508, 0.980378533453213, -0.024494167554926064), 35, 34, 7, 8, bear_brown_up_a_color, bear_brown_up_a_shading),
        face_3d(bear_vertices, vertex_3d(0.2575737745834991, 0.9660170407493316, -0.021606194220948626), 2, 37, 38, 8, bear_brown_up_a_color, bear_brown_up_a_shading),
        face_3d(bear_vertices, vertex_3d(-0.9585053553659841, -0.2850745231246181, 0.0), 13, 4, 1, 10, bear_brown_front_a_color, bear_brown_front_a_shading),
        face_3d(bear_vertices, vertex_3d(-0.9751915913528848, 0.22136250846660577, 0.0), 7, 17, 14, 2, bear_brown_front_a_color, bear_brown_front_a_shading),
        face_3d(bear_vertices, vertex_3d(-0.9648369911680132, -0.26284896894196697, 0.0), 13, 25, 26, 27, bear_brown_front_b_color, bear_brown_front_b_shading),
        face_3d(bear_vertices, vertex_3d(-0.9921121078892451, -0.12535376093105066, 0.0), 25, 10, 30, 31, bear_brown_front_b_color, bear_brown_front_b_shading),
        face_3d(bear_vertices, vertex_3d(-0.1436145110300136, 0.28603389436768306, 0.9473961597423051), 17, 16, 18, 19, bear_brown_side_a_color, bear_brown_side_a_shading),
        face_3d(bear_vertices, vertex_3d(-0.15285768865379268, 0.3044433299678866, -0.9401855060877532), 15, 14, 20, 21, bear_brown_side_a_color, bear_brown_side_a_shading),
        face_3d(bear_vertices, vertex_3d(-0.04350397333881871, 0.14605819284194577, 0.98831898120368), 12, 13, 27, 23, bear_brown_side_a_color, bear_brown_side_a_shading),
        face_3d(bear_vertices, vertex_3d(-0.029611098022830767, 0.09941490703841563, -0.9946053785961673), 10, 11, 29, 30, bear_brown_side_a_color, bear_brown_side_a_shading),
        face_3d(bear_vertices, vertex_3d(-0.18994782253080708, 0.11435630409283604, 0.9751115117923855), 8, 38, 39, 9, bear_brown_side_a_color, bear_brown_side_a_shading),
        face_3d(bear_vertices, vertex_3d(-0.08966263121472891, 0.30134038944331565, -0.9492916212912671), 0, 1, 2, 3, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(-0.1126968877931359, 0.37875448887177643, 0.9186100634334575), 4, 5, 6, 7, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(0.0, 0.0, -1.0), 10, 1, 0, 11, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(-0.0, 0.0, 1.0), 12, 5, 4, 13, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(0.0051752413143885114, 0.1266209041587054, -0.9919376812619681), 3, 2, 14, 15, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(0.0032851434011533574, 0.08037650854821861, 0.9967591608339659), 7, 6, 16, 17, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(0.04554561740158727, -0.15291271254091723, -0.9871895963181981), 25, 22, 24, 26, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(-0.01093312537494722, -0.14679720386412837, 0.9891061862647555), 25, 31, 28, 22, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(-0.04569770940906185, 0.3454448129677285, 0.9373257707694035), 7, 34, 32, 4, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(-0.3142129987806884, 0.021479518729231685, -0.9491094887695557), 8, 9, 33, 35, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(-0.01751313924448241, 0.2944095567998804, -0.9555188657576057), 1, 36, 37, 2, bear_brown_side_b_color, bear_brown_side_b_shading),
        face_3d(bear_vertices, vertex_3d(0.9583563422094348, 0.2855750712981662, 0.0), 11, 0, 5, 12, bear_brown_back_a_color, bear_brown_back_a_shading),
        face_3d(bear_vertices, vertex_3d(0.9920789697576653, -0.12561575444413597, 0.0), 3, 15, 16, 6, bear_brown_back_a_color, bear_brown_back_a_shading),
        face_3d(bear_vertices, vertex_3d(0.9292988459235874, 0.3693286544056506, -0.0), 6, 5, 0, 3, bear_brown_back_b_color, bear_brown_back_b_shading),
        face_3d(bear_vertices, vertex_3d(0.9619363804414962, 0.2732734893529065, 0.0), 22, 12, 23, 24, bear_brown_back_b_color, bear_brown_back_b_shading),
        face_3d(bear_vertices, vertex_3d(0.9541712145336532, 0.2992612459964929, 0.0), 11, 22, 28, 29, bear_brown_back_b_color, bear_brown_back_b_shading),
    };

    constexpr inline model_3d_item bear(bear_vertices, bear_faces, &bear_collision_face);
}

#endif

