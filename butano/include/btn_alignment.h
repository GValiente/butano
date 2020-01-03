#ifndef BTN_ALIGNMENT_H
#define BTN_ALIGNMENT_H

#include "btn_common.h"

namespace btn
{

[[nodiscard]] inline bool aligned(const void* pointer, size_t byte_count)
{
    return uintptr_t(pointer) % byte_count == 0;
}

}

#endif
