/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_SHOW_H
#define BN_HW_SHOW_H

#include "bn_config_assert.h"
#include "bn_config_profiler.h"

#if BN_CFG_ASSERT_ENABLED
    #include "bn_string_fwd.h"

    namespace bn
    {
        class string_view;
    }
#endif

namespace bn::hw::show
{
    #if BN_CFG_ASSERT_ENABLED
        void error(const string_view& condition, const string_view& file_name, const string_view& function, int line,
                   const string_view& message);
    #endif

    #if BN_CFG_PROFILER_ENABLED
        [[noreturn]] void profiler_results();
    #endif
}

#endif
