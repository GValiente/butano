/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_UNORDERED_SET_FWD_H
#define BN_UNORDERED_SET_FWD_H

/**
 * @file
 * bn::iunordered_set and bn::unordered_set declaration header file.
 *
 * @ingroup unordered_set
 */

#include "bn_functional.h"

namespace bn
{
    /**
     * @brief Base class of unordered_set.
     *
     * Can be used as a reference type for all unordered_set containers containing a specific type.
     *
     * @tparam Key Element type.
     * @tparam KeyHash Functor used to calculate the hash of a given key.
     * @tparam KeyEqual Functor used for all key comparisons.
     *
     * @ingroup unordered_set
     */
    template<typename Key, typename KeyHash = hash<Key>, typename KeyEqual = equal_to<Key>>
    class iunordered_set;

    /**
     * @brief Unordered set implementation that uses a fixed size buffer.
     *
     * @tparam Key Element type.
     * @tparam MaxSize Maximum number of elements that can be stored.
     * @tparam KeyHash Functor used to calculate the hash of a given key.
     * @tparam KeyEqual Functor used for all key comparisons.
     *
     * @ingroup unordered_set
     */
    template<typename Key, int MaxSize, typename KeyHash = hash<Key>, typename KeyEqual = equal_to<Key>>
    class unordered_set;
}

#endif
