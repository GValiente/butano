#ifndef BTN_HW_GAME_PAK_H
#define BTN_HW_GAME_PAK_H

#include "tonc.h"
#include "btn_common.h"
#include "btn_config_game_pak.h"

namespace btn::hw::game_pak
{
    inline void init()
    {
        BIT_SET(REG_WAITCNT, BTN_CFG_GAME_PAK_WAIT_STATE_FIRST);
        BIT_SET(REG_WAITCNT, BTN_CFG_GAME_PAK_WAIT_STATE_SECOND);

        if(BTN_CFG_GAME_PAK_PREFETCH_ENABLED)
        {
            BIT_SET(REG_WAITCNT, WS_PREFETCH);
        }
        else
        {
            BIT_CLEAR(REG_WAITCNT, WS_PREFETCH);
        }
    }
}

#endif
