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
    constexpr explicit nullopt_t(int)
    {
    }
};

constexpr const nullopt_t nullopt{0};

template<typename Type>
class optional
{

public:
    optional() :
        _valid(false)
    {
    }

    optional(nullopt_t) :
        _valid(false)
    {
    }

    optional(const optional& other) :
        _valid(other)
    {
        if(_valid)
        {
            ::new(_storage) Type(other._value_impl());
        }
    }

    optional(optional&& other) noexcept :
        _valid(other)
    {
        if(_valid)
        {
            ::new(_storage) Type(move(other._value_impl()));
        }
    }

    template<typename OtherType>
    optional(const optional<OtherType>& other) :
        _valid(other)
    {
        if(_valid)
        {
            ::new(_storage) Type(other._value_impl());
        }
    }

    template<typename OtherType>
    optional(optional<OtherType>&& other) noexcept :
        _valid(other)
    {
        if(_valid)
        {
            ::new(_storage) Type(move(other._value_impl()));
        }
    }

    optional(const Type& value) :
        _valid(true)
    {
        ::new(_storage) Type(value);
    }

    optional(Type&& value) :
        _valid(true)
    {
        ::new(_storage) Type(move(value));
    }

    ~optional()
    {
        _clean();
    }

    optional& operator=(nullopt_t)
    {
        reset();
        return *this;
    }

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

    optional& operator=(const Type& value)
    {
        _clean();
        ::new(_storage) Type(value);
        _valid = true;
        return *this;
    }

    optional& operator=(Type&& value)
    {
        _clean();
        ::new(_storage) Type(move(value));
        _valid = true;
        return *this;
    }

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

    [[nodiscard]] const Type* operator->() const
    {
        BTN_ASSERT(_valid, "Optional is not valid");

        return &_value_impl();
    }

    [[nodiscard]] Type* operator->()
    {
        BTN_ASSERT(_valid, "Optional is not valid");

        return &_value_impl();
    }

    [[nodiscard]] const Type& operator*() const
    {
        BTN_ASSERT(_valid, "Optional is not valid");

        return _value_impl();
    }

    [[nodiscard]] Type& operator*()
    {
        BTN_ASSERT(_valid, "Optional is not valid");

        return _value_impl();
    }

    [[nodiscard]] explicit operator bool() const
    {
        return _valid;
    }

    [[nodiscard]] bool has_value() const
    {
        return _valid;
    }

    [[nodiscard]] const Type& value() const
    {
        BTN_ASSERT(_valid, "Optional is not valid");

        return _value_impl();
    }

    [[nodiscard]] Type& value()
    {
        BTN_ASSERT(_valid, "Optional is not valid");

        return _value_impl();
    }

    [[nodiscard]] Type value_or(const Type& default_value) const
    {
        return _valid ? _value_impl() : default_value;
    }

    [[nodiscard]] Type value_or(Type&& default_value) const
    {
        return _valid ? _value_impl() : move(default_value);
    }

    void reset()
    {
        if(_valid)
        {
            _value_impl().~Type();
            _valid = false;
        }
    }

    template<typename... Args>
    void emplace(Args&&... args)
    {
        _clean();
        ::new(_storage) Type(forward<Args>(args)...);
        _valid = true;
    }

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

    friend void swap(optional& a, optional& b)
    {
        a.swap(b);
    }

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

    [[nodiscard]] friend bool operator!=(const optional& a, const optional& b)
    {
        return ! (a == b);
    }

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

    [[nodiscard]] friend bool operator>(const optional& a, const optional& b)
    {
        return b < a;
    }

    [[nodiscard]] friend bool operator<=(const optional& a, const optional& b)
    {
        return ! (a > b);
    }

    [[nodiscard]] friend bool operator>=(const optional& a, const optional& b)
    {
        return ! (a < b);
    }

    [[nodiscard]] friend bool operator==(const optional& a, nullopt_t)
    {
        return ! a._valid;
    }

    [[nodiscard]] friend bool operator!=(const optional& a, nullopt_t)
    {
        return a._valid;
    }

    [[nodiscard]] friend bool operator<(const optional&, nullopt_t)
    {
        return false;
    }

    [[nodiscard]] friend bool operator>(const optional& a, nullopt_t)
    {
        return a._valid;
    }

    [[nodiscard]] friend bool operator<=(const optional& a, nullopt_t)
    {
        return ! a._valid;
    }

    [[nodiscard]] friend bool operator>=(const optional&, nullopt_t)
    {
        return true;
    }

    template<typename OtherType>
    [[nodiscard]] friend bool operator==(const optional& a, const OtherType& b)
    {
        return a._valid ? a._value_impl() == b : false;
    }

    template<typename OtherType>
    [[nodiscard]] friend bool operator!=(const optional& a, const OtherType& b)
    {
        return ! (a == b);
    }

    template<typename OtherType>
    [[nodiscard]] friend bool operator<(const optional& a, const OtherType& b)
    {
        return a._valid ? a._value_impl() < b : true;
    }

    template<typename OtherType>
    [[nodiscard]] friend bool operator>(const optional& a, const OtherType& b)
    {
        return b < a;
    }

    template<typename OtherType>
    [[nodiscard]] friend bool operator<=(const optional& a, const OtherType& b)
    {
        return ! (a > b);
    }

    template<typename OtherType>
    [[nodiscard]] friend bool operator>=(const optional& a, const OtherType& b)
    {
        return ! (a < b);
    }

    template<typename... Args>
    [[nodiscard]] friend optional make_optional(Args&&... args)
    {
        optional result;
        ::new(result._storage) Type(forward<Args>(args)...);
        result._valid = true;
        return result;
    }

private:
    alignas(alignof(Type)) char _storage[sizeof(Type)];
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


template<typename Type>
struct hash<optional<Type>>
{
    [[nodiscard]] unsigned operator()(const optional<Type>& optional) const
    {
        return optional.has_value() ? make_hash(optional.value()) : make_hash(0);
    }
};

}

#endif
