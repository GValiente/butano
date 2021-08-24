/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_MEMORY_H
#define BN_MEMORY_H

/**
 * @file
 * bn::unique_ptr and memory functions header file.
 *
 * @ingroup unique_ptr
 * @ingroup memory
 */

#include <memory>
#include "bn_assert.h"
#include "bn_utility.h"
#include "bn_alignment.h"
#include "bn_functional.h"

namespace bn
{

using std::construct_at;
using std::destroy_at;

/**
 * @brief Default deleter.
 *
 * @tparam Type Type of the object to delete.
 *
 * @ingroup unique_ptr
 */
template<typename Type>
struct default_delete
{
    /**
     * @brief Deletes the object pointed by the given pointer.
     */
    void operator()(Type* ptr) const noexcept
    {
        delete ptr;
    }
};


/**
 * @brief std::shared_ptr like smart pointer that owns and manages another object through a pointer
 * and disposes of that object when the unique_ptr goes out of scope.
 *
 * @tparam Type Type of the managed object.
 * @tparam Deleter Type of the object which disposes the managed object when the unique_ptr goes out of scope.
 *
 * @ingroup unique_ptr
 */
template<typename Type, typename Deleter = default_delete<Type>>
class unique_ptr
{

public:
    using pointer = Type*; //!< Pointer to the managed object type alias.
    using reference = Type&; //!< Reference to the managed object type alias.
    using element_type = Type; //!< Managed object type alias.
    using deleter_type = Deleter; //!< Deleter type alias.

    unique_ptr() = default;

    /**
     * @brief Constructor.
     * @param ptr Pointer to the object to manage.
     */
    explicit unique_ptr(pointer ptr) :
        _ptr(ptr)
    {
    }

    unique_ptr(const unique_ptr& other) = delete;

    /**
     * @brief Move constructor.
     * @param other unique_ptr object to move.
     */
    unique_ptr(unique_ptr&& other) noexcept :
        _ptr(other.release()),
        _deleter(move(other._deleter))
    {
    }

    unique_ptr& operator=(const unique_ptr& other) = delete;

    /**
     * @brief Move assignment operator.
     * @param other unique_ptr object to move.
     * @return Reference to this.
     */
    unique_ptr& operator=(unique_ptr&& other) noexcept
    {
        reset(other.release());
        _deleter = move(other._deleter);
        return *this;
    }

    /**
     * @brief Class destructor, which disposes the managed object.
     */
    ~unique_ptr()
    {
        reset();
    }

    /**
     * @brief Indicates if it contains a managed object or not.
     */
    [[nodiscard]] operator bool() const
    {
        return _ptr != nullptr;
    }

    /**
     * @brief Returns a const pointer to the managed object.
     */
    [[nodiscard]] const Type* get() const
    {
        return _ptr;
    }

    /**
     * @brief Returns a pointer to the managed object.
     */
    [[nodiscard]] pointer get()
    {
        return _ptr;
    }

    /**
     * @brief Returns a const reference to the managed object deleter.
     */
    [[nodiscard]] const Deleter& get_deleter() const
    {
        return _deleter;
    }

    /**
     * @brief Returns a reference to the managed object deleter.
     */
    [[nodiscard]] Deleter& get_deleter()
    {
        return _deleter;
    }

    /**
     * @brief Returns a const reference to the managed object.
     */
    [[nodiscard]] const Type& operator*() const
    {
        BN_ASSERT(_ptr, "Managed pointer is null");

        return *_ptr;
    }

    /**
     * @brief Returns a reference to the managed object.
     */
    [[nodiscard]] reference operator*()
    {
        BN_ASSERT(_ptr, "Managed pointer is null");

        return *_ptr;
    }

    /**
     * @brief Returns a const pointer to the managed object.
     */
    [[nodiscard]] const Type* operator->() const
    {
        BN_ASSERT(_ptr, "Managed pointer is null");

        return _ptr;
    }

    /**
     * @brief Returns a pointer to the managed object.
     */
    [[nodiscard]] pointer operator->()
    {
        BN_ASSERT(_ptr, "Managed pointer is null");

        return _ptr;
    }

    /**
     * @brief Releases the ownership of the managed object.
     * @return Pointer to the released object.
     */
    pointer release()
    {
        pointer result = _ptr;
        _ptr = nullptr;
        return result;
    }

    /**
     * @brief Disposes the managed object.
     */
    void reset()
    {
        _deleter(_ptr);
        _ptr = nullptr;
    }

    /**
     * @brief Disposes the managed object and replaces it with the given one.
     * @param ptr Pointer to the new object to manage.
     */
    void reset(pointer ptr)
    {
        if(ptr != _ptr)
        {
            reset();
            _ptr = ptr;
        }
    }

    /**
     * @brief Exchanges the contents of this unique_ptr with those of the other one.
     * @param other unique_ptr to exchange the contents with.
     */
    void swap(unique_ptr& other)
    {
        bn::swap(_ptr, other._ptr);
        bn::swap(_deleter, other._deleter);
    }

    /**
     * @brief Exchanges the contents of a unique_ptr with those of another one.
     * @param a First unique_ptr to exchange the contents with.
     * @param b Second unique_ptr to exchange the contents with.
     */
    friend void swap(unique_ptr& a, unique_ptr& b)
    {
        a.swap(b);
    }

    /**
     * @brief Equal operator.
     * @param a First unique_ptr to compare.
     * @param b Second unique_ptr to compare.
     * @return `true` if the first managed object is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator==(const unique_ptr& a, const unique_ptr& b)
    {
        return a._ptr == b._ptr;
    }

    /**
     * @brief Not equal operator.
     * @param a First unique_ptr to compare.
     * @param b Second unique_ptr to compare.
     * @return `true` if the first managed object is not equal to the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator!=(const unique_ptr& a, const unique_ptr& b)
    {
        return a._ptr != b._ptr;
    }

    /**
     * @brief Less than operator.
     * @param a First unique_ptr to compare.
     * @param b Second unique_ptr to compare.
     * @return `true` if the first managed object is lexicographically less than the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator<(const unique_ptr& a, const unique_ptr& b)
    {
        return a._ptr < b._ptr;
    }

    /**
     * @brief Greater than operator.
     * @param a First unique_ptr to compare.
     * @param b Second unique_ptr to compare.
     * @return `true` if the first managed object is lexicographically greater than the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator>(const unique_ptr& a, const unique_ptr& b)
    {
        return a._ptr > b._ptr;
    }

    /**
     * @brief Less than or equal operator.
     * @param a First unique_ptr to compare.
     * @param b Second unique_ptr to compare.
     * @return `true` if the first managed object is lexicographically less than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator<=(const unique_ptr& a, const unique_ptr& b)
    {
        return a._ptr <= b._ptr;
    }

    /**
     * @brief Greater than or equal operator.
     * @param a First unique_ptr to compare.
     * @param b Second unique_ptr to compare.
     * @return `true` if the first managed object is lexicographically greater than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator>=(const unique_ptr& a, const unique_ptr& b)
    {
        return a._ptr >= b._ptr;
    }

    /**
     * @brief Equal operator.
     * @param a unique_ptr to compare.
     * @param b Null pointer to compare.
     * @return `true` if the unique_ptr does not have a managed object, otherwise `false`.
     */
    [[nodiscard]] friend bool operator==(const unique_ptr& a, [[maybe_unused]] nullptr_t b)
    {
        return ! a._ptr;
    }

    /**
     * @brief Not equal operator.
     * @param a unique_ptr to compare.
     * @param b Null pointer to compare.
     * @return `true` if the unique_ptr has a managed object, otherwise `false`.
     */
    [[nodiscard]] friend bool operator!=(const unique_ptr& a, [[maybe_unused]] nullptr_t b)
    {
        return a._ptr;
    }

    /**
     * @brief Less than operator.
     * @param a unique_ptr to compare.
     * @param b Null pointer to compare.
     * @return `false`.
     */
    [[nodiscard]] friend bool operator<([[maybe_unused]] const unique_ptr& a, [[maybe_unused]] nullptr_t b)
    {
        return false;
    }

    /**
     * @brief Greater than operator.
     * @param a unique_ptr to compare.
     * @param b Null pointer to compare.
     * @return `true` if the unique_ptr has a managed object, otherwise `false`.
     */
    [[nodiscard]] friend bool operator>(const unique_ptr& a, [[maybe_unused]] nullptr_t b)
    {
        return a._ptr;
    }

    /**
     * @brief Less than or equal operator.
     * @param a unique_ptr to compare.
     * @param b Null pointer to compare.
     * @return `true` if the unique_ptr does not have a managed object, otherwise `false`.
     */
    [[nodiscard]] friend bool operator<=(const unique_ptr& a, [[maybe_unused]] nullptr_t b)
    {
        return ! a._ptr;
    }

    /**
     * @brief Greater than or equal than operator.
     * @param a unique_ptr to compare.
     * @param b Null pointer to compare.
     * @return `true`.
     */
    [[nodiscard]] friend bool operator>=([[maybe_unused]] const unique_ptr& a, [[maybe_unused]] nullptr_t b)
    {
        return true;
    }

private:
    pointer _ptr = nullptr;
    [[no_unique_address]] deleter_type _deleter;
};


/**
 * @brief Constructs an object and wraps it in a unique_ptr.
 *
 * @tparam Type Type of the object to construct.
 * @tparam Args Type of the arguments of the object to construct.
 *
 * @param args Parameters of the object to construct.
 * @return A unique_ptr managing the new object.
 *
 * @ingroup unique_ptr
 */
template<typename Type, class... Args>
[[nodiscard]] unique_ptr<Type> make_unique(Args&&... args)
{
    return unique_ptr<Type>(new Type(forward<Args>(args)...));
}

/**
 * @brief Hash support for unique_ptr.
 *
 * @tparam Type Type of the managed object.
 * @tparam Deleter Type of the object which disposes the managed object when the unique_ptr goes out of scope.
 *
 * @ingroup unique_ptr
 * @ingroup functional
 */
template<typename Type, typename Deleter>
struct hash<unique_ptr<Type, Deleter>>
{
    /**
     * @brief Returns the hash of the given unique_ptr.
     */
    [[nodiscard]] unsigned operator()(const unique_ptr<Type, Deleter>& value) const
    {
        return make_hash(value.get());
    }
};

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
     * To avoid a memory leak, the returned pointer must be deallocated with bn::ewram_free.
     * On failure, returns a null pointer.
     */
    [[nodiscard]] void* ewram_alloc(int bytes);

    /**
     * @brief Deallocates the space previously allocated by bn::ewram_alloc.
     * @param ptr Pointer to the memory to deallocate.
     *
     * If ptr is a null pointer, the function does nothing.
     *
     * If ptr was not previously allocated by bn::ewram_alloc, the behavior is undefined.
     */
    void ewram_free(void* ptr);

    /**
     * @brief Returns the bytes of all allocated items in EWRAM with ewram_alloc.
     */
    [[nodiscard]] int used_alloc_ewram();

    /**
     * @brief Returns the bytes that still can be allocated in EWRAM with ewram_alloc.
     */
    [[nodiscard]] int available_alloc_ewram();

    /**
     * @brief Returns the items allocated in EWRAM with ewram_alloc.
     */
    [[nodiscard]] int used_items_ewram();

    /**
     * @brief Returns the items that still can be allocated in EWRAM with ewram_alloc.
     */
    [[nodiscard]] int available_items_ewram();

    /**
     * @brief Returns the IWRAM used by the stack in bytes.
     */
    [[nodiscard]] int used_stack_iwram();

    /**
     * @brief Returns the bytes of all static objects in IWRAM.
     */
    [[nodiscard]] int used_static_iwram();

    /**
     * @brief Returns the bytes of all static objects in EWRAM.
     */
    [[nodiscard]] int used_static_ewram();

    /**
     * @brief Copies the given amount of elements from the object referenced by source_ref
     * to the object referenced by destination_ref.
     *
     * If the objects overlap, the behavior is undefined.
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

        unsigned bytes = unsigned(elements) * sizeof(Type);

        if(aligned<4>(source_ref) && aligned<4>(destination_ref) && bytes % 4 == 0)
        {
            _bn::memory::unsafe_copy_words(&source_ref, int(bytes / 4), &destination_ref);
        }
        else if(aligned<2>(source_ref) && aligned<2>(destination_ref) && bytes % 2 == 0)
        {
            _bn::memory::unsafe_copy_half_words(&source_ref, int(bytes / 2), &destination_ref);
        }
        else
        {
            _bn::memory::unsafe_copy_bytes(&source_ref, int(bytes), &destination_ref);
        }
    }

    /**
     * @brief Clears (set to zero) the memory of the given amount of elements
     * from the object referenced by destination_ref.
     * @param elements Number of elements to clear (not bytes).
     * @param destination_ref Reference to the memory location to clear.
     */
    template<typename Type>
    void clear(int elements, Type& destination_ref)
    {
        static_assert(is_trivial<Type>(), "Type is not trivial");
        BN_ASSERT(elements >= 0, "Invalid elements: ", elements);

        unsigned bytes = unsigned(elements) * sizeof(Type);

        if(aligned<4>(destination_ref) && bytes % 4 == 0)
        {
            _bn::memory::unsafe_clear_words(int(bytes / 4), &destination_ref);
        }
        else if(aligned<2>(destination_ref) && bytes % 2 == 0)
        {
            _bn::memory::unsafe_clear_half_words(int(bytes / 2), &destination_ref);
        }
        else
        {
            _bn::memory::unsafe_clear_bytes(int(bytes), &destination_ref);
        }
    }

    /**
     * @brief Copies the given value into each of the first bytes of the object pointed to by destination_ptr.
     *
     * If the given size is greater than the size of the object pointed to by destination_ptr or it is not trivial,
     * the behavior is undefined.
     *
     * @param value Value to copy.
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
     * @param value Value to copy.
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
     * @param value Value to copy.
     * @param words Number of words to fill.
     * @param destination_ptr Pointer to the object to fill.
     */
    void set_words(unsigned value, int words, void* destination_ptr);
}

#endif
