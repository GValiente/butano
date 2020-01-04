#include "btn_profiler.h"

#if BTN_CFG_PROFILER_ENABLED
    #include "btn_timer.h"
    #include "btn_optional.h"
    #include "btn_hash_map.h"

    namespace _btn::profiler
    {
        namespace
        {
            static_assert(BTN_CFG_PROFILER_TICKS_DIVISOR > 0);
            static_assert(btn::power_of_two(BTN_CFG_PROFILER_TICKS_DIVISOR));

            static_assert(BTN_CFG_PROFILER_MAX_ENTRIES > 0);
            static_assert(btn::power_of_two(BTN_CFG_PROFILER_MAX_ENTRIES));

            class static_data
            {

            public:
                ticks_map ticks_per_entry;
                int64_t total_ticks = 0;
                btn::optional<btn::timer> current_timer;
                const char* current_id = nullptr;
                size_t current_id_hash;
            };

            BTN_DATA_EWRAM static_data data;
        }

        void start(const char* id, size_t id_hash)
        {
            BTN_ASSERT(id, "Id is null");
            BTN_ASSERT(! data.current_id, "There's already an active id: ", data.current_id);

            data.current_timer = btn::timer();
            data.current_id = id;
            data.current_id_hash = id_hash;
        }

        void stop()
        {
            BTN_ASSERT(data.current_id, "There's no active id");

            int timer_ticks = data.current_timer->elapsed_ticks();
            auto profiler_ticks = int64_t(timer_ticks / BTN_CFG_PROFILER_TICKS_DIVISOR);
            data.ticks_per_entry(data.current_id_hash, data.current_id) += profiler_ticks;
            data.total_ticks += profiler_ticks;
            data.current_id = nullptr;
        }

        int64_t total_ticks()
        {
            BTN_ASSERT(! data.current_id, "There's an active id: ", data.current_id);

            return data.total_ticks;
        }

        const ticks_map& ticks_per_entry()
        {
            BTN_ASSERT(! data.current_id, "There's an active id: ", data.current_id);

            return data.ticks_per_entry;
        }

        void reset()
        {
            BTN_ASSERT(! data.current_id, "There's an active id: ", data.current_id);

            data.ticks_per_entry.clear();
            data.total_ticks = 0;
        }
    }
#endif
