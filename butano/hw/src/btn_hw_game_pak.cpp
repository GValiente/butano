#include "../include/btn_hw_game_pak.h"

#include "tonc.h"
#include "btn_config_game_pak.h"

namespace btn::hw::game_pak
{

void init()
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
