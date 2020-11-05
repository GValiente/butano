/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_PROFILER_H
#define BTN_PROFILER_H

#include "btn_config_profiler.h"

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
        using ticks_map = btn::unordered_map<const char*, int64_t, BTN_CFG_PROFILER_MAX_ENTRIES * 2>;

        void start(const char* id, unsigned id_hash);

        void stop();

        [[nodiscard]] int64_t total_ticks();

        [[nodiscard]] const ticks_map& ticks_per_entry();

        void reset();
    }

    /// @endcond

    /**
     * @def BTN_PROFILER_START
     *
     * Defines the start of a code block in which elapsed time is going to be measured.
     *
     * @param id Small text string which identifies the code block.
     *
     * @ingroup profiler
     */
    #define BTN_PROFILER_START(id) \
        _btn::profiler::start(id, btn::hash<const char*>()(id))

    /**
     * @def BTN_PROFILER_STOP
     *
     * Defines the end of a code block in which elapsed time is going to be measured.
     *
     * @ingroup profiler
     */
    #define BTN_PROFILER_STOP() \
        _btn::profiler::stop()

    /**
     * @def BTN_PROFILER_RESET
     *
     * Forgets all elapsed time measures.
     *
     * @ingroup profiler
     */
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
