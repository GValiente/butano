/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_STACKTRACE_H
#define BN_HW_STACKTRACE_H

#include <unwind.h>
#include "bn_log.h"
#include "bn_config_assert.h"

#if BN_CFG_ASSERT_STACKTRACE_DEMANGLE
    #include <cxxabi.h>
#endif

namespace bn::hw::stacktrace
{

namespace
{
    // http://consoledev.de/project/stack-trace-for-nds/

    constexpr unsigned opcode_function_name_tag_id = 0x00ff0000;
    constexpr int maximum_function_name_visits = 0x1000;
    constexpr int minimum_log_depth = 5;

    struct opcode_function_name_tag
    {
        unsigned length: 8;
        unsigned id: 24;
    };

    const opcode_function_name_tag* function_name_tag(unsigned address)
    {
        auto tag = reinterpret_cast<opcode_function_name_tag*>(address & unsigned(~3));

        for(int index = 0; index < maximum_function_name_visits; ++index)
        {
            if(tag->id == opcode_function_name_tag_id)
            {
                return tag;
            }

            --tag;
        }

        return nullptr;
    }

    _Unwind_Reason_Code trace_fcn(_Unwind_Context* ctx, void* depth_ptr)
    {
        int& depth = *static_cast<int*>(depth_ptr);
        ++depth;

        if(depth >= minimum_log_depth)
        {
            if(unsigned address = _Unwind_GetIP(ctx))
            {
                if(const opcode_function_name_tag* func_name_tag = function_name_tag(address))
                {
                    auto name = reinterpret_cast<const char*>(unsigned(func_name_tag) - func_name_tag->length);

                    #if BN_CFG_ASSERT_STACKTRACE_DEMANGLE
                        int status;
                        char* demangled_name = abi::__cxa_demangle(name, nullptr, nullptr, &status);

                        if(status == 0 && demangled_name)
                        {
                            name = demangled_name;
                        }

                        BN_LOG(reinterpret_cast<void*>(address), " - ", name);
                        free(demangled_name);
                    #else
                        BN_LOG(reinterpret_cast<void*>(address), " - ", name);
                    #endif
                }
                else
                {
                    BN_LOG(reinterpret_cast<void*>(address));
                }
            }
            else
            {
                BN_LOG("instruction address retrieve failed");
            }
        }

        return _URC_NO_REASON;
    }
}

inline void log()
{
    BN_LOG("- stack trace -");

    int depth = 0;
    _Unwind_Backtrace(&trace_fcn, &depth);
}

}

#endif
