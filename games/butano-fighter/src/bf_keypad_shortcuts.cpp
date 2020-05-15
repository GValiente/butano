#include "bf_keypad_shortcuts.h"

#include "btn_span.h"
#include "btn_core.h"
#include "btn_keypad.h"

namespace bf
{

void keypad_shortcuts::update()
{
    bool b_held = btn::keypad::b_held();
    bool a_held = btn::keypad::a_held();
    bool l_held = btn::keypad::l_held();
    bool r_held = btn::keypad::r_held();
    bool select_held = btn::keypad::select_held();
    bool start_held = btn::keypad::start_held();

    if(_sleep_ready)
    {
        if(select_held && l_held && r_held)
        {
            const btn::keypad::key_type wake_up_keys[] = {
                btn::keypad::key_type::SELECT,
                btn::keypad::key_type::L,
                btn::keypad::key_type::R
            };

            _sleep_ready = false;
            btn::core::sleep(wake_up_keys);
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
