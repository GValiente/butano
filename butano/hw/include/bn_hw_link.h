/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_LINK_H
#define BN_HW_LINK_H

#include "bn_hw_irq.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"
#pragma GCC diagnostic ignored "-Wpedantic"

#include "../3rd_party/gba-link-connection/include/LinkConnection.h"

#pragma GCC diagnostic pop

namespace bn::hw::link
{
    void _serial_intr();

    void _timer_intr();

    inline void enable()
    {
        irq::enable(irq::id::SERIAL);
        irq::enable(irq::id::TIMER1);
        linkConnection->activate();
    }

    inline void disable()
    {
        linkConnection->deactivate();
        irq::disable(irq::id::TIMER1);
        irq::disable(irq::id::SERIAL);
    }

    void init(LinkConnection& connection_ref);

    void send(int data_to_send);

    [[nodiscard]] bool receive(LinkConnection::Response& response);

    inline void commit()
    {
        linkConnection->_onVBlank();
    }
}

#endif
