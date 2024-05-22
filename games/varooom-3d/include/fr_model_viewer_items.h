/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_MODEL_VIEWER_ITEMS_H
#define FR_MODEL_VIEWER_ITEMS_H

#include "fr_model_viewer_item.h"

#include "models/fr_model_3d_items_bear.h"
#include "models/fr_model_3d_items_pool.h"
#include "models/fr_model_3d_items_spam.h"
#include "models/fr_model_3d_items_house.h"
#include "models/fr_model_3d_items_turbo.h"
#include "models/fr_model_3d_items_antenna.h"
#include "models/fr_model_3d_items_van_car.h"
#include "models/fr_model_3d_items_big_tree.h"
#include "models/fr_model_3d_items_fat_tree.h"
#include "models/fr_model_3d_items_small_car.h"
#include "models/fr_model_3d_items_a_van_car.h"
#include "models/fr_model_3d_items_common_car.h"
#include "models/fr_model_3d_items_pickup_car.h"
#include "models/fr_model_3d_items_player_car.h"
#include "models/fr_model_3d_items_simple_car.h"
#include "models/fr_model_3d_items_small_tree.h"
#include "models/fr_model_3d_items_start_flag.h"
#include "models/fr_model_3d_items_camera_full.h"
#include "models/fr_model_3d_items_explosion_6.h"
#include "models/fr_model_3d_items_stands_full.h"
#include "models/fr_model_3d_items_station_car.h"
#include "models/fr_model_3d_items_statue_full.h"
#include "models/fr_model_3d_items_arbutus_tree.h"
#include "models/fr_model_3d_items_fence_curve_1.h"
#include "models/fr_model_3d_items_gable_house_full.h"
#include "models/fr_model_3d_items_butano_house_full.h"
#include "models/fr_model_3d_items_fancy_stands_full.h"
#include "models/fr_model_3d_items_chinese_house_full.h"
#include "models/fr_model_3d_items_checkpoint_flag_forwards.h"

namespace fr
{

constexpr model_viewer_item model_viewer_items[] = {
    model_viewer_item(model_3d_items::player_car, "Player car", 200, 28672, 0, 40704),
    model_viewer_item(model_3d_items::small_car, "Microcar", 210, 28672, 0, 40704 + 256),
    model_viewer_item(model_3d_items::pickup_car, "Pickup", 200, 28672, 0, 40704),
    model_viewer_item(model_3d_items::simple_car, "Rental car", 200, 28672, 0, 40704),
    model_viewer_item(model_3d_items::van_car, "Candy van", 190, 28672 + 32678, 0, 40704 + 16384),
    model_viewer_item(model_3d_items::common_car, "Taxi", 200, 28672, 0, 40704),
    model_viewer_item(model_3d_items::station_car, "Limo", 190, 28672 + 32678, 0, 40704 + 16384),
    model_viewer_item(model_3d_items::rival_car, "Tuned car", 200, 28672, 0, 40704),
    model_viewer_item(model_3d_items::a_van_car, "AT van", 190, 28672 + 32678, 0, 40704 + 256 + 16384),
    model_viewer_item(model_3d_items::small_tree_full, "Small tree", 90, 37500, 0, 49152),
    model_viewer_item(model_3d_items::fence_curve_1_full, "Fences", 10, 22000, 0, 49152),
    model_viewer_item(model_3d_items::arbutus_tree_full, "Arbutus tree", 110, 62000, 0, 58500),
    model_viewer_item(model_3d_items::pool_full, "Pool", 32, 6400, 0, 56000),
    model_viewer_item(model_3d_items::fat_tree_full, "Fat tree", 40, 46000, 0, 49152),
    model_viewer_item(model_3d_items::big_tree_full, "Big tree", -20, 46000, 0, 49152),
    model_viewer_item(model_3d_items::start_flag_full, "Start flag", 48, 41500, 0, 49152),
    model_viewer_item(model_3d_items::checkpoint_flag_forwards_full, "Checkpoint flag", 48, 44828, 0, 49152),
    model_viewer_item(model_3d_items::spam_full, "Spam post", 32, 28000, 0, 49152),
    model_viewer_item(model_3d_items::antenna_full, "Antenna", 20, 26500, 0, 49152),
    model_viewer_item(model_3d_items::camera_full, "Camera", 5, 40000, 0, 49152),
    model_viewer_item(model_3d_items::stands_full, "Poor stands", 32, 26500, 0, 46000),
    model_viewer_item(model_3d_items::house_full, "Hip house", 32, 10000, 0, 49152),
    model_viewer_item(model_3d_items::fancy_stands_full, "VIP stands", 20, 43000, 0, 49152),
    model_viewer_item(model_3d_items::gable_house_full, "Gable house", -20, 56000, 0, 49152),
    model_viewer_item(model_3d_items::bear_full, "Hungry bear", 32, -8192, 0, 49152),
    model_viewer_item(model_3d_items::chinese_house_full, "Chinese house", 40, 47000, 0, 49152),
    model_viewer_item(model_3d_items::butano_house_full, "Butano house", 60, 16384, 0, 49152),
    model_viewer_item(model_3d_items::turbo, "Turbo boost!", 150, 28672, 0, 40704),
    model_viewer_item(model_3d_items::explosion_6, "Explosion", 150, 28672, 0, 40704 + 2048),
    model_viewer_item(model_3d_items::statue_full, "Timid statue", 24, -4096, 0, 49152),
};

}

#endif
