/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_PROFILER_H
#define BN_PROFILER_H

/**
 * @file
 * Profiling header file.
 *
 * @ingroup profiler
 */

#include "bn_config_doxygen.h"
#include "bn_config_profiler.h"

/**
 * @def BN_PROFILER_START
 *
 * Defines the start of a code block in which elapsed time is going to be measured.
 *
 * @param id Small text string which identifies the code block.
 *
 * @ingroup profiler
 */

/**
 * @def BN_PROFILER_STOP
 *
 * Defines the end of a code block in which elapsed time is going to be measured.
 *
 * @ingroup profiler
 */

/**
 * @def BN_PROFILER_RESET
 *
 * Forgets all elapsed time measures.
 *
 * @ingroup profiler
 */

#if BN_CFG_PROFILER_ENABLED || BN_DOXYGEN
    #include "bn_unordered_map_fwd.h"

    /**
     * @brief Profiler related functions.
     *
     * @ingroup profiler
     */
    namespace bn::profiler
    {
        /**
         * @brief Stops the execution and shows the profiling results on the screen.
         */
        [[noreturn]] void show();
    }

    /// @cond DO_NOT_DOCUMENT

    namespace _bn::profiler
    {
        struct ticks
        {
            int64_t total = 0;
            int max = 0;
        };

        using ticks_map = bn::unordered_map<const char*, ticks, BN_CFG_PROFILER_MAX_ENTRIES * 2>;

        void start(const char* id, unsigned id_hash);

        void stop();

        [[nodiscard]] const ticks_map& ticks_per_entry();

        void reset();
    }

    /// @endcond

    #define BN_PROFILER_START(id) \
        _bn::profiler::start(id, bn::hash<const char*>()(id))

    #define BN_PROFILER_STOP() \
        _bn::profiler::stop()

    #define BN_PROFILER_RESET() \
        _bn::profiler::reset()
#else
    #define BN_PROFILER_START(id) \
        do \
        { \
        } while(false)

    #define BN_PROFILER_STOP() \
        do \
        { \
        } while(false)

    #define BN_PROFILER_RESET() \
        do \
        { \
        } while(false)
#endif

#endif
