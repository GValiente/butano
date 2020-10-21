/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_TYPE_ID_H
#define BTN_TYPE_ID_H

#include "btn_common.h"

namespace btn
{

/**
 * @brief Unique type ID without RTTI.
 *
 * See https://codereview.stackexchange.com/questions/48594/unique-type-id-no-rtti
 *
 * @ingroup other
 */
class type_id_t
{

public:
    using id_type = type_id_t(); //!< Internal id type alias.

    template<typename Type>
    friend type_id_t type_id();

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
     * @brief Equal operator.
     * @param a First element to compare.
     * @param b Second element to compare.
     * @return <b>true</b> if the first element is equal to the second one, otherwise <b>false</b>.
     */
    [[nodiscard]] constexpr friend bool operator==(type_id_t a, type_id_t b) = default;

private:
    id_type* _id = nullptr;

    constexpr type_id_t(id_type* id) :
        _id(id)
    {
    }
};


/**
 * @brief Returns the type_id_t of the specified type.
 */
template<typename Type>
[[nodiscard]] type_id_t type_id()
{
    return &type_id<Type>;
}

}

#endif
