/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_LINK_H
#define BN_HW_LINK_H

#include "bn_common.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"

#include "../3rd_party/gba-link-connection/include/LinkConnection.h"

#pragma GCC diagnostic pop

namespace bn::hw::link
{
    void _serial_intr();

    void _timer_intr();

    void init();

    [[nodiscard]] bool active();

    void enable();

    void disable();

    void deactivate();

    void send(int data_to_send);

    [[nodiscard]] bool receive(lc::LinkResponse& response);

    void commit();
}

#endif
