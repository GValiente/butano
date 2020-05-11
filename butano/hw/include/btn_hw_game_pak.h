#ifndef BTN_HW_GAME_PAK_H
#define BTN_HW_GAME_PAK_H

#include "btn_config_game_pak.h"
#include "btn_hw_tonc.h"

#define REG_WAITCNT_NV *(u16*)(REG_BASE+0x0204)

namespace btn::hw::game_pak
{
    inline void init()
    {
        BIT_SET(REG_WAITCNT_NV, BTN_CFG_GAME_PAK_WAIT_STATE_FIRST);
        BIT_SET(REG_WAITCNT_NV, BTN_CFG_GAME_PAK_WAIT_STATE_SECOND);

        if(BTN_CFG_GAME_PAK_PREFETCH_ENABLED)
        {
            BIT_SET(REG_WAITCNT_NV, WS_PREFETCH);
        }
        else
        {
            BIT_CLEAR(REG_WAITCNT_NV, WS_PREFETCH);
        }
    }
}

#endif
