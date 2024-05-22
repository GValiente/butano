/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BGS_MOSAIC_H
#define BN_BGS_MOSAIC_H

/**
 * @file
 * bn::bgs_mosaic header file.
 *
 * @ingroup bg
 * @ingroup mosaic
 */

#include "bn_fixed.h"

/**
 * @brief Functions related with the mosaic applied to the backgrounds.
 *
 * @ingroup bg
 * @ingroup mosaic
 */
namespace bn::bgs_mosaic
{
    /**
     * @brief Returns the horizontal stretch of the mosaic applied to the backgrounds.
     */
    [[nodiscard]] fixed horizontal_stretch();

    /**
     * @brief Sets the horizontal stretch of the mosaic applied to the backgrounds.
     * @param horizontal_stretch Horizontal stretch in the range [0..1].
     */
    void set_horizontal_stretch(fixed horizontal_stretch);

    /**
     * @brief Returns the vertical stretch of the mosaic applied to the backgrounds.
     */
    [[nodiscard]] fixed vertical_stretch();

    /**
     * @brief Sets the vertical stretch of the mosaic applied to the backgrounds.
     * @param vertical_stretch Vertical stretch in the range [0..1].
     */
    void set_vertical_stretch(fixed vertical_stretch);

    /**
     * @brief Sets the stretch of the mosaic applied to the backgrounds.
     * @param stretch Stretch in the range [0..1].
     */
    void set_stretch(fixed stretch);

    /**
     * @brief Sets the stretch of the mosaic applied to the backgrounds.
     * @param horizontal_stretch Horizontal stretch in the range [0..1].
     * @param vertical_stretch Vertical stretch in the range [0..1].
     */
    void set_stretch(fixed horizontal_stretch, fixed vertical_stretch);
}

#endif
