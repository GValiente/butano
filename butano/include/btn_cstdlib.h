/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CSTDLIB_H
#define BTN_CSTDLIB_H

#include "btn_common.h"

namespace btn
{
    /**
     * @brief Allocates size bytes of uninitialized storage in the EWRAM.
     * @param bytes Number of bytes to allocate.
     * @return The pointer to the beginning of the allocated memory if it could be allocated; `nullptr` otherwise.
     *
     * @ingroup std
     */
    [[nodiscard]] void* malloc(int bytes);

    /**
     * @brief Deallocates the space previously allocated in the EWRAM by btn::malloc.
     * @param ptr Pointer to the memory to deallocate.
     *
     * If ptr is a null pointer, the function does nothing.
     *
     * @ingroup std
     */
    void free(void* ptr);
}

#endif
