#include "bf_keypad_shortcuts.h"

#include "btn_span.h"
#include "btn_core.h"
#include "btn_keypad.h"

namespace bf
{

void keypad_shortcuts::update()
{
    if(! btn::keypad::held(btn::keypad::button_type::SELECT))
    {
        _select_released = true;
    }

    if(_select_released && btn::keypad::held(btn::keypad::button_type::SELECT))
    {
        if(btn::keypad::held(btn::keypad::button_type::B))
        {
            const btn::keypad::button_type wake_up_buttons[] = {
                btn::keypad::button_type::SELECT,
                btn::keypad::button_type::B
            };

            btn::core::sleep(wake_up_buttons);
        }
        else if(btn::keypad::held(btn::keypad::button_type::START) &&
                btn::keypad::held(btn::keypad::button_type::L) &&
                btn::keypad::held(btn::keypad::button_type::R))
        {
            btn::core::reset();
        }
    }
}

}
