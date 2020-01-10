#ifndef BTN_GREEN_SWAP_H
#define BTN_GREEN_SWAP_H

#include "btn_common.h"

namespace btn::green_swap
{
    [[nodiscard]] bool enabled();

    void set_enabled(bool enabled);
}

#endif
