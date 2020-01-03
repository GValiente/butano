#include "../include/btn_hw_log.h"

#include "btn_config_log.h"

#if BTN_CFG_LOG_ENABLED
    #include "btn_string.h"

    #if BTN_CFG_LOG_IMPLEMENTATION == BTN_LOG_IMPLEMENTATION_NOCASHGBA
        #include "tonc.h"
    #endif

    namespace btn::hw
    {
        #if BTN_CFG_LOG_IMPLEMENTATION == BTN_LOG_IMPLEMENTATION_VBA
            void log(const istring& message)
            {
                asm volatile
                (
                    "mov r0, %0;"
                    "swi 0xff;"
                    :
                    : "r" (message.c_str())
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
            void log(const istring& message)
            {
                nocash_puts(message.c_str());
            }
        #elif BTN_CFG_LOG_IMPLEMENTATION == BTN_LOG_IMPLEMENTATION_MGBA
            void log(const istring& message)
            {
                // https://forum.gbadev.org/viewtopic.php?f=14&p=179241&sid=aec6b23d11c25ec75966b3bbc89c91c0

                #define REG_DEBUG_ENABLE    *(volatile uint16_t*) 0x4FFF780
                #define REG_DEBUG_FLAGS     *(volatile uint16_t*) 0x4FFF700
                #define REG_DEBUG_STRING    *(char*) 0x4FFF600

                REG_DEBUG_ENABLE = 0xC0DE;
                memcpy(&REG_DEBUG_STRING, message.data(), message.size());
                REG_DEBUG_FLAGS = 2 | 0x100;
            }
        #else
        #endif
    }
#endif
