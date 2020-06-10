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

    // Disable strtol (used by tte_cmd_default) to reduce IWRAM usage:
    long strtol(const char* str, char**, int base)
    {
        BTN_ERROR("strtol not supported: ", str, " - ", base);
        return 0;
    }
}
