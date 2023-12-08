/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_show.h"

#if BN_CFG_ASSERT_ENABLED || BN_CFG_PROFILER_ENABLED
    #include "bn_colors.h"
    #include "bn_string.h"
    #include "bn_display.h"
    #include "bn_system_font.h"
    #include "../include/bn_hw_tonc.h"
    #include "../include/bn_hw_display.h"
#endif

#if BN_CFG_ASSERT_ENABLED
    #include "bn_config_log.h"
#endif

#if BN_CFG_PROFILER_ENABLED
    #include "bn_core.h"
    #include "bn_vector.h"
    #include "bn_keypad.h"
    #include "bn_profiler.h"
    #include "bn_unordered_map.h"
#endif

namespace bn::hw::show
{

namespace
{
    #if BN_CFG_ASSERT_ENABLED || BN_CFG_PROFILER_ENABLED
        constexpr color light_red(31, 7, 7);
        constexpr color light_blue(10, 10, 31);
        constexpr int tte_margin = 12;

        void init_tte(const system_font& system_font)
        {
            hw::display::set_show_mode();
            m3_fill(0);

            // Init TTE in mode 3:
            tte_init_bmp(3, &system_font.tte_font(), nullptr);
            tte_set_margins(tte_margin, tte_margin, display::width() - tte_margin, display::height() - tte_margin);
            tte_write("\n");
        }
    #endif
}

#if BN_CFG_ASSERT_ENABLED
    void error(const system_font& system_font, const string_view& condition, const string_view& file_name,
               const string_view& function, int line, const string_view& message, const string_view& tag)
    {
        string<BN_CFG_ASSERT_BUFFER_SIZE> buffer;
        init_tte(system_font);

        // Show file name:
        tte_set_ink(light_red.data());
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
        tte_set_ink(light_blue.data());

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
            tte_set_ink(colors::yellow.data());
            tte_write(buffer.c_str());
            tte_write("\n\n");
        }

        // Show message:
        buffer.clear();
        buffer.append(message.begin(), message.end());
        tte_set_ink(colors::white.data());
        tte_write(buffer.c_str());

        // Show stacktrace warning:
        #ifdef BN_STACKTRACE
            #if BN_CFG_LOG_ENABLED
                const char* stacktrace_warning = "Stack trace logged";
                POINT16 stacktrace_warning_size = tte_get_text_size(stacktrace_warning);
                tte_set_pos(tte_margin, display::height() - tte_margin - stacktrace_warning_size.y);
                tte_set_ink(colors::purple.data());
                tte_write(stacktrace_warning);
            #endif
        #endif

        // Show tag:
        if(! tag.empty())
        {
            buffer.clear();
            buffer.append(tag.data(), tag.size());

            POINT16 tag_size = tte_get_text_size(buffer.c_str());
            tte_set_pos(display::width() - tte_margin - tag_size.x, display::height() - tte_margin - tag_size.y);
            tte_set_ink(colors::orange.data());
            tte_write(buffer.c_str());
        }
    }
#endif

#if BN_CFG_PROFILER_ENABLED
    void profiler_results(const system_font& system_font)
    {
        const auto& ticks_per_entry = _bn::profiler::ticks_per_entry();
        init_tte(system_font);
        tte_set_ink(colors::green.data());

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
            // Collect entries:
            struct entry
            {
                string_view id;
                int64_t total_ticks;
                int max_ticks;
            };

            vector<entry, BN_CFG_PROFILER_MAX_ENTRIES * 2> entries;
            int64_t total_ticks = 0;
            int64_t max_ticks = 0;
            bool show_total = true;
            bool rebuild = true;

            for(const auto& ticks_per_entry_pair : ticks_per_entry)
            {
                auto& ticks_entry = ticks_per_entry_pair.second;
                entries.push_back({ ticks_per_entry_pair.first, ticks_entry.total, ticks_entry.max });
                total_ticks += ticks_entry.total;
                max_ticks = bn::max(max_ticks, int64_t(ticks_entry.max));
            }

            // Retrieve max width for indexes, labels and ticks:
            string<BN_CFG_ASSERT_BUFFER_SIZE> buffer;
            ostringstream buffer_stream(buffer);
            int num_entries = entries.size();
            int max_index_width = 0;
            int max_id_width = 0;
            int max_ticks_width = 0;

            const int margin = 8;
            const int index_margin = 4;
            const int max_visible_entries = 8;
            int current_index = 0;
            int init_x;
            int init_y;
            tte_get_pos(&init_x, &init_y);

            while(true)
            {
                if(rebuild)
                {
                    max_index_width = 0;
                    max_id_width = 0;
                    max_ticks_width = 0;
                    current_index = 0;

                    // Sort entries by ticks (higher to lower):
                    if(show_total)
                    {
                        sort(entries.begin(), entries.end(), [](const entry& a, const entry& b) {
                            return a.total_ticks > b.total_ticks;
                        });
                    }
                    else
                    {
                        sort(entries.begin(), entries.end(), [](const entry& a, const entry& b) {
                            return a.max_ticks > b.max_ticks;
                        });
                    }

                    // Calculate columns width:
                    for(int index = 0; index < num_entries; ++index)
                    {
                        const entry& entry = entries[index];
                        buffer.clear();
                        buffer_stream << index + 1 << '.';
                        max_index_width = max(max_index_width, int(tte_get_text_size(buffer_stream.str().c_str()).x));

                        buffer.clear();
                        buffer_stream << entry.id;
                        max_id_width = max(max_id_width, int(tte_get_text_size(buffer_stream.str().c_str()).x));

                        buffer.clear();
                        buffer_stream << (show_total ? entry.total_ticks : entry.max_ticks);
                        max_ticks_width = max(max_ticks_width, int(tte_get_text_size(buffer_stream.str().c_str()).x));
                    }

                    rebuild = false;
                }

                // Print title:
                int64_t global_var;
                tte_set_pos(init_x, init_y);
                tte_set_ink(colors::green.data());

                if(show_total)
                {
                    tte_write("PROFILER results - TOTAL ticks");
                    global_var = total_ticks;
                }
                else
                {
                    tte_write("PROFILER results - MAX ticks");
                    global_var = max_ticks;
                }

                if(num_entries > max_visible_entries)
                {
                    tte_write("\n(UP and DOWN to scroll, A to switch mode)");
                }
                else
                {
                    tte_write("\n(A to switch mode)");
                }

                tte_write("\n\n");

                // Print entries:
                for(int index = current_index, limit = min(current_index + max_visible_entries, num_entries);
                    index < limit; ++index)
                {
                    int x;
                    int y;
                    tte_get_pos(&x, &y);

                    const entry& entry = entries[index];
                    buffer.clear();
                    buffer_stream << index + 1 << '.';
                    tte_set_ink(light_blue.data());
                    tte_write(buffer.c_str());

                    tte_set_pos(x + max_index_width + index_margin, y);
                    tte_get_pos(&x, &y);

                    buffer.clear();
                    buffer_stream << entry.id;
                    tte_set_ink(colors::white.data());
                    tte_write(buffer.c_str());

                    tte_set_pos(x + max_id_width + margin, y);
                    tte_get_pos(&x, &y);

                    int64_t entry_var = show_total ? entry.total_ticks : entry.max_ticks;
                    buffer.clear();
                    buffer_stream << entry_var;
                    tte_set_ink(colors::yellow.data());
                    tte_write(buffer.c_str());

                    if(global_var)
                    {
                        auto pct = int((entry_var * 100) / global_var);
                        buffer.clear();
                        buffer_stream << pct << '%';
                        tte_set_pos(x + max_ticks_width + margin, y);
                        tte_set_ink(colors::orange.data());
                        tte_write(buffer.c_str());
                    }

                    tte_write("\n");
                }

                // Scroll through entries:
                while(true)
                {
                    core::update();

                    if(keypad::a_pressed())
                    {
                        show_total = ! show_total;
                        rebuild = true;
                        tte_erase_screen();
                        break;
                    }
                    else if(current_index && keypad::up_pressed())
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
