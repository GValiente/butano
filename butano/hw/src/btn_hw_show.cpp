#include "../include/btn_hw_show.h"

#if BTN_CFG_ASSERT_ENABLED || BTN_CFG_PROFILER_ENABLED
    #include "btn_string.h"
    #include "btn_display.h"
    #include "btn_string_view.h"
    #include "../include/btn_hw_tonc.h"
    #include "../include/btn_hw_display.h"
#endif

#if BTN_CFG_PROFILER_ENABLED
    #include "btn_core.h"
    #include "btn_vector.h"
    #include "btn_keypad.h"
    #include "btn_profiler.h"
    #include "btn_hash_map.h"
#endif

namespace btn::hw::show
{

namespace
{
    #if BTN_CFG_ASSERT_ENABLED || BTN_CFG_PROFILER_ENABLED
        void init_tte()
        {
            btn::hw::display::set_show_mode();
            m3_fill(0);

            // Init TTE in mode 3:
            auto margin = 12;
            tte_init_bmp(3, nullptr, nullptr);
            tte_set_margins(margin, margin, btn::display::width() - margin, btn::display::height() - margin);
            tte_write("\n");
        }
    #endif
}

#if BTN_CFG_ASSERT_ENABLED
    void error(const string_view& condition, const string_view& file_name, const string_view& function, int line,
               const string_view& message)
    {
        string<BTN_CFG_ASSERT_BUFFER_SIZE> buffer;
        init_tte();

        // Show file name:
        tte_write("ERROR in ");

        if(file_name.empty())
        {
            tte_write("unknown file name");
        }
        else
        {
            buffer.append(file_name.data(), file_name.size());
            tte_write(buffer.c_str());
        }

        tte_write("\n");

        // Show function and line:
        if(function.empty())
        {
            tte_write("Line: ");
        }
        else
        {
            buffer.clear();
            buffer.append(function.data(), function.size());
            tte_write(buffer.c_str());
            tte_write("::");
        }

        ostringstream buffer_stream(buffer);
        buffer.clear();
        buffer_stream << line;
        tte_write(buffer.c_str());
        tte_write("\n\n");

        // Show condition:
        if(! condition.empty())
        {
            buffer.clear();
            buffer.append(condition.data(), condition.size());
            tte_write(buffer.c_str());
            tte_write("\n\n");
        }

        // Show message:
        buffer.clear();
        buffer.append(message.begin(), message.end());
        tte_write(buffer.c_str());
    }
#endif

#if BTN_CFG_PROFILER_ENABLED
    void profiler_results()
    {
        const auto& ticks_per_entry = _btn::profiler::ticks_per_entry();
        init_tte();

        if(ticks_per_entry.empty())
        {
            tte_write("PROFILER results\n\nNo entries found");

            while(true)
            {
                core::update();
            }
        }
        else
        {
            // Sort entries by ticks (higher to lower):
            struct entry
            {
                string_view id;
                int64_t ticks;
            };

            vector<entry, BTN_CFG_PROFILER_MAX_ENTRIES> entries;

            for(const auto& ticks_per_entry_pair : ticks_per_entry)
            {
                entries.push_back({ ticks_per_entry_pair.first, ticks_per_entry_pair.second });
            }

            sort(entries.begin(), entries.end(), [](const entry& a, const entry& b) {
                return a.ticks > b.ticks;
            });

            // Retrieve max width for indexes,  labels and ticks:
            string<BTN_CFG_ASSERT_BUFFER_SIZE> buffer;
            ostringstream buffer_stream(buffer);
            int num_entries = entries.size();
            int max_index_width = 0;
            int max_id_width = 0;
            int max_ticks_width = 0;

            for(int index = 0; index < num_entries; ++index)
            {
                const entry& entry = entries[index];
                buffer.clear();
                buffer_stream << index + 1 << '.';
                max_index_width = max(max_index_width, int(tte_get_text_size(buffer_stream.string().c_str()).x));

                buffer.clear();
                buffer_stream << entry.id;
                max_id_width = max(max_id_width, int(tte_get_text_size(buffer_stream.string().c_str()).x));

                buffer.clear();
                buffer_stream << entry.ticks;
                max_ticks_width = max(max_ticks_width, int(tte_get_text_size(buffer_stream.string().c_str()).x));
            }

            // Print entries:
            constexpr const int margin = 8;
            constexpr const int index_margin = 4;
            constexpr const int max_visible_entries = 9;
            int64_t total_ticks = _btn::profiler::total_ticks();
            int current_index = 0;
            int init_x, init_y;
            tte_get_pos(&init_x, &init_y);

            while(true)
            {
                tte_set_pos(init_x, init_y);
                tte_write("PROFILER results");

                if(num_entries > max_visible_entries)
                {
                    tte_write(" (UP and DOWN to scroll)");
                }

                tte_write("\n\n");

                for(int index = current_index, limit = min(current_index + max_visible_entries, num_entries);
                    index < limit; ++index)
                {
                    int x, y;
                    tte_get_pos(&x, &y);

                    const entry& entry = entries[index];
                    buffer.clear();
                    buffer_stream << index + 1 << '.';
                    tte_write(buffer.c_str());

                    tte_set_pos(x + max_index_width + index_margin, y);
                    tte_get_pos(&x, &y);

                    buffer.clear();
                    buffer_stream << entry.id;
                    tte_write(buffer.c_str());

                    tte_set_pos(x + max_id_width + margin, y);
                    tte_get_pos(&x, &y);

                    buffer.clear();
                    buffer_stream << entry.ticks;
                    tte_write(buffer.c_str());

                    if(total_ticks)
                    {
                        auto pct = int((entry.ticks * 100) / total_ticks);
                        buffer.clear();
                        buffer_stream << pct << '%';
                        tte_set_pos(x + max_ticks_width + margin, y);
                        tte_write(buffer.c_str());
                    }

                    tte_write("\n");
                }

                // Scroll through entries:
                while(true)
                {
                    core::update();

                    if(current_index && keypad::up_pressed())
                    {
                        --current_index;
                        tte_erase_screen();
                        break;
                    }
                    else if(num_entries > max_visible_entries && current_index + max_visible_entries < num_entries &&
                            keypad::down_pressed())
                    {
                        ++current_index;
                        tte_erase_screen();
                        break;
                    }
                }
            }
        }
    }
#endif

}
