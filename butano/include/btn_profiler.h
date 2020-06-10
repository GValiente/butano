#ifndef BTN_PROFILER_H
#define BTN_PROFILER_H

#include "btn_config_profiler.h"

#if BTN_CFG_PROFILER_ENABLED
    #include "btn_hash_map_fwd.h"

    namespace btn::profiler
    {
        [[noreturn]] void show();
    }

    namespace _btn::profiler
    {
        using ticks_map = btn::hash_map<const char*, int64_t, BTN_CFG_PROFILER_MAX_ENTRIES>;

        void start(const char* id, unsigned id_hash);

        void stop();

        [[nodiscard]] int64_t total_ticks();

        [[nodiscard]] const ticks_map& ticks_per_entry();

        void reset();
    }

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
