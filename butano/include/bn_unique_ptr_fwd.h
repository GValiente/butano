/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_UNIQUE_PTR_FWD_H
#define BN_UNIQUE_PTR_FWD_H

/**
 * @file
 * bn::unique_ptr declaration header file.
 *
 * @ingroup unique_ptr
 */

#include "bn_common.h"

namespace bn
{

/**
 * @brief Default deleter.
 *
 * @tparam Type Type of the object to delete.
 *
 * @ingroup memory
 */
template<typename Type>
struct default_delete
{
    /**
     * @brief Default constructor.
     */
    constexpr default_delete() = default;

    /**
     * @brief Copy constructor.
     */
    template<typename OtherType>
    default_delete(const default_delete<OtherType>&) noexcept
    {
    }

    /**
     * @brief Deletes the object pointed by the given pointer.
     */
    void operator()(Type* ptr) const noexcept
    {
        delete ptr;
    }
};

/**
 * @brief `std::unique_ptr` like container.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * Unlike most Butano containers, it uses the heap instead of the stack.
 *
 * @tparam Type Type of the managed object.
 * @tparam Deleter Type of the object which disposes the managed object when the unique_ptr goes out of scope.
 *
 * @ingroup unique_ptr
 */
template<typename Type, typename Deleter = default_delete<Type>>
class unique_ptr;

}

#endif
