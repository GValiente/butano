#include "../include/btn_hw_core.h"

#include "tonc.h"

namespace btn::hw::core
{

void wait_for_vblank()
{
    VBlankIntrWait();
}

void sleep()
{
    Stop();
}

void reset()
{
    RegisterRamReset(RESET_GFX | RESET_REG_SOUND);
    SoftReset();

    while(true)
    {
    }
}

}
