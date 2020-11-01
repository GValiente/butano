/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_HW_LINK_H
#define BTN_HW_LINK_H

#include "btn_hw_irq.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"
#pragma GCC diagnostic ignored "-Wpedantic"

#include "../3rd_party/gba-link-connection/include/LinkConnection.h"

#pragma GCC diagnostic pop

namespace btn::hw::link
{
    using connection = LinkConnection;
    using state = LinkState;

    BTN_CODE_IWRAM void _intr();

    inline void init(int baud_rate, connection& connection_ref)
    {
        connection::init(connection::BaudRate(baud_rate));
        linkConnection = &connection_ref;
        irq::replace_or_push_back(irq::id::SERIAL, _intr);
    }

    [[nodiscard]] inline state tick(int data_to_send)
    {
        return linkConnection->tick(uint16_t(data_to_send));
    }

    inline void enable()
    {
        irq::enable(irq::id::SERIAL);
    }

    inline void disable()
    {
        irq::disable(irq::id::SERIAL);
    }
}

#endif
