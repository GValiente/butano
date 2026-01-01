/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_log.h"

#if BN_CFG_LOG_ENABLED
    #include "bn_log_level.h"
    #include "../hw/include/bn_hw_log.h"

    namespace bn
    {
        void log(const istring_base& message)
        {
            hw::log(log_level::WARN, message);
        }

        void log(log_level level, const istring_base& message)
        {
            hw::log(level, message);
        }
    }
#endif
