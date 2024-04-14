/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
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
 * Specifies if Butano subsystems must be profiled or not.
 *
 * @ingroup profiler
 */
#ifndef BN_CFG_PROFILER_LOG_ENGINE
    #define BN_CFG_PROFILER_LOG_ENGINE false
#endif

/**
 * @def BN_CFG_PROFILER_LOG_ENGINE_DETAILED
 *
 * Specifies if each Butano subsystem must be profiled separately or not.
 *
 * @ref BN_CFG_PROFILER_LOG_ENGINE must be `true` to enable Butano subsystems profiling.
 *
 * @ingroup profiler
 */
#ifndef BN_CFG_PROFILER_LOG_ENGINE_DETAILED
    #define BN_CFG_PROFILER_LOG_ENGINE_DETAILED false
#endif

/**
 * @def BN_CFG_PROFILER_MAX_ENTRIES
 *
 * Specifies the maximum number of code blocks that can be profiled without too many performance issues.
 *
 * @ingroup profiler
 */
#ifndef BN_CFG_PROFILER_MAX_ENTRIES
    #define BN_CFG_PROFILER_MAX_ENTRIES 64
#endif

#endif
