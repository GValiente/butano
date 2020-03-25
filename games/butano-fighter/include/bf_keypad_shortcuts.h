#ifndef BF_KEYPAD_SHORTCUTS_H
#define BF_KEYPAD_SHORTCUTS_H

namespace bf
{

class keypad_shortcuts
{

public:
    void update();

private:
    bool _sleep_ready = false;
    bool _reset_ready = false;
};

}

#endif
