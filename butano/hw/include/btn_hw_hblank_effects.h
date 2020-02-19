#ifndef BTN_HW_HBLANK_EFFECTS_H
#define BTN_HW_HBLANK_EFFECTS_H

#include "btn_hw_irq.h"
#include "btn_hw_display.h"

namespace btn::hw::hblank_effects
{
    class entry
    {

    public:
        uint16_t src[display::height()];
        volatile uint16_t* dest;
    };

    BTN_CODE_IWRAM void _intr();

    BTN_CODE_IWRAM void _commit_impl(entry* entries, int entries_count);

    inline void init()
    {
        irq::add(irq::id::HBLANK, _intr);
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

    inline void commit(entry* entries, int entries_count)
    {
        disable();

        if(entries_count)
        {
            _commit_impl(entries, entries_count);
            enable();
        }
    }
}

#endif
