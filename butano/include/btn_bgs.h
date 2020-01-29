#ifndef BTN_BGS_H
#define BTN_BGS_H

#include "btn_common.h"

namespace btn::bgs
{
    [[nodiscard]] int used_count();

    [[nodiscard]] int available_count();
}

#endif
