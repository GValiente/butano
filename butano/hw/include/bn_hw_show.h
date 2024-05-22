/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_SHOW_H
#define BN_HW_SHOW_H

#include "bn_config_assert.h"
#include "bn_config_profiler.h"

#if BN_CFG_ASSERT_ENABLED || BN_CFG_PROFILER_ENABLED
    #include "bn_string_view.h"

    namespace bn
    {
        class system_font;
    }
#endif

namespace bn::hw::show
{
    #if BN_CFG_ASSERT_ENABLED
        void error(const system_font& system_font, const string_view& condition, const string_view& file_name,
                   const string_view& function, int line, const string_view& message, const string_view& tag);
    #endif

    #if BN_CFG_PROFILER_ENABLED
        [[noreturn]] void profiler_results(const system_font& system_font);
    #endif
}

#endif
