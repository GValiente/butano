/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HDMA_MANAGER_H
#define BN_HDMA_MANAGER_H

#include "bn_hdma.h"

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

    [[nodiscard]] hdma::interrupt_handler_type high_priority_interrupt_handler();

    void set_high_priority_interrupt_handler(hdma::interrupt_handler_type interrupt_handler);

    void update();

    void commit_interrupt_handler();

    bool commit_entries(bool use_dma);
}

#endif
