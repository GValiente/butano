#include "../include/btn_hw_log.h"

#if BTN_CFG_LOG_ENABLED
    #include "btn_istring_base.h"

    #if BTN_CFG_LOG_IMPLEMENTATION == BTN_LOG_IMPLEMENTATION_NOCASHGBA
        #include "../include/btn_hw_tonc.h"
    #elif BTN_CFG_LOG_IMPLEMENTATION == BTN_LOG_IMPLEMENTATION_MGBA
        #include "btn_memory.h"
    #else
    #endif

    namespace btn::hw
    {
        #if BTN_CFG_LOG_IMPLEMENTATION == BTN_LOG_IMPLEMENTATION_VBA
            void log(const istring_base& message)
            {
                asm volatile
                (
                    "mov r0, %0;"
                    "swi 0xff;"
                    :
                    : "r" (message.data())
                    : "r0"
                );

                asm volatile
                (
                    "mov r0, %0;"
                    "swi 0xff;"
                    :
                    : "r" ("\n")
                    : "r0"
                );
            }
        #elif BTN_CFG_LOG_IMPLEMENTATION == BTN_LOG_IMPLEMENTATION_NOCASHGBA
            void log(const istring_base& message)
            {
                nocash_puts(message.data());
            }
        #elif BTN_CFG_LOG_IMPLEMENTATION == BTN_LOG_IMPLEMENTATION_MGBA
            void log(const istring_base& message)
            {
                // https://forum.gbadev.org/viewtopic.php?f=14&p=179241&sid=aec6b23d11c25ec75966b3bbc89c91c0

                volatile uint16_t& reg_debug_enable = *reinterpret_cast<uint16_t*>(0x4FFF780);
                reg_debug_enable = 0xC0DE;

                char& reg_debug_string = *reinterpret_cast<char*>(0x4FFF600);
                memory::copy(*message.data(), message.size(), reg_debug_string);

                volatile uint16_t& reg_debug_flags = *reinterpret_cast<uint16_t*>(0x4FFF700);
                reg_debug_flags = 2 | 0x100;
            }
        #else
        #endif
    }
#endif
