/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_profiler.h"

#if BN_CFG_PROFILER_ENABLED
    #include "bn_timer.h"
    #include "bn_optional.h"
    #include "bn_unordered_map.h"

    namespace _bn::profiler
    {
        namespace
        {
            static_assert(BN_CFG_PROFILER_MAX_ENTRIES > 0);
            static_assert(bn::power_of_two(BN_CFG_PROFILER_MAX_ENTRIES));

            class static_data
            {

            public:
                ticks_map ticks_per_entry;
                bn::optional<bn::timer> current_timer;
                const char* current_id = nullptr;
                unsigned current_id_hash;
            };

            BN_DATA_EWRAM static_data data;
        }

        void start(const char* id, unsigned id_hash)
        {
            BN_BASIC_ASSERT(id, "Id is null");
            BN_BASIC_ASSERT(! data.current_id, "There's already an active id: ", data.current_id);

            data.current_id = id;
            data.current_id_hash = id_hash;
            data.current_timer = bn::timer();
        }

        void stop()
        {
            BN_BASIC_ASSERT(data.current_id, "There's no active id");

            int timer_ticks = data.current_timer->elapsed_ticks();
            auto timer_ticks_64 = int64_t(timer_ticks);
            ticks& ticks = data.ticks_per_entry(data.current_id_hash, data.current_id);
            ticks.total += timer_ticks_64;
            ticks.max = bn::max(ticks.max, timer_ticks);
            data.current_id = nullptr;
        }

        const ticks_map& ticks_per_entry()
        {
            BN_BASIC_ASSERT(! data.current_id, "There's an active id: ", data.current_id);

            return data.ticks_per_entry;
        }

        void reset()
        {
            BN_BASIC_ASSERT(! data.current_id, "There's an active id: ", data.current_id);

            data.ticks_per_entry.clear();
        }
    }
#endif
