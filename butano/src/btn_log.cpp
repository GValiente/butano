#include "btn_log.h"

#if BTN_CFG_LOG_ENABLED
    #include "../hw/include/btn_hw_log.h"

    namespace btn
    {
        static_assert(BTN_CFG_LOG_MAX_SIZE >= 16);

        void log(const istring_base& message)
        {
            hw::log(message);
        }
    }
#endif
