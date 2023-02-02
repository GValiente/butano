/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CSTRING_H
#define BN_CSTRING_H

/**
 * @file
 * Standard library cstring aliases header file.
 *
 * @ingroup std
 */

#include "bn_common.h"

namespace bn
{
    /**
     * @brief Copies the given bytes from the object pointed to by source to the object pointed to by destination.
     * @param destination Pointer to the memory location to copy to.
     * @param source Pointer to the memory location to copy from.
     * @param bytes Number of bytes to copy.
     *
     * @ingroup std
     */
    void memcpy(void* destination, const void* source, int bytes);

    /**
     * @brief Copies the given value into each of the first given bytes of the object pointed to by destination.
     * @param destination Pointer to the object to fill.
     * @param value Fill byte.
     * @param bytes Number of bytes to fill.
     *
     * @ingroup std
     */
    void memset(void* destination, uint8_t value, int bytes);

    /**
     * @brief Copies the value `0` into each of the first given bytes of the object pointed to by destination.
     * @param destination Pointer to the object to clear.
     * @param bytes Number of bytes to clear.
     *
     * @ingroup std
     */
    void memclear(void* destination, int bytes);
}

#endif
