#ifndef BTN_OPTIONAL_H
#define BTN_OPTIONAL_H

#include <new>
#include "btn_assert.h"
#include "btn_utility.h"
#include "btn_functional.h"

namespace btn
{

class nullopt_t
{

public:
    explicit constexpr nullopt_t(int)
    {
    }
};

inline constexpr nullopt_t nullopt{0};

template<typename Type>
class optional
{

public:
    constexpr optional() :
        _valid(false)
    {
    }

    constexpr optional(nullopt_t) :
        _valid(false)
    {
    }

    constexpr optional(const optional& other) :
        _valid(other)
    {
        if(_valid)
        {
            ::new(_storage) Type(other._value_impl());
        }
    }

    constexpr optional(optional&& other) :
        _valid(other)
    {
        if(_valid)
        {
            ::new(_storage) Type(move(other._value_impl()));
        }
    }

    template<typename OtherType>
    constexpr optional(const optional<OtherType>& other) :
        _valid(other)
    {
        if(_valid)
        {
            ::new(_storage) Type(other._value_impl());
        }
    }

    template<typename OtherType>
    constexpr optional(optional<OtherType>&& other) :
        _valid(other)
    {
        if(_valid)
        {
            ::new(_storage) Type(move(other._value_impl()));
        }
    }

    template<typename OtherType>
    constexpr optional(OtherType&& value) :
        _valid(true)
    {
        ::new(_storage) Type(forward<OtherType>(value));
    }

    ~optional()
    {
        reset();
    }

    constexpr optional& operator=(nullopt_t)
    {
        reset();
        return *this;
    }

    constexpr optional& operator=(const optional& other)
    {
        if(this != &other)
        {
            if(other)
            {
                if(_valid)
                {
                    _value_impl() = other._value_impl();
                }
                else
                {
                    ::new(_storage) Type(other._value_impl());
                    _valid = true;
                }
            }
            else
            {
                reset();
            }
        }

        return *this;
    }

    constexpr optional& operator=(optional&& other)
    {
        if(this != &other)
        {
            if(other)
            {
                if(_valid)
                {
                    _value_impl() = move(other._value_impl());
                }
                else
                {
                    ::new(_storage) Type(move(other._value_impl()));
                    _valid = true;
                }
            }
            else
            {
                reset();
            }
        }

        return *this;
    }

    template<typename OtherType>
    constexpr optional& operator=(OtherType&& value)
    {
        if(_valid)
        {
            _value_impl() = forward<OtherType>(value);
        }
        else
        {
            ::new(_storage) Type(forward<OtherType>(value));
            _valid = true;
        }

        return *this;
    }

    template<typename OtherType>
    constexpr optional& operator=(const optional<OtherType>& other)
    {
        if(this != &other)
        {
            if(other)
            {
                if(_valid)
                {
                    _value_impl() = other._value_impl();
                }
                else
                {
                    ::new(_storage) Type(other._value_impl());
                    _valid = true;
                }
            }
            else
            {
                reset();
            }
        }

        return *this;
    }

    template<typename OtherType>
    constexpr optional& operator=(optional<OtherType>&& other)
    {
        if(this != &other)
        {
            if(other)
            {
                if(_valid)
                {
                    _value_impl() = move(other._value_impl());
                }
                else
                {
                    ::new(_storage) Type(move(other._value_impl()));
                    _valid = true;
                }
            }
            else
            {
                reset();
            }
        }

        return *this;
    }

    [[nodiscard]] constexpr const Type* operator->() const
    {
        BTN_CONSTEXPR_ASSERT(_valid, "Optional is not valid");

        return &_value_impl();
    }

    [[nodiscard]] constexpr Type* operator->()
    {
        BTN_CONSTEXPR_ASSERT(_valid, "Optional is not valid");

        return &_value_impl();
    }

    [[nodiscard]] constexpr const Type& operator*() const
    {
        BTN_CONSTEXPR_ASSERT(_valid, "Optional is not valid");

        return _value_impl();
    }

    [[nodiscard]] constexpr Type& operator*()
    {
        BTN_CONSTEXPR_ASSERT(_valid, "Optional is not valid");

        return _value_impl();
    }

    [[nodiscard]] constexpr explicit operator bool() const
    {
        return _valid;
    }

    [[nodiscard]] constexpr bool has_value() const
    {
        return _valid;
    }

    [[nodiscard]] constexpr const Type& value() const
    {
        BTN_CONSTEXPR_ASSERT(_valid, "Optional is not valid");

        return _value_impl();
    }

    [[nodiscard]] constexpr Type& value()
    {
        BTN_CONSTEXPR_ASSERT(_valid, "Optional is not valid");

        return _value_impl();
    }

    template<typename OtherType>
    [[nodiscard]] constexpr Type value_or(OtherType&& default_value) const
    {
        return _valid ? _value_impl() : forward<OtherType>(default_value);
    }

    constexpr void swap(optional& other)
    {
        optional temp(move(*this));
        *this = move(other);
        other = move(temp);
    }

    constexpr void reset()
    {
        if(_valid)
        {
            _value_impl().~Type();
            _valid = false;
        }
    }

    template<typename... Args>
    constexpr void emplace(Args&&... args)
    {
        if(_valid)
        {
            _value_impl() = Type(forward<Args>(args)...);
        }
        else
        {
            ::new(_storage) Type(forward<Args>(args)...);
            _valid = true;
        }
    }

    [[nodiscard]] constexpr bool operator==(const optional& other) const
    {
        if(_valid != other._valid)
        {
            return false;
        }

        if(! _valid && ! other._valid)
        {
            return true;
        }

        return _value_impl() == other._value_impl();
    }

    [[nodiscard]] constexpr bool operator!=(const optional& other) const
    {
        return ! (*this == other);
    }

    [[nodiscard]] constexpr bool operator<(const optional& other) const
    {
        if(! other._valid)
        {
            return false;
        }

        if(! _valid)
        {
            return true;
        }

        return _value_impl() < other._value_impl();
    }

    [[nodiscard]] constexpr bool operator<=(const optional& other) const
    {
        if(! _valid)
        {
            return true;
        }

        if(! other._valid)
        {
            return false;
        }

        return _value_impl() <= other._value_impl();
    }

    [[nodiscard]] constexpr bool operator>(const optional& other) const
    {
        return ! (*this <= other);
    }

    [[nodiscard]] constexpr bool operator>=(const optional& other) const
    {
        return ! (*this < other);
    }

    [[nodiscard]] constexpr bool operator==(nullopt_t) const
    {
        return ! _valid;
    }

    [[nodiscard]] constexpr bool operator!=(nullopt_t) const
    {
        return _valid;
    }

    [[nodiscard]] constexpr bool operator<(nullopt_t) const
    {
        return false;
    }

    [[nodiscard]] constexpr bool operator<=(nullopt_t) const
    {
        return ! _valid;
    }

    [[nodiscard]] constexpr bool operator>(nullopt_t) const
    {
        return _valid;
    }

    [[nodiscard]] constexpr bool operator>=(nullopt_t) const
    {
        return true;
    }

    template<typename OtherType>
    [[nodiscard]] constexpr bool operator==(const OtherType& value) const
    {
        return _valid ? _value_impl() == value : false;
    }

    template<typename OtherType>
    [[nodiscard]] constexpr bool operator!=(const OtherType& value) const
    {
        return ! (*this == value);
    }

    template<typename OtherType>
    [[nodiscard]] constexpr bool operator<(const OtherType& value) const
    {
        return _valid ? _value_impl() < value : true;
    }

    template<typename OtherType>
    [[nodiscard]] constexpr bool operator<=(const OtherType& value) const
    {
        return _valid ? _value_impl() <= value : true;
    }

    template<typename OtherType>
    [[nodiscard]] constexpr bool operator>(const OtherType& value) const
    {
        return ! (*this <= value);
    }

    template<typename OtherType>
    [[nodiscard]] constexpr bool operator>=(const OtherType& value) const
    {
        return ! (*this < value);
    }

private:
    alignas(alignof(Type)) char _storage[sizeof(Type)];
    bool _valid;

    [[nodiscard]] constexpr const Type& _value_impl() const
    {
        return *(reinterpret_cast<const Type*>(_storage));
    }

    [[nodiscard]] constexpr Type& _value_impl()
    {
        return *(reinterpret_cast<Type*>(_storage));
    }
};


template<typename Type>
struct hash<optional<Type>>
{
    [[nodiscard]] constexpr size_t operator()(const optional<Type>& optional) const
    {
        return bool(optional) ? make_hash(optional.value()) : make_hash(0);
    }
};

}

#endif
