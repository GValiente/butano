/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_sram.h"

#include "bn_config_sram.h"
#include "../include/bn_hw_tonc.h"

namespace bn::hw::sram
{

static_assert(BN_CFG_SRAM_WAIT_STATE == BN_SRAM_WAIT_STATE_4 ||
        BN_CFG_SRAM_WAIT_STATE == BN_SRAM_WAIT_STATE_3 ||
        BN_CFG_SRAM_WAIT_STATE == BN_SRAM_WAIT_STATE_2 ||
        BN_CFG_SRAM_WAIT_STATE == BN_SRAM_WAIT_STATE_8);

void init()
{
    asm volatile(".align 2; .ascii \"SRAM_V113\"; .align");

    BIT_SET(REG_WAITCNT_NV, BN_CFG_SRAM_WAIT_STATE);
}

}
