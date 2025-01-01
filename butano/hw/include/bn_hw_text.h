/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_TEXT_H
#define BN_HW_TEXT_H

#include "bn_array_fwd.h"

namespace bn::hw::text
{
    [[nodiscard]] int parse(int value, array<char, 32>& output);

    [[nodiscard]] int parse(long value, array<char, 32>& output);

    [[nodiscard]] int parse(int64_t value, array<char, 32>& output);

    [[nodiscard]] int parse(unsigned value, array<char, 32>& output);

    [[nodiscard]] int parse(unsigned long value, array<char, 32>& output);

    [[nodiscard]] int parse(uint64_t value, array<char, 32>& output);

    [[nodiscard]] int parse(const void* ptr, array<char, 32>& output);
}

#endif
