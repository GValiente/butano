/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 jwellbelove, https://github.com/ETLCPP/etl, http://www.etlcpp.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef BN_GENERIC_POOL_H
#define BN_GENERIC_POOL_H

/**
 * @file
 * bn::igeneric_pool and bn::generic_pool implementation header file.
 *
 * @ingroup pool
 */

#include "bn_assert.h"
#include "bn_utility.h"
#include "bn_generic_pool_fwd.h"

namespace bn
{

template<int MaxElementSize>
class igeneric_pool
{

public:
    using size_type = int; //!< Size type alias.

    igeneric_pool(const igeneric_pool&) = delete;

    igeneric_pool& operator=(const igeneric_pool&) = delete;

    /**
     * @brief Destructor.
     *
     * It doesn't destroy its elements, they must be destroyed manually.
     */
    ~igeneric_pool()
    {
        BN_ASSERT(empty(), "Pool is not empty");
    }

    /**
     * @brief Returns the current elements count.
     */
    [[nodiscard]] size_type size() const
    {
        return _allocated_items_count;
    }

    /**
     * @brief Returns the maximum possible elements count.
     */
    [[nodiscard]] size_type max_size() const
    {
        return _max_size;
    }

    /**
     * @brief Returns the remaining element capacity.
     */
    [[nodiscard]] size_type available() const
    {
        return _max_size - _allocated_items_count;
    }

    /**
     * @brief Indicates if it doesn't contain any element.
     */
    [[nodiscard]] bool empty() const
    {
        return _allocated_items_count == 0;
    }

    /**
     * @brief Indicates if it can't contain any more elements.
     */
    [[nodiscard]] bool full() const
    {
        return _allocated_items_count == _max_size;
    }

    /**
     * @brief Constructs a value inside of the igeneric_pool.
     * @tparam Type Type of the value to construct.
     * @tparam Args Type of the arguments of the value to construct.
     * @param args Parameters of the value to construct.
     * @return Reference to the new value.
     */
    template<typename Type, typename... Args>
    [[nodiscard]] Type& create(Args&&... args)
    {
        static_assert(sizeof(Type) <= MaxElementSize);
        BN_ASSERT(! full(), "Pool is full");

        auto result = reinterpret_cast<Type*>(_allocate());
        ::new(result) Type(forward<Args>(args)...);
        return *result;
    }

    /**
     * @brief Destroys the given value, previously allocated with the create method.
     */
    template<typename Type>
    void destroy(Type& value)
    {
        static_assert(sizeof(Type) <= MaxElementSize);
        BN_ASSERT(_contains(value), "Pool does not contain this value");

        value.~Type();
        _free(reinterpret_cast<char*>(&value));
    }

protected:
    /// @cond DO_NOT_DOCUMENT

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
        ptrdiff_t index = ptr - _buffer;
        return index >= 0 && index <= (MaxElementSize * _max_size) - MaxElementSize;
    }

    [[nodiscard]] char* _allocate()
    {
        if(_initialised_items_count < _max_size)
        {
            char* ptr = _buffer + (_initialised_items_count * MaxElementSize);
            char* next_ptr = ptr + MaxElementSize;
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

    /// @endcond

private:
    char* _buffer;
    char* _next_ptr;
    size_type _max_size;
    size_type _allocated_items_count;
    size_type _initialised_items_count;
};


/// @cond DO_NOT_DOCUMENT

template<int MaxElementSize>
union generic_pool_element
{
    char* next;
    char buffer[MaxElementSize];
};

/// @endcond


template<int MaxElementSize, int MaxSize>
class generic_pool : public igeneric_pool<sizeof(generic_pool_element<MaxElementSize>)>
{

private:
    using element = generic_pool_element<MaxElementSize>;
    using base_type = igeneric_pool<sizeof(element)>;

public:
    using size_type = typename base_type::size_type; //!< Size type alias.

    /**
     * @brief Default constructor.
     */
    generic_pool() :
        base_type(reinterpret_cast<char*>(&_buffer[0]), MaxSize)
    {
    }

    /**
     * @brief Indicates if the given value belongs to the generic_pool or not.
     */
    template<typename Type>
    [[nodiscard]] bool contains(const Type& value) const
    {
        return base_type::_contains(value);
    }

    /**
     * @brief Constructs a value inside of the generic_pool.
     * @tparam Type Type of the value to construct.
     * @tparam Args Type of the arguments of the value to construct.
     * @param args Parameters of the value to construct.
     * @return Reference to the new value.
     */
    template<typename Type, typename... Args>
    [[nodiscard]] Type& create(Args&&... args)
    {
        auto result = reinterpret_cast<Type*>(base_type::_allocate());
        ::new(result) Type(forward<Args>(args)...);
        return *result;
    }

    /**
     * @brief Destroys the given value, previously allocated with the create method.
     */
    template<typename Type>
    void destroy(Type& value)
    {
        BN_ASSERT(contains(value), "Pool does not contain this value");

        value.~Type();
        base_type::_free(reinterpret_cast<char*>(&value));
    }

private:
    alignas(element) char _buffer[sizeof(element) * MaxSize];
};

}

#endif
