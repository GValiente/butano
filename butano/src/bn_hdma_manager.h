/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HDMA_MANAGER_H
#define BN_HDMA_MANAGER_H

#include "bn_common.h"

namespace bn::hdma_manager
{
    void start(const uint16_t& source_ref, int elements, uint16_t& destination_ref);

    void stop();

    void update();

    void commit();
}

#endif
