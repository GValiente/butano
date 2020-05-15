#include "btn_keypad.h"

#include "btn_keypad_manager.h"

namespace btn::keypad
{

bool down(key_type key)
{
    return keypad_manager::down(key);
}

bool up(key_type key)
{
    return keypad_manager::up(key);
}

bool pressed(key_type key)
{
    return keypad_manager::pressed(key);
}

bool released(key_type key)
{
    return keypad_manager::released(key);
}

}
