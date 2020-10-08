#include "../include/btn_hw_log.h"

#if BTN_CFG_LOG_ENABLED
    #include "btn_istring_base.h"

    #if BTN_CFG_LOG_BACKEND == BTN_LOG_BACKEND_NOCASHGBA
        #include "../include/btn_hw_tonc.h"
    #elif BTN_CFG_LOG_BACKEND == BTN_LOG_BACKEND_MGBA
        #include "btn_memory.h"
        #include "btn_algorithm.h"
    #else
    #endif

    namespace btn::hw
    {
        #if BTN_CFG_LOG_BACKEND == BTN_LOG_BACKEND_VBA
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
        #elif BTN_CFG_LOG_BACKEND == BTN_LOG_BACKEND_NOCASHGBA
            void log(const istring_base& message)
            {
                nocash_puts(message.data());
            }
        #elif BTN_CFG_LOG_BACKEND == BTN_LOG_BACKEND_MGBA
            void log(const istring_base& message)
            {
                // https://github.com/mgba-emu/mgba/blob/master/opt/libgba/mgba.c

                volatile uint16_t& reg_debug_enable = *reinterpret_cast<uint16_t*>(0x4FFF780);
                reg_debug_enable = 0xC0DE;

                int max_characters_per_line = 256;
                char& reg_debug_string = *reinterpret_cast<char*>(0x4FFF600);
                volatile uint16_t& reg_debug_flags = *reinterpret_cast<uint16_t*>(0x4FFF700);
                const char* message_data = message.data();
                int characters_left = message.size();

                while(characters_left > 0)
                {
                    int characters_to_write = btn::min(characters_left, max_characters_per_line);
                    memory::copy(*message_data, characters_to_write, reg_debug_string);
                    reg_debug_flags = 2 | 0x100;
                    message_data += characters_to_write;
                    characters_left -= characters_to_write;
                }
            }
        #else
            static_assert(false, "Unknown log backend");
        #endif
    }
#endif
