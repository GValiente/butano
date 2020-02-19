#include "../include/btn_hw_log.h"

#if BTN_CFG_LOG_ENABLED
    #include "btn_string_view.h"

    #if BTN_CFG_LOG_IMPLEMENTATION == BTN_LOG_IMPLEMENTATION_NOCASHGBA
        #include "tonc.h"
    #elif BTN_CFG_LOG_IMPLEMENTATION == BTN_LOG_IMPLEMENTATION_MGBA
        #include "btn_memory.h"
    #else
    #endif

    namespace btn::hw
    {
        #if BTN_CFG_LOG_IMPLEMENTATION == BTN_LOG_IMPLEMENTATION_VBA
            void log(const string_view& message)
            {
                string<0x100> message_string;

                if(! message.empty())
                {
                    message_string.append(message.data(), message.size());
                }

                asm volatile
                (
                    "mov r0, %0;"
                    "swi 0xff;"
                    :
                    : "r" (message_string.c_str())
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
            void log(const string_view& message)
            {
                string<0x100> message_string;

                if(! message.empty())
                {
                    message_string.append(message.data(), message.size());
                }

                nocash_puts(message_string.c_str());
            }
        #elif BTN_CFG_LOG_IMPLEMENTATION == BTN_LOG_IMPLEMENTATION_MGBA
            void log(const string_view& message)
            {
                // https://forum.gbadev.org/viewtopic.php?f=14&p=179241&sid=aec6b23d11c25ec75966b3bbc89c91c0

                if(int size = message.size())
                {
                    volatile uint16_t& reg_debug_enable = *reinterpret_cast<uint16_t*>(0x4FFF780);
                    reg_debug_enable = 0xC0DE;

                    char& reg_debug_string = *reinterpret_cast<char*>(0x4FFF600);
                    memory::copy(message.front(), size, reg_debug_string);

                    volatile uint16_t& reg_debug_flags = *reinterpret_cast<uint16_t*>(0x4FFF700);
                    reg_debug_flags = 2 | 0x100;
                }
            }
        #else
        #endif
    }
#endif
