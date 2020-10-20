/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CSTRING_H
#define BTN_CSTRING_H

#include "btn_common.h"

namespace btn
{
    void memcpy(void* destination, const void* source, int bytes);

    void memset(void* destination, uint8_t value, int bytes);

    void memclear(void* destination, int bytes);
}

#endif
