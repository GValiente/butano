#ifndef BTN_HW_CORE_H
#define BTN_HW_CORE_H

#include "btn_common.h"

namespace btn::hw::core
{
    void wait_for_vblank();

    void sleep();

    [[noreturn]] void reset();
}

#endif
