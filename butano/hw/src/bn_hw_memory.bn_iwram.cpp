/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_memory.h"

namespace bn::hw::memory
{

#if BN_CFG_EWRAM_WAIT_STATE == BN_EWRAM_WAIT_STATE_1
    namespace
    {
        BN_DATA_EWRAM unsigned ewram_data;
    }

    void init()
    {
        volatile unsigned& memctrl_register = *reinterpret_cast<unsigned*>(0x4000800);
        memctrl_register = 0x0E000020;

        unsigned test_values[] =
        {
            0x12345678,
            0x9ABCDEF1,
        };

        for(unsigned test_value : test_values)
        {
            volatile unsigned& volatile_ewram_data = ewram_data;
            volatile_ewram_data = test_value;

            if(volatile_ewram_data != test_value)
            {
                memctrl_register = 0x0D000020;
                return;
            }
        }
    }
#endif

}
