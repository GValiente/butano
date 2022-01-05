/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_memory.h"

#include "bn_random.h"
#include "bn_config_ewram.h"

extern unsigned __iwram_start__;
extern unsigned __iwram_top;
extern unsigned __fini_array_end;
extern unsigned __ewram_start;
extern unsigned __ewram_end;
extern char __eheap_start[], __eheap_end[];

namespace bn::hw::memory
{

static_assert(BN_CFG_EWRAM_WAIT_STATE == BN_EWRAM_WAIT_STATE_2 ||
        BN_CFG_EWRAM_WAIT_STATE == BN_EWRAM_WAIT_STATE_1);

#if BN_CFG_EWRAM_WAIT_STATE == BN_EWRAM_WAIT_STATE_1
    namespace
    {
        BN_DATA_EWRAM unsigned ewram_data;
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

int used_stack_iwram(int current_stack_address)
{
    auto iwram_top = reinterpret_cast<uint8_t*>(&__iwram_top);
    auto iwram_stack = reinterpret_cast<uint8_t*>(&current_stack_address);
    return iwram_top - iwram_stack;
}

int used_static_iwram()
{
    auto iwram_start = reinterpret_cast<uint8_t*>(&__iwram_start__);
    auto iwram_end = reinterpret_cast<uint8_t*>(&__fini_array_end);
    return iwram_end - iwram_start;
}

int used_static_ewram()
{
    auto ewram_start = reinterpret_cast<uint8_t*>(&__ewram_start);
    auto ewram_end = reinterpret_cast<uint8_t*>(&__ewram_end);
    return ewram_end - ewram_start;
}

char* ewram_heap_start()
{
    return __eheap_start;
}

char* ewram_heap_end()
{
    return __eheap_end;
}

}
