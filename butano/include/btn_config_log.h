#ifndef BTN_CONFIG_LOG_H
#define BTN_CONFIG_LOG_H

#include "btn_log_implementation.h"

#ifndef BTN_CFG_LOG_ENABLED
    #define BTN_CFG_LOG_ENABLED false
#endif

#ifndef BTN_CFG_LOG_IMPLEMENTATION
    #define BTN_CFG_LOG_IMPLEMENTATION BTN_LOG_IMPLEMENTATION_MGBA
#endif

#ifndef BTN_CFG_LOG_MAX_SIZE
    #define BTN_CFG_LOG_MAX_SIZE 0x100
#endif

#endif
