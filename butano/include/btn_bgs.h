#ifndef BTN_BGS_H
#define BTN_BGS_H

#include "../hw/include/btn_hw_bgs_constants.h"

namespace btn::bgs
{
    [[nodiscard]] int used_count();

    [[nodiscard]] int available_count();
}

#endif
