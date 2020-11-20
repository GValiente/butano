/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_link.h"

LinkConnection* linkConnection = nullptr;

namespace bn::hw::link
{

void _serial_intr()
{
    linkConnection->_onSerial();
}

void _timer_intr()
{
    linkConnection->_onTimer();
}

}
