/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BITMAP_BG_ATTRIBUTES_H
#define BN_BITMAP_BG_ATTRIBUTES_H

/**
 * @file
 * bn::bitmap_bg_attributes header file.
 *
 * @ingroup bitmap_bg
 */

#include "bn_common.h"

namespace bn
{

enum class green_swap_mode : uint8_t;

/**
 * @brief Manages the attributes to commit to the GBA register of a bitmap background.
 *
 * @ingroup bitmap_bg
 */
class bitmap_bg_attributes
{

public:
    /**
     * @brief Constructor.
     * @param priority Priority of a background relative to sprites and other backgrounds,
     * in the range [0..3].
     *
     * Backgrounds with higher priority are drawn first
     * (and therefore can be covered by later sprites and backgrounds).
     * @param mosaic_enabled Indicates if the mosaic effect is applied to an affine background or not.
     */
    bitmap_bg_attributes(int priority, bool mosaic_enabled);

    /**
     * @brief Constructor.
     * @param priority Priority of a background relative to sprites and other backgrounds,
     * in the range [0..3].
     *
     * Backgrounds with higher priority are drawn first
     * (and therefore can be covered by later sprites and backgrounds).
     * @param mosaic_enabled Indicates if the mosaic effect is applied to a background or not.
     * @param green_swap_mode Indicates how a background must be displayed when green swap is enabled.
     */
    bitmap_bg_attributes(int priority, bool mosaic_enabled, bn::green_swap_mode green_swap_mode);

    /**
     * @brief Returns the priority of a background relative to sprites and other backgrounds.
     *
     * Backgrounds with higher priority are drawn first
     * (and therefore can be covered by later sprites and backgrounds).
     */
    [[nodiscard]] int priority() const
    {
        return _priority;
    }

    /**
     * @brief Sets the priority of a background relative to sprites and other backgrounds.
     *
     * Backgrounds with higher priority are drawn first
     * (and therefore can be covered by later sprites and backgrounds).
     *
     * @param priority Priority in the range [0..3].
     */
    void set_priority(int priority);

    /**
     * @brief Indicates if the mosaic effect is applied to a background or not.
     */
    [[nodiscard]] bool mosaic_enabled() const
    {
        return _mosaic_enabled;
    }

    /**
     * @brief Sets if the mosaic effect must be applied to a background or not.
     */
    void set_mosaic_enabled(bool mosaic_enabled)
    {
        _mosaic_enabled = mosaic_enabled;
    }

    /**
     * @brief Indicates how a background must be displayed when green swap is enabled.
     */
    [[nodiscard]] bn::green_swap_mode green_swap_mode() const
    {
        return _green_swap_mode;
    }

    /**
     * @brief Sets how a background must be displayed when green swap is enabled.
     */
    void set_green_swap_mode(bn::green_swap_mode green_swap_mode)
    {
        _green_swap_mode = green_swap_mode;
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] friend bool operator==(const bitmap_bg_attributes& a, const bitmap_bg_attributes& b) = default;

private:
    int8_t _priority;
    bool _mosaic_enabled;
    bn::green_swap_mode _green_swap_mode;
};

}

#endif

