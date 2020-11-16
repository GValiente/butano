/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_HBLANK_EFFECTS_H
#define BN_HW_HBLANK_EFFECTS_H

#include "bn_config_hblank_effects.h"
#include "bn_hw_irq.h"

namespace bn::hw::hblank_effects
{
    class entry
    {

    public:
        const uint16_t* src;
        volatile uint16_t* dest;
    };

    BN_CODE_IWRAM void commit_entries_ptr(entry* entries_ptr);

    BN_CODE_IWRAM void _intr_0();

    BN_CODE_IWRAM void _intr_1();

    #if BN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 2
        BN_CODE_IWRAM void _intr_2();
    #endif

    #if BN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 3
        BN_CODE_IWRAM void _intr_3();
    #endif

    #if BN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 4
        BN_CODE_IWRAM void _intr_4();
    #endif

    #if BN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 5
        BN_CODE_IWRAM void _intr_5();
    #endif

    #if BN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 6
        BN_CODE_IWRAM void _intr_6();
    #endif

    #if BN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 7
        BN_CODE_IWRAM void _intr_7();
    #endif

    #if BN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 8
        BN_CODE_IWRAM void _intr_8();
    #endif

    inline void init()
    {
        irq::replace_or_push_back(irq::id::HBLANK, _intr_0);
        irq::disable(irq::id::HBLANK);
    }

    inline void enable()
    {
        irq::enable(irq::id::HBLANK);
    }

    inline void disable()
    {
        irq::disable(irq::id::HBLANK);
    }

    inline void commit_entries_count(int entries_count)
    {
        switch(entries_count)
        {

        case 1:
            irq::replace_or_push_back(irq::id::HBLANK, _intr_1);
            break;

        #if BN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 2
            case 2:
                irq::replace_or_push_back(irq::id::HBLANK, _intr_2);
                break;
        #endif

        #if BN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 3
            case 3:
                irq::replace_or_push_back(irq::id::HBLANK, _intr_3);
                break;
        #endif

        #if BN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 4
            case 4:
                irq::replace_or_push_back(irq::id::HBLANK, _intr_4);
                break;
        #endif

        #if BN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 5
            case 5:
                irq::replace_or_push_back(irq::id::HBLANK, _intr_5);
                break;
        #endif

        #if BN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 6
            case 6:
                irq::replace_or_push_back(irq::id::HBLANK, _intr_6);
                break;
        #endif

        #if BN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 7
            case 7:
                irq::replace_or_push_back(irq::id::HBLANK, _intr_7);
                break;
        #endif

        #if BN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 8
            case 8:
                irq::replace_or_push_back(irq::id::HBLANK, _intr_8);
                break;
        #endif

        default:
            break;
        }
    }
}

#endif
