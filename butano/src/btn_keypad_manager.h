#ifndef BTN_KEYPAD_MANAGER_H
#define BTN_KEYPAD_MANAGER_H

#include "btn_keypad.h"
#include "btn_span_fwd.h"

namespace btn
{
    class string_view;
}

namespace btn::keypad_manager
{
    using key_type = keypad::key_type;

    void init(const string_view& commands);

    [[nodiscard]] bool held(key_type key);

    [[nodiscard]] bool pressed(key_type key);

    [[nodiscard]] bool released(key_type key);

    void update();

    void set_interrupt(const span<const key_type>& keys);

    void stop();
}

#endif
