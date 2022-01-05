/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_common.h"

#include "bn_assert.h"

extern "C"
{
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
