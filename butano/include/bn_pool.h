/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
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

#ifndef BN_POOL_H
#define BN_POOL_H

/**
 * @file
 * bn::ipool and bn::pool implementation header file.
 *
 * @ingroup pool
 */

#include "bn_pool_fwd.h"
#include "bn_type_traits.h"
#include "bn_generic_pool.h"

namespace bn
{

template<typename Type>
class ipool : public igeneric_pool<sizeof(generic_pool_element<sizeof(Type)>)>
{

protected:
    /// @cond DO_NOT_DOCUMENT

    using element = generic_pool_element<sizeof(Type)>;

    /// @endcond

private:
    using base_type = igeneric_pool<sizeof(element)>;

public:
    using size_type = typename base_type::size_type; //!< Size type alias.

    /**
     * @brief Destructor.
     */
    ~ipool() noexcept = default;

    /**
     * @brief Destructor.
     */
    ~ipool() noexcept
    requires(is_trivially_destructible_v<Type>)
    {
        this->_mark_as_empty();
    }

    /**
     * @brief Indicates if the given value belongs to the ipool or not.
     */
    [[nodiscard]] bool contains(const Type& value) const
    {
        auto ptr = reinterpret_cast<const char*>(&value);
        return base_type::_contains_ptr(ptr);
    }

    /**
     * @brief Constructs a value inside of the ipool.
     * @param args Parameters of the value to construct.
     * @return Reference to the new value.
     */
    template<typename... Args>
    [[nodiscard]] Type& create(Args&&... args)
    {
        BN_BASIC_ASSERT(! base_type::full(), "Pool is full");

        auto result = reinterpret_cast<Type*>(base_type::_allocate());
        new(result) Type(forward<Args>(args)...);
        return *result;
    }

    /**
     * @brief Destroys the given value, previously allocated with the create method.
     */
    void destroy(Type& value)
    {
        BN_BASIC_ASSERT(contains(value), "Pool does not contain this value");

        value.~Type();
        base_type::_free(reinterpret_cast<char*>(&value));
    }

protected:
    /// @cond DO_NOT_DOCUMENT

    ipool(char* buffer, size_type max_size) :
        base_type(reinterpret_cast<char*>(&buffer[0]), max_size)
    {
    }

    /// @endcond
};


template<typename Type, int MaxSize>
class pool : public ipool<Type>
{

private:
    using base_type = ipool<Type>;

public:
    /**
     * @brief Default constructor.
     */
    pool() :
        base_type(reinterpret_cast<char*>(&_data_buffer[0]), MaxSize)
    {
    }

private:
    alignas(typename base_type::element) char _data_buffer[sizeof(typename base_type::element) * MaxSize];
};

}

#endif
