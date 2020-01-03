#ifndef BTN_HW_KEYPAD_H
#define BTN_HW_KEYPAD_H

#include "btn_span_fwd.h"

namespace btn
{
namespace keypad
{
    enum class button_type;
}

namespace hw::keypad
{
    [[nodiscard]] bool down(btn::keypad::button_type button);

    [[nodiscard]] bool up(btn::keypad::button_type button);

    [[nodiscard]] bool held(btn::keypad::button_type button);

    [[nodiscard]] bool pressed(btn::keypad::button_type button);

    [[nodiscard]] bool released(btn::keypad::button_type button);

    void update();

    void set_interrupt(const span<const btn::keypad::button_type>& buttons);
}
}

#endif
