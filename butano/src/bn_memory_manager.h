/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_MEMORY_MANAGER_H
#define BN_MEMORY_MANAGER_H

#include "bn_common.h"

namespace bn::memory_manager
{
    void init();

    [[nodiscard]] void* ewram_alloc(int bytes);

    [[nodiscard]] void* ewram_calloc(int bytes);

    [[nodiscard]] void* ewram_realloc(void* ptr, int new_bytes);

    void ewram_free(void* ptr);

    [[nodiscard]] int used_alloc_ewram();

    [[nodiscard]] int available_alloc_ewram();

    [[nodiscard]] int used_items_ewram();

    [[nodiscard]] int available_items_ewram();
}

#endif
