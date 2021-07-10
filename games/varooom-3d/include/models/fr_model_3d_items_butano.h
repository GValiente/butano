/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * CA3
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#ifndef FR_MODEL_3D_ITEMS_BUTANO_H
#define FR_MODEL_3D_ITEMS_BUTANO_H

#include "fr_model_3d_item.h"

namespace fr::model_3d_items
{
    constexpr inline vertex_3d butano_vertices[] = {
        vertex_3d(-2.0, 35.0, 2.0),
        vertex_3d(2.0, 35.0, 2.0),
        vertex_3d(2.0, 35.0, -2.0),
        vertex_3d(-2.0, 35.0, -2.0),
        vertex_3d(2.0, 29.0, 2.0),
        vertex_3d(2.0, 29.0, -2.0),
        vertex_3d(-2.0, 29.0, 2.0),
        vertex_3d(-2.0, 29.0, -2.0),
        vertex_3d(5.0, 28.18, -8.0),
        vertex_3d(5.0, 29.0, -5.0),
        vertex_3d(10.0, 30.5, -3.5),
        vertex_3d(12.0, 31.18, -6.0),
        vertex_3d(10.0, 30.5, 3.5),
        vertex_3d(5.0, 29.0, 5.0),
        vertex_3d(5.0, 28.18, 8.0),
        vertex_3d(12.0, 31.18, 6.0),
        vertex_3d(-5.0, 28.18, -8.0),
        vertex_3d(-12.0, 31.18, -6.0),
        vertex_3d(-10.0, 30.5, -3.5),
        vertex_3d(-5.0, 29.0, -5.0),
        vertex_3d(-10.0, 30.5, 3.5),
        vertex_3d(-12.0, 31.18, 6.0),
        vertex_3d(-5.0, 28.18, 8.0),
        vertex_3d(-5.0, 29.0, 5.0),
        vertex_3d(13.2, 29.67, 3.52),
        vertex_3d(15.0, 30.18, 6.0),
        vertex_3d(8.0, 27.18, 8.0),
        vertex_3d(8.0, 28.18, 5.0),
        vertex_3d(8.0, 27.18, -8.0),
        vertex_3d(15.0, 30.18, -6.0),
        vertex_3d(13.06, 29.63, -3.55),
        vertex_3d(8.0, 28.18, -5.0),
        vertex_3d(-13.2, 29.67, 3.52),
        vertex_3d(-8.0, 28.18, 5.0),
        vertex_3d(-8.0, 27.18, 8.0),
        vertex_3d(-15.0, 30.18, 6.0),
        vertex_3d(-8.0, 27.18, -8.0),
        vertex_3d(-8.0, 28.18, -5.0),
        vertex_3d(-13.06, 29.63, -3.55),
        vertex_3d(-15.0, 30.18, -6.0),
        vertex_3d(0.0, 27.0, -16.0),
        vertex_3d(0.0, 27.0, 16.0),
        vertex_3d(-12.0, 24.0, -12.0),
        vertex_3d(-16.0, 27.0, 0.0),
        vertex_3d(-12.0, 24.0, 12.0),
        vertex_3d(12.0, 24.0, 12.0),
        vertex_3d(16.0, 27.0, 0.0),
        vertex_3d(12.0, 24.0, -12.0),
        vertex_3d(15.0, 0.0, 15.0),
        vertex_3d(0.0, 0.0, 21.0),
        vertex_3d(15.0, 0.0, -15.0),
        vertex_3d(21.0, 0.0, 0.0),
        vertex_3d(14.0, -26.0, 14.0),
        vertex_3d(20.0, -26.0, 0.0),
        vertex_3d(0.0, 0.0, -21.0),
        vertex_3d(-15.0, 0.0, -15.0),
        vertex_3d(-14.0, -26.0, -14.0),
        vertex_3d(-20.0, -26.0, 0.0),
        vertex_3d(-21.0, 0.0, 0.0),
        vertex_3d(-15.0, 0.0, 15.0),
        vertex_3d(15.0, -32.0, -15.0),
        vertex_3d(14.5, -29.0, -14.5),
        vertex_3d(20.5, -29.0, 0.0),
        vertex_3d(21.0, -32.0, 0.0),
        vertex_3d(15.0, -32.0, 15.0),
        vertex_3d(14.5, -29.0, 14.5),
        vertex_3d(0.0, -29.0, 20.5),
        vertex_3d(0.0, -32.0, 21.0),
        vertex_3d(-14.0, -26.0, 14.0),
        vertex_3d(0.0, -26.0, 20.0),
        vertex_3d(14.0, -26.0, -14.0),
        vertex_3d(0.0, -26.0, -20.0),
        vertex_3d(0.0, -32.0, -21.0),
        vertex_3d(-15.0, -32.0, -15.0),
        vertex_3d(-14.5, -29.0, -14.5),
        vertex_3d(0.0, -29.0, -20.5),
        vertex_3d(-21.0, -32.0, 0.0),
        vertex_3d(-15.0, -32.0, 15.0),
        vertex_3d(-14.5, -29.0, 14.5),
        vertex_3d(-20.5, -29.0, 0.0),
    };

    constexpr inline int butano_cap_top_color = 1;
    constexpr inline int butano_cap_top_shading = 0;

    constexpr inline int butano_cap_a_color = 1;
    constexpr inline int butano_cap_a_shading = 7;

    constexpr inline int butano_cap_b_color = 1;
    constexpr inline int butano_cap_b_shading = 6;

    constexpr inline int butano_handle_up_color = 0;
    constexpr inline int butano_handle_up_shading = 4;

    constexpr inline int butano_handle_down_color = 1;
    constexpr inline int butano_handle_down_shading = 0;

    constexpr inline int butano_handle_ext_a_color = 0;
    constexpr inline int butano_handle_ext_a_shading = 0;

    constexpr inline int butano_handle_ext_b_color = 0;
    constexpr inline int butano_handle_ext_b_shading = 1;

    constexpr inline int butano_handle_in_a_color = 1;
    constexpr inline int butano_handle_in_a_shading = 2;

    constexpr inline int butano_handle_in_b_color = 1;
    constexpr inline int butano_handle_in_b_shading = 7;

    constexpr inline int butano_cylinder_top_a_color = 2;
    constexpr inline int butano_cylinder_top_a_shading = 5;

    constexpr inline int butano_cylinder_top_b_color = 2;
    constexpr inline int butano_cylinder_top_b_shading = 6;

    constexpr inline int butano_cylinder_top_c_color = 2;
    constexpr inline int butano_cylinder_top_c_shading = 7;

    constexpr inline int butano_cylinder_a_color = 3;
    constexpr inline int butano_cylinder_a_shading = 7;

    constexpr inline int butano_cylinder_b_color = 3;
    constexpr inline int butano_cylinder_b_shading = 6;

    constexpr inline int butano_hole_color = 1;
    constexpr inline int butano_hole_shading = 0;

    constexpr inline int butano_cylinder_down_color = 1;
    constexpr inline int butano_cylinder_down_shading = 2;

    constexpr inline face_3d butano_faces[] = {
        face_3d(butano_vertices, vertex_3d(0.0, 1.0, -0.0), 0, 1, 2, 3, butano_cap_top_color, butano_cap_top_shading),
        face_3d(butano_vertices, vertex_3d(1.0, 0.0, 0.0), 4, 5, 2, 1, butano_cap_a_color, butano_cap_a_shading),
        face_3d(butano_vertices, vertex_3d(-1.0, 0.0, 0.0), 6, 0, 3, 7, butano_cap_a_color, butano_cap_a_shading),
        face_3d(butano_vertices, vertex_3d(0.0, 0.0, 1.0), 6, 4, 1, 0, butano_cap_b_color, butano_cap_b_shading),
        face_3d(butano_vertices, vertex_3d(0.0, 0.0, -1.0), 3, 2, 5, 7, butano_cap_b_color, butano_cap_b_shading),
        face_3d(butano_vertices, vertex_3d(-0.20578540386293998, 0.9439697424905504, -0.2580183962807506), 8, 9, 10, 11, butano_handle_up_color, butano_handle_up_shading),
        face_3d(butano_vertices, vertex_3d(-0.20578540386293998, 0.9439697424905504, 0.2580183962807506), 12, 13, 14, 15, butano_handle_up_color, butano_handle_up_shading),
        face_3d(butano_vertices, vertex_3d(-0.3219027332387022, 0.9467727448197127, 0.0), 12, 15, 11, 10, butano_handle_up_color, butano_handle_up_shading),
        face_3d(butano_vertices, vertex_3d(0.38046879689265184, 0.9232594958455999, -0.053248454644118405), 16, 17, 18, 19, butano_handle_up_color, butano_handle_up_shading),
        face_3d(butano_vertices, vertex_3d(0.38046879689265184, 0.9232594958455999, 0.053248454644118405), 20, 21, 22, 23, butano_handle_up_color, butano_handle_up_shading),
        face_3d(butano_vertices, vertex_3d(0.3219027332387022, 0.9467727448197127, 0.0), 20, 18, 17, 21, butano_handle_up_color, butano_handle_up_shading),
        face_3d(butano_vertices, vertex_3d(0.3730667542621848, -0.9242883178883448, -0.0806988369148705), 24, 25, 26, 27, butano_handle_down_color, butano_handle_down_shading),
        face_3d(butano_vertices, vertex_3d(0.37143510221305737, -0.9244118579337982, 0.0865949291549965), 28, 29, 30, 31, butano_handle_down_color, butano_handle_down_shading),
        face_3d(butano_vertices, vertex_3d(0.27275559114962583, -0.9620833578732241, 0.0), 30, 29, 25, 24, butano_handle_down_color, butano_handle_down_shading),
        face_3d(butano_vertices, vertex_3d(-0.1788976131210202, -0.9333788510661905, -0.31112628368873013), 32, 33, 34, 35, butano_handle_down_color, butano_handle_down_shading),
        face_3d(butano_vertices, vertex_3d(-0.17833208891237362, -0.9334762447206325, 0.3111587482402109), 36, 37, 38, 39, butano_handle_down_color, butano_handle_down_shading),
        face_3d(butano_vertices, vertex_3d(-0.2725430936434614, -0.9621435755293604, 4.6635065218212886e-05), 38, 32, 35, 39, butano_handle_down_color, butano_handle_down_shading),
        face_3d(butano_vertices, vertex_3d(0.31622776601683794, 0.9486832980505138, 0.0), 11, 15, 25, 29, butano_handle_ext_a_color, butano_handle_ext_a_shading),
        face_3d(butano_vertices, vertex_3d(-0.31622776601683794, 0.9486832980505138, 0.0), 17, 39, 35, 21, butano_handle_ext_a_color, butano_handle_ext_a_shading),
        face_3d(butano_vertices, vertex_3d(0.11624763874381928, 0.34874291623145787, -0.9299811099505543), 8, 11, 29, 28, butano_handle_ext_b_color, butano_handle_ext_b_shading),
        face_3d(butano_vertices, vertex_3d(0.11624763874381928, 0.34874291623145787, 0.9299811099505543), 26, 25, 15, 14, butano_handle_ext_b_color, butano_handle_ext_b_shading),
        face_3d(butano_vertices, vertex_3d(-0.11624763874381928, 0.34874291623145787, -0.9299811099505543), 16, 36, 39, 17, butano_handle_ext_b_color, butano_handle_ext_b_shading),
        face_3d(butano_vertices, vertex_3d(-0.11624763874381928, 0.34874291623145787, 0.9299811099505543), 34, 22, 21, 35, butano_handle_ext_b_color, butano_handle_ext_b_shading),
        face_3d(butano_vertices, vertex_3d(-0.2511146165249198, -0.9679009632696971, 0.010448668294806073), 30, 24, 12, 10, butano_handle_in_a_color, butano_handle_in_a_shading),
        face_3d(butano_vertices, vertex_3d(0.27347540112413954, -0.9618789970573175, 0.0), 38, 18, 20, 32, butano_handle_in_a_color, butano_handle_in_a_shading),
        face_3d(butano_vertices, vertex_3d(-0.12011461014773052, -0.4726461082235893, 0.8730281420493577), 30, 10, 9, 31, butano_handle_in_b_color, butano_handle_in_b_shading),
        face_3d(butano_vertices, vertex_3d(-0.11824083964952523, -0.47685606113800383, -0.870992193303088), 13, 12, 24, 27, butano_handle_in_b_color, butano_handle_in_b_shading),
        face_3d(butano_vertices, vertex_3d(0.12357783235262483, -0.4521140208022859, 0.8833580426673074), 38, 37, 19, 18, butano_handle_in_b_color, butano_handle_in_b_shading),
        face_3d(butano_vertices, vertex_3d(0.12292547904916168, -0.4497273623749817, -0.8846663925637597), 23, 33, 32, 20, butano_handle_in_b_color, butano_handle_in_b_shading),
        face_3d(butano_vertices, vertex_3d(0.0, 0.9646152654368905, 0.2636615058860835), 13, 23, 22, 14, butano_cylinder_top_a_color, butano_cylinder_top_a_shading),
        face_3d(butano_vertices, vertex_3d(0.0, 0.9646152654368905, -0.2636615058860835), 9, 8, 16, 19, butano_cylinder_top_a_color, butano_cylinder_top_a_shading),
        face_3d(butano_vertices, vertex_3d(0.0, 0.9892962173722717, -0.14592119206241005), 16, 8, 40, butano_cylinder_top_a_color, butano_cylinder_top_a_shading),
        face_3d(butano_vertices, vertex_3d(0.0, 0.9892962173722717, 0.14592119206241005), 14, 22, 41, butano_cylinder_top_a_color, butano_cylinder_top_a_shading),
        face_3d(butano_vertices, vertex_3d(-0.35627553086147146, 0.8716220938506948, -0.33666403374983084), 36, 42, 43, 37, butano_cylinder_top_a_color, butano_cylinder_top_a_shading),
        face_3d(butano_vertices, vertex_3d(-0.3198408554016154, 0.8988501791146101, 0.2996167263715367), 34, 33, 43, 44, butano_cylinder_top_a_color, butano_cylinder_top_a_shading),
        face_3d(butano_vertices, vertex_3d(0.35627553086147146, 0.8716220938506948, 0.33666403374983084), 26, 45, 46, 27, butano_cylinder_top_a_color, butano_cylinder_top_a_shading),
        face_3d(butano_vertices, vertex_3d(0.3198408554016154, 0.8988501791146101, -0.2996167263715367), 28, 31, 46, 47, butano_cylinder_top_a_color, butano_cylinder_top_a_shading),
        face_3d(butano_vertices, vertex_3d(0.2636615058860835, 0.9646152654368905, 0.0), 31, 9, 13, 27, butano_cylinder_top_b_color, butano_cylinder_top_b_shading),
        face_3d(butano_vertices, vertex_3d(-0.2636615058860835, 0.9646152654368905, 0.0), 37, 33, 23, 19, butano_cylinder_top_b_color, butano_cylinder_top_b_shading),
        face_3d(butano_vertices, vertex_3d(0.33666403374983084, 0.8716220938506948, -0.35627553086147146), 28, 47, 40, 8, butano_cylinder_top_b_color, butano_cylinder_top_b_shading),
        face_3d(butano_vertices, vertex_3d(-0.2996167263715367, 0.8988501791146101, -0.3198408554016154), 36, 16, 40, 42, butano_cylinder_top_b_color, butano_cylinder_top_b_shading),
        face_3d(butano_vertices, vertex_3d(-0.33666403374983084, 0.8716220938506948, 0.35627553086147146), 34, 44, 41, 22, butano_cylinder_top_b_color, butano_cylinder_top_b_shading),
        face_3d(butano_vertices, vertex_3d(0.2996167263715367, 0.8988501791146101, 0.3198408554016154), 26, 14, 41, 45, butano_cylinder_top_b_color, butano_cylinder_top_b_shading),
        face_3d(butano_vertices, vertex_3d(-0.14592119206241005, 0.9892962173722717, 0.0), 33, 37, 43, butano_cylinder_top_b_color, butano_cylinder_top_b_shading),
        face_3d(butano_vertices, vertex_3d(0.14592119206241005, 0.9892962173722717, -0.0), 27, 46, 31, butano_cylinder_top_b_color, butano_cylinder_top_b_shading),
        face_3d(butano_vertices, vertex_3d(0.0, 1.0, 0.0), 13, 9, 19, 23, butano_cylinder_top_c_color, butano_cylinder_top_c_shading),
        face_3d(butano_vertices, vertex_3d(0.347746943654898, 0.15897003138509624, 0.9240133074258718), 48, 45, 41, 49, butano_cylinder_a_color, butano_cylinder_a_shading),
        face_3d(butano_vertices, vertex_3d(0.9240133074258718, 0.15897003138509624, -0.347746943654898), 50, 47, 46, 51, butano_cylinder_a_color, butano_cylinder_a_shading),
        face_3d(butano_vertices, vertex_3d(0.9185712193700032, -0.03532966228346166, 0.3936733797300014), 52, 53, 51, 48, butano_cylinder_a_color, butano_cylinder_a_shading),
        face_3d(butano_vertices, vertex_3d(-0.36658314486585103, 0.16038012587880981, -0.9164578621646275), 54, 55, 42, 40, butano_cylinder_a_color, butano_cylinder_a_shading),
        face_3d(butano_vertices, vertex_3d(-0.9179751277670554, -0.05043819383335469, -0.3934179119001666), 55, 56, 57, 58, butano_cylinder_a_color, butano_cylinder_a_shading),
        face_3d(butano_vertices, vertex_3d(-0.9164578621646275, 0.16038012587880981, 0.36658314486585103), 58, 59, 44, 43, butano_cylinder_a_color, butano_cylinder_a_shading),
        face_3d(butano_vertices, vertex_3d(0.9028642395813629, 0.2127438725450338, -0.3735989956888398), 60, 61, 62, 63, butano_cylinder_a_color, butano_cylinder_a_shading),
        face_3d(butano_vertices, vertex_3d(0.3735989956888398, 0.2127438725450338, 0.9028642395813629), 64, 65, 66, 67, butano_cylinder_a_color, butano_cylinder_a_shading),
        face_3d(butano_vertices, vertex_3d(-0.3709274010152048, -0.04993253475204681, 0.9273185025380121), 49, 59, 68, 69, butano_cylinder_a_color, butano_cylinder_a_shading),
        face_3d(butano_vertices, vertex_3d(0.3934179119001666, -0.05043819383335469, -0.9179751277670554), 50, 70, 71, 54, butano_cylinder_a_color, butano_cylinder_a_shading),
        face_3d(butano_vertices, vertex_3d(-0.3629703624113421, 0.21173271140661623, -0.9074259060283553), 72, 73, 74, 75, butano_cylinder_a_color, butano_cylinder_a_shading),
        face_3d(butano_vertices, vertex_3d(-0.9074259060283553, 0.21173271140661623, 0.3629703624113421), 76, 77, 78, 79, butano_cylinder_a_color, butano_cylinder_a_shading),
        face_3d(butano_vertices, vertex_3d(0.3497127647436729, 0.17059159255788922, -0.9211945998126018), 54, 40, 47, 50, butano_cylinder_b_color, butano_cylinder_b_shading),
        face_3d(butano_vertices, vertex_3d(0.3709274010152048, -0.04993253475204681, 0.9273185025380121), 52, 48, 49, 69, butano_cylinder_b_color, butano_cylinder_b_shading),
        face_3d(butano_vertices, vertex_3d(0.9273185025380121, -0.04993253475204681, -0.3709274010152048), 70, 50, 51, 53, butano_cylinder_b_color, butano_cylinder_b_shading),
        face_3d(butano_vertices, vertex_3d(0.9211945998126018, 0.17059159255788922, 0.3497127647436729), 51, 46, 45, 48, butano_cylinder_b_color, butano_cylinder_b_shading),
        face_3d(butano_vertices, vertex_3d(-0.3934179119001666, -0.05043819383335469, -0.9179751277670554), 71, 56, 55, 54, butano_cylinder_b_color, butano_cylinder_b_shading),
        face_3d(butano_vertices, vertex_3d(-0.3660196887737918, 0.1694535596174962, 0.9150492219344795), 59, 49, 41, 44, butano_cylinder_b_color, butano_cylinder_b_shading),
        face_3d(butano_vertices, vertex_3d(-0.9150492219344795, 0.1694535596174962, -0.3660196887737918), 55, 58, 43, 42, butano_cylinder_b_color, butano_cylinder_b_shading),
        face_3d(butano_vertices, vertex_3d(-0.9179751277670554, -0.05043819383335469, 0.3934179119001666), 57, 68, 59, 58, butano_cylinder_b_color, butano_cylinder_b_shading),
        face_3d(butano_vertices, vertex_3d(-0.9175556253099242, 0.15292593755165404, -0.36702225012396966), 73, 76, 57, 56, butano_cylinder_b_color, butano_cylinder_b_shading),
        face_3d(butano_vertices, vertex_3d(0.36702225012396966, 0.15292593755165404, -0.9175556253099242), 60, 72, 71, 70, butano_cylinder_b_color, butano_cylinder_b_shading),
        face_3d(butano_vertices, vertex_3d(0.9175556253099242, 0.15292593755165404, 0.36702225012396966), 64, 63, 53, 52, butano_cylinder_b_color, butano_cylinder_b_shading),
        face_3d(butano_vertices, vertex_3d(-0.36702225012396966, 0.15292593755165404, 0.9175556253099242), 77, 67, 69, 68, butano_cylinder_b_color, butano_cylinder_b_shading),
        face_3d(butano_vertices, vertex_3d(0.9085462272638302, 0.15142437121063834, -0.3893769545416415), 70, 53, 62, 61, butano_hole_color, butano_hole_shading),
        face_3d(butano_vertices, vertex_3d(0.3893769545416415, 0.15142437121063834, 0.9085462272638302), 52, 69, 66, 65, butano_hole_color, butano_hole_shading),
        face_3d(butano_vertices, vertex_3d(-0.3893769545416415, 0.15142437121063834, -0.9085462272638302), 56, 71, 75, 74, butano_hole_color, butano_hole_shading),
        face_3d(butano_vertices, vertex_3d(-0.9132505739593965, 0.15220842899323273, 0.3778967892245778), 57, 79, 78, 68, butano_hole_color, butano_hole_shading),
        face_3d(butano_vertices, vertex_3d(0.0, -1.0, 0.0), 60, 63, 64, 67, butano_cylinder_down_color, butano_cylinder_down_shading),
        face_3d(butano_vertices, vertex_3d(0.0, -1.0, 0.0), 72, 60, 67, 77, butano_cylinder_down_color, butano_cylinder_down_shading),
        face_3d(butano_vertices, vertex_3d(0.0, -1.0, 0.0), 72, 77, 76, 73, butano_cylinder_down_color, butano_cylinder_down_shading),
    };

    constexpr inline model_3d_item butano(butano_vertices, butano_faces);
}

#endif

