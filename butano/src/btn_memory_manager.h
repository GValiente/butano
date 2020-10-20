/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_MEMORY_MANAGER_H
#define BTN_MEMORY_MANAGER_H

#include "btn_common.h"

namespace btn::memory_manager
{
    void init();

    [[nodiscard]] void* ewram_alloc(int bytes);

    void ewram_free(void* ptr);

    [[nodiscard]] int used_alloc_ewram();

    [[nodiscard]] int available_alloc_ewram();
}

#endif
