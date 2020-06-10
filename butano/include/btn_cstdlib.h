#ifndef BTN_CSTDLIB_H
#define BTN_CSTDLIB_H

#include "btn_common.h"

namespace btn
{
    [[nodiscard]] void* malloc(int bytes);

    void free(void* ptr);
}

#endif
