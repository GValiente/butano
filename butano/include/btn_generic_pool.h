/*
 * Copyright (c) 2014 jwellbelove Embedded Template Library https://www.etlcpp.com
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_GENERIC_POOL_H
#define BTN_GENERIC_POOL_H

#include "btn_assert.h"
#include "btn_utility.h"
#include "btn_generic_pool_fwd.h"

namespace btn
{

template<int TypeSize>
class igeneric_pool
{

public:
    using size_type = int;

    igeneric_pool(const igeneric_pool&) = delete;

    igeneric_pool& operator=(const igeneric_pool&) = delete;

    ~igeneric_pool()
    {
        BTN_ASSERT(empty(), "Pool is not empty");
    }

    [[nodiscard]] size_type size() const
    {
        return _allocated_items_count;
    }

    [[nodiscard]] size_type max_size() const
    {
        return _max_size;
    }

    [[nodiscard]] size_type available() const
    {
        return _max_size - _allocated_items_count;
    }

    [[nodiscard]] bool empty() const
    {
        return _allocated_items_count == 0;
    }

    [[nodiscard]] bool full() const
    {
        return _allocated_items_count == _max_size;
    }

    template<typename Type, typename... Args>
    [[nodiscard]] Type& create(Args&&... args)
    {
        static_assert(sizeof(Type) <= TypeSize);
        BTN_ASSERT(! full(), "Pool is full");

        auto result = reinterpret_cast<Type*>(_allocate());
        ::new(result) Type(forward<Args>(args)...);
        return *result;
    }

    template<typename Type>
    void destroy(Type& value)
    {
        static_assert(sizeof(Type) <= TypeSize);
        BTN_ASSERT(_contains(value), "Pool does not contain this value");

        value.~Type();
        _free(reinterpret_cast<char*>(&value));
    }

protected:
    igeneric_pool(char* buffer, size_type max_size) :
        _buffer(buffer),
        _next_ptr(buffer),
        _max_size(max_size),
        _allocated_items_count(0),
        _initialised_items_count(0)
    {
    }

    template<typename Type>
    [[nodiscard]] bool _contains(const Type& value) const
    {
        auto ptr = reinterpret_cast<const char*>(&value);
        size_type index = ptr - _buffer;
        return index >= 0 && index <= (TypeSize * _max_size) - TypeSize;
    }

    [[nodiscard]] char* _allocate()
    {
        if(_initialised_items_count < _max_size)
        {
            char* ptr = _buffer + (_initialised_items_count * TypeSize);
            char* next_ptr = ptr + TypeSize;
            *reinterpret_cast<char**>(ptr) = next_ptr;
            ++_initialised_items_count;
        }

        char* result = _next_ptr;
        ++_allocated_items_count;

        if(_allocated_items_count != _max_size)
        {
            _next_ptr = *reinterpret_cast<char**>(_next_ptr);
        }
        else
        {
            _next_ptr = nullptr;
        }

        return result;
    }

    void _free(char* ptr)
    {
        auto typed_ptr = reinterpret_cast<uintptr_t*>(ptr);

        if(_next_ptr)
        {
            *typed_ptr = reinterpret_cast<uintptr_t>(_next_ptr);
        }
        else
        {
            *typed_ptr = 0;
        }

        _next_ptr = ptr;
        --_allocated_items_count;
    }

private:
    char* _buffer;
    char* _next_ptr;
    size_type _max_size;
    size_type _allocated_items_count;
    size_type _initialised_items_count;
};


template<int TypeSize, int TypeAlignment>
union alignas(TypeAlignment) generic_pool_element
{
    static_assert(TypeAlignment >= alignof(char*));

    char* next;
    char value[TypeSize];
};


template<int TypeSize, int TypeAlignment, int MaxSize>
class generic_pool : public igeneric_pool<sizeof(generic_pool_element<TypeSize, TypeAlignment>)>
{

private:
    using element = generic_pool_element<TypeSize, TypeAlignment>;
    using base_type = igeneric_pool<sizeof(element)>;

public:
    using size_type = typename base_type::size_type;

    generic_pool() :
        base_type(reinterpret_cast<char*>(&_buffer[0]), MaxSize)
    {
    }

    template<typename Type>
    [[nodiscard]] bool contains(const Type& value) const
    {
        static_assert(alignof(Type) == TypeAlignment);

        return base_type::_contains(value);
    }

    template<typename Type, typename... Args>
    [[nodiscard]] Type& create(Args&&... args)
    {
        static_assert(alignof(Type) == TypeAlignment);

        auto result = reinterpret_cast<Type*>(base_type::_allocate());
        ::new(result) Type(forward<Args>(args)...);
        return *result;
    }

    template<typename Type>
    void destroy(Type& value)
    {
        static_assert(alignof(Type) == TypeAlignment);
        BTN_ASSERT(contains(value), "Pool does not contain this value");

        value.~Type();
        base_type::_free(reinterpret_cast<char*>(&value));
    }

private:
    alignas(TypeAlignment) char _buffer[sizeof(element) * MaxSize];
};

}

#endif
