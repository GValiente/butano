/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_MEMORY_MANAGER_H
#define BN_MEMORY_MANAGER_H

#include "bn_config_log.h"

namespace bn::memory_manager
{
    void init();

    [[nodiscard]] void* ewram_alloc(int bytes);

    [[nodiscard]] void* ewram_calloc(int num, int bytes);

    [[nodiscard]] void* ewram_realloc(void* ptr, int new_bytes);

    void ewram_free(void* ptr);

    [[nodiscard]] int used_alloc_ewram();

    [[nodiscard]] int available_alloc_ewram();

    #if BN_CFG_LOG_ENABLED
        void log_alloc_ewram_status();
    #endif
}

#endif
