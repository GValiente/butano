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
            reset();

            if(other)
            {
                ::new(_storage) Type(other._value_impl());
                _valid = true;
            }
        }

        return *this;
    }

    constexpr optional& operator=(optional&& other)
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

    template<typename OtherType>
    constexpr optional& operator=(OtherType&& value)
    {
        reset();
        ::new(_storage) Type(forward<OtherType>(value));
        _valid = true;
        return *this;
    }

    template<typename OtherType>
    constexpr optional& operator=(const optional<OtherType>& other)
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

    template<typename OtherType>
    constexpr optional& operator=(optional<OtherType>&& other)
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
        reset();
        ::new(_storage) Type(forward<Args>(args)...);
        _valid = true;
    }

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

        return a._value_impl() == b._value_impl();
    }

    [[nodiscard]] constexpr friend bool operator!=(const optional& a, const optional& b)
    {
        return ! (a == b);
    }

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

        return a._value_impl() < b._value_impl();
    }

    [[nodiscard]] constexpr friend bool operator<=(const optional& a, const optional& b)
    {
        if(! a._valid)
        {
            return true;
        }

        if(! b._valid)
        {
            return false;
        }

        return a._value_impl() <= b._value_impl();
    }

    [[nodiscard]] constexpr friend bool operator>(const optional& a, const optional& b)
    {
        return ! (a <= b);
    }

    [[nodiscard]] constexpr friend bool operator>=(const optional& a, const optional& b)
    {
        return ! (a < b);
    }

    [[nodiscard]] constexpr friend bool operator==(const optional& a, nullopt_t)
    {
        return ! a._valid;
    }

    [[nodiscard]] constexpr friend bool operator!=(const optional& a, nullopt_t)
    {
        return a._valid;
    }

    [[nodiscard]] constexpr friend bool operator<(const optional&, nullopt_t)
    {
        return false;
    }

    [[nodiscard]] constexpr friend bool operator<=(const optional& a, nullopt_t)
    {
        return ! a._valid;
    }

    [[nodiscard]] constexpr friend bool operator>(const optional& a, nullopt_t)
    {
        return a._valid;
    }

    [[nodiscard]] constexpr friend bool operator>=(const optional&, nullopt_t)
    {
        return true;
    }

    template<typename OtherType>
    [[nodiscard]] constexpr friend bool operator==(const optional& a, const OtherType& b)
    {
        return a._valid ? a._value_impl() == b : false;
    }

    template<typename OtherType>
    [[nodiscard]] constexpr friend bool operator!=(const optional& a, const OtherType& b)
    {
        return ! (a == b);
    }

    template<typename OtherType>
    [[nodiscard]] constexpr friend bool operator<(const optional& a, const OtherType& b)
    {
        return a._valid ? a._value_impl() < b : true;
    }

    template<typename OtherType>
    [[nodiscard]] constexpr friend bool operator<=(const optional& a, const OtherType& b)
    {
        return a._valid ? a._value_impl() <= b : true;
    }

    template<typename OtherType>
    [[nodiscard]] constexpr friend bool operator>(const optional& a, const OtherType& b)
    {
        return ! (a <= b);
    }

    template<typename OtherType>
    [[nodiscard]] constexpr friend bool operator>=(const optional& a, const OtherType& b)
    {
        return ! (a < b);
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
