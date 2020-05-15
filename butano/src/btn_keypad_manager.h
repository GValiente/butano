#ifndef BTN_KEYPAD_MANAGER_H
#define BTN_KEYPAD_MANAGER_H

#include "btn_keypad.h"
#include "btn_span_fwd.h"

namespace btn::keypad_manager
{
    using key_type = keypad::key_type;

    [[nodiscard]] bool down(key_type key);

    [[nodiscard]] bool up(key_type key);

    [[nodiscard]] bool pressed(key_type key);

    [[nodiscard]] bool released(key_type key);

    void update();

    void set_interrupt(const span<const key_type>& keys);
}

#endif
