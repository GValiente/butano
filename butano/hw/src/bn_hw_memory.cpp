/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_memory.h"

#include "bn_config_ewram.h"

#if BN_CFG_EWRAM_WAIT_STATE == BN_EWRAM_WAIT_STATE_1
    #include "bn_random.h"
#endif
	
namespace bn::hw::memory
{

static_assert(BN_CFG_EWRAM_WAIT_STATE == BN_EWRAM_WAIT_STATE_2 ||
        BN_CFG_EWRAM_WAIT_STATE == BN_EWRAM_WAIT_STATE_1);

#if BN_CFG_EWRAM_WAIT_STATE == BN_EWRAM_WAIT_STATE_1
    namespace
    {
        BN_DATA_EWRAM_BSS unsigned ewram_data;
    }
#endif

void init()
{
    #if BN_CFG_EWRAM_WAIT_STATE == BN_EWRAM_WAIT_STATE_1
        volatile unsigned& memctrl_register = *reinterpret_cast<unsigned*>(0x4000800);
        memctrl_register = 0x0E000020;

        bn::random random;

        for(int index = 8; index >= 0; --index)
        {
            volatile unsigned& volatile_ewram_data = ewram_data;
            unsigned test_value = random.get();
            volatile_ewram_data = test_value;

            if(volatile_ewram_data != test_value)
            {
                memctrl_register = 0x0D000020;
                return;
            }
        }
    #endif
}

}
