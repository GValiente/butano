#include "btn_assert.h"

extern "C"
{
    // Disable strtol (used by tonc's tte_cmd_default) to reduce IWRAM usage:
    long strtol([[maybe_unused]] const char* str, char**, [[maybe_unused]] int base)
    {
        BTN_ERROR("strtol not supported: ", str, " - ", base);
        return 0;
    }
}
