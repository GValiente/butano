/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_PROFILER_H
#define BTN_PROFILER_H

/**
 * @file
 * Profiling header file.
 *
 * @ingroup profiler
 */

#include "btn_config_profiler.h"

/**
 * @def BTN_PROFILER_START
 *
 * Defines the start of a code block in which elapsed time is going to be measured.
 *
 * @param id Small text string which identifies the code block.
 *
 * @ingroup profiler
 */

/**
 * @def BTN_PROFILER_STOP
 *
 * Defines the end of a code block in which elapsed time is going to be measured.
 *
 * @ingroup profiler
 */

/**
 * @def BTN_PROFILER_RESET
 *
 * Forgets all elapsed time measures.
 *
 * @ingroup profiler
 */

#if BTN_CFG_PROFILER_ENABLED
    #include "btn_unordered_map_fwd.h"

    /**
     * @brief Profiler related functions.
     *
     * @ingroup profiler
     */
    namespace btn::profiler
    {
        /**
         * @brief Stops the execution and shows the profiling results on the screen.
         */
        [[noreturn]] void show();
    }

    /// @cond DO_NOT_DOCUMENT

    namespace _btn::profiler
    {
        struct ticks
        {
            int64_t total = 0;
            int max = 0;
        };

        using ticks_map = btn::unordered_map<const char*, ticks, BTN_CFG_PROFILER_MAX_ENTRIES * 2>;

        void start(const char* id, unsigned id_hash);

        void stop();

        [[nodiscard]] const ticks_map& ticks_per_entry();

        void reset();
    }

    /// @endcond

    #define BTN_PROFILER_START(id) \
        _btn::profiler::start(id, btn::hash<const char*>()(id))

    #define BTN_PROFILER_STOP() \
        _btn::profiler::stop()

    #define BTN_PROFILER_RESET() \
        _btn::profiler::reset()
#else
    #define BTN_PROFILER_START(id) \
        do \
        { \
        } while(false)

    #define BTN_PROFILER_STOP() \
        do \
        { \
        } while(false)

    #define BTN_PROFILER_RESET() \
        do \
        { \
        } while(false)
#endif

#endif
