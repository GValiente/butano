/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CAMERAS_MANAGER_H
#define BN_CAMERAS_MANAGER_H

#include "bn_fixed_fwd.h"
#include "bn_fixed_point_fwd.h"

namespace bn::cameras_manager
{
    void init();

    [[nodiscard]] int used_items_count();

    [[nodiscard]] int available_items_count();

    [[nodiscard]] int create(const fixed_point& position);

    [[nodiscard]] int create_optional(const fixed_point& position);

    void increase_usages(int id);

    void decrease_usages(int id);

    [[nodiscard]] const fixed_point& position(int id);

    void set_x(int id, fixed x);

    void set_y(int id, fixed y);

    void set_position(int id, const fixed_point& position);

    void update();
}

#endif
