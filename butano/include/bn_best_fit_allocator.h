/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BEST_FIT_ALLOCATOR_H
#define BN_BEST_FIT_ALLOCATOR_H

/**
 * @file
 * bn::ibest_fit_allocator and bn::best_fit_allocator implementation header file.
 *
 * @ingroup allocator
 */

#include "bn_list.h"
#include "bn_vector.h"
#include "bn_best_fit_allocator_fwd.h"

namespace bn
{

/**
 * @brief Base class of best_fit_allocator.
 *
 * Can be used as a reference type for all best fit allocators.
 *
 * @ingroup allocator
 */
class ibest_fit_allocator
{

public:
    using size_type = int; //!< Size type alias.

    ibest_fit_allocator(const ibest_fit_allocator&) = delete;

    ibest_fit_allocator& operator=(const ibest_fit_allocator&) = delete;

    /**
     * @brief Destructor.
     *
     * It doesn't destroy its elements, they must be destroyed manually.
     */
    ~ibest_fit_allocator() noexcept;

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
     * @brief Returns the number of allocated items.
     */
    [[nodiscard]] size_type used_items() const
    {
        return _items.size();
    }

    /**
     * @brief Returns the number of items that still can be allocated.
     */
    [[nodiscard]] size_type available_items() const
    {
        return _items.available();
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
        return available_bytes() == 0 || available_items() == 0;
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
        BN_ASSERT(result, "Allocation failed");

        ::new(result) Type(forward<Args>(args)...);
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

protected:
    /// @cond DO_NOT_DOCUMENT

    class item_type
    {

    public:
        char* data = nullptr;
        size_type size: 24 = 0;
        bool used = false;
    };

    using items_iterator = ilist<item_type>::iterator;

    ibest_fit_allocator(ilist<item_type>& items, ivector<items_iterator>& free_items);

    void _init(void* start, size_type bytes);

    /// @endcond

private:
    static_assert(sizeof(items_iterator) == sizeof(int));
    static_assert(alignof(items_iterator) == alignof(int));

    constexpr static auto _lower_bound_comparator = [](const items_iterator& items_it, size_type size)
    {
        return items_it->size < size;
    };

    constexpr static auto _upper_bound_comparator = [](size_type size, const items_iterator& items_it)
    {
        return size < items_it->size;
    };

    ilist<item_type>& _items;
    ivector<items_iterator>& _free_items;
    size_type _total_bytes_count;
    size_type _free_bytes_count;

    void _insert_free_item(items_iterator items_it, ivector<items_iterator>::iterator free_items_last);

    void _erase_free_item(items_iterator items_it);
};


template<int MaxItems>
class best_fit_allocator : public ibest_fit_allocator
{
    static_assert(MaxItems > 0);

public:
    /**
     * @brief Default constructor.
     * @param start Pointer to the first element of the memory to manage.
     * @param bytes Size in bytes of the memory to manage.
     */
    best_fit_allocator(void* start, size_type bytes) :
        ibest_fit_allocator(_items_buffer, _free_items_buffer)
    {
        _init(start, bytes);
    }

private:
    list<item_type, MaxItems> _items_buffer;
    vector<items_iterator, (MaxItems / 2) + 2> _free_items_buffer;
};

}

#endif
