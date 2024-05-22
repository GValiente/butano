/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_link.h"

#include "bn_link_state.h"
#include "bn_link_manager.h"

namespace bn::link
{

void send(int data_to_send)
{
    BN_ASSERT(data_to_send >= 0 && data_to_send <= 65533, "Invalid data to send: ", data_to_send);

    link_manager::send(data_to_send);
}

optional<link_state> receive()
{
    return link_manager::receive();
}

void deactivate()
{
    link_manager::deactivate();
}

}
