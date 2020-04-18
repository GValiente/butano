#include "bf_keypad_shortcuts.h"

#include "btn_span.h"
#include "btn_core.h"
#include "btn_keypad.h"

namespace bf
{

void keypad_shortcuts::update()
{
    bool b_held = btn::keypad::held(btn::keypad::button_type::B);
    bool a_held = btn::keypad::held(btn::keypad::button_type::A);
    bool l_held = btn::keypad::held(btn::keypad::button_type::L);
    bool r_held = btn::keypad::held(btn::keypad::button_type::R);
    bool select_held = btn::keypad::held(btn::keypad::button_type::SELECT);
    bool start_held = btn::keypad::held(btn::keypad::button_type::START);

    if(_sleep_ready)
    {
        if(select_held && l_held && r_held)
        {
            const btn::keypad::button_type wake_up_buttons[] = {
                btn::keypad::button_type::SELECT,
                btn::keypad::button_type::L,
                btn::keypad::button_type::R
            };

            _sleep_ready = false;
            btn::core::sleep(wake_up_buttons);
        }
    }
    else
    {
        if(! select_held || ! l_held || ! r_held)
        {
            _sleep_ready = true;
        }
    }

    if(_reset_ready)
    {
        if(select_held && start_held && b_held && a_held)
        {
            btn::core::reset();
        }
    }
    else
    {
        if(! select_held || ! start_held || ! b_held || ! a_held)
        {
            _reset_ready = true;
        }
    }
}

}
