/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/btn_hw_sram.h"

#include "btn_config_sram.h"
#include "../include/btn_hw_tonc.h"

namespace btn::hw::sram
{

static_assert(BTN_CFG_SRAM_WAIT_STATE == BTN_SRAM_WAIT_STATE_4 ||
        BTN_CFG_SRAM_WAIT_STATE == BTN_SRAM_WAIT_STATE_3 ||
        BTN_CFG_SRAM_WAIT_STATE == BTN_SRAM_WAIT_STATE_2 ||
        BTN_CFG_SRAM_WAIT_STATE == BTN_SRAM_WAIT_STATE_8);

namespace
{
    // https://forum.gbadev.org/viewtopic.php?f=4&t=2825
    alignas(int) __attribute__((used)) const char save_type[] = "SRAM_V113";
}

void init()
{
    BIT_SET(REG_WAITCNT_NV, BTN_CFG_SRAM_WAIT_STATE);
}

}
