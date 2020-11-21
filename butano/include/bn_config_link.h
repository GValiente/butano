/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CONFIG_LINK_H
#define BN_CONFIG_LINK_H

#include "bn_link_baud_rate.h"

#ifndef BN_CFG_LINK_BAUD_RATE
    #define BN_CFG_LINK_BAUD_RATE BN_LINK_BAUD_RATE_9600_BPS
#endif

#ifndef BN_CFG_LINK_SEND_WAIT
    #define BN_CFG_LINK_SEND_WAIT 100
#endif

#ifndef BN_CFG_LINK_RECV_WAIT
    #define BN_CFG_LINK_RECV_WAIT 0
#endif

#endif
