/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_KEYPAD_H
#define BN_HW_KEYPAD_H

#include "bn_span.h"
#include "bn_keypad.h"
#include "bn_hw_tonc.h"

namespace bn::hw::keypad
{
    [[nodiscard]] inline unsigned get()
    {
        return ~REG_KEYINPUT & KEY_MASK;
    }

    inline void set_interrupt(const span<const bn::keypad::key_type>& keys)
    {
        unsigned p1_cnt = BIT(14) | BIT(15);

        for(bn::keypad::key_type key : keys)
        {
            p1_cnt |= unsigned(key);
        }

        REG_P1CNT = uint16_t(p1_cnt);
    }
}

#endif
