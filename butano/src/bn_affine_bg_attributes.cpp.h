/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_affine_bg_attributes.h"

#include "bn_bgs.h"
#include "bn_assert.h"
#include "bn_green_swap_mode.h"

namespace bn
{

affine_bg_attributes::affine_bg_attributes(
        const affine_bg_map_ptr& map, int priority, bool wrapping_enabled, bool mosaic_enabled) :
    _map(map),
    _priority(int8_t(priority)),
    _wrapping_enabled(wrapping_enabled),
    _mosaic_enabled(mosaic_enabled),
    _green_swap_mode(bn::green_swap_mode::DEFAULT)
{
    BN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);
}

affine_bg_attributes::affine_bg_attributes(
        const affine_bg_map_ptr& map, int priority, bool wrapping_enabled, bool mosaic_enabled,
        bn::green_swap_mode green_swap_mode) :
    _map(map),
    _priority(int8_t(priority)),
    _wrapping_enabled(wrapping_enabled),
    _mosaic_enabled(mosaic_enabled),
    _green_swap_mode(green_swap_mode)
{
    BN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);
}

affine_bg_attributes::affine_bg_attributes(
        affine_bg_map_ptr&& map, int priority, bool wrapping_enabled, bool mosaic_enabled) :
    _map(move(map)),
    _priority(int8_t(priority)),
    _wrapping_enabled(wrapping_enabled),
    _mosaic_enabled(mosaic_enabled),
    _green_swap_mode(bn::green_swap_mode::DEFAULT)
{
    BN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);
}

affine_bg_attributes::affine_bg_attributes(
        affine_bg_map_ptr&& map, int priority, bool wrapping_enabled, bool mosaic_enabled,
        bn::green_swap_mode green_swap_mode) :
    _map(move(map)),
    _priority(int8_t(priority)),
    _wrapping_enabled(wrapping_enabled),
    _mosaic_enabled(mosaic_enabled),
    _green_swap_mode(green_swap_mode)
{
    BN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);
}

void affine_bg_attributes::set_priority(int priority)
{
    BN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);

    _priority = int8_t(priority);
}

}
