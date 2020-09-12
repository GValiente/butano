#include "../include/btn_hw_sram.h"

#include "btn_config_sram.h"
#include "../include/btn_hw_tonc.h"

namespace btn::hw::sram
{

namespace
{
    // https://forum.gbadev.org/viewtopic.php?f=4&t=2825
    alignas(4) __attribute__((used)) const char save_type[] = "SRAM_V113";
}

void init()
{
    BIT_SET(REG_WAITCNT_NV, BTN_CFG_SRAM_WAIT_STATE);
}

}
