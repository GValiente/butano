/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_cstdlib.h"

#include <cstdlib>

void* malloc(std::size_t bytes)
{
    return btn::malloc(bytes);
}

void free(void* ptr)
{
    btn::free(ptr);
}
