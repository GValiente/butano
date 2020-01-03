#include "btn_keypad.h"

#include "../hw/include/btn_hw_keypad.h"

namespace btn::keypad
{

bool down(button_type button)
{
    return hw::keypad::down(button);
}

bool up(button_type button)
{
    return hw::keypad::up(button);
}

bool held(button_type button)
{
    return hw::keypad::held(button);
}

bool pressed(button_type button)
{
    return hw::keypad::pressed(button);
}

bool released(button_type button)
{
    return hw::keypad::released(button);
}

}
