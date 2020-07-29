#ifndef BTN_HW_TONC_H
#define BTN_HW_TONC_H

#include "btn_common.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"
#pragma GCC diagnostic ignored "-Wpedantic"

#include "tonc.h"
#define REG_WAITCNT_NV *(u16*)(REG_BASE + 0x0204)

#pragma GCC diagnostic pop

#endif
