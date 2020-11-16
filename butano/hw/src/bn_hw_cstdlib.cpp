/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_cstdlib.h"

#include <cstdlib>

void* malloc(std::size_t bytes)
{
    return bn::malloc(bytes);
}

void free(void* ptr)
{
    bn::free(ptr);
}
