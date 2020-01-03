#include "../include/btn_hw_timer.h"

#include "tonc.h"

namespace btn::hw::timer
{

void init()
{
    REG_TM3CNT = 0;
    REG_TM2CNT = 0;

    REG_TM3D = 0;
    REG_TM2D = 0;

    REG_TM3CNT = TM_ENABLE | TM_CASCADE;
    REG_TM2CNT = TM_ENABLE;
}

unsigned ticks()
{
    return (unsigned(REG_TM3D) << 16) | REG_TM2D;
}

}
