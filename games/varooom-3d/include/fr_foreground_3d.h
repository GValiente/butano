/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_FOREGROUND_3D_H
#define FR_FOREGROUND_3D_H

#include "fr_visible_model_3d_grid.h"

namespace fr
{

class stage;
class models_3d;
class camera_3d;
class announcer;
class player_car;

class foreground_3d
{

public:
    foreground_3d(const stage& stage, const camera_3d& camera, models_3d& models);

    void check_collision(const stage& stage, player_car& player_car, announcer& announcer);

    void update(const stage& stage, const camera_3d& camera, models_3d& models);

private:
    const model_3d_item* _static_model_items[constants_3d::max_static_models];

    void _reload_static_model_items(const stage& stage, const camera_3d& camera, models_3d& models);

    [[nodiscard]] BN_CODE_IWRAM int _reload_static_model_items_impl(
            const camera_3d& camera, const model_3d_item* model_items,
            const visible_model_3d_grid::cell& visible_cell);
};

}

#endif
