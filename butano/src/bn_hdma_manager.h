/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HDMA_MANAGER_H
#define BN_HDMA_MANAGER_H

#include "bn_common.h"

namespace bn::hdma_manager
{
    void init();

    void enable();

    void force_stop();

    void disable();

    [[nodiscard]] bool low_priority_running();

    void low_priority_start(const uint16_t& source_ref, int elements, uint16_t& destination_ref);

    void low_priority_stop();

    [[nodiscard]] bool high_priority_running();

    void high_priority_start(const uint16_t& source_ref, int elements, uint16_t& destination_ref);

    void high_priority_stop();

    void update();

    bool commit(bool use_dma);
}

#endif
