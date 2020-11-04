/*
 * Copyright (c) 2014 jwellbelove Embedded Template Library https://www.etlcpp.com
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_POOL_H
#define BTN_POOL_H

#include "btn_pool_fwd.h"
#include "btn_generic_pool.h"

namespace btn
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
     * @brief Indicates if the given value belongs to the ipool or not.
     */
    [[nodiscard]] bool contains(const Type& value) const
    {
        return base_type::_contains(value);
    }

    /**
     * @brief Constructs a value inside of the ipool.
     * @param args Parameters of the value to construct.
     * @return Reference to the new value.
     */
    template<typename... Args>
    [[nodiscard]] Type& create(Args&&... args)
    {
        BTN_ASSERT(! base_type::full(), "Pool is full");

        auto result = reinterpret_cast<Type*>(base_type::_allocate());
        ::new(result) Type(forward<Args>(args)...);
        return *result;
    }

    /**
     * @brief Destroys the given value, previously allocated with the create method.
     */
    void destroy(Type& value)
    {
        BTN_ASSERT(contains(value), "Pool does not contain this value");

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
        base_type(reinterpret_cast<char*>(&_buffer[0]), MaxSize)
    {
    }

private:
    alignas(Type) char _buffer[sizeof(typename base_type::element) * MaxSize];
};

}

#endif
