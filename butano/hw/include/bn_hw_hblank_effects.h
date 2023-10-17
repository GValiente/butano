/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_HBLANK_EFFECTS_H
#define BN_HW_HBLANK_EFFECTS_H

#include "bn_config_hbes.h"
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
        const uint32_t* src;
        volatile uint32_t* dest;
    };

    [[nodiscard]] constexpr int max_uint32_entries()
    {
        return 4;
    }

    class entries
    {

    public:
        int uint16_entries_count = 0;
        uint16_entry uint16_entries[BN_CFG_HBES_MAX_ITEMS];
        int uint32_entries_count = 0;
        uint32_entry uint32_entries[max_uint32_entries()];
    };

    extern entries* data;

    BN_CODE_IWRAM void _intr();

    inline void commit_entries(entries& entries_ref)
    {
        data = &entries_ref;
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
