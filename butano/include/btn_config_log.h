/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CONFIG_LOG_H
#define BTN_CONFIG_LOG_H

#include "btn_log_backend.h"

#ifndef BTN_CFG_LOG_ENABLED
    #define BTN_CFG_LOG_ENABLED true
#endif

#ifndef BTN_CFG_LOG_BACKEND
    #define BTN_CFG_LOG_BACKEND BTN_LOG_BACKEND_MGBA
#endif

#ifndef BTN_CFG_LOG_MAX_SIZE
    #define BTN_CFG_LOG_MAX_SIZE 0x100
#endif

#endif
