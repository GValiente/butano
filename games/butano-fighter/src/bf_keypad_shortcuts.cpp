#include "bf_keypad_shortcuts.h"

#include "btn_span.h"
#include "btn_core.h"
#include "btn_keypad.h"

namespace bf
{

void keypad_shortcuts::update()
{
    bool b_held = btn::keypad::held(btn::keypad::button_type::B);
    bool select_held = btn::keypad::held(btn::keypad::button_type::SELECT);

    if(_sleep_ready)
    {
        if(b_held && select_held)
        {
            const btn::keypad::button_type wake_up_buttons[] = {
                btn::keypad::button_type::SELECT,
                btn::keypad::button_type::B
            };

            _sleep_ready = false;
            btn::core::sleep(wake_up_buttons);
        }
    }
    else
    {
        if(! b_held || ! select_held)
        {
            _sleep_ready = true;
        }
    }

    bool l_held = btn::keypad::held(btn::keypad::button_type::L);
    bool r_held = btn::keypad::held(btn::keypad::button_type::R);
    bool start_held = btn::keypad::held(btn::keypad::button_type::START);

    if(_reset_ready)
    {
        if(l_held && r_held && start_held)
        {
            btn::core::reset();
        }
    }
    else
    {
        if(! l_held || ! r_held || ! start_held)
        {
            _reset_ready = true;
        }
    }
}

}
