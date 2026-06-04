/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_TEXT_H
#define BN_HW_TEXT_H

#include "bn_common.h"

namespace bn::hw::text
{
    [[nodiscard]] int parse(int value, char* output);

    [[nodiscard]] int parse(long value, char* output);

    [[nodiscard]] int parse(int64_t value, char* output);

    [[nodiscard]] int parse(unsigned value, char* output);

    [[nodiscard]] int parse(unsigned long value, char* output);

    [[nodiscard]] int parse(uint64_t value, char* output);

    [[nodiscard]] int parse(const void* ptr, char* output);
}

#endif
