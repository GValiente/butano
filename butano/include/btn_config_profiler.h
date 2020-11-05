/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CONFIG_PROFILER_H
#define BTN_CONFIG_PROFILER_H

#include "btn_common.h"

/**
 * @def BTN_CFG_PROFILER_ENABLED
 *
 * Specifies if the profiler is enabled or not.
 *
 * @ingroup profiler
 */
#ifndef BTN_CFG_PROFILER_ENABLED
    #define BTN_CFG_PROFILER_ENABLED false
#endif

/**
 * @def BTN_CFG_PROFILER_LOG_ENGINE
 *
 * Specifies if butano subsystems must be profiled or not.
 *
 * @ingroup profiler
 */
#ifndef BTN_CFG_PROFILER_LOG_ENGINE
    #define BTN_CFG_PROFILER_LOG_ENGINE false
#endif

/**
 * @def BTN_CFG_PROFILER_MAX_ENTRIES
 *
 * Specifies the maximum number of code blocks that can be profiled without too much performance issues.
 *
 * @ingroup profiler
 */
#ifndef BTN_CFG_PROFILER_MAX_ENTRIES
    #define BTN_CFG_PROFILER_MAX_ENTRIES 32
#endif

#endif
