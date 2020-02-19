#include "btn_log.h"

#if BTN_CFG_LOG_ENABLED
    #include "btn_string_view.h"
    #include "../hw/include/btn_hw_log.h"

    namespace btn
    {
        void log(const char* message)
        {
            hw::log(message);
        }
    }
#endif
