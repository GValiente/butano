/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BGS_H
#define BTN_BGS_H

#include "../hw/include/btn_hw_bgs_constants.h"

namespace btn::bgs
{
    [[nodiscard]] int used_count();

    [[nodiscard]] int available_count();
}

#endif
