/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
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
#include "bn_span_fwd.h"
#include "../hw/include/bn_hw_sram_constants.h"

/// @cond DO_NOT_DOCUMENT

namespace _bn::sram
{
    void unsafe_read(void* destination, int size, int offset);

    void unsafe_write(const void* source, int size, int offset);
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
     * @brief Copies SRAM data into the given value.
     * @param destination SRAM data is copied into this trivially copyable value.
     */
    template<typename Type>
    void read(Type& destination)
    {
        static_assert(is_trivially_copyable<Type>(), "Destination is not trivially copyable");
        static_assert(int(sizeof(Type)) <= size(), "Destination size is too high");

        _bn::sram::unsafe_read(&destination, int(sizeof(Type)), 0);
    }

    /**
     * @brief Copies SRAM data into the given value.
     * @param destination SRAM data is copied into this trivially copyable value.
     * @param offset Copying starts from SRAM start address + this offset.
     */
    template<typename Type>
    void read_offset(Type& destination, int offset)
    {
        static_assert(is_trivially_copyable<Type>(), "Destination is not trivially copyable");
        static_assert(int(sizeof(Type)) <= size(), "Destination size is too high");
        BN_ASSERT(offset >= 0, "Invalid offset: ", offset);
        BN_ASSERT(int(sizeof(Type)) + offset <= size(),
                  "Destination size and offset are too high: ", sizeof(Type), " - ", offset);

        _bn::sram::unsafe_read(&destination, int(sizeof(Type)), offset);
    }

    /**
     * @brief Copies SRAM data into the given span.
     * @param destination SRAM data is copied into this span.
     */
    void read_span(span<uint8_t>& destination);

    /**
     * @brief Copies SRAM data into the given span.
     * @param destination SRAM data is copied into this span.
     * @param offset Copying starts from SRAM start address + this offset.
     */
    void read_span_offset(span<uint8_t>& destination, int offset);

    /**
     * @brief Copies the given value into SRAM.
     * @param source Value to copy. It must be trivially copyable.
     */
    template<typename Type>
    void write(const Type& source)
    {
        static_assert(is_trivially_copyable<Type>(), "Source is not trivially copyable");
        static_assert(int(sizeof(Type)) <= size(), "Source size is too high");

        _bn::sram::unsafe_write(&source, int(sizeof(Type)), 0);
    }

    /**
     * @brief Copies the given value into SRAM.
     * @param source Value to copy. It must be trivially copyable.
     * @param offset The given value is copied into SRAM start address + this offset.
     */
    template<typename Type>
    void write_offset(const Type& source, int offset)
    {
        static_assert(is_trivially_copyable<Type>(), "Source is not trivially copyable");
        static_assert(int(sizeof(Type)) <= size(), "Source size is too high");
        BN_ASSERT(offset >= 0, "Invalid offset: ", offset);
        BN_ASSERT(int(sizeof(Type)) + offset <= size(),
                  "Source size and offset are too high: ", sizeof(Type), " - ", offset);

        _bn::sram::unsafe_write(&source, int(sizeof(Type)), offset);
    }

    /**
     * @brief Copies the given span into SRAM.
     * @param source Span to copy.
     */
    void write_span(const span<const uint8_t>& source);

    /**
     * @brief Copies the given span into SRAM.
     * @param source Span to copy.
     * @param offset The given span is copied into SRAM start address + this offset.
     */
    void write_span_offset(const span<const uint8_t>& source, int offset);

    /**
     * @brief Clears (fills with zero) SRAM.
     * @param bytes Number of bytes to clear.
     */
    void clear(int bytes);

    /**
     * @brief Clears (fills with zero) SRAM.
     * @param bytes Number of bytes to clear.
     * @param offset Clear starts from SRAM start address + this offset.
     */
    void clear(int bytes, int offset);

    /**
     * @brief Fills SRAM with the given value.
     * @param value Value to fill with.
     * @param bytes Number of bytes to fill.
     */
    void set_bytes(uint8_t value, int bytes);

    /**
     * @brief Fills SRAM with the given value.
     * @param value Value to fill with.
     * @param bytes Number of bytes to fill.
     * @param offset Fill starts from SRAM start address + this offset.
     */
    void set_bytes(uint8_t value, int bytes, int offset);
}

#endif
