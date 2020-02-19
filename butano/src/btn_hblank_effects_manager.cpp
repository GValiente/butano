#include "btn_hblank_effects_manager.h"

#include "btn_vector.h"
#include "btn_config_hblank_effects.h"
#include "../hw/include/btn_hw_hblank_effects.h"

namespace btn::hblank_effects_manager
{

namespace
{
    using hw_entry = hw::hblank_effects::entry;

    class static_data
    {

    public:
        vector<hw_entry, BTN_CFG_HBLANK_EFFECTS_MAX_RUNNING> entries_a;
        vector<hw_entry, BTN_CFG_HBLANK_EFFECTS_MAX_RUNNING> entries_b;
        bool commit = false;
    };

    BTN_DATA_EWRAM static_data data;
}

void init()
{
    hw::hblank_effects::init();
}

void enable()
{
    hw::hblank_effects::enable();
}

void disable()
{
    hw::hblank_effects::disable();
}

void update()
{
    /*if(data.entries_a.empty())
    {
        data.entries_a.push_back(hw_entry());
        data.entries_a.push_back(hw_entry());
        data.commit = true;

        hw_entry& entry_0 = data.entries_a[0];
        entry_0.dest = &REG_BG3HOFS;

        hw_entry& entry_1 = data.entries_a[1];
        entry_1.dest = &REG_BG2HOFS;

        uint16_t* src_0 = entry_0.src;
        uint16_t* src_1 = entry_1.src;

        for(int index = 0; index < 160; ++index)
        {
            src_0[index] = uint16_t(index);
            src_1[index] = uint16_t(255 - index);
        }
    }*/
}

void commit()
{
    if(data.commit)
    {
        data.commit = false;
        hw::hblank_effects::commit(data.entries_a.data(), data.entries_a.size());
    }
}

void stop()
{
    disable();
}

}
