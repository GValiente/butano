#ifndef BTN_HW_KEYPAD_H
#define BTN_HW_KEYPAD_H

#include "btn_span.h"
#include "btn_keypad.h"
#include "btn_hw_tonc.h"

namespace btn::hw::keypad
{
    [[nodiscard]] inline unsigned get()
    {
        return ~REG_KEYINPUT & KEY_MASK;
    }

    inline void set_interrupt(const span<const btn::keypad::key_type>& keys)
    {
        unsigned p1_cnt = BIT(14) | BIT(15);

        for(btn::keypad::key_type key : keys)
        {
            p1_cnt |= unsigned(key);
        }

        REG_P1CNT = uint16_t(p1_cnt);
    }
}

#endif
