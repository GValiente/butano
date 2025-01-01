/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
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

namespace
{
    // https://gbadev.net/forum-archive/thread/4/2825.html
    alignas(int) const char save_string[] = "SRAM_V113";
}

const char* init()
{
    BIT_SET(REG_WAITCNT_NV, BN_CFG_SRAM_WAIT_STATE);

    return save_string;
}

}
