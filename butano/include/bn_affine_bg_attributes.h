/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AFFINE_BG_ATTRIBUTES_H
#define BN_AFFINE_BG_ATTRIBUTES_H

/**
 * @file
 * bn::affine_bg_attributes header file.
 *
 * @ingroup affine_bg
 */

#include "bn_affine_bg_map_ptr.h"

namespace bn
{

/**
 * @brief Manages the attributes to commit to the GBA register of an affine background.
 *
 * @ingroup affine_bg
 */
class affine_bg_attributes
{

public:
    /**
     * @brief Constructor.
     * @param map affine_bg_map_ptr of an affine background to copy.
     * @param priority Priority of an affine background relative to sprites and other backgrounds,
     * in the range [0..3].
     *
     * Backgrounds with higher priority are drawn first
     * (and therefore can be covered by later sprites and backgrounds).
     * @param wrapping_enabled Indicates if an affine background wraps around at the edges or not.
     * @param mosaic_enabled Indicates if the mosaic effect is applied to an affine background or not.
     */
    affine_bg_attributes(const affine_bg_map_ptr& map, int priority, bool wrapping_enabled, bool mosaic_enabled);

    /**
     * @brief Constructor.
     * @param map affine_bg_map_ptr of an affine background to move.
     * @param priority Priority of an affine background relative to sprites and other backgrounds,
     * in the range [0..3].
     *
     * Backgrounds with higher priority are drawn first
     * (and therefore can be covered by later sprites and backgrounds).
     * @param wrapping_enabled Indicates if an affine background wraps around at the edges or not.
     * @param mosaic_enabled Indicates if the mosaic effect is applied to an affine background or not.
     */
    affine_bg_attributes(affine_bg_map_ptr&& map, int priority, bool wrapping_enabled, bool mosaic_enabled);

    /**
     * @brief Returns the affine_bg_map_ptr of an affine background.
     */
    [[nodiscard]] const affine_bg_map_ptr& map() const
    {
        return _map;
    }

    /**
     * @brief Sets the affine_bg_map_ptr of an affine background.
     * @param map affine_bg_map_ptr of an affine background to copy.
     */
    void set_map(const affine_bg_map_ptr& map)
    {
        _map = map;
    }

    /**
     * @brief Sets the affine_bg_map_ptr of an affine background.
     * @param map affine_bg_map_ptr of an affine background to move.
     */
    void set_map(affine_bg_map_ptr&& map)
    {
        _map = move(map);
    }

    /**
     * @brief Returns the priority of an affine background relative to sprites and other backgrounds.
     *
     * Backgrounds with higher priority are drawn first
     * (and therefore can be covered by later sprites and backgrounds).
     */
    [[nodiscard]] int priority() const
    {
        return _priority;
    }

    /**
     * @brief Sets the priority of an affine background relative to sprites and other backgrounds.
     *
     * Backgrounds with higher priority are drawn first
     * (and therefore can be covered by later sprites and backgrounds).
     *
     * @param priority Priority in the range [0..3].
     */
    void set_priority(int priority);

    /**
     * @brief Indicates if an affine background wraps around at the edges or not.
     */
    [[nodiscard]] bool wrapping_enabled() const
    {
        return _wrapping_enabled;
    }

    /**
     * @brief Sets if an affine background must wrap around at the edges or not.
     */
    void set_wrapping_enabled(bool wrapping_enabled)
    {
        _wrapping_enabled = wrapping_enabled;
    }

    /**
     * @brief Indicates if the mosaic effect is applied to an affine background or not.
     */
    [[nodiscard]] bool mosaic_enabled() const
    {
        return _mosaic_enabled;
    }

    /**
     * @brief Sets if the mosaic effect must be applied to an affine background or not.
     */
    void set_mosaic_enabled(bool mosaic_enabled)
    {
        _mosaic_enabled = mosaic_enabled;
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] friend bool operator==(const affine_bg_attributes& a, const affine_bg_attributes& b) = default;

private:
    affine_bg_map_ptr _map;
    int8_t _priority;
    bool _wrapping_enabled;
    bool _mosaic_enabled;
};

}

#endif

