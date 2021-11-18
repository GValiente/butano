/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
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
     * @brief Deletes the object pointed by the given pointer.
     */
    void operator()(Type* ptr) const noexcept
    {
        delete ptr;
    }
};

/**
 * @brief Smart pointer that owns and manages another object through a pointer
 * and disposes of that object when the unique_ptr goes out of scope.
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
