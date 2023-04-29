/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

/*
 * The MIT License
 *
 * Copyright (c) 2015 Manuel Sánchez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef BN_TYPE_ID_H
#define BN_TYPE_ID_H

/**
 * @file
 * bn::type_id_t header file.
 *
 * @ingroup other
 */

#include "bn_functional.h"

namespace bn
{

/**
 * @brief Unique type ID without RTTI.
 *
 * See https://github.com/Manu343726/ctti/blob/master/include/ctti/type_id.hpp
 *
 * @ingroup other
 */
class type_id_t
{

public:
    using id_type = type_id_t(); //!< Internal id type alias.

    template<typename Type>
    friend constexpr type_id_t type_id();

    /**
     * @brief Default constructor.
     */
    constexpr type_id_t() = default;

    /**
     * @brief Returns the internal id.
     */
    [[nodiscard]] constexpr id_type* internal_id() const
    {
        return _id;
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(type_id_t a, type_id_t b) = default;

private:
    id_type* _id = nullptr;

    constexpr explicit type_id_t(id_type* id) :
        _id(id)
    {
    }
};


/**
 * @brief Returns the type_id_t of the specified type.
 */
template<typename Type>
[[nodiscard]] constexpr type_id_t type_id()
{
    return type_id_t(&type_id<Type>);
}


/**
 * @brief Hash support for type_id_t.
 *
 * @ingroup other
 * @ingroup functional
 */
template<>
struct hash<type_id_t>
{
    /**
     * @brief Returns the hash of the given type_id_t.
     */
    [[nodiscard]] constexpr unsigned operator()(type_id_t value) const
    {
        return make_hash(value.internal_id());
    }
};

}

#endif
