/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_MEMORY_H
#define BN_MEMORY_H

/**
 * @file
 * Memory functions header file.
 *
 * @ingroup memory
 */

#include "bn_alignment.h"
#include "bn_unique_ptr.h"
#include "bn_memory_alias.h"

namespace bn
{
    enum class compression_type : uint8_t;
}


/// @cond DO_NOT_DOCUMENT

namespace _bn::memory
{
    void unsafe_copy_bytes(const void* source, int bytes, void* destination);

    void unsafe_copy_half_words(const void* source, int half_words, void* destination);

    void unsafe_copy_words(const void* source, int words, void* destination);

    void unsafe_clear_bytes(int bytes, void* destination);

    void unsafe_clear_half_words(int half_words, void* destination);

    void unsafe_clear_words(int words, void* destination);
}

/// @endcond


/**
 * @brief Memory related functions.
 *
 * @ingroup memory
 */
namespace bn::memory
{
    /**
     * @brief Allocates uninitialized storage in EWRAM.
     * @param bytes Bytes to allocate.
     * @return On success, returns the pointer to the beginning of newly allocated memory.
     * On failure, returns `nullptr`.
     *
     * To avoid a memory leak, the returned pointer must be deallocated with bn::memory::ewram_free.
     */
    [[nodiscard]] void* ewram_alloc(int bytes);

    /**
     * @brief Allocates storage in EWRAM for an array of num objects of bytes size
     * and initializes all bytes in it to zero.
     * @param num Number of objects.
     * @param bytes Size in bytes of each object.
     * @return On success, returns the pointer to the beginning of newly allocated memory.
     * On failure, returns `nullptr`.
     *
     * To avoid a memory leak, the returned pointer must be deallocated with bn::memory::ewram_free.
     */
    [[nodiscard]] void* ewram_calloc(int num, int bytes);

    /**
     * @brief Reallocates the given storage in the EWRAM.
     * @param ptr Pointer to the storage to reallocate.
     *
     * If ptr was not previously allocated by bn::memory::ewram_alloc, bn::memory::ewram_calloc or
     * bn::memory::ewram_realloc, the behavior is undefined.
     *
     * @param new_bytes New size in bytes of the reallocated storage.
     * @return On success, returns the pointer to the beginning of newly allocated storage.
     * On failure, returns `nullptr`.
     *
     * On success, the original pointer ptr is invalidated and any access to it is undefined behavior
     * (even if reallocation was in-place).
     *
     * To avoid a memory leak, the returned pointer must be deallocated with bn::memory::ewram_free.
     */
    [[nodiscard]] void* ewram_realloc(void* ptr, int new_bytes);

    /**
     * @brief Deallocates the storage previously allocated by bn::memory::ewram_alloc,
     * bn::memory::ewram_calloc or bn::memory::ewram_realloc.
     * @param ptr Pointer to the storage to deallocate.
     * It is invalidated and any access to it is undefined behavior.
     *
     * If ptr is `nullptr`, the function does nothing.
     *
     * If ptr was not previously allocated by bn::memory::ewram_alloc, bn::memory::ewram_calloc or
     * bn::memory::ewram_realloc, the behavior is undefined.
     */
    void ewram_free(void* ptr);

    /**
     * @brief Returns the size in bytes of all allocated items in EWRAM with bn::memory::ewram_alloc,
     * bn::memory::ewram_calloc and bn::memory::ewram_realloc.
     */
    [[nodiscard]] int used_alloc_ewram();

    /**
     * @brief Returns the number of bytes that still can be allocated in EWRAM with bn::memory::ewram_alloc,
     * bn::memory::ewram_calloc and bn::memory::ewram_realloc.
     */
    [[nodiscard]] int available_alloc_ewram();

    /**
     * @brief Logs the current status of the EWRAM allocator.
     */
    void log_alloc_ewram_status();

    /**
     * @brief Returns the number of bytes of IWRAM used by the stack.
     */
    [[nodiscard]] int used_stack_iwram();

    /**
     * @brief Returns the size in bytes of all static objects in IWRAM.
     */
    [[nodiscard]] int used_static_iwram();

    /**
     * @brief Returns the size in bytes of all static objects in EWRAM.
     */
    [[nodiscard]] int used_static_ewram();

    /**
     * @brief Returns the size of the ROM in bytes.
     */
    [[nodiscard]] int used_rom();

    /**
     * @brief Indicates if EWRAM access time is 1+1 clock cycles for sequential accesses or not.
     *
     * See @ref BN_EWRAM_WAIT_STATE_1.
     */
    [[nodiscard]] bool fast_ewram();

    /**
     * @brief Copies the given amount of elements from the object referenced by source_ref
     * to the object referenced by destination_ref.
     *
     * If the source and destination objects overlap, the behavior is undefined.
     *
     * @param source_ref Const reference to the memory location to copy from.
     * @param elements Number of elements to copy (not bytes).
     * @param destination_ref Reference to the memory location to copy to.
     */
    template<typename Type>
    void copy(const Type& source_ref, int elements, Type& destination_ref)
    {
        static_assert(is_trivially_copyable<Type>(), "Type is not trivially copyable");
        BN_ASSERT(elements >= 0, "Invalid elements: ", elements);

        if constexpr(sizeof(Type) == 4)
        {
            _bn::memory::unsafe_copy_words(&source_ref, elements, &destination_ref);
        }
        else if constexpr(sizeof(Type) == 2)
        {
            _bn::memory::unsafe_copy_half_words(&source_ref, elements, &destination_ref);
        }
        else
        {
            unsigned bytes = unsigned(elements) * sizeof(Type);

            if constexpr(sizeof(Type) % 4 == 0)
            {
                _bn::memory::unsafe_copy_words(&source_ref, int(bytes / 4), &destination_ref);
            }
            else if constexpr(sizeof(Type) % 2 == 0)
            {
                _bn::memory::unsafe_copy_half_words(&source_ref, int(bytes / 2), &destination_ref);
            }
            else if(aligned<4>(&source_ref) && aligned<4>(&destination_ref) && bytes % 4 == 0)
            {
                _bn::memory::unsafe_copy_words(&source_ref, int(bytes / 4), &destination_ref);
            }
            else if(aligned<2>(&source_ref) && aligned<2>(&destination_ref) && bytes % 2 == 0)
            {
                _bn::memory::unsafe_copy_half_words(&source_ref, int(bytes / 2), &destination_ref);
            }
            else
            {
                _bn::memory::unsafe_copy_bytes(&source_ref, int(bytes), &destination_ref);
            }
        }
    }

    /**
     * @brief Clears (fills with zero) the memory of the given amount of elements
     * from the object referenced by destination_ref.
     * @param elements Number of elements to clear (not bytes).
     * @param destination_ref Reference to the memory location to clear.
     */
    template<typename Type>
    void clear(int elements, Type& destination_ref)
    {
        static_assert(is_trivial<Type>(), "Type is not trivial");
        BN_ASSERT(elements >= 0, "Invalid elements: ", elements);

        if constexpr(sizeof(Type) == 4)
        {
            _bn::memory::unsafe_clear_words(elements, &destination_ref);
        }
        else if constexpr(sizeof(Type) == 2)
        {
            _bn::memory::unsafe_clear_half_words(elements, &destination_ref);
        }
        else
        {
            unsigned bytes = unsigned(elements) * sizeof(Type);

            if constexpr(sizeof(Type) % 4 == 0)
            {
                _bn::memory::unsafe_clear_words(int(bytes / 4), &destination_ref);
            }
            else if constexpr(sizeof(Type) % 2 == 0)
            {
                _bn::memory::unsafe_clear_half_words(int(bytes / 2), &destination_ref);
            }
            else if(aligned<4>(&destination_ref) && bytes % 4 == 0)
            {
                _bn::memory::unsafe_clear_words(int(bytes / 4), &destination_ref);
            }
            else if(aligned<2>(&destination_ref) && bytes % 2 == 0)
            {
                _bn::memory::unsafe_clear_half_words(int(bytes / 2), &destination_ref);
            }
            else
            {
                _bn::memory::unsafe_clear_bytes(int(bytes), &destination_ref);
            }
        }
    }

    /**
     * @brief Copies the given value into each of the first bytes of the object pointed to by destination_ptr.
     *
     * If the given size is greater than the size of the object pointed to by destination_ptr or it is not trivial,
     * the behavior is undefined.
     *
     * @param value Value to fill with.
     * @param bytes Number of bytes to fill.
     * @param destination_ptr Pointer to the object to fill.
     */
    void set_bytes(uint8_t value, int bytes, void* destination_ptr);

    /**
     * @brief Copies the given value into each of the first half_words of the object pointed to by destination_ptr.
     *
     * If the given size is greater than the size of the object pointed to by destination_ptr or it is not trivial,
     * the behavior is undefined.
     *
     * @param value Value to fill with.
     * @param half_words Number of half words to fill.
     * @param destination_ptr Pointer to the object to fill.
     */
    void set_half_words(uint16_t value, int half_words, void* destination_ptr);

    /**
     * @brief Copies the given value into each of the first words of the object pointed to by destination_ptr.
     *
     * If the given size is greater than the size of the object pointed to by destination_ptr or it is not trivial,
     * the behavior is undefined.
     *
     * @param value Value to fill with.
     * @param words Number of words to fill.
     * @param destination_ptr Pointer to the object to fill.
     */
    void set_words(unsigned value, int words, void* destination_ptr);

    /**
     * @brief Decompresses the compressed data pointed to by source_ptr
     * in the first bytes of the object pointed to by destination_ptr.
     *
     * If the source and destination data overlap, the behavior is undefined.
     *
     * @param compression Compression type.
     * @param source_ptr Compressed data to decompress.
     * @param bytes Number of decompressed bytes.
     * @param destination_ptr Destination of the decompressed data.
     */
    void decompress(compression_type compression, const void* source_ptr, int bytes, void* destination_ptr);

    /**
     * @brief Indicates if Butano can use DMA for memory copies when it's safe to do so.
     */
    [[nodiscard]] bool dma_enabled();

    /**
     * @brief Sets if Butano can use DMA for memory copies when it's safe to do so.
     */
    void set_dma_enabled(bool dma_enabled);
}

#endif
