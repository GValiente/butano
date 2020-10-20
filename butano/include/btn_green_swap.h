/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_GREEN_SWAP_H
#define BTN_GREEN_SWAP_H

#include "btn_common.h"

namespace btn::green_swap
{
    [[nodiscard]] bool enabled();

    void set_enabled(bool enabled);
}

#endif
