/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CONFIG_SRAM_H
#define BTN_CONFIG_SRAM_H

/**
 * @file
 * SRAM configuration header file.
 *
 * @ingroup sram
 */

#include "btn_sram_wait_state.h"

/**
 * @def BTN_CFG_SRAM_WAIT_STATE
 *
 * Specifies SRAM access time in clock cycles.
 *
 * Values not specified in BTN_SRAM_WAIT_STATE_* macros are not allowed.
 *
 * @ingroup sram
 */
#ifndef BTN_CFG_SRAM_WAIT_STATE
    #define BTN_CFG_SRAM_WAIT_STATE BTN_SRAM_WAIT_STATE_8
#endif

#endif
