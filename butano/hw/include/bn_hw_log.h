/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_LOG_H
#define BN_HW_LOG_H

#include "bn_config_log.h"

#if BN_CFG_LOG_ENABLED
    namespace bn
    {
        class istring_base;
    }

    namespace bn::hw
    {
        void log(const istring_base& message);
    }
#endif

#endif
