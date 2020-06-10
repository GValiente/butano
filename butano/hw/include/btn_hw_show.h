#ifndef BTN_HW_SHOW_H
#define BTN_HW_SHOW_H

#include "btn_config_assert.h"
#include "btn_config_profiler.h"

#if BTN_CFG_ASSERT_ENABLED
    #include "btn_string_fwd.h"

    namespace btn
    {
        class string_view;
    }
#endif

namespace btn::hw::show
{
    #if BTN_CFG_ASSERT_ENABLED
        void error(const string_view& condition, const string_view& file_name, const string_view& function, int line,
                   const string_view& message);
    #endif

    #if BTN_CFG_PROFILER_ENABLED
        [[noreturn]] void profiler_results();
    #endif
}

#endif
