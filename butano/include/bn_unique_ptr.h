/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_UNIQUE_PTR_H
#define BN_UNIQUE_PTR_H

/**
 * @file
 * bn::unique_ptr implementation header file.
 *
 * @ingroup unique_ptr
 * @ingroup memory
 */

#include "bn_assert.h"
#include "bn_utility.h"
#include "bn_functional.h"
#include "bn_unique_ptr_fwd.h"

namespace bn
{

template<typename Type, typename Deleter>
class unique_ptr
{

public:
    using pointer = Type*; //!< Pointer to the managed object type alias.
    using reference = Type&; //!< Reference to the managed object type alias.
    using element_type = Type; //!< Managed object type alias.
    using deleter_type = Deleter; //!< Deleter type alias.

    constexpr unique_ptr() = default;

    /**
     * @brief Nullptr constructor.
     */
    constexpr unique_ptr(nullptr_t) :
        unique_ptr()
    {
    }

    /**
     * @brief Constructor.
     * @param ptr Pointer to the object to manage.
     */
    constexpr explicit unique_ptr(pointer ptr) :
        _ptr(ptr)
    {
    }

    constexpr unique_ptr(const unique_ptr& other) = delete;

    /**
     * @brief Move constructor.
     * @param other unique_ptr object to move.
     */
    constexpr unique_ptr(unique_ptr&& other) noexcept :
        _ptr(other.release()),
        _deleter(move(other._deleter))
    {
    }

    /**
     * @brief Move constructor.
     * @param other unique_ptr object to move with different type and deleter.
     */
    template<typename OtherType, typename OtherDeleter>
    constexpr unique_ptr(unique_ptr<OtherType, OtherDeleter>&& other) :
        _ptr(other.release()),
        _deleter(forward<OtherDeleter>(other.get_deleter()))
    {
    }

    constexpr unique_ptr& operator=(const unique_ptr& other) = delete;

    /**
     * @brief Move assignment operator.
     * @param other unique_ptr object to move.
     * @return Reference to this.
     */
    constexpr unique_ptr& operator=(unique_ptr&& other) noexcept
    {
        reset(other.release());
        _deleter = move(other._deleter);
        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other unique_ptr object to move with different type and deleter.
     * @return Reference to this.
     */
    template<typename OtherType, typename OtherDeleter>
    constexpr unique_ptr& operator=(unique_ptr<OtherType, OtherDeleter>&& other)
    {
        reset(other.release());
        _deleter = forward<OtherDeleter>(other.get_deleter());
        return *this;
    }

    /**
     * @brief Effectively the same as calling reset().
     */
    constexpr unique_ptr& operator=(nullptr_t) noexcept
    {
        reset();
    }

    /**
     * @brief Class destructor, which disposes the managed object.
     */
    constexpr ~unique_ptr()
    {
        reset();
    }

    /**
     * @brief Indicates if it contains a managed object or not.
     */
    [[nodiscard]] constexpr operator bool() const
    {
        return _ptr != nullptr;
    }

    /**
     * @brief Returns a const pointer to the managed object.
     */
    [[nodiscard]] constexpr const Type* get() const
    {
        return _ptr;
    }

    /**
     * @brief Returns a pointer to the managed object.
     */
    [[nodiscard]] constexpr pointer get()
    {
        return _ptr;
    }

    /**
     * @brief Returns a const reference to the managed object deleter.
     */
    [[nodiscard]] constexpr const Deleter& get_deleter() const
    {
        return _deleter;
    }

    /**
     * @brief Returns a reference to the managed object deleter.
     */
    [[nodiscard]] constexpr Deleter& get_deleter()
    {
        return _deleter;
    }

    /**
     * @brief Returns a const reference to the managed object.
     */
    [[nodiscard]] constexpr const Type& operator*() const
    {
        BN_BASIC_ASSERT(_ptr, "Managed pointer is null");

        return *_ptr;
    }

    /**
     * @brief Returns a reference to the managed object.
     */
    [[nodiscard]] constexpr reference operator*()
    {
        BN_BASIC_ASSERT(_ptr, "Managed pointer is null");

        return *_ptr;
    }

    /**
     * @brief Returns a const pointer to the managed object.
     */
    [[nodiscard]] constexpr const Type* operator->() const
    {
        BN_BASIC_ASSERT(_ptr, "Managed pointer is null");

        return _ptr;
    }

    /**
     * @brief Returns a pointer to the managed object.
     */
    [[nodiscard]] constexpr pointer operator->()
    {
        BN_BASIC_ASSERT(_ptr, "Managed pointer is null");

        return _ptr;
    }

    /**
     * @brief Releases the ownership of the managed object.
     * @return Pointer to the released object.
     */
    constexpr pointer release()
    {
        pointer result = _ptr;
        _ptr = nullptr;
        return result;
    }

    /**
     * @brief Disposes the managed object.
     */
    constexpr void reset()
    {
        _deleter(_ptr);
        _ptr = nullptr;
    }

    /**
     * @brief Disposes the managed object and replaces it with the given one.
     * @param ptr Pointer to the new object to manage.
     */
    constexpr void reset(pointer ptr)
    {
        if(ptr != _ptr)
        {
            reset();
            _ptr = ptr;
        }
    }

    /**
     * @brief Disposes the managed object and replaces it with the given one.
     * @param ptr Pointer to the new object to manage with different type.
     */
    template<typename OtherType>
    constexpr void reset(OtherType* ptr)
    {
        if(ptr != _ptr)
        {
            reset();
            _ptr = ptr;
        }
    }

    /**
     * @brief Disposes the managed object.
     */
    constexpr void reset(nullptr_t)
    {
        reset();
    }

    /**
     * @brief Exchanges the contents of this unique_ptr with those of the other one.
     * @param other unique_ptr to exchange the contents with.
     */
    constexpr void swap(unique_ptr& other)
    {
        bn::swap(_ptr, other._ptr);
        bn::swap(_deleter, other._deleter);
    }

    /**
     * @brief Exchanges the contents of a unique_ptr with those of another one.
     * @param a First unique_ptr to exchange the contents with.
     * @param b Second unique_ptr to exchange the contents with.
     */
    constexpr friend void swap(unique_ptr& a, unique_ptr& b)
    {
        a.swap(b);
    }

    /**
     * @brief Equal operator.
     * @param a First unique_ptr to compare.
     * @param b Second unique_ptr to compare.
     * @return `true` if the first managed object is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator==(const unique_ptr& a, const unique_ptr& b)
    {
        return a._ptr == b._ptr;
    }

    /**
     * @brief Not equal operator.
     * @param a First unique_ptr to compare.
     * @param b Second unique_ptr to compare.
     * @return `true` if the first managed object is not equal to the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator!=(const unique_ptr& a, const unique_ptr& b)
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
    [[nodiscard]] constexpr friend bool operator<(const unique_ptr& a, const unique_ptr& b)
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
    [[nodiscard]] constexpr friend bool operator>(const unique_ptr& a, const unique_ptr& b)
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
    [[nodiscard]] constexpr friend bool operator<=(const unique_ptr& a, const unique_ptr& b)
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
    [[nodiscard]] constexpr friend bool operator>=(const unique_ptr& a, const unique_ptr& b)
    {
        return a._ptr >= b._ptr;
    }

    /**
     * @brief Equal operator.
     * @param a unique_ptr to compare.
     * @param b Null pointer to compare.
     * @return `true` if the unique_ptr does not have a managed object, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator==(const unique_ptr& a, [[maybe_unused]] nullptr_t b)
    {
        return ! a._ptr;
    }

    /**
     * @brief Not equal operator.
     * @param a unique_ptr to compare.
     * @param b Null pointer to compare.
     * @return `true` if the unique_ptr has a managed object, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator!=(const unique_ptr& a, [[maybe_unused]] nullptr_t b)
    {
        return a._ptr;
    }

    /**
     * @brief Less than operator.
     * @param a unique_ptr to compare.
     * @param b Null pointer to compare.
     * @return `false`.
     */
    [[nodiscard]] constexpr friend bool operator<([[maybe_unused]] const unique_ptr& a, [[maybe_unused]] nullptr_t b)
    {
        return false;
    }

    /**
     * @brief Greater than operator.
     * @param a unique_ptr to compare.
     * @param b Null pointer to compare.
     * @return `true` if the unique_ptr has a managed object, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator>(const unique_ptr& a, [[maybe_unused]] nullptr_t b)
    {
        return a._ptr;
    }

    /**
     * @brief Less than or equal operator.
     * @param a unique_ptr to compare.
     * @param b Null pointer to compare.
     * @return `true` if the unique_ptr does not have a managed object, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator<=(const unique_ptr& a, [[maybe_unused]] nullptr_t b)
    {
        return ! a._ptr;
    }

    /**
     * @brief Greater than or equal than operator.
     * @param a unique_ptr to compare.
     * @param b Null pointer to compare.
     * @return `true`.
     */
    [[nodiscard]] constexpr friend bool operator>=([[maybe_unused]] const unique_ptr& a, [[maybe_unused]] nullptr_t b)
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
[[nodiscard]] constexpr unique_ptr<Type> make_unique(Args&&... args)
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
    [[nodiscard]] constexpr unsigned operator()(const unique_ptr<Type, Deleter>& value) const
    {
        return make_hash(value.get());
    }
};

}

#endif
