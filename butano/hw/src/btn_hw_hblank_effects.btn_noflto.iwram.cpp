#include "../include/btn_hw_hblank_effects.h"

namespace btn::hw::hblank_effects
{

namespace
{
    class static_data
    {

    public:
        entry* entries_ptr = nullptr;
    };

    static_data data;
}

void commit_entries_ptr(entry* entries_ptr)
{
    data.entries_ptr = entries_ptr;
}

void _intr_1()
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

    entry* entries_ptr = data.entries_ptr;
    *entries_ptr[0].dest = entries_ptr[0].src[vcount];
}

#if BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 2
    void _intr_2()
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

        entry* entries_ptr = data.entries_ptr;
        *entries_ptr[0].dest = entries_ptr[0].src[vcount];
        *entries_ptr[1].dest = entries_ptr[1].src[vcount];
    }
#endif

#if BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 3
    void _intr_3()
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

        entry* entries_ptr = data.entries_ptr;
        *entries_ptr[0].dest = entries_ptr[0].src[vcount];
        *entries_ptr[1].dest = entries_ptr[1].src[vcount];
        *entries_ptr[2].dest = entries_ptr[2].src[vcount];
    }
#endif

#if BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 4
    void _intr_4()
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

        entry* entries_ptr = data.entries_ptr;
        *entries_ptr[0].dest = entries_ptr[0].src[vcount];
        *entries_ptr[1].dest = entries_ptr[1].src[vcount];
        *entries_ptr[2].dest = entries_ptr[2].src[vcount];
        *entries_ptr[3].dest = entries_ptr[3].src[vcount];
    }
#endif

#if BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 5
    void _intr_5()
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

        entry* entries_ptr = data.entries_ptr;
        *entries_ptr[0].dest = entries_ptr[0].src[vcount];
        *entries_ptr[1].dest = entries_ptr[1].src[vcount];
        *entries_ptr[2].dest = entries_ptr[2].src[vcount];
        *entries_ptr[3].dest = entries_ptr[3].src[vcount];
        *entries_ptr[4].dest = entries_ptr[4].src[vcount];
    }
#endif

#if BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 6
    void _intr_6()
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

        entry* entries_ptr = data.entries_ptr;
        *entries_ptr[0].dest = entries_ptr[0].src[vcount];
        *entries_ptr[1].dest = entries_ptr[1].src[vcount];
        *entries_ptr[2].dest = entries_ptr[2].src[vcount];
        *entries_ptr[3].dest = entries_ptr[3].src[vcount];
        *entries_ptr[4].dest = entries_ptr[4].src[vcount];
        *entries_ptr[5].dest = entries_ptr[5].src[vcount];
    }
#endif

#if BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 7
    void _intr_7()
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

        entry* entries_ptr = data.entries_ptr;
        *entries_ptr[0].dest = entries_ptr[0].src[vcount];
        *entries_ptr[1].dest = entries_ptr[1].src[vcount];
        *entries_ptr[2].dest = entries_ptr[2].src[vcount];
        *entries_ptr[3].dest = entries_ptr[3].src[vcount];
        *entries_ptr[4].dest = entries_ptr[4].src[vcount];
        *entries_ptr[5].dest = entries_ptr[5].src[vcount];
        *entries_ptr[6].dest = entries_ptr[6].src[vcount];
    }
#endif

#if BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS >= 8
    void _intr_8()
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

        entry* entries_ptr = data.entries_ptr;
        *entries_ptr[0].dest = entries_ptr[0].src[vcount];
        *entries_ptr[1].dest = entries_ptr[1].src[vcount];
        *entries_ptr[2].dest = entries_ptr[2].src[vcount];
        *entries_ptr[3].dest = entries_ptr[3].src[vcount];
        *entries_ptr[4].dest = entries_ptr[4].src[vcount];
        *entries_ptr[5].dest = entries_ptr[5].src[vcount];
        *entries_ptr[6].dest = entries_ptr[6].src[vcount];
        *entries_ptr[7].dest = entries_ptr[7].src[vcount];
    }
#endif

}
