/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
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

/**
 * @brief Base class of igeneric_pool.
 *
 * @ingroup pool
 */
class igeneric_pool_base
{

public:
    igeneric_pool_base(const igeneric_pool_base&) = delete;

    igeneric_pool_base& operator=(const igeneric_pool_base&) = delete;

    /**
     * @brief Destructor.
     *
     * It doesn't destroy its elements, they must be destroyed manually.
     */
    ~igeneric_pool_base() noexcept;

protected:
    /// @cond DO_NOT_DOCUMENT

    using size_type = int;

    char* _buffer;
    char* _next_ptr;
    size_type _max_size;
    size_type _allocated_items_count = 0;
    size_type _initialised_items_count = 0;

    igeneric_pool_base(char* buffer, size_type max_size);

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

    [[nodiscard]] char* _allocate();

    void _free(char* ptr);

    void _mark_as_empty()
    {
        _allocated_items_count = 0;
    }

    /// @endcond
};


template<int MaxElementSize>
class igeneric_pool : public igeneric_pool_base
{

public:
    using size_type = igeneric_pool_base::size_type; //!< Size type alias.

    /**
     * @brief Returns the current elements count.
     */
    [[nodiscard]] size_type size() const
    {
        return igeneric_pool_base::size();
    }

    /**
     * @brief Returns the maximum possible elements count.
     */
    [[nodiscard]] size_type max_size() const
    {
        return igeneric_pool_base::max_size();
    }

    /**
     * @brief Returns the remaining element capacity.
     */
    [[nodiscard]] size_type available() const
    {
        return igeneric_pool_base::available();
    }

    /**
     * @brief Indicates if it doesn't contain any element.
     */
    [[nodiscard]] bool empty() const
    {
        return igeneric_pool_base::empty();
    }

    /**
     * @brief Indicates if it can't contain any more elements.
     */
    [[nodiscard]] bool full() const
    {
        return igeneric_pool_base::full();
    }

    /**
     * @brief Indicates if the given value belongs to the igeneric_pool or not.
     */
    template<typename Type>
    [[nodiscard]] bool contains(const Type& value) const
    {
        static_assert(sizeof(Type) <= MaxElementSize);

        return _contains_ptr(reinterpret_cast<const char*>(&value));
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

        auto result = reinterpret_cast<Type*>(_allocate());
        new(result) Type(forward<Args>(args)...);
        return *result;
    }

    /**
     * @brief Destroys the given value, previously allocated with the create method.
     */
    template<typename Type>
    void destroy(Type& value)
    {
        static_assert(sizeof(Type) <= MaxElementSize);

        BN_BASIC_ASSERT(contains(value), "Pool does not contain this value");

        value.~Type();
        _free(reinterpret_cast<char*>(&value));
    }

protected:
    /// @cond DO_NOT_DOCUMENT

    igeneric_pool(char* buffer, size_type max_size) :
        igeneric_pool_base(buffer, max_size)
    {
    }

    [[nodiscard]] bool _contains_ptr(const char* ptr) const
    {
        ptrdiff_t index = ptr - _buffer;
        return index >= 0 && index <= (MaxElementSize * _max_size) - MaxElementSize;
    }

    [[nodiscard]] char* _allocate()
    {
        size_type initialised_count = _initialised_items_count;

        if(initialised_count < _max_size)
        {
            char* ptr = _buffer + (_initialised_items_count * MaxElementSize);
            char* next_ptr = ptr + MaxElementSize;
            *reinterpret_cast<char**>(ptr) = next_ptr;
            _initialised_items_count = initialised_count + 1;
        }

        return igeneric_pool_base::_allocate();
    }

    /// @endcond
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
        base_type(reinterpret_cast<char*>(&_data_buffer[0]), MaxSize)
    {
    }

private:
    alignas(element) char _data_buffer[sizeof(element) * MaxSize];
};

}

#endif
