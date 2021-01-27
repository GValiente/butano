/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HDMA_MANAGER_H
#define BN_HDMA_MANAGER_H

#include "bn_common.h"

namespace bn::hdma_manager
{
    void enable();

    void disable();

    void low_priority_start(const uint16_t& source_ref, int elements, uint16_t& destination_ref);

    void low_priority_stop();

    void high_priority_start(const uint16_t& source_ref, int elements, uint16_t& destination_ref);

    void high_priority_stop();

    void update();

    void commit();
}

#endif
