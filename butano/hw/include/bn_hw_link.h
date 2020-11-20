/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
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
    using connection = LinkConnection;
    using state = LinkState;

    void _serial_intr();

    void _timer_intr();

    inline void init(connection& connection_ref)
    {
        connection_ref.init();
        linkConnection = &connection_ref;
        irq::replace_or_push_back(irq::id::SERIAL, _serial_intr);
        irq::replace_or_push_back(irq::id::TIMER1, _timer_intr);
        connection_ref.activate();
    }

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
}

#endif
