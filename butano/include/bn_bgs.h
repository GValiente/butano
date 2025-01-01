/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BGS_H
#define BN_BGS_H

/**
 * @file
 * bn::bgs header file.
 *
 * @ingroup bg
 */

#include "../hw/include/bn_hw_bgs_constants.h"

/**
 * @brief Backgrounds related functions.
 *
 * @ingroup bg
 */
namespace bn::bgs
{
    /**
     * @brief Returns the number of used background items.
     *
     * A background item which is hidden is not committed to the GBA, so there can be more than 4 background items.
     */
    [[nodiscard]] int used_items_count();

    /**
     * @brief Returns the number of available background items.
     *
     * A background item which is hidden is not committed to the GBA, so there can be more than 4 background items.
     */
    [[nodiscard]] int available_items_count();

    /**
     * @return Returns the minimum priority of a background relative to sprites and other backgrounds.
     */
    [[nodiscard]] constexpr int min_priority()
    {
        return hw::bgs::min_priority();
    }

    /**
     * @return Returns the maximum priority of a background relative to sprites and other backgrounds.
     */
    [[nodiscard]] constexpr int max_priority()
    {
        return hw::bgs::max_priority();
    }

    /**
     * @return Returns the minimum priority of a background relative to other backgrounds.
     */
    [[nodiscard]] constexpr int min_z_order()
    {
        return -32767;
    }

    /**
     * @return Returns the maximum priority of a background relative to other backgrounds.
     */
    [[nodiscard]] constexpr int max_z_order()
    {
        return 32767;
    }
}

#endif
