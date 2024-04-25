/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BEST_FIT_ALLOCATOR_H
#define BN_BEST_FIT_ALLOCATOR_H

/**
 * @file
 * bn::best_fit_allocator header file.
 *
 * @ingroup allocator
 */

#include "bn_assert.h"
#include "bn_utility.h"
#include "bn_config_best_fit_allocator.h"

namespace bn
{

/**
 * @brief Manages a chunk of memory with a best fit allocation strategy.
 *
 * @ingroup allocator
 */
class best_fit_allocator
{

public:
    using size_type = int; //!< Size type alias.

    /**
     * @brief Default constructor.
     */
    best_fit_allocator() = default;

    /**
     * @brief Constructor.
     * @param start Pointer to the first element of the memory to manage.
     * @param bytes Size in bytes of the memory to manage.
     */
    best_fit_allocator(void* start, size_type bytes)
    {
        reset(start, bytes);
    }

    best_fit_allocator(const best_fit_allocator&) = delete;

    best_fit_allocator& operator=(const best_fit_allocator&) = delete;

    /**
     * @brief Destructor.
     *
     * It doesn't destroy its elements, they must be destroyed manually.
     */
    ~best_fit_allocator() noexcept;

    /**
     * @brief Returns the size in bytes of all allocated items.
     */
    [[nodiscard]] size_type used_bytes() const
    {
        return _total_bytes_count - _free_bytes_count;
    }

    /**
     * @brief Returns the number of bytes that still can be allocated.
     */
    [[nodiscard]] size_type available_bytes() const
    {
        return _free_bytes_count;
    }

    /**
     * @brief Indicates if it doesn't contain any item.
     */
    [[nodiscard]] bool empty() const
    {
        return used_bytes() == 0;
    }

    /**
     * @brief Indicates if it can't contain any more items.
     */
    [[nodiscard]] bool full() const
    {
        return available_bytes() <= _sizeof_free_item;
    }

    /**
     * @brief Allocates uninitialized storage.
     * @param bytes Bytes to allocate.
     * @return On success, returns the pointer to the beginning of newly allocated memory.
     * On failure, returns `nullptr`.
     *
     * To avoid a memory leak, the returned pointer must be deallocated with free.
     */
    [[nodiscard]] void* alloc(size_type bytes);

    /**
     * @brief Allocates storage for an array of num objects of bytes size
     * and initializes all bytes in it to zero.
     * @param num Number of objects.
     * @param bytes Size in bytes of each object.
     * @return On success, returns the pointer to the beginning of newly allocated memory.
     * On failure, returns `nullptr`.
     *
     * To avoid a memory leak, the returned pointer must be deallocated with free.
     */
    [[nodiscard]] void* calloc(size_type num, size_type bytes);

    /**
     * @brief Reallocates the given storage.
     * @param ptr Pointer to the storage to reallocate.
     *
     * If ptr was not previously allocated by alloc, calloc or realloc, the behavior is undefined.
     *
     * @param new_bytes New size in bytes of the reallocated storage.
     * @return On success, returns the pointer to the beginning of newly allocated storage.
     * On failure, returns `nullptr`.
     *
     * On success, the original pointer ptr is invalidated and any access to it is undefined behavior
     * (even if reallocation was in-place).
     *
     * To avoid a memory leak, the returned pointer must be deallocated with free.
     */
    [[nodiscard]] void* realloc(void* ptr, size_type new_bytes);

    /**
     * @brief Deallocates the storage previously allocated by alloc, calloc or realloc.
     * @param ptr Pointer to the storage to deallocate.
     * It is invalidated and any access to it is undefined behavior.
     *
     * If ptr is `nullptr`, the function does nothing.
     *
     * If ptr was not previously allocated by alloc, calloc or realloc, the behavior is undefined.
     */
    void free(void* ptr);

    /**
     * @brief Constructs a value inside of the allocator.
     * @param args Parameters of the value to construct.
     * @return Reference to the new value.
     */
    template<typename Type, typename... Args>
    [[nodiscard]] Type& create(Args&&... args)
    {
        auto result = reinterpret_cast<Type*>(alloc(sizeof(Type)));
        BN_BASIC_ASSERT(result, "Allocation failed");

        new(result) Type(forward<Args>(args)...);
        return *result;
    }

    /**
     * @brief Destroys the given value, previously allocated with the create method.
     */
    template<typename Type>
    void destroy(Type& value)
    {
        value.~Type();
        free(&value);
    }

    /**
     * @brief Setups the allocator to manage a new chunk of memory.
     * @param start Pointer to the first element of the memory to manage.
     * @param bytes Size in bytes of the memory to manage.
     */
    void reset(void* start, size_type bytes);

    /**
     * @brief Logs the current status of the allocator.
     */
    void log_status() const;

private:
    class item_type;

    struct free_items_pair
    {
        item_type* previous = nullptr;
        item_type* next = nullptr;
    };

    class item_type
    {

    public:
        item_type* previous = nullptr;
        size_type size: 31 = 0;
        bool used: 1 = false;
        free_items_pair free_items;

        [[nodiscard]] const item_type* next() const
        {
            const uint8_t* next_ptr = reinterpret_cast<const uint8_t*>(this) + size;
            return reinterpret_cast<const item_type*>(next_ptr);
        }

        [[nodiscard]] item_type* next()
        {
            uint8_t* next_ptr = reinterpret_cast<uint8_t*>(this) + size;
            return reinterpret_cast<item_type*>(next_ptr);
        }

        [[nodiscard]] const void* data() const
        {
            return reinterpret_cast<const uint8_t*>(this) + _sizeof_used_item;
        }

        [[nodiscard]] void* data()
        {
            return reinterpret_cast<uint8_t*>(this) + _sizeof_used_item;
        }
    };

    static constexpr size_type _sizeof_free_item = sizeof(item_type);
    static constexpr size_type _sizeof_used_item = sizeof(item_type) - sizeof(free_items_pair);

    uint8_t* _start_ptr = nullptr;
    item_type* _first_free_item = nullptr;
    size_type _total_bytes_count = 0;
    size_type _free_bytes_count = 0;

    [[nodiscard]] const item_type* _begin_item() const
    {
        return reinterpret_cast<const item_type*>(_start_ptr);
    }

    [[nodiscard]] item_type* _begin_item()
    {
        return reinterpret_cast<item_type*>(_start_ptr);
    }

    [[nodiscard]] const item_type* _end_item() const
    {
        return reinterpret_cast<const item_type*>(_start_ptr + _total_bytes_count);
    }

    [[nodiscard]] item_type* _end_item()
    {
        return reinterpret_cast<item_type*>(_start_ptr + _total_bytes_count);
    }

    [[nodiscard]] item_type* _best_free_item(size_type bytes);

    #if BN_CFG_BEST_FIT_ALLOCATOR_SANITY_CHECK_ENABLED
        void _sanity_check() const;
    #endif

    #if BN_CFG_BEST_FIT_ALLOCATOR_FREE_CHECK_ENABLED
        void _free_check(const item_type* item) const;
    #endif
};

}

#endif
