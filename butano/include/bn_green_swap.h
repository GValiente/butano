/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_GREEN_SWAP_H
#define BTN_GREEN_SWAP_H

/**
 * @file
 * btn::green_swap header file.
 *
 * @ingroup green_swap
 */

#include "btn_common.h"


/**
 * @brief When it is enabled, green intensity of each two screen pixels are exchanged.
 *
 * It produces an interesting dirt effect.
 *
 * @ingroup green_swap
 */
namespace btn::green_swap
{
    /**
     * @brief Indicates if green swap is enabled or not.
     */
    [[nodiscard]] bool enabled();

    /**
     * @brief Sets if green swap must be enabled or not.
     */
    void set_enabled(bool enabled);
}

#endif
