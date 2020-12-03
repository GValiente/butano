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

    switch(entries_ptr->uint16_entries_count)
    {

    case 0:
        break;

    case 1:
        uint16_entries[0].update(vcount);
        break;

    case 2:
        uint16_entries[0].update(vcount);
        uint16_entries[1].update(vcount);
        break;

    case 3:
        uint16_entries[0].update(vcount);
        uint16_entries[1].update(vcount);
        uint16_entries[2].update(vcount);
        break;

    case 4:
        uint16_entries[0].update(vcount);
        uint16_entries[1].update(vcount);
        uint16_entries[2].update(vcount);
        uint16_entries[3].update(vcount);
        break;

    case 5:
        uint16_entries[0].update(vcount);
        uint16_entries[1].update(vcount);
        uint16_entries[2].update(vcount);
        uint16_entries[3].update(vcount);
        uint16_entries[4].update(vcount);
        break;

    case 6:
        uint16_entries[0].update(vcount);
        uint16_entries[1].update(vcount);
        uint16_entries[2].update(vcount);
        uint16_entries[3].update(vcount);
        uint16_entries[4].update(vcount);
        uint16_entries[5].update(vcount);
        break;

    case 7:
        uint16_entries[0].update(vcount);
        uint16_entries[1].update(vcount);
        uint16_entries[2].update(vcount);
        uint16_entries[3].update(vcount);
        uint16_entries[4].update(vcount);
        uint16_entries[5].update(vcount);
        uint16_entries[6].update(vcount);
        break;

    case 8:
        uint16_entries[0].update(vcount);
        uint16_entries[1].update(vcount);
        uint16_entries[2].update(vcount);
        uint16_entries[3].update(vcount);
        uint16_entries[4].update(vcount);
        uint16_entries[5].update(vcount);
        uint16_entries[6].update(vcount);
        uint16_entries[7].update(vcount);
        break;

    default:
        break;
    }

    uint32_entry* uint32_entries = entries_ptr->uint32_entries;

    switch(entries_ptr->uint32_entries_count)
    {

    case 0:
        break;

    case 1:
        uint32_entries[0].update(vcount);
        break;

    case 2:
        uint32_entries[0].update(vcount);
        uint32_entries[1].update(vcount);
        break;

    case 3:
        uint32_entries[0].update(vcount);
        uint32_entries[1].update(vcount);
        uint32_entries[2].update(vcount);
        break;

    case 4:
        uint32_entries[0].update(vcount);
        uint32_entries[1].update(vcount);
        uint32_entries[2].update(vcount);
        uint32_entries[3].update(vcount);
        break;

    default:
        break;
    }
}

}
