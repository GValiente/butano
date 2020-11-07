/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_OPTIONAL_H
#define BTN_OPTIONAL_H

#include <new>
#include "btn_assert.h"
#include "btn_utility.h"
#include "btn_functional.h"

namespace btn
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

constexpr const nullopt_t nullopt{0}; //!< Constant of type nullopt_t used to indicate an optional type with uninitialized state.

template<typename Type>
class optional
{

public:
    /**
     * @brief Default constructor.
     */
    optional() :
        _valid(false)
    {
    }

    /**
     * @brief Null constructor.
     */
    optional(nullopt_t) :
        _valid(false)
    {
    }

    /**
     * @brief Copy constructor.
     * @param other optional to copy.
     */
    optional(const optional& other) :
        _valid(other)
    {
        if(_valid)
        {
            ::new(_storage) Type(other._value_impl());
        }
    }

    /**
     * @brief Move constructor.
     * @param other optional to move.
     */
    optional(optional&& other) noexcept :
        _valid(other)
    {
        if(_valid)
        {
            ::new(_storage) Type(move(other._value_impl()));
        }
    }

    /**
     * @brief Copy constructor.
     * @param other Different type optional to copy.
     */
    template<typename OtherType>
    optional(const optional<OtherType>& other) :
        _valid(other)
    {
        if(_valid)
        {
            ::new(_storage) Type(other._value_impl());
        }
    }

    /**
     * @brief Move constructor.
     * @param other Different type optional to move.
     */
    template<typename OtherType>
    optional(optional<OtherType>&& other) noexcept :
        _valid(other)
    {
        if(_valid)
        {
            ::new(_storage) Type(move(other._value_impl()));
        }
    }

    /**
     * @brief Constructor.
     * @param value Value to copy.
     */
    optional(const Type& value) :
        _valid(true)
    {
        ::new(_storage) Type(value);
    }

    /**
     * @brief Constructor.
     * @param value Value to move.
     */
    optional(Type&& value) :
        _valid(true)
    {
        ::new(_storage) Type(move(value));
    }

    /**
     * @brief Destructor.
     */
    ~optional()
    {
        _clean();
    }

    /**
     * @brief Null assignment operator.
     * @return Reference to this.
     */
    optional& operator=(nullopt_t)
    {
        reset();
        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param other optional to copy.
     * @return Reference to this.
     */
    optional& operator=(const optional& other)
    {
        if(this != &other)
        {
            reset();

            if(other)
            {
                ::new(_storage) Type(other._value_impl());
                _valid = true;
            }
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other optional to move.
     * @return Reference to this.
     */
    optional& operator=(optional&& other) noexcept
    {
        if(this != &other)
        {
            reset();

            if(other)
            {
                ::new(_storage) Type(move(other._value_impl()));
                _valid = true;
            }
        }

        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param other Different type optional to copy.
     * @return Reference to this.
     */
    template<typename OtherType>
    optional& operator=(const optional<OtherType>& other)
    {
        if(this != &other)
        {
            reset();

            if(other)
            {
                ::new(_storage) Type(other._value_impl());
                _valid = true;
            }
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other Different type optional to move.
     * @return Reference to this.
     */
    template<typename OtherType>
    optional& operator=(optional<OtherType>&& other) noexcept
    {
        if(this != &other)
        {
            reset();

            if(other)
            {
                ::new(_storage) Type(move(other._value_impl()));
                _valid = true;
            }
        }

        return *this;
    }

    /**
     * @brief Assignment operator.
     * @param value Value to copy.
     * @return Reference to this.
     */
    optional& operator=(const Type& value)
    {
        _clean();
        ::new(_storage) Type(value);
        _valid = true;
        return *this;
    }

    /**
     * @brief Assignment operator.
     * @param value Value to move.
     * @return Reference to this.
     */
    optional& operator=(Type&& value)
    {
        _clean();
        ::new(_storage) Type(move(value));
        _valid = true;
        return *this;
    }

    /**
     * @brief Indicates if it contains a value or not.
     */
    [[nodiscard]] bool has_value() const
    {
        return _valid;
    }

    /**
     * @brief Indicates if it contains a value or not.
     */
    [[nodiscard]] explicit operator bool() const
    {
        return _valid;
    }

    /**
     * @brief Returns a const reference to the contained value.
     */
    [[nodiscard]] const Type& value() const
    {
        BTN_ASSERT(_valid, "Optional is not valid");

        return _value_impl();
    }

    /**
     * @brief Returns a reference to the contained value.
     */
    [[nodiscard]] Type& value()
    {
        BTN_ASSERT(_valid, "Optional is not valid");

        return _value_impl();
    }

    /**
     * @brief Returns a copy of the contained value if this optional has one;
     * otherwise it returns a copy of the given default value.
     */
    [[nodiscard]] Type value_or(const Type& default_value) const
    {
        return _valid ? _value_impl() : default_value;
    }

    /**
     * @brief Returns a copy of the contained value if this optional has one;
     * otherwise it returns a moved copy of the given default value.
     */
    [[nodiscard]] Type value_or(Type&& default_value) const
    {
        return _valid ? _value_impl() : move(default_value);
    }

    /**
     * @brief Returns a const pointer to the contained value.
     */
    [[nodiscard]] const Type* operator->() const
    {
        BTN_ASSERT(_valid, "Optional is not valid");

        return &_value_impl();
    }

    /**
     * @brief Returns a pointer to the contained value.
     */
    [[nodiscard]] Type* operator->()
    {
        BTN_ASSERT(_valid, "Optional is not valid");

        return &_value_impl();
    }

    /**
     * @brief Returns a const reference to the contained value.
     */
    [[nodiscard]] const Type& operator*() const
    {
        BTN_ASSERT(_valid, "Optional is not valid");

        return _value_impl();
    }

    /**
     * @brief Returns a reference to the contained value.
     */
    [[nodiscard]] Type& operator*()
    {
        BTN_ASSERT(_valid, "Optional is not valid");

        return _value_impl();
    }

    /**
     * @brief Constructs the contained value in-place.
     * @param args Parameters of the value to construct.
     * @return Reference to the constructed value.
     */
    template<typename... Args>
    Type& emplace(Args&&... args)
    {
        _clean();
        ::new(_storage) Type(forward<Args>(args)...);
        _valid = true;
        return _value_impl();
    }

    /**
     * @brief Disposes the contained value.
     */
    void reset()
    {
        if(_valid)
        {
            _value_impl().~Type();
            _valid = false;
        }
    }

    /**
     * @brief Exchanges the contents of this optional with those of the other one.
     * @param other optional to exchange the contents with.
     */
    void swap(optional& other)
    {
        if(_valid)
        {
            if(other._valid)
            {
                btn::swap(_value_impl(), other._value_impl());
            }
            else
            {
                ::new(other._storage) Type(move(_value_impl()));
                _value_impl().~Type();
                other._valid = true;
                _valid = false;
            }
        }
        else
        {
            if(other._valid)
            {
                ::new(_storage) Type(move(other._value_impl()));
                other.value_impl().~Type();
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
    friend void swap(optional& a, optional& b)
    {
        a.swap(b);
    }

    /**
     * @brief Equal operator.
     */
    [[nodiscard]] friend bool operator==(const optional& a, const optional& b)
    {
        if(a._valid != b._valid)
        {
            return false;
        }

        if(! a._valid && ! b._valid)
        {
            return true;
        }

        return a._value_impl() == b._value_impl();
    }

    /**
     * @brief Not equal operator.
     */
    [[nodiscard]] friend bool operator!=(const optional& a, const optional& b)
    {
        return ! (a == b);
    }

    /**
     * @brief Less than operator.
     */
    [[nodiscard]] friend bool operator<(const optional& a, const optional& b)
    {
        if(! b._valid)
        {
            return false;
        }

        if(! a._valid)
        {
            return true;
        }

        return a._value_impl() < b._value_impl();
    }

    /**
     * @brief Greater than operator.
     */
    [[nodiscard]] friend bool operator>(const optional& a, const optional& b)
    {
        return b < a;
    }

    /**
     * @brief Less than or equal operator.
     */
    [[nodiscard]] friend bool operator<=(const optional& a, const optional& b)
    {
        return ! (a > b);
    }

    /**
     * @brief Greater than or equal operator.
     */
    [[nodiscard]] friend bool operator>=(const optional& a, const optional& b)
    {
        return ! (a < b);
    }

    /**
     * @brief Equal operator.
     */
    [[nodiscard]] friend bool operator==(const optional& a, nullopt_t)
    {
        return ! a._valid;
    }

    /**
     * @brief Not equal operator.
     */
    [[nodiscard]] friend bool operator!=(const optional& a, nullopt_t)
    {
        return a._valid;
    }

    /**
     * @brief Less than operator.
     */
    [[nodiscard]] friend bool operator<(const optional&, nullopt_t)
    {
        return false;
    }

    /**
     * @brief Greater than operator.
     */
    [[nodiscard]] friend bool operator>(const optional& a, nullopt_t)
    {
        return a._valid;
    }

    /**
     * @brief Less than or equal operator.
     */
    [[nodiscard]] friend bool operator<=(const optional& a, nullopt_t)
    {
        return ! a._valid;
    }

    /**
     * @brief Greater than or equal operator.
     */
    [[nodiscard]] friend bool operator>=(const optional&, nullopt_t)
    {
        return true;
    }

    /**
     * @brief Equal operator.
     */
    template<typename OtherType>
    [[nodiscard]] friend bool operator==(const optional& a, const OtherType& b)
    {
        return a._valid ? a._value_impl() == b : false;
    }

    /**
     * @brief Not equal operator.
     */
    template<typename OtherType>
    [[nodiscard]] friend bool operator!=(const optional& a, const OtherType& b)
    {
        return ! (a == b);
    }

    /**
     * @brief Less than operator.
     */
    template<typename OtherType>
    [[nodiscard]] friend bool operator<(const optional& a, const OtherType& b)
    {
        return a._valid ? a._value_impl() < b : true;
    }

    /**
     * @brief Greater than operator.
     */
    template<typename OtherType>
    [[nodiscard]] friend bool operator>(const optional& a, const OtherType& b)
    {
        return b < a;
    }

    /**
     * @brief Less than or equal operator.
     */
    template<typename OtherType>
    [[nodiscard]] friend bool operator<=(const optional& a, const OtherType& b)
    {
        return ! (a > b);
    }

    /**
     * @brief Greater than or equal operator.
     */
    template<typename OtherType>
    [[nodiscard]] friend bool operator>=(const optional& a, const OtherType& b)
    {
        return ! (a < b);
    }

private:
    alignas(Type) char _storage[sizeof(Type)];
    bool _valid;

    [[nodiscard]] const Type& _value_impl() const
    {
        return *(reinterpret_cast<const Type*>(_storage));
    }

    [[nodiscard]] Type& _value_impl()
    {
        return *(reinterpret_cast<Type*>(_storage));
    }

    void _clean()
    {
        if(_valid)
        {
            _value_impl().~Type();
        }
    }
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
[[nodiscard]] optional<Type> make_optional(Args&&... args)
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
    [[nodiscard]] unsigned operator()(const optional<Type>& optional) const
    {
        return optional.has_value() ? make_hash(optional.value()) : make_hash(0);
    }
};

}

#endif
