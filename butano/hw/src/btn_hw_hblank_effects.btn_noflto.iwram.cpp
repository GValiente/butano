#include "../include/btn_hw_hblank_effects.h"

#include "tonc.h"

namespace btn::hw::hblank_effects
{

namespace
{
    class static_data
    {

    public:
        entry* entries = nullptr;
        int entries_count = 0;
    };

    BTN_DATA_EWRAM static_data data;
}

void commit(entry* entries, int entries_count)
{
    data.entries = entries;
    data.entries_count = entries_count;
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

    entry* entries = data.entries;
    int entries_count = data.entries_count;

    for(int index = 0; index < entries_count; ++index)
    {
        entry& entry = entries[index];
        *entry.dest = entry.src[vcount];
    }
}

}
