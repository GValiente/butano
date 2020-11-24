/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CONFIG_PROFILER_H
#define BN_CONFIG_PROFILER_H

/**
 * @file
 * Profiler configuration header file.
 *
 * @ingroup profiler
 */

#include "bn_common.h"

/**
 * @def BN_CFG_PROFILER_ENABLED
 *
 * Specifies if the profiler is enabled or not.
 *
 * @ingroup profiler
 */
#ifndef BN_CFG_PROFILER_ENABLED
    #define BN_CFG_PROFILER_ENABLED false
#endif

/**
 * @def BN_CFG_PROFILER_LOG_ENGINE
 *
 * Specifies if butano subsystems must be profiled or not.
 *
 * @ingroup profiler
 */
#ifndef BN_CFG_PROFILER_LOG_ENGINE
    #define BN_CFG_PROFILER_LOG_ENGINE false
#endif

/**
 * @def BN_CFG_PROFILER_MAX_ENTRIES
 *
 * Specifies the maximum number of code blocks that can be profiled without too much performance issues.
 *
 * @ingroup profiler
 */
#ifndef BN_CFG_PROFILER_MAX_ENTRIES
    #define BN_CFG_PROFILER_MAX_ENTRIES 64
#endif

#endif
