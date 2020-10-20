/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_HW_HBLANK_EFFECTS_H
#define BTN_HW_HBLANK_EFFECTS_H

#include "btn_config_hblank_effects.h"
#include "btn_hw_irq.h"

namespace btn::hw::hblank_effects
{
    class entry
    {

    public:
        const uint16_t* src;
        volatile uint16_t* dest;
    };

    BTN_CODE_IWRAM void commit_entries_ptr(entry* entries_ptr);

    BTN_CODE_IWRAM void _intr_1();

    #if BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 2
        BTN_CODE_IWRAM void _intr_2();
    #endif

    #if BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 3
        BTN_CODE_IWRAM void _intr_3();
    #endif

    #if BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 4
        BTN_CODE_IWRAM void _intr_4();
    #endif

    #if BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 5
        BTN_CODE_IWRAM void _intr_5();
    #endif

    #if BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 6
        BTN_CODE_IWRAM void _intr_6();
    #endif

    #if BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 7
        BTN_CODE_IWRAM void _intr_7();
    #endif

    #if BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 8
        BTN_CODE_IWRAM void _intr_8();
    #endif

    inline void init()
    {
        irq::replace_or_push_back(irq::id::HBLANK, _intr_1);
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

        #if BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 2
            case 2:
                irq::replace_or_push_back(irq::id::HBLANK, _intr_2);
                break;
        #endif

        #if BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 3
            case 3:
                irq::replace_or_push_back(irq::id::HBLANK, _intr_3);
                break;
        #endif

        #if BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 4
            case 4:
                irq::replace_or_push_back(irq::id::HBLANK, _intr_4);
                break;
        #endif

        #if BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 5
            case 5:
                irq::replace_or_push_back(irq::id::HBLANK, _intr_5);
                break;
        #endif

        #if BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 6
            case 6:
                irq::replace_or_push_back(irq::id::HBLANK, _intr_6);
                break;
        #endif

        #if BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 7
            case 7:
                irq::replace_or_push_back(irq::id::HBLANK, _intr_7);
                break;
        #endif

        #if BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 8
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
