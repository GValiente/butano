#include "btn_log.h"

#if BTN_CFG_LOG_ENABLED
    #include "btn_string.h"
    #include "../hw/include/btn_hw_log.h"

    namespace btn
    {
        void log(const char* message)
        {
            string<0x100> message_string;

            if(message)
            {
                message_string.append(message);
            }

            hw::log(message_string);
        }
    }
#endif
