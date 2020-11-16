/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SRAM_H
#define BN_SRAM_H

/**
 * @file
 * bn::sram header file.
 *
 * @ingroup sram
 */

#include "bn_assert.h"
#include "../hw/include/bn_hw_sram_constants.h"

/// @cond DO_NOT_DOCUMENT

namespace _bn::sram
{
    void unsafe_write(const void* source, int size, int offset);

    void unsafe_read(void* destination, int size, int offset);
}

/// @endcond


/**
 * @brief SRAM related functions.
 *
 * @ingroup sram
 */
namespace bn::sram
{
    /**
     * @brief Returns the total SRAM size in bytes.
     */
    [[nodiscard]] constexpr int size()
    {
        return hw::sram::size();
    }

    /**
     * @brief Copies the given value into SRAM.
     * @param source Value to copy.
     */
    template<typename Type>
    void write(const Type& source)
    {
        static_assert(is_trivially_copyable<Type>(), "Type is not trivially copyable");
        static_assert(int(sizeof(Type)) <= size(), "Size is too high");

        _bn::sram::unsafe_write(&source, int(sizeof(Type)), 0);
    }

    /**
     * @brief Copies the given value into SRAM.
     * @param source Value to copy.
     * @param offset The given value is copied into SRAM start address + this offset.
     */
    template<typename Type>
    void write_offset(const Type& source, int offset)
    {
        static_assert(is_trivially_copyable<Type>(), "Type is not trivially copyable");
        static_assert(int(sizeof(Type)) <= size(), "Size is too high");
        BN_ASSERT(offset >= 0, "Invalid offset: ", offset);
        BN_ASSERT(int(sizeof(Type)) + offset <= size(),
                   "Size and offset are too high: ", sizeof(Type), " - ", offset);

        _bn::sram::unsafe_write(&source, int(sizeof(Type)), offset);
    }

    /**
     * @brief Copies SRAM data into the given value.
     * @param destination SRAM data is copied into this value.
     */
    template<typename Type>
    void read(Type& destination)
    {
        static_assert(is_trivially_copyable<Type>(), "Type is not trivially copyable");
        static_assert(int(sizeof(Type)) <= size(), "Size is too high");

        _bn::sram::unsafe_read(&destination, int(sizeof(Type)), 0);
    }

    /**
     * @brief Copies SRAM data into the given value.
     * @param destination SRAM data is copied into this value.
     * @param offset Copying starts from SRAM start address + this offset.
     */
    template<typename Type>
    void read_offset(Type& destination, int offset)
    {
        static_assert(is_trivially_copyable<Type>(), "Type is not trivially copyable");
        static_assert(int(sizeof(Type)) <= size(), "Size is too high");
        BN_ASSERT(offset >= 0, "Invalid offset: ", offset);
        BN_ASSERT(int(sizeof(Type)) + offset <= size(),
                   "Size and offset are too high: ", sizeof(Type), " - ", offset);

        _bn::sram::unsafe_read(&destination, int(sizeof(Type)), offset);
    }
}

#endif
