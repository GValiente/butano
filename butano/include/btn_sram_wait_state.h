/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SRAM_WAIT_STATE_H
#define BTN_SRAM_WAIT_STATE_H

#include "btn_common.h"

/**
 * @def BTN_SRAM_WAIT_STATE_4
 *
 * SRAM access time is 4+1 clock cycles.
 *
 * @ingroup sram
 */
#define BTN_SRAM_WAIT_STATE_4   0x0000

/**
 * @def BTN_SRAM_WAIT_STATE_3
 *
 * SRAM access time is 3+1 clock cycles.
 *
 * @ingroup sram
 */
#define BTN_SRAM_WAIT_STATE_3   0x0001

/**
 * @def BTN_SRAM_WAIT_STATE_2
 *
 * SRAM access time is 2+1 clock cycles.
 *
 * @ingroup sram
 */
#define BTN_SRAM_WAIT_STATE_2   0x0002

/**
 * @def BTN_SRAM_WAIT_STATE_8
 *
 * SRAM access time is 8+1 clock cycles.
 *
 * @ingroup sram
 */
#define BTN_SRAM_WAIT_STATE_8   0x0003

#endif
