#include "../include/btn_hw_sram.h"

namespace btn::hw::sram
{
    namespace
    {
        // https://forum.gbadev.org/viewtopic.php?f=4&t=2825
        alignas(4) __attribute__((used)) const char save_type[] = "SRAM_V113";
    }
}
