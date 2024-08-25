/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_OPTIONAL_H
#define BN_OPTIONAL_H

/**
 * @file
 * bn::optional implementation header file.
 *
 * @ingroup optional
 */

#include "bn_assert.h"
#include "bn_utility.h"
#include "bn_functional.h"
#include "bn_memory_alias.h"

namespace bn
{

/**
 * @brief Empty class type used to indicate an optional type with uninitialized state.
 *
 * @ingroup optional
 */
class nullopt_t
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr explicit nullopt_t(int)
    {
    }
};

constexpr nullopt_t nullopt{0}; //!< Constant of type nullopt_t used to indicate an optional type with uninitialized state.

template<typename Type>
class optional
{

public:
    using value_type = Type; //!< Value type alias.

    /**
     * @brief Default constructor.
     */
    constexpr optional() :
        _empty(0)
    {
    }

    /**
     * @brief Null constructor.
     */
    constexpr optional(nullopt_t) :
        optional()
    {
    }

    /**
     * @brief Copy constructor.
     * @param other optional to copy.
     */
    constexpr optional(const optional& other) = default;

    /**
     * @brief Copy constructor.
     * @param other optional to copy.
     */
    constexpr optional(const optional& other)
    requires(is_copy_constructible_v<Type> && ! is_trivially_copy_constructible_v<Type>) :
        _empty(0),
        _valid(other._valid)
    {
        if(_valid)
        {
            construct_at(&_value, other._value);
        }
    }

    /**
     * @brief Move constructor.
     * @param other optional to move.
     */
    constexpr optional(optional&& other) noexcept = default;

    /**
     * @brief Move constructor.
     * @param other optional to move.
     */
    constexpr optional(optional&& other) noexcept
    requires(is_move_constructible_v<Type> && ! is_trivially_move_constructible_v<Type>) :
        _empty(0),
        _valid(other._valid)
    {
        if(_valid)
        {
            construct_at(&_value, move(other._value));
        }
    }

    /**
     * @brief Copy constructor.
     * @param other Different type optional to copy.
     */
    template<typename OtherType>
    constexpr optional(const optional<OtherType>& other) :
        _empty(0),
        _valid(other)
    {
        if(_valid)
        {
            construct_at(&_value, *other);
        }
    }

    /**
     * @brief Move constructor.
     * @param other Different type optional to move.
     */
    template<typename OtherType>
    constexpr optional(optional<OtherType>&& other) noexcept :
        _empty(0),
        _valid(other)
    {
        if(_valid)
        {
            construct_at(&_value, move(*other));
        }
    }

    /**
     * @brief Constructor.
     * @param value Value to copy.
     */
    constexpr optional(const Type& value) :
        _value(value),
        _valid(true)
    {
    }

    /**
     * @brief Constructor.
     * @param value Value to move.
     */
    constexpr optional(Type&& value) :
        _value(move(value)),
        _valid(true)
    {
    }

    /**
     * @brief Destructor.
     */
    constexpr ~optional() noexcept
    {
    }

    /**
     * @brief Destructor.
     */
    constexpr ~optional() noexcept
    requires(! is_trivially_destructible_v<Type>)
    {
        if(_valid)
        {
            destroy_at(&_value);
        }
    }

    /**
     * @brief Null assignment operator.
     * @return Reference to this.
     */
    constexpr optional& operator=(nullopt_t)
    {
        reset();
        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param other optional to copy.
     * @return Reference to this.
     */
    constexpr optional& operator=(const optional& other) = default;

    /**
     * @brief Copy assignment operator.
     * @param other optional to copy.
     * @return Reference to this.
     */
    constexpr optional& operator=(const optional& other)
    requires(is_copy_assignable_v<Type> && ! is_trivially_copy_assignable_v<Type>)
    {
        if(other)
        {
            if(_valid)
            {
                _value = other._value;
            }
            else
            {
                construct_at(&_value, other._value);
                _valid = true;
            }
        }
        else
        {
            reset();
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other optional to move.
     * @return Reference to this.
     */
    constexpr optional& operator=(optional&& other) noexcept = default;

    /**
     * @brief Move assignment operator.
     * @param other optional to move.
     * @return Reference to this.
     */
    constexpr optional& operator=(optional&& other) noexcept
    requires(is_move_constructible_v<Type> && ! is_trivially_move_constructible_v<Type>)
    {
        if(other)
        {
            if(_valid)
            {
                _value = move(other._value);
            }
            else
            {
                construct_at(&_value, move(other._value));
                _valid = true;
            }
        }
        else
        {
            reset();
        }

        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param other Different type optional to copy.
     * @return Reference to this.
     */
    template<typename OtherType>
    constexpr optional& operator=(const optional<OtherType>& other)
    {
        if(other)
        {
            if(_valid)
            {
                _value = *other;
            }
            else
            {
                construct_at(&_value, *other);
                _valid = true;
            }
        }
        else
        {
            reset();
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other Different type optional to move.
     * @return Reference to this.
     */
    template<typename OtherType>
    constexpr optional& operator=(optional<OtherType>&& other) noexcept
    {
        if(other)
        {
            if(_valid)
            {
                _value = move(*other);
            }
            else
            {
                construct_at(&_value, move(*other));
                _valid = true;
            }
        }
        else
        {
            reset();
        }

        return *this;
    }

    /**
     * @brief Assignment operator.
     * @param value Value to copy.
     * @return Reference to this.
     */
    constexpr optional& operator=(const Type& value)
    {
        if(_valid)
        {
            _value = value;
        }
        else
        {
            construct_at(&_value, value);
            _valid = true;
        }

        return *this;
    }

    /**
     * @brief Assignment operator.
     * @param value Value to move.
     * @return Reference to this.
     */
    constexpr optional& operator=(Type&& value)
    {
        if(_valid)
        {
            _value = move(value);
        }
        else
        {
            construct_at(&_value, move(value));
            _valid = true;
        }

        return *this;
    }

    /**
     * @brief Indicates if it contains a value or not.
     */
    [[nodiscard]] constexpr bool has_value() const
    {
        return _valid;
    }

    /**
     * @brief Indicates if it contains a value or not.
     */
    [[nodiscard]] constexpr explicit operator bool() const
    {
        return _valid;
    }

    /**
     * @brief Returns a const pointer to the contained value.
     */
    [[nodiscard]] constexpr const Type* get() const
    {
        return _valid ? &_value : nullptr;
    }

    /**
     * @brief Returns a pointer to the contained value.
     */
    [[nodiscard]] constexpr Type* get()
    {
        return _valid ? &_value : nullptr;
    }

    /**
     * @brief Returns a const reference to the contained value.
     */
    [[nodiscard]] constexpr const Type& value() const
    {
        BN_BASIC_ASSERT(_valid, "Optional is not valid");

        return _value;
    }

    /**
     * @brief Returns a reference to the contained value.
     */
    [[nodiscard]] constexpr Type& value()
    {
        BN_BASIC_ASSERT(_valid, "Optional is not valid");

        return _value;
    }

    /**
     * @brief Returns a copy of the contained value if this optional has one;
     * otherwise it returns a copy of the given default value.
     */
    [[nodiscard]] constexpr Type value_or(const Type& default_value) const
    {
        return _valid ? _value : default_value;
    }

    /**
     * @brief Returns a copy of the contained value if this optional has one;
     * otherwise it returns a moved copy of the given default value.
     */
    [[nodiscard]] constexpr Type value_or(Type&& default_value) const
    {
        return _valid ? _value : move(default_value);
    }

    /**
     * @brief Returns a const pointer to the contained value.
     */
    [[nodiscard]] constexpr const Type* operator->() const
    {
        BN_BASIC_ASSERT(_valid, "Optional is not valid");

        return &_value;
    }

    /**
     * @brief Returns a pointer to the contained value.
     */
    [[nodiscard]] constexpr Type* operator->()
    {
        BN_BASIC_ASSERT(_valid, "Optional is not valid");

        return &_value;
    }

    /**
     * @brief Returns a const reference to the contained value.
     */
    [[nodiscard]] constexpr const Type& operator*() const
    {
        BN_BASIC_ASSERT(_valid, "Optional is not valid");

        return _value;
    }

    /**
     * @brief Returns a reference to the contained value.
     */
    [[nodiscard]] constexpr Type& operator*()
    {
        BN_BASIC_ASSERT(_valid, "Optional is not valid");

        return _value;
    }

    /**
     * @brief Constructs the contained value in-place.
     * @param args Parameters of the value to construct.
     * @return Reference to the constructed value.
     */
    template<typename... Args>
    constexpr Type& emplace(Args&&... args)
    {
        if(_valid)
        {
            destroy_at(&_value);
        }
        else
        {
            _valid = true;
        }

        construct_at(&_value, forward<Args>(args)...);
        return _value;
    }

    /**
     * @brief Disposes the contained value.
     */
    constexpr void reset()
    {
        if(_valid)
        {
            destroy_at(&_value);
            _valid = false;
        }
    }

    /**
     * @brief Exchanges the contents of this optional with those of the other one.
     * @param other optional to exchange the contents with.
     */
    constexpr void swap(optional& other)
    {
        if(_valid)
        {
            if(other._valid)
            {
                bn::swap(_value, other._value);
            }
            else
            {
                construct_at(&other._value, move(_value));
                destroy_at(&_value);
                other._valid = true;
                _valid = false;
            }
        }
        else
        {
            if(other._valid)
            {
                construct_at(&_value, move(other._value));
                destroy_at(&other._value);
                _valid = true;
                other._valid = false;
            }
        }
    }

    /**
     * @brief Exchanges the contents of a optional with those of another one.
     * @param a First optional to exchange the contents with.
     * @param b Second optional to exchange the contents with.
     */
    constexpr friend void swap(optional& a, optional& b)
    {
        a.swap(b);
    }

    /**
     * @brief Equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const optional& a, const optional& b)
    {
        if(a._valid != b._valid)
        {
            return false;
        }

        if(! a._valid && ! b._valid)
        {
            return true;
        }

        return a._value == b._value;
    }

    /**
     * @brief Less than operator.
     */
    [[nodiscard]] constexpr friend bool operator<(const optional& a, const optional& b)
    {
        if(! b._valid)
        {
            return false;
        }

        if(! a._valid)
        {
            return true;
        }

        return a._value < b._value;
    }

    /**
     * @brief Greater than operator.
     */
    [[nodiscard]] constexpr friend bool operator>(const optional& a, const optional& b)
    {
        return b < a;
    }

    /**
     * @brief Less than or equal operator.
     */
    [[nodiscard]] constexpr friend bool operator<=(const optional& a, const optional& b)
    {
        return ! (a > b);
    }

    /**
     * @brief Greater than or equal operator.
     */
    [[nodiscard]] constexpr friend bool operator>=(const optional& a, const optional& b)
    {
        return ! (a < b);
    }

    /**
     * @brief Equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const optional& a, nullopt_t)
    {
        return ! a._valid;
    }

    /**
     * @brief Not equal operator.
     */
    [[nodiscard]] constexpr friend bool operator!=(const optional& a, nullopt_t)
    {
        return a._valid;
    }

    /**
     * @brief Less than operator.
     */
    [[nodiscard]] constexpr friend bool operator<(const optional&, nullopt_t)
    {
        return false;
    }

    /**
     * @brief Greater than operator.
     */
    [[nodiscard]] constexpr friend bool operator>(const optional& a, nullopt_t)
    {
        return a._valid;
    }

    /**
     * @brief Less than or equal operator.
     */
    [[nodiscard]] constexpr friend bool operator<=(const optional& a, nullopt_t)
    {
        return ! a._valid;
    }

    /**
     * @brief Greater than or equal operator.
     */
    [[nodiscard]] constexpr friend bool operator>=(const optional&, nullopt_t)
    {
        return true;
    }

    /**
     * @brief Equal operator.
     */
    template<typename OtherType>
    [[nodiscard]] constexpr friend bool operator==(const optional& a, const OtherType& b)
    {
        return a._valid ? a._value == b : false;
    }

    /**
     * @brief Less than operator.
     */
    template<typename OtherType>
    [[nodiscard]] constexpr friend bool operator<(const optional& a, const OtherType& b)
    {
        return a._valid ? a._value < b : true;
    }

    /**
     * @brief Greater than operator.
     */
    template<typename OtherType>
    [[nodiscard]] constexpr friend bool operator>(const optional& a, const OtherType& b)
    {
        return b < a;
    }

    /**
     * @brief Less than or equal operator.
     */
    template<typename OtherType>
    [[nodiscard]] constexpr friend bool operator<=(const optional& a, const OtherType& b)
    {
        return ! (a > b);
    }

    /**
     * @brief Greater than or equal operator.
     */
    template<typename OtherType>
    [[nodiscard]] constexpr friend bool operator>=(const optional& a, const OtherType& b)
    {
        return ! (a < b);
    }

private:
    union
    {
        Type _value;
        char _empty;
    };

    bool _valid = false;
};


/**
 * @brief Constructs an object and wraps it in a optional.
 *
 * @tparam Type Type of the object to construct.
 * @tparam Args Type of the arguments of the object to construct.
 *
 * @param args Parameters of the object to construct.
 * @return An optional managing the new object.
 *
 * @ingroup optional
 */
template<typename Type, class... Args>
[[nodiscard]] constexpr optional<Type> make_optional(Args&&... args)
{
    return optional<Type>(Type(forward<Args>(args)...));
}


/**
 * @brief Hash support for optional.
 *
 * @tparam Type Type of the contained value.
 *
 * @ingroup optional
 * @ingroup functional
 */
template<typename Type>
struct hash<optional<Type>>
{
    /**
     * @brief Returns the hash of the given optional.
     */
    [[nodiscard]] constexpr unsigned operator()(const optional<Type>& optional) const
    {
        return optional.has_value() ? make_hash(optional.value()) : make_hash(0);
    }
};

}

#endif
