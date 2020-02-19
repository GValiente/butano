#ifndef BTN_HW_LOG_H
#define BTN_HW_LOG_H

#include "btn_common.h"
#include "btn_config_log.h"

#if BTN_CFG_LOG_ENABLED
    namespace btn
    {
        class string_view;
    }

    namespace btn::hw
    {
        void log(const string_view& message);
    }
#endif

#endif
