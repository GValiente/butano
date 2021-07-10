/*
 * Copyright (c) 2021
 * Gustavo Valiente gustavo.valiente@protonmail.com
 * Licensed under the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0) license
 */

#include "fr_stage.h"

#include "fr_model_colors.h"
#include "fr_transformed_model_3d_item.h"

#include "bn_music_items.h"
#include "bn_sprite_items_van_car.h"
#include "bn_sprite_items_a_van_car.h"
#include "bn_sprite_items_rival_car.h"
#include "bn_sprite_items_small_car.h"
#include "bn_sprite_items_player_car.h"
#include "bn_sprite_items_pickup_car.h"
#include "bn_sprite_items_simple_car.h"
#include "bn_sprite_items_common_car.h"
#include "bn_sprite_items_station_car.h"
#include "bn_affine_bg_items_stage_1_clouds.h"
#include "bn_affine_bg_items_stage_1_snowflakes.h"
#include "bn_affine_bg_items_stage_1_ground_forwards.h"
#include "bn_affine_bg_items_stage_1_ground_backwards.h"

#include "bn_sprite_palette_items_van_car_dawn.h"
#include "bn_sprite_palette_items_small_car_dawn.h"
#include "bn_bg_palette_items_stage_1_ground_dawn.h"
#include "bn_sprite_palette_items_player_car_dawn.h"
#include "bn_sprite_palette_items_common_car_dawn.h"
#include "bn_sprite_palette_items_pickup_car_dawn.h"
#include "bn_sprite_palette_items_simple_car_dawn.h"

#include "bn_sprite_palette_items_van_car_snow.h"
#include "bn_sprite_palette_items_small_car_snow.h"
#include "bn_bg_palette_items_stage_1_ground_snow.h"
#include "bn_sprite_palette_items_player_car_snow.h"
#include "bn_sprite_palette_items_common_car_snow.h"
#include "bn_sprite_palette_items_pickup_car_snow.h"
#include "bn_sprite_palette_items_simple_car_snow.h"
#include "bn_sprite_palette_items_station_car_snow.h"

#include "bn_sprite_palette_items_van_car_desert.h"
#include "bn_sprite_palette_items_rival_car_desert.h"
#include "bn_sprite_palette_items_small_car_desert.h"
#include "bn_bg_palette_items_stage_1_ground_desert.h"
#include "bn_sprite_palette_items_player_car_desert.h"
#include "bn_sprite_palette_items_common_car_desert.h"
#include "bn_sprite_palette_items_pickup_car_desert.h"
#include "bn_sprite_palette_items_simple_car_desert.h"
#include "bn_sprite_palette_items_station_car_desert.h"

#include "models/fr_model_3d_items_bear.h"
#include "models/fr_model_3d_items_bush.h"
#include "models/fr_model_3d_items_pool.h"
#include "models/fr_model_3d_items_spam.h"
#include "models/fr_model_3d_items_fence.h"
#include "models/fr_model_3d_items_house.h"
#include "models/fr_model_3d_items_camera.h"
#include "models/fr_model_3d_items_statue.h"
#include "models/fr_model_3d_items_antenna.h"
#include "models/fr_model_3d_items_big_tree.h"
#include "models/fr_model_3d_items_fat_tree.h"
#include "models/fr_model_3d_items_mountain.h"
#include "models/fr_model_3d_items_small_tree.h"
#include "models/fr_model_3d_items_start_flag.h"
#include "models/fr_model_3d_items_fence_small.h"
#include "models/fr_model_3d_items_gable_house.h"
#include "models/fr_model_3d_items_arbutus_tree.h"
#include "models/fr_model_3d_items_butano_house.h"
#include "models/fr_model_3d_items_fancy_stands.h"
#include "models/fr_model_3d_items_chinese_house.h"
#include "models/fr_model_3d_items_fence_curve_1.h"
#include "models/fr_model_3d_items_mountain_flipped.h"
#include "models/fr_model_3d_items_checkpoint_flag_forwards.h"
#include "models/fr_model_3d_items_checkpoint_flag_backwards.h"
#include "models/fr_model_3d_items_stands_left.h"
#include "models/fr_model_3d_items_stands_right.h"
#include "models/fr_model_3d_items_stands_middle.h"

#include "rival_checkpoints/fr_stage_1_easy_forwards_rival_checkpoints.h"
#include "rival_checkpoints/fr_stage_1_hard_forwards_rival_checkpoints.h"
#include "rival_checkpoints/fr_stage_1_easy_backwards_rival_checkpoints.h"
#include "rival_checkpoints/fr_stage_1_hard_backwards_rival_checkpoints.h"
#include "rival_checkpoints/fr_stage_1_normal_forwards_rival_checkpoints.h"
#include "rival_checkpoints/fr_stage_1_normal_backwards_rival_checkpoints.h"

namespace fr
{

namespace
{
    // Rect 1:

    constexpr auto rect_1_start_flag_forwards =
            transformed_model_3d_item<model_3d_items::start_flag>(174, 436, 32768 + 16384);

    constexpr auto rect_1_start_flag_backwards =
            transformed_model_3d_item<model_3d_items::start_flag>(174, 604, 32768 + 16384);

    constexpr auto rect_1_fancy_stands_left =
            transformed_model_3d_item<model_3d_items::fancy_stands>(135, 850, 32768 + 4096);

    constexpr auto rect_1_fence_white_left_1 =
            transformed_model_3d_item<model_3d_items::fence_solid_white>(100, 745, 16384 + 32768);

    constexpr auto rect_1_fence_red_left_1 =
            transformed_model_3d_item<model_3d_items::fence_solid_red>(100, 675, 16384 + 32768);

    constexpr auto rect_1_stands_l_left_1 =
            transformed_model_3d_item<model_3d_items::stands_left>(75, 600, 16384 + 32768);

    constexpr auto rect_1_stands_m_left_1 =
            transformed_model_3d_item<model_3d_items::stands_middle>(75, 530, 16384 + 32768);

    constexpr auto rect_1_stands_m_left_2 =
            transformed_model_3d_item<model_3d_items::stands_middle>(75, 460, 16384 + 32768);

    constexpr auto rect_1_stands_r_left_2 =
            transformed_model_3d_item<model_3d_items::stands_right>(75, 390, 16384 + 32768);

    constexpr auto rect_1_fence_white_left_2 =
            transformed_model_3d_item<model_3d_items::fence_solid_white>(100, 310, 16384 + 32768);

    constexpr auto rect_1_fence_black_right_1 =
            transformed_model_3d_item<model_3d_items::fence_solid_black>(245, 670, 16384);

    constexpr auto rect_1_fence_yellow_right_1 =
            transformed_model_3d_item<model_3d_items::fence_yellow>(245, 600, 16384);

    constexpr auto rect_1_fence_black_right_2 =
            transformed_model_3d_item<model_3d_items::fence_black>(245, 530, 16384);

    constexpr auto rect_1_fence_yellow_right_2 =
            transformed_model_3d_item<model_3d_items::fence_yellow>(245, 460, 16384);

    constexpr auto rect_1_fence_black_right_3 =
            transformed_model_3d_item<model_3d_items::fence_solid_black>(245, 390, 16384);


    // Curve 1:

    constexpr auto curve_1_fence_curve_left =
            transformed_model_3d_item<model_3d_items::fence_curve_1>(172, 198, 8192);

    constexpr auto curve_1_bush_1 =
            transformed_model_3d_item<model_3d_items::bush>(70, 300, 16000);

    constexpr auto curve_1_bush_2 =
            transformed_model_3d_item<model_3d_items::bush>(80, 230, 12000);

    constexpr auto curve_1_bush_3 =
            transformed_model_3d_item<model_3d_items::bush>(250, 160, 12000);

    constexpr auto curve_1_small_tree_1 =
            transformed_model_3d_item<model_3d_items::small_tree>(80, 160, 4000);

    constexpr auto curve_1_small_tree_2 =
            transformed_model_3d_item<model_3d_items::small_tree>(130, 190, 40000);

    constexpr auto curve_1_fence_white_right =
            transformed_model_3d_item<model_3d_items::fence_solid_white>(275, 325, 8192);


    // Rect 2:

    constexpr auto rect_2_mountain_left_1 =
            transformed_model_3d_item<model_3d_items::mountain>(40, 90, 32768);

    constexpr auto rect_2_mountain_flipped_left_1 =
            transformed_model_3d_item<model_3d_items::mountain_flipped>(110, 90, 32768);

    constexpr auto rect_2_mountain_left_2 =
            transformed_model_3d_item<model_3d_items::mountain>(180, 90, 32768);

    constexpr auto rect_2_mountain_flipped_left_2 =
            transformed_model_3d_item<model_3d_items::mountain_flipped>(250, 90, 32768);

    constexpr auto rect_2_mountain_left_3 =
            transformed_model_3d_item<model_3d_items::mountain>(320, 90, 32768);

    constexpr auto rect_2_mountain_flipped_left_3 =
            transformed_model_3d_item<model_3d_items::mountain_flipped>(390, 90, 32768);

    constexpr auto rect_2_mountain_left_4 =
            transformed_model_3d_item<model_3d_items::mountain>(460, 90, 32768);

    constexpr auto rect_2_mountain_flipped_left_4 =
            transformed_model_3d_item<model_3d_items::mountain_flipped>(530, 90, 32768);

    constexpr auto rect_2_mountain_left_5 =
            transformed_model_3d_item<model_3d_items::mountain>(600, 90, 32768);

    constexpr auto rect_2_mountain_flipped_left_5 =
            transformed_model_3d_item<model_3d_items::mountain_flipped>(670, 90, 32768);

    constexpr auto rect_2_mountain_left_6 =
            transformed_model_3d_item<model_3d_items::mountain>(740, 90, 32768);

    constexpr auto rect_2_mountain_flipped_left_6 =
            transformed_model_3d_item<model_3d_items::mountain_flipped>(810, 90, 32768);

    constexpr auto rect_2_mountain_left_7 =
            transformed_model_3d_item<model_3d_items::mountain>(880, 90, 32768);

    constexpr auto rect_2_mountain_flipped_left_7 =
            transformed_model_3d_item<model_3d_items::mountain_flipped>(950, 90, 32768);

    constexpr auto rect_2_fence_red_right_1 =
            transformed_model_3d_item<model_3d_items::fence_solid_red>(345, 300, 0);

    constexpr auto rect_2_spam_right_1 =
            transformed_model_3d_item<model_3d_items::spam>(390, 255, 0);

    constexpr auto rect_2_fence_white_right =
            transformed_model_3d_item<model_3d_items::fence_solid_white>(435, 300, 0);

    constexpr auto rect_2_fence_red_right_2 =
            transformed_model_3d_item<model_3d_items::fence_solid_red>(505, 300, 0);

    constexpr auto rect_2_spam_right_2 =
            transformed_model_3d_item<model_3d_items::spam>(550, 255, 0);

    constexpr auto rect_2_pool_right =
            transformed_model_3d_item<model_3d_items::pool>(613, 315, 16384);

    constexpr auto rect_2_house_right =
            transformed_model_3d_item<model_3d_items::house>(710, 345, 16384);


    // Curve 2:

    constexpr auto curve_2_fence_curve_left =
            transformed_model_3d_item<model_3d_items::fence_curve_1>(848, 200, 16384 + 8192);

    constexpr auto curve_2_small_tree =
            transformed_model_3d_item<model_3d_items::small_tree>(890, 190, 25000);

    constexpr auto curve_2_bush =
            transformed_model_3d_item<model_3d_items::bush>(920, 230, 23000);

    constexpr auto curve_2_fence_black_left_1 =
            transformed_model_3d_item<model_3d_items::fence_solid_black>(920, 315, 16384);

    constexpr auto curve_2_fence_yellow_left_1 =
            transformed_model_3d_item<model_3d_items::fence_yellow>(920, 385, 16384);

    constexpr auto curve_2_fence_black_left_2 =
            transformed_model_3d_item<model_3d_items::fence_black>(920, 455, 16384);

    constexpr auto curve_2_fence_yellow_left_2 =
            transformed_model_3d_item<model_3d_items::fence_yellow>(920, 525, 16384);

    constexpr auto curve_2_fence_red_left_1 =
            transformed_model_3d_item<model_3d_items::fence_red>(920, 595, 16384);

    constexpr auto curve_2_fence_white_left_1 =
            transformed_model_3d_item<model_3d_items::fence_white>(920, 665, 16384);

    constexpr auto curve_2_fence_red_left_2 =
            transformed_model_3d_item<model_3d_items::fence_red>(920, 735, 16384);

    constexpr auto curve_2_fence_white_left_2 =
            transformed_model_3d_item<model_3d_items::fence_white>(920, 805, 16384);

    constexpr auto curve_2_fence_red_left_3 =
            transformed_model_3d_item<model_3d_items::fence_solid_red>(920, 875, 16384);


    // Curve 3:

    constexpr auto curve_3_gable_house_left =
            transformed_model_3d_item<model_3d_items::gable_house>(860, 570, 0);


    // Curve X:

    constexpr auto curve_x_fence_red_left_1 =
            transformed_model_3d_item<model_3d_items::fence_solid_red>(770, 572, 0);

    constexpr auto curve_x_fence_white_left_1 =
            transformed_model_3d_item<model_3d_items::fence_white>(700, 572, 0);

    constexpr auto curve_x_fence_red_left_2 =
            transformed_model_3d_item<model_3d_items::fence_solid_red>(630, 572, 0);

    constexpr auto curve_x_fence_white_left_2 =
            transformed_model_3d_item<model_3d_items::fence_solid_white>(555, 558, -4096);

    constexpr auto curve_x_chinese_house_left =
            transformed_model_3d_item<model_3d_items::chinese_house>(472, 512, 0);

    constexpr auto curve_x_statue_right =
            transformed_model_3d_item<model_3d_items::statue>(332, 380, 8192);

    constexpr auto curve_x_camera_right =
            transformed_model_3d_item<model_3d_items::camera>(385, 676, 32768 + 16384);


    // Curve Y:

    constexpr auto curve_y_checkpoint_flag_forwards =
            transformed_model_3d_item<model_3d_items::checkpoint_flag_forwards>(564, 636, 0);

    constexpr auto curve_y_checkpoint_flag_backwards =
            transformed_model_3d_item<model_3d_items::checkpoint_flag_backwards>(684, 657, 0);

    constexpr auto curve_y_antenna_left =
            transformed_model_3d_item<model_3d_items::antenna>(880, 880, 32768 + 16384);

    constexpr auto curve_y_fence_white_left_1 =
            transformed_model_3d_item<model_3d_items::fence_solid_white>(890, 925, 0);

    constexpr auto curve_y_fence_red_left_1 =
            transformed_model_3d_item<model_3d_items::fence_red>(820, 925, 0);

    constexpr auto curve_y_fence_white_left_2 =
            transformed_model_3d_item<model_3d_items::fence_white>(750, 925, 0);

    constexpr auto curve_y_fence_red_left_2 =
            transformed_model_3d_item<model_3d_items::fence_red>(680, 925, 0);

    constexpr auto curve_y_fence_white_left_3 =
            transformed_model_3d_item<model_3d_items::fence_white>(610, 925, 0);

    constexpr auto curve_y_fence_red_left_3 =
            transformed_model_3d_item<model_3d_items::fence_red>(540, 925, 0);

    constexpr auto curve_y_fence_white_left_4 =
            transformed_model_3d_item<model_3d_items::fence_white>(470, 925, 0);

    constexpr auto curve_y_fence_red_left_4 =
            transformed_model_3d_item<model_3d_items::fence_red>(400, 925, 0);

    constexpr auto curve_y_fence_white_left_5 =
            transformed_model_3d_item<model_3d_items::fence_white>(330, 925, 0);

    constexpr auto curve_y_fence_red_left_5 =
            transformed_model_3d_item<model_3d_items::fence_red>(260, 925, 0);

    constexpr auto curve_y_fence_white_left_6 =
            transformed_model_3d_item<model_3d_items::fence_solid_white>(190, 925, 0);

    constexpr auto curve_y_fat_tree_right_forwards =
            transformed_model_3d_item<model_3d_items::fat_tree>(668, 688, -12000);

    constexpr auto curve_y_fat_tree_right_backwards =
            transformed_model_3d_item<model_3d_items::fat_tree>(578, 688, -12000);

    constexpr auto curve_y_big_tree_right =
            transformed_model_3d_item<model_3d_items::big_tree>(790, 744, 16384);

    constexpr auto curve_y_fence_yellow_right_1 =
            transformed_model_3d_item<model_3d_items::fence_solid_yellow>(740, 744, 0);

    constexpr auto curve_y_fence_black_right_1 =
            transformed_model_3d_item<model_3d_items::fence_black>(670, 744, 0);

    constexpr auto curve_y_fence_yellow_right_2 =
            transformed_model_3d_item<model_3d_items::fence_yellow>(600, 744, 0);

    constexpr auto curve_y_fence_black_right_2 =
            transformed_model_3d_item<model_3d_items::fence_black>(530, 744, 0);

    constexpr auto curve_y_fence_yellow_right_3 =
            transformed_model_3d_item<model_3d_items::fence_yellow>(460, 744, 0);

    constexpr auto curve_y_fence_black_right_3 =
            transformed_model_3d_item<model_3d_items::fence_black>(390, 744, 0);

    constexpr auto curve_y_fence_yellow_right_4 =
            transformed_model_3d_item<model_3d_items::fence_solid_yellow>(320, 744, 0);

    constexpr auto fence_small_solid_right =
            transformed_model_3d_item<model_3d_items::fence_small_solid>(250 + 15 - 2, 744 - 15, -8192);


    // S:

    constexpr auto s_butano_house_left =
            transformed_model_3d_item<model_3d_items::butano_house>(524, 874, 0);

    constexpr auto s_arbutus_tree_right =
            transformed_model_3d_item<model_3d_items::arbutus_tree>(712, 792, 8000);

    constexpr auto s_bear_right =
            transformed_model_3d_item<model_3d_items::bear>(350, 795, 16384);


    // Model items:

    constexpr model_3d_item forwards_model_items[] = {

        // Rect 1:

        rect_1_start_flag_forwards.item(),

        rect_1_fancy_stands_left.item(),
        rect_1_fence_white_left_1.item(),
        rect_1_fence_red_left_1.item(),

        rect_1_stands_l_left_1.item(),
        rect_1_stands_m_left_1.item(),
        rect_1_stands_m_left_2.item(),
        rect_1_stands_r_left_2.item(),

        rect_1_fence_white_left_2.item(),

        rect_1_fence_black_right_1.item(),
        rect_1_fence_yellow_right_1.item(),
        rect_1_fence_black_right_2.item(),
        rect_1_fence_yellow_right_2.item(),
        rect_1_fence_black_right_3.item(),

        // Curve 1:

        curve_1_fence_curve_left.item(),
        curve_1_bush_1.item(),
        curve_1_bush_2.item(),
        curve_1_bush_3.item(),
        curve_1_small_tree_1.item(),
        curve_1_small_tree_2.item(),

        curve_1_fence_white_right.item(),

        // Rect 2:

        rect_2_mountain_left_1.item(),
        rect_2_mountain_flipped_left_1.item(),
        rect_2_mountain_left_2.item(),
        rect_2_mountain_flipped_left_2.item(),
        rect_2_mountain_left_3.item(),
        rect_2_mountain_flipped_left_3.item(),
        rect_2_mountain_left_4.item(),
        rect_2_mountain_flipped_left_4.item(),
        rect_2_mountain_left_5.item(),
        rect_2_mountain_flipped_left_5.item(),
        rect_2_mountain_left_6.item(),
        rect_2_mountain_flipped_left_6.item(),
        rect_2_mountain_left_7.item(),
        rect_2_mountain_flipped_left_7.item(),

        rect_2_fence_red_right_1.item(),
        rect_2_spam_right_1.item(),
        rect_2_fence_white_right.item(),
        rect_2_fence_red_right_2.item(),
        rect_2_spam_right_2.item(),
        rect_2_pool_right.item(),
        rect_2_house_right.item(),

        // Curve 2:

        curve_2_fence_curve_left.item(),
        curve_2_small_tree.item(),
        curve_2_bush.item(),
        curve_2_fence_black_left_1.item(),
        curve_2_fence_yellow_left_1.item(),
        curve_2_fence_black_left_2.item(),
        curve_2_fence_yellow_left_2.item(),
        curve_2_fence_red_left_1.item(),
        curve_2_fence_white_left_1.item(),
        curve_2_fence_red_left_2.item(),
        curve_2_fence_white_left_2.item(),
        curve_2_fence_red_left_3.item(),

        // Curve 3:

        curve_3_gable_house_left.item(),

        // Curve X:

        curve_x_fence_red_left_1.item(),
        curve_x_fence_white_left_1.item(),
        curve_x_fence_red_left_2.item(),
        curve_x_fence_white_left_2.item(),
        curve_x_chinese_house_left.item(),
        curve_x_statue_right.item(),
        curve_x_camera_right.item(),

        // Curve Y:

        curve_y_checkpoint_flag_forwards.item(),

        curve_y_antenna_left.item(),
        curve_y_fence_white_left_1.item(),
        curve_y_fence_red_left_1.item(),
        curve_y_fence_white_left_2.item(),
        curve_y_fence_red_left_2.item(),
        curve_y_fence_white_left_3.item(),
        curve_y_fence_red_left_3.item(),
        curve_y_fence_white_left_4.item(),
        curve_y_fence_red_left_4.item(),
        curve_y_fence_white_left_5.item(),
        curve_y_fence_red_left_5.item(),
        curve_y_fence_white_left_6.item(),

        curve_y_fat_tree_right_forwards.item(),
        curve_y_big_tree_right.item(),
        curve_y_fence_yellow_right_1.item(),
        curve_y_fence_black_right_1.item(),
        curve_y_fence_yellow_right_2.item(),
        curve_y_fence_black_right_2.item(),
        curve_y_fence_yellow_right_3.item(),
        curve_y_fence_black_right_3.item(),
        curve_y_fence_yellow_right_4.item(),
        fence_small_solid_right.item(),

        // S:

        s_butano_house_left.item(),

        s_arbutus_tree_right.item(),
        s_bear_right.item(),
    };

    constexpr model_3d_item backwards_model_items[] = {

        // Rect 1:

        rect_1_start_flag_backwards.item(),

        rect_1_fancy_stands_left.item(),
        rect_1_fence_white_left_1.item(),
        rect_1_fence_red_left_1.item(),

        rect_1_stands_l_left_1.item(),
        rect_1_stands_m_left_1.item(),
        rect_1_stands_m_left_2.item(),
        rect_1_stands_r_left_2.item(),

        rect_1_fence_white_left_2.item(),

        rect_1_fence_black_right_1.item(),
        rect_1_fence_yellow_right_1.item(),
        rect_1_fence_black_right_2.item(),
        rect_1_fence_yellow_right_2.item(),
        rect_1_fence_black_right_3.item(),

        // Curve 1:

        curve_1_fence_curve_left.item(),
        curve_1_bush_1.item(),
        curve_1_bush_2.item(),
        curve_1_bush_3.item(),
        curve_1_small_tree_1.item(),
        curve_1_small_tree_2.item(),

        curve_1_fence_white_right.item(),

        // Rect 2:

        rect_2_mountain_left_1.item(),
        rect_2_mountain_flipped_left_1.item(),
        rect_2_mountain_left_2.item(),
        rect_2_mountain_flipped_left_2.item(),
        rect_2_mountain_left_3.item(),
        rect_2_mountain_flipped_left_3.item(),
        rect_2_mountain_left_4.item(),
        rect_2_mountain_flipped_left_4.item(),
        rect_2_mountain_left_5.item(),
        rect_2_mountain_flipped_left_5.item(),
        rect_2_mountain_left_6.item(),
        rect_2_mountain_flipped_left_6.item(),
        rect_2_mountain_left_7.item(),
        rect_2_mountain_flipped_left_7.item(),

        rect_2_fence_red_right_1.item(),
        rect_2_spam_right_1.item(),
        rect_2_fence_white_right.item(),
        rect_2_fence_red_right_2.item(),
        rect_2_spam_right_2.item(),
        rect_2_pool_right.item(),
        rect_2_house_right.item(),

        // Curve 2:

        curve_2_fence_curve_left.item(),
        curve_2_small_tree.item(),
        curve_2_bush.item(),
        curve_2_fence_black_left_1.item(),
        curve_2_fence_yellow_left_1.item(),
        curve_2_fence_black_left_2.item(),
        curve_2_fence_yellow_left_2.item(),
        curve_2_fence_red_left_1.item(),
        curve_2_fence_white_left_1.item(),
        curve_2_fence_red_left_2.item(),
        curve_2_fence_white_left_2.item(),
        curve_2_fence_red_left_3.item(),

        // Curve 3:

        curve_3_gable_house_left.item(),

        // Curve X:

        curve_x_fence_red_left_1.item(),
        curve_x_fence_white_left_1.item(),
        curve_x_fence_red_left_2.item(),
        curve_x_fence_white_left_2.item(),
        curve_x_chinese_house_left.item(),
        curve_x_statue_right.item(),
        curve_x_camera_right.item(),

        // Curve Y:

        curve_y_checkpoint_flag_backwards.item(),

        curve_y_antenna_left.item(),
        curve_y_fence_white_left_1.item(),
        curve_y_fence_red_left_1.item(),
        curve_y_fence_white_left_2.item(),
        curve_y_fence_red_left_2.item(),
        curve_y_fence_white_left_3.item(),
        curve_y_fence_red_left_3.item(),
        curve_y_fence_white_left_4.item(),
        curve_y_fence_red_left_4.item(),
        curve_y_fence_white_left_5.item(),
        curve_y_fence_red_left_5.item(),
        curve_y_fence_white_left_6.item(),

        curve_y_fat_tree_right_backwards.item(),
        curve_y_big_tree_right.item(),
        curve_y_fence_yellow_right_1.item(),
        curve_y_fence_black_right_1.item(),
        curve_y_fence_yellow_right_2.item(),
        curve_y_fence_black_right_2.item(),
        curve_y_fence_yellow_right_3.item(),
        curve_y_fence_black_right_3.item(),
        curve_y_fence_yellow_right_4.item(),
        fence_small_solid_right.item(),

        // S:

        s_butano_house_left.item(),

        s_arbutus_tree_right.item(),
        s_bear_right.item(),
    };


    // Ground bg items:

    constexpr const bn::affine_bg_item& default_forwards_ground_bg_item =
        bn::affine_bg_items::stage_1_ground_forwards;

    constexpr const bn::affine_bg_item& default_backwards_ground_bg_item =
        bn::affine_bg_items::stage_1_ground_backwards;

    constexpr bn::affine_bg_item dawn_forwards_ground_bg_item(
        bn::affine_bg_items::stage_1_ground_forwards.tiles_item(),
        bn::bg_palette_items::stage_1_ground_dawn,
        bn::affine_bg_items::stage_1_ground_forwards.map_item());

    constexpr bn::affine_bg_item dawn_backwards_ground_bg_item(
        bn::affine_bg_items::stage_1_ground_backwards.tiles_item(),
        bn::bg_palette_items::stage_1_ground_dawn,
        bn::affine_bg_items::stage_1_ground_backwards.map_item());

    constexpr bn::affine_bg_item snow_forwards_ground_bg_item(
        bn::affine_bg_items::stage_1_ground_forwards.tiles_item(),
        bn::bg_palette_items::stage_1_ground_snow,
        bn::affine_bg_items::stage_1_ground_forwards.map_item());

    constexpr bn::affine_bg_item snow_backwards_ground_bg_item(
        bn::affine_bg_items::stage_1_ground_backwards.tiles_item(),
        bn::bg_palette_items::stage_1_ground_snow,
        bn::affine_bg_items::stage_1_ground_backwards.map_item());

    constexpr bn::affine_bg_item desert_forwards_ground_bg_item(
        bn::affine_bg_items::stage_1_ground_forwards.tiles_item(),
        bn::bg_palette_items::stage_1_ground_desert,
        bn::affine_bg_items::stage_1_ground_forwards.map_item());

    constexpr bn::affine_bg_item desert_backwards_ground_bg_item(
        bn::affine_bg_items::stage_1_ground_backwards.tiles_item(),
        bn::bg_palette_items::stage_1_ground_desert,
        bn::affine_bg_items::stage_1_ground_backwards.map_item());


    // Car sprite items:

    constexpr const bn::sprite_item& default_player_car_sprite_item =
        bn::sprite_items::player_car;

    constexpr bn::sprite_item dawn_player_car_sprite_item(
        bn::sprite_items::player_car.shape_size(),
        bn::sprite_items::player_car.tiles_item(),
        bn::sprite_palette_items::player_car_dawn);

    constexpr bn::sprite_item snow_player_car_sprite_item(
        bn::sprite_items::player_car.shape_size(),
        bn::sprite_items::player_car.tiles_item(),
        bn::sprite_palette_items::player_car_snow);

    constexpr bn::sprite_item desert_player_car_sprite_item(
        bn::sprite_items::player_car.shape_size(),
        bn::sprite_items::player_car.tiles_item(),
        bn::sprite_palette_items::player_car_desert);

    constexpr const bn::sprite_item& default_small_car_sprite_item =
        bn::sprite_items::small_car;

    constexpr bn::sprite_item dawn_small_car_sprite_item(
        bn::sprite_items::small_car.shape_size(),
        bn::sprite_items::small_car.tiles_item(),
        bn::sprite_palette_items::small_car_dawn);

    constexpr bn::sprite_item snow_small_car_sprite_item(
        bn::sprite_items::small_car.shape_size(),
        bn::sprite_items::small_car.tiles_item(),
        bn::sprite_palette_items::small_car_snow);

    constexpr bn::sprite_item desert_small_car_sprite_item(
        bn::sprite_items::small_car.shape_size(),
        bn::sprite_items::small_car.tiles_item(),
        bn::sprite_palette_items::small_car_desert);

    constexpr const bn::sprite_item& default_pickup_car_sprite_item =
        bn::sprite_items::pickup_car;

    constexpr bn::sprite_item dawn_pickup_car_sprite_item(
        bn::sprite_items::pickup_car.shape_size(),
        bn::sprite_items::pickup_car.tiles_item(),
        bn::sprite_palette_items::pickup_car_dawn);

    constexpr bn::sprite_item snow_pickup_car_sprite_item(
        bn::sprite_items::pickup_car.shape_size(),
        bn::sprite_items::pickup_car.tiles_item(),
        bn::sprite_palette_items::pickup_car_snow);

    constexpr bn::sprite_item desert_pickup_car_sprite_item(
        bn::sprite_items::pickup_car.shape_size(),
        bn::sprite_items::pickup_car.tiles_item(),
        bn::sprite_palette_items::pickup_car_desert);

    constexpr const bn::sprite_item& default_simple_car_sprite_item =
        bn::sprite_items::simple_car;

    constexpr bn::sprite_item dawn_simple_car_sprite_item(
        bn::sprite_items::simple_car.shape_size(),
        bn::sprite_items::simple_car.tiles_item(),
        bn::sprite_palette_items::simple_car_dawn);

    constexpr bn::sprite_item snow_simple_car_sprite_item(
        bn::sprite_items::simple_car.shape_size(),
        bn::sprite_items::simple_car.tiles_item(),
        bn::sprite_palette_items::simple_car_snow);

    constexpr bn::sprite_item desert_simple_car_sprite_item(
        bn::sprite_items::simple_car.shape_size(),
        bn::sprite_items::simple_car.tiles_item(),
        bn::sprite_palette_items::simple_car_desert);

    constexpr const bn::sprite_item& default_van_car_sprite_item =
        bn::sprite_items::van_car;

    constexpr bn::sprite_item dawn_van_car_sprite_item(
        bn::sprite_items::van_car.shape_size(),
        bn::sprite_items::van_car.tiles_item(),
        bn::sprite_palette_items::van_car_dawn);

    constexpr bn::sprite_item snow_van_car_sprite_item(
        bn::sprite_items::van_car.shape_size(),
        bn::sprite_items::van_car.tiles_item(),
        bn::sprite_palette_items::van_car_snow);

    constexpr bn::sprite_item desert_van_car_sprite_item(
        bn::sprite_items::van_car.shape_size(),
        bn::sprite_items::van_car.tiles_item(),
        bn::sprite_palette_items::van_car_desert);

    constexpr const bn::sprite_item& default_common_car_sprite_item =
        bn::sprite_items::common_car;

    constexpr bn::sprite_item dawn_common_car_sprite_item(
        bn::sprite_items::common_car.shape_size(),
        bn::sprite_items::common_car.tiles_item(),
        bn::sprite_palette_items::common_car_dawn);

    constexpr bn::sprite_item snow_common_car_sprite_item(
        bn::sprite_items::common_car.shape_size(),
        bn::sprite_items::common_car.tiles_item(),
        bn::sprite_palette_items::common_car_snow);

    constexpr bn::sprite_item desert_common_car_sprite_item(
        bn::sprite_items::common_car.shape_size(),
        bn::sprite_items::common_car.tiles_item(),
        bn::sprite_palette_items::common_car_desert);

    constexpr const bn::sprite_item& default_station_car_sprite_item =
        bn::sprite_items::station_car;

    constexpr bn::sprite_item snow_station_car_sprite_item(
        bn::sprite_items::station_car.shape_size(),
        bn::sprite_items::station_car.tiles_item(),
        bn::sprite_palette_items::station_car_snow);

    constexpr bn::sprite_item desert_station_car_sprite_item(
        bn::sprite_items::station_car.shape_size(),
        bn::sprite_items::station_car.tiles_item(),
        bn::sprite_palette_items::station_car_desert);

    constexpr const bn::sprite_item& default_rival_car_sprite_item =
        bn::sprite_items::rival_car;

    constexpr bn::sprite_item desert_rival_car_sprite_item(
        bn::sprite_items::rival_car.shape_size(),
        bn::sprite_items::rival_car.tiles_item(),
        bn::sprite_palette_items::rival_car_desert);


    // Player car engines:

    constexpr car_engine easy_forwards_player_car_engine(0.125, 0.125 * 1.6, 0.001);
    constexpr car_engine easy_backwards_player_car_engine(0.15, 0.15 * 1.6, 0.001);
    constexpr car_engine normal_forwards_player_car_engine(0.175, 0.175 * 1.55, 0.001125);
    constexpr car_engine normal_backwards_player_car_engine(0.2, 0.2 * 1.55, 0.001125);
    constexpr car_engine hard_forwards_player_car_engine(0.225, 0.225 * 1.5, 0.00125);
    constexpr car_engine hard_backwards_player_car_engine(0.225, 0.225 * 1.5, 0.00125);


    // Player checkpoints:

    constexpr bn::fixed pi(3.14159265358979);
    constexpr bn::fixed angle_up = 0;
    constexpr bn::fixed angle_right = pi / 2;
    constexpr bn::fixed angle_down = pi;
    constexpr bn::fixed angle_left = (3 * pi) / 2;

    template<const bn::span<const checkpoint>& checkpoints>
    constexpr bn::array<bn::point, checkpoints.size()> checkpoint_vectors()
    {
        constexpr bn::fixed pi_2(3.14159265358979 + 3.14159265358979);
        constexpr bn::rule_of_three_approximation angle_16_rule_of_three(pi_2.data(), 65536);
        constexpr int num_checkpoints = checkpoints.size();
        bn::array<bn::point, num_checkpoints> vectors;

        for(int index = 0; index < num_checkpoints; ++index)
        {
            int angle_16_data = angle_16_rule_of_three.calculate(checkpoints[index].angle().data());
            int angle = angle_16_data + 16384;
            bn::fixed angle_sin = sin(angle);
            bn::fixed angle_cos = cos(angle);
            vectors[index] = bn::point(angle_sin.data() / 4, angle_cos.data() / 4);
        }

        return vectors;
    }

    constexpr checkpoint forwards_player_checkpoints[] = {
        checkpoint(172, 436 + 14, angle_up),
        checkpoint(172, 376, angle_up),
        checkpoint(172, 326, angle_up),
        checkpoint(316, 186, angle_right),
        checkpoint(400, 186, angle_right),
        checkpoint(476, 186, angle_right),
        checkpoint(556, 186, angle_right),
        checkpoint(636, 186, angle_right),
        checkpoint(852, 348, angle_down),
        checkpoint(696, 508, angle_left),
        checkpoint(476, 404, angle_left),
        checkpoint(356, 512, angle_down),
        checkpoint(484, 636, angle_right),
        checkpoint(564, 636, angle_right), // check
        checkpoint(664, 636, angle_right),
        checkpoint(756, 636, angle_right),
        checkpoint(850, 744, angle_down),
        checkpoint(760, 852, angle_left),
        checkpoint(664, 852, angle_left),
        checkpoint(524, 816, angle_left),
        checkpoint(384, 852, angle_left),
        checkpoint(312, 852, angle_left),
        checkpoint(172, 712, angle_up),
        checkpoint(172, 612, angle_up),
        checkpoint(172, 512, angle_up),
    };

    constexpr bn::span<const checkpoint> forwards_player_checkpoints_span(forwards_player_checkpoints);
    constexpr auto forwards_player_checkpoint_vectors = checkpoint_vectors<forwards_player_checkpoints_span>();

    constexpr checkpoint backwards_player_checkpoints[] = {
        checkpoint(172, 604 - 14, angle_down),
        checkpoint(172, 712, angle_down),
        checkpoint(312, 852, angle_right),
        checkpoint(384, 852, angle_right),
        checkpoint(524, 816, angle_right),
        checkpoint(664, 852, angle_right),
        checkpoint(760, 852, angle_right),
        checkpoint(850, 744, angle_up),
        checkpoint(756, 636, angle_left),
        checkpoint(684, 636, angle_left), // check
        checkpoint(584, 636, angle_left),
        checkpoint(484, 636, angle_left),
        checkpoint(356, 512, angle_up),
        checkpoint(476, 404, angle_right),
        checkpoint(696, 508, angle_right),
        checkpoint(852, 348, angle_up),
        checkpoint(636, 186, angle_left),
        checkpoint(556, 186, angle_left),
        checkpoint(476, 186, angle_left),
        checkpoint(400, 186, angle_left),
        checkpoint(316, 186, angle_left),
        checkpoint(172, 326, angle_down),
        checkpoint(172, 376, angle_down),
        checkpoint(172, 436, angle_down),
    };

    constexpr bn::span<const checkpoint> backwards_player_checkpoints_span(backwards_player_checkpoints);
    constexpr auto backwards_player_checkpoint_vectors = checkpoint_vectors<backwards_player_checkpoints_span>();


    // Rival cars:

    constexpr rival_car_info easy_forwards_rival_cars[] = {
        rival_car_info(default_common_car_sprite_item, 1600),
        rival_car_info(default_van_car_sprite_item, 1200),
        rival_car_info(default_simple_car_sprite_item, 800),
        rival_car_info(default_pickup_car_sprite_item, 400),
        rival_car_info(default_small_car_sprite_item, 0),
    };

    constexpr rival_car_info easy_backwards_rival_cars[] = {
        rival_car_info(dawn_common_car_sprite_item, 1800),
        rival_car_info(dawn_van_car_sprite_item, 1350),
        rival_car_info(dawn_simple_car_sprite_item, 900),
        rival_car_info(dawn_pickup_car_sprite_item, 450),
        rival_car_info(dawn_small_car_sprite_item, 0),
    };

    constexpr rival_car_info normal_forwards_rival_cars[] = {
        rival_car_info(snow_station_car_sprite_item, 2100),
        rival_car_info(snow_common_car_sprite_item, 1750),
        rival_car_info(snow_van_car_sprite_item, 1350),
        rival_car_info(snow_simple_car_sprite_item, 900),
        rival_car_info(snow_pickup_car_sprite_item, 450),
        rival_car_info(snow_small_car_sprite_item, 0),
    };

    constexpr rival_car_info normal_backwards_rival_cars[] = {
        rival_car_info(desert_station_car_sprite_item, 1800),
        rival_car_info(desert_common_car_sprite_item, 1500),
        rival_car_info(desert_van_car_sprite_item, 1200),
        rival_car_info(desert_simple_car_sprite_item, 800),
        rival_car_info(desert_pickup_car_sprite_item, 400),
        rival_car_info(desert_small_car_sprite_item, 0),
    };

    constexpr rival_car_info hard_forwards_rival_cars[] = {
        rival_car_info(desert_rival_car_sprite_item, 1800),
        rival_car_info(desert_station_car_sprite_item, 1500),
        rival_car_info(desert_common_car_sprite_item, 1200),
        rival_car_info(desert_van_car_sprite_item, 900),
        rival_car_info(desert_simple_car_sprite_item, 600),
        rival_car_info(desert_pickup_car_sprite_item, 300),
        rival_car_info(desert_small_car_sprite_item, 0),
    };

    constexpr rival_car_info hard_backwards_rival_cars[] = {
        rival_car_info(bn::sprite_items::a_van_car, 0),
    };


    // Final stage info:

    constexpr stage easy_forwards_stage(
            forwards_model_items, default_forwards_ground_bg_item, 0x03, bn::affine_bg_items::stage_1_clouds,
            bn::fixed_point(bn::fixed::from_data(1 << 8), 0), 0.15, default_player_car_sprite_item,
            default_model_colors, bn::music_items::forward, 22, easy_forwards_player_car_engine,
            forwards_player_checkpoints, forwards_player_checkpoint_vectors, 13, easy_forwards_rival_cars,
            stage_1_easy_forwards_rival_checkpoints, point_3d(156, 0, 644), angle_up, 3, 30, 10);

    constexpr stage easy_backwards_stage(
            backwards_model_items, dawn_backwards_ground_bg_item, 0x03, bn::affine_bg_items::stage_1_clouds,
            bn::fixed_point(bn::fixed::from_data(1 << 8), 0), 0.15, dawn_player_car_sprite_item,
            dawn_model_colors, bn::music_items::reverse, 24, easy_backwards_player_car_engine,
            backwards_player_checkpoints, backwards_player_checkpoint_vectors, 9, easy_backwards_rival_cars,
            stage_1_easy_backwards_rival_checkpoints, point_3d(156, 0, 396), angle_down, 4, 28, 8);

    constexpr stage normal_forwards_stage(
            forwards_model_items, snow_forwards_ground_bg_item, 0x03, bn::affine_bg_items::stage_1_snowflakes,
            bn::fixed_point(10, 1), 0.35, snow_player_car_sprite_item,
            snow_model_colors, bn::music_items::forward, 22, normal_forwards_player_car_engine,
            forwards_player_checkpoints, forwards_player_checkpoint_vectors, 13, normal_forwards_rival_cars,
            stage_1_normal_forwards_rival_checkpoints, point_3d(156, 0, 644), angle_up, 5, 28, 7);

    constexpr stage normal_backwards_stage(
            backwards_model_items, desert_backwards_ground_bg_item, 0x03, bn::affine_bg_items::stage_1_clouds,
            bn::fixed_point(bn::fixed::from_data(1 << 8), 0), 0.08, desert_player_car_sprite_item,
            desert_model_colors, bn::music_items::reverse, 24, normal_backwards_player_car_engine,
            backwards_player_checkpoints, backwards_player_checkpoint_vectors, 9, normal_backwards_rival_cars,
            stage_1_normal_backwards_rival_checkpoints, point_3d(156, 0, 396), angle_down, 6, 31, 6);

    constexpr stage hard_forwards_stage(
            forwards_model_items, desert_forwards_ground_bg_item, 0x03, bn::affine_bg_items::stage_1_clouds,
            bn::fixed_point(bn::fixed::from_data(1 << 8), 0), 0.08, desert_player_car_sprite_item,
            desert_model_colors, bn::music_items::forward, 22, hard_forwards_player_car_engine,
            forwards_player_checkpoints, forwards_player_checkpoint_vectors, 13, hard_forwards_rival_cars,
            stage_1_hard_forwards_rival_checkpoints, point_3d(156, 0, 644), angle_up, 7, 25, 6);

    constexpr stage hard_backwards_stage(
            backwards_model_items, default_backwards_ground_bg_item, 0x03, bn::affine_bg_items::stage_1_clouds,
            bn::fixed_point(bn::fixed::from_data(1 << 8), 0), 0.15, default_player_car_sprite_item,
            default_model_colors, bn::music_items::reverse, 24, hard_backwards_player_car_engine,
            backwards_player_checkpoints, backwards_player_checkpoint_vectors, 9, hard_backwards_rival_cars,
            stage_1_hard_backwards_rival_checkpoints, point_3d(156, 0, 396), angle_down, 5, 27, 5);
}

const stage& stage::stage_1(difficulty_level difficulty, bool reverse)
{
    switch(difficulty)
    {

    case difficulty_level::EASY:
        return reverse ? easy_backwards_stage : easy_forwards_stage;

    case difficulty_level::NORMAL:
        return reverse ? normal_backwards_stage : normal_forwards_stage;

    case difficulty_level::HARD:
        return reverse ? hard_backwards_stage : hard_forwards_stage;

    default:
        BN_ERROR("Invalid difficulty: ", int(difficulty));
        return easy_forwards_stage;
    }
}

}
