/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/btn_hw_link.h"

LinkConnection* linkConnection = nullptr;

namespace btn::hw::link
{

void _intr()
{
    ISR_serial();
}

}
