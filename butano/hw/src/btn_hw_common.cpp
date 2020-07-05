#include "../include/btn_hw_common.h"

#include "btn_assert.h"

extern "C"
{
    // Disable __aeabi_atexit to reduce IWRAM usage:
    // https://arobenko.gitbooks.io/bare_metal_cpp/content/compiler_output/static.html
    int __aeabi_atexit(void*, void (*)(void*), void*)
    {
        return 0;
    }

    // Disable strtol (used by tonc's tte_cmd_default) to reduce IWRAM usage:
    long strtol([[maybe_unused]] const char* str, char**, [[maybe_unused]] int base)
    {
        BTN_ERROR("strtol not supported: ", str, " - ", base);
        return 0;
    }

    // Disable __cxa_pure_virtual to reduce IWRAM usage:
    // https://stackoverflow.com/questions/14689639/can-i-disable-exceptions-for-when-a-pure-virtual-function-is-called
    void __cxa_pure_virtual()
    {
        BTN_ERROR("__cxa_pure_virtual not supported");
    }
}
