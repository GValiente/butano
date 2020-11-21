/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_LINK_H
#define BN_LINK_H

#include "bn_optional_fwd.h"

namespace bn
{
    class link_state;
}

namespace bn::link
{
    void send(int data);

    [[nodiscard]] optional<link_state> receive();

    void deactivate();
}

#endif
