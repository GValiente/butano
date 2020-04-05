#ifndef BTN_HW_BGS_CONSTANTS_H
#define BTN_HW_BGS_CONSTANTS_H

#include "btn_common.h"

namespace btn::bgs
{
    [[nodiscard]] constexpr int max_priority()
    {
        return 3;
    }
}

#endif
