/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_hblank_effects.h"

namespace bn::hw::hblank_effects
{

namespace
{
    class static_data
    {

    public:
        entries* entries_ptr = nullptr;
    };

    static_data data;
}

void commit_entries(entries& entries_ref)
{
    data.entries_ptr = &entries_ref;
}

void _intr()
{
    unsigned vcount = REG_VCOUNT;

    if(vcount < 159)
    {
        ++vcount;
    }
    else if(vcount > 226)
    {
        vcount = 0;
    }
    else
    {
        return;
    }

    entries* entries_ptr = data.entries_ptr;
    uint16_entry* uint16_entries = entries_ptr->uint16_entries;
    uint32_entry* uint32_entries = entries_ptr->uint32_entries;

    for(int index = 0, limit = entries_ptr->uint16_entries_count; index < limit; ++index)
    {
        uint16_entry& entry = uint16_entries[index];
        *entry.dest = entry.src[vcount];
    }

    for(int index = 0, limit = entries_ptr->uint32_entries_count; index < limit; ++index)
    {
        uint32_entry& entry = uint32_entries[index];
        *entry.dest = entry.src[vcount];
    }
}

}
