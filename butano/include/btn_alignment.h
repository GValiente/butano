#ifndef BTN_ALIGNMENT_H
#define BTN_ALIGNMENT_H

#include "btn_common.h"

namespace btn
{
    template<int Bytes>
    [[nodiscard]] inline bool aligned(const void* pointer)
    {
        static_assert(Bytes > 0);

        return uintptr_t(pointer) % unsigned(Bytes) == 0;
    }
}

#endif
