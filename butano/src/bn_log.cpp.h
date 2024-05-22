/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_log.h"

#if BN_CFG_LOG_ENABLED
    #include "../hw/include/bn_hw_log.h"

    namespace bn
    {
        void log(const istring_base& message)
        {
            hw::log(message);
        }
    }
#endif
