#include "btn_log.h"

#if BTN_CFG_LOG_ENABLED
    #include "../hw/include/btn_hw_log.h"

    namespace btn
    {

    void log(const istring& message)
    {
        btn::hw::log(message);
    }

    }
#endif
