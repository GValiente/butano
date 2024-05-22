/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CSTDLIB_H
#define BN_CSTDLIB_H

/**
 * @file
 * Standard library cstdlib aliases header file.
 *
 * @ingroup std
 */

#include "bn_common.h"

namespace bn
{
    /**
     * @brief Allocates uninitialized storage in EWRAM.
     * @param bytes Bytes to allocate.
     * @return On success, returns the pointer to the beginning of newly allocated memory.
     * On failure, returns `nullptr`.
     *
     * To avoid a memory leak, the returned pointer must be deallocated with bn::free.
     *
     * @ingroup std
     */
    [[nodiscard]] void* malloc(int bytes);

    /**
     * @brief Allocates storage in EWRAM for an array of num objects of bytes size
     * and initializes all bytes in it to zero.
     * @param num Number of objects.
     * @param bytes Size in bytes of each object.
     * @return On success, returns the pointer to the beginning of newly allocated memory.
     * On failure, returns `nullptr`.
     *
     * To avoid a memory leak, the returned pointer must be deallocated with bn::free.
     *
     * @ingroup std
     */
    [[nodiscard]] void* calloc(int num, int bytes);

    /**
     * @brief Reallocates the given storage in the EWRAM.
     * @param ptr Pointer to the storage to reallocate.
     *
     * If ptr was not previously allocated by bn::malloc, bn::calloc or bn::realloc, the behavior is undefined.
     *
     * @param new_bytes New size in bytes of the reallocated storage.
     * @return On success, returns the pointer to the beginning of newly allocated storage.
     * On failure, returns `nullptr`.
     *
     * On success, the original pointer ptr is invalidated and any access to it is undefined behavior
     * (even if reallocation was in-place).
     *
     * To avoid a memory leak, the returned pointer must be deallocated with bn::free.
     *
     * @ingroup std
     */
    [[nodiscard]] void* realloc(void* ptr, int new_bytes);

    /**
     * @brief Deallocates the storage previously allocated by bn::malloc, bn::calloc or bn::realloc.
     * @param ptr Pointer to the storage to deallocate.
     * It is invalidated and any access to it is undefined behavior.
     *
     * If ptr is `nullptr`, the function does nothing.
     *
     * If ptr was not previously allocated by bn::malloc, bn::calloc or bn::realloc, the behavior is undefined.
     *
     * @ingroup std
     */
    void free(void* ptr);
}

#endif
