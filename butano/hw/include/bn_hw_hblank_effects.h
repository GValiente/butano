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
    class uint16_entry
    {

    public:
        const uint16_t* src;
        volatile uint16_t* dest;
    };

    class uint32_entry
    {

    public:
        const unsigned* src;
        volatile unsigned* dest;
    };

    class entries
    {

    public:
        uint16_entry uint16_entries[BN_CFG_HBLANK_EFFECTS_MAX_ITEMS];
        uint32_entry uint32_entries[BN_CFG_HBLANK_EFFECTS_MAX_ITEMS];
        int uint16_entries_count = 0;
        int uint32_entries_count = 0;
    };

    BN_CODE_IWRAM void commit_entries(entries& entries_ref);

    BN_CODE_IWRAM void _intr();

    inline void init(entries& entries_ref)
    {
        commit_entries(entries_ref);
        irq::replace_or_push_back(irq::id::HBLANK, _intr);
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
}

#endif
