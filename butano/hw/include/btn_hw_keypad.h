#ifndef BTN_HW_KEYPAD_H
#define BTN_HW_KEYPAD_H

#include "tonc.h"
#include "btn_span.h"
#include "btn_assert.h"
#include "btn_keypad.h"

namespace btn::hw::keypad
{
    [[nodiscard]] inline bool down(btn::keypad::button_type button)
    {
        return key_is_down(unsigned(button));
    }

    [[nodiscard]] inline bool up(btn::keypad::button_type button)
    {
        return key_is_up(unsigned(button));
    }

    [[nodiscard]] inline bool held(btn::keypad::button_type button)
    {
        return key_held(unsigned(button));
    }

    [[nodiscard]] inline bool pressed(btn::keypad::button_type button)
    {
        return key_hit(unsigned(button));
    }

    [[nodiscard]] inline bool released(btn::keypad::button_type button)
    {
        return key_released(unsigned(button));
    }

    inline void update()
    {
        key_poll();
    }

    inline void set_interrupt(const span<const btn::keypad::button_type>& buttons)
    {
        BTN_ASSERT(! buttons.empty(), "There's no buttons");

        unsigned p1_cnt = BIT(14) | BIT(15);

        for(btn::keypad::button_type button : buttons)
        {
            p1_cnt |= unsigned(button);
        }

        REG_P1CNT = uint16_t(p1_cnt);
    }
}

#endif
