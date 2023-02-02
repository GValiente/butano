/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_TONC_H
#define BN_HW_TONC_H

#include "bn_common.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"
#pragma GCC diagnostic ignored "-Wpedantic"

#include "tonc.h"
#define REG_WAITCNT_NV *(u16*)(REG_BASE + 0x0204)

#pragma GCC diagnostic pop

#endif
