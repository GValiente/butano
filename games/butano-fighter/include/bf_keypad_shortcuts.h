#ifndef BF_KEYPAD_SHORTCUTS_H
#define BF_KEYPAD_SHORTCUTS_H

namespace bf
{

class keypad_shortcuts
{

public:
    void update();

private:
    bool _select_released = false;
};

}

#endif
