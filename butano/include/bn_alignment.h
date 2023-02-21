/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_ALIGNMENT_H
#define BN_ALIGNMENT_H

/**
 * @file
 * Alignment functions header file.
 *
 * @ingroup memory
 */

#include "bn_common.h"

namespace bn
{
    /**
     * @brief Indicates if the given `void` pointer is aligned to the given Bytes.
     *
     * @ingroup memory
     */
    template<int Bytes>
    [[nodiscard]] bool aligned(const void* pointer)
    {
        static_assert(Bytes > 0);

        return uintptr_t(pointer) % unsigned(Bytes) == 0;
    }

    /**
     * @brief Indicates if the given Type pointer is aligned to the given Bytes.
     *
     * @ingroup memory
     */
    template<int Bytes, typename Type>
    [[nodiscard]] bool aligned(const Type* pointer)
    {
        static_assert(Bytes > 0);

        if constexpr(alignof(Type) % unsigned(Bytes) == 0)
        {
            return true;
        }

        return uintptr_t(pointer) % unsigned(Bytes) == 0;
    }
}

#endif
