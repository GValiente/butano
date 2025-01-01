/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_common.h"

#include "bn_assert.h"

extern "C"
{
    // No need for shared objects:
    void* __dso_handle __attribute__((weak)) = nullptr;

    // Call init routines:
    // https://stackoverflow.com/a/30972459
    extern void (*__preinit_array_start[])(void);
    extern void (*__preinit_array_end[])(void);
    extern void (*__init_array_start[])(void);
    extern void (*__init_array_end[])(void);

    void __libc_init_array(void)
    {
        for(int index = 0, limit = __preinit_array_end - __preinit_array_start; index < limit; ++index)
        {
            __preinit_array_start[index]();
        }

        for(int index = 0, limit = __init_array_end - __init_array_start; index < limit; ++index)
        {
            __init_array_start[index]();
        }
    }

    // Standard assert function:
    #if BN_CFG_ASSERT_ENABLED
        void __assert_func(const char* file, int line, const char* func, const char* failedexpr)
        {
            _bn::assert::show(failedexpr, file, func, line);
        }
    #else
        void __assert_func(const char*, int, const char*, const char*)
        {
            while(true)
            {
            }
        }
    #endif

    // Disable __aeabi_atexit to reduce IWRAM usage:
    // https://arobenko.gitbooks.io/bare_metal_cpp/content/compiler_output/static.html
    int __aeabi_atexit(void*, void (*)(void*), void*)
    {
        return 0;
    }

    // Override __cxa_pure_virtual to reduce IWRAM usage:
    // https://stackoverflow.com/questions/14689639/can-i-disable-exceptions-for-when-a-pure-virtual-function-is-called
    void __cxa_pure_virtual()
    {
        BN_ERROR("Pure virtual function call");

        while(true)
        {
        }
    }

    // Provides a trap for when a 32bit integer division by 0 is detected:
    // https://github.com/JoaoBaptMG/gba-modern/blob/5720c35a2e01cb56dbf17add8b086d580e7096b8/source/math/trapdiv0.cpp
    [[noreturn]] int __aeabi_idiv0()
    {
        BN_ERROR("32bit integer division by zero detected");

        while(true)
        {
        }
    }

    // Provides a trap for when a 64bit integer division by 0 is detected:
    // https://github.com/JoaoBaptMG/gba-modern/blob/5720c35a2e01cb56dbf17add8b086d580e7096b8/source/math/trapdiv0.cpp
    [[noreturn]] long long __aeabi_ldiv0()
    {
        BN_ERROR("64bit integer division by zero detected");

        while(true)
        {
        }
    }
}
