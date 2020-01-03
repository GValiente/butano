#ifndef BTN_HW_LOG_H
#define BTN_HW_LOG_H

#include "btn_common.h"
#include "btn_config_log.h"

#if BTN_CFG_LOG_ENABLED
    #include "btn_string_fwd.h"

    namespace btn::hw
    {
        void log(const istring& message);
    }
#endif

#endif
