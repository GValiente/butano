/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_LINK_MANAGER_H
#define BN_LINK_MANAGER_H

#include "bn_optional.h"

namespace bn
{
    class link_state;
}

namespace bn::link_manager
{
    void init();

    [[nodiscard]] bool active();

    void send(int data_to_send);

    [[nodiscard]] optional<link_state> receive();

    void deactivate();

    void enable();

    void disable();
}

#endif
