/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CSTDLIB_H
#define BTN_CSTDLIB_H

#include "btn_common.h"

namespace btn
{
    [[nodiscard]] void* malloc(int bytes);

    void free(void* ptr);
}

#endif
