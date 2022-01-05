/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_UTILITY_H
#define BN_UTILITY_H

/**
 * @file
 * Standard library utility aliases header file.
 *
 * @ingroup utility
 */

#include <utility>
#include "bn_compare.h"
#include "bn_type_traits.h"

namespace bn
{
    using std::move;

    using std::forward;

    using std::swap;

    using std::index_sequence;
    using std::make_index_sequence;


    /**
     * @brief Provides a way to store two heterogeneous objects as a single unit.
     *
     * @tparam Type1 First element type.
     * @tparam Type2 Second element type.
     *
     * @ingroup utility
     */
    template<typename Type1, typename Type2>
    class pair
    {

    public:
        using first_type = Type1; //!< First element type alias.
        using second_type = Type2; //!< Second element type alias.

        first_type first; //!< First element.
        second_type second; //!< Second element.

        /**
         * @brief Default constructor.
         */
        constexpr pair() = default;

        /**
         * @brief Initializes first with x and second with y.
         */
        constexpr pair(const first_type& x, const second_type& y) :
            first(x),
            second(y)
        {
        }

        /**
         * @brief Initializes first with forward<OtherType1>(x) and second with forward<OtherType2>(y).
         */
        template<typename OtherType1, typename OtherType2>
        constexpr pair(OtherType1&& x, OtherType2&& y) :
            first(forward<OtherType1>(x)),
            second(forward<OtherType2>(y))
        {
        }

        /**
         * @brief Default copy constructor.
         */
        constexpr pair(const pair& other) = default;

        /**
         * @brief Initializes first with other.first and second with other.second.
         */
        template<typename OtherType1, typename OtherType2>
        constexpr pair(const pair<OtherType1, OtherType2>& other) :
            first(other.first),
            second(other.second)
        {
        }

        /**
         * @brief Default move constructor.
         */
        constexpr pair(pair&& other) noexcept = default;

        /**
         * @brief Initializes first with move(other.first) and second with move(other.second).
         */
        template<typename OtherType1, typename OtherType2>
        constexpr pair(pair<OtherType1, OtherType2>&& other) :
            first(move(other.first)),
            second(move(other.second))
        {
        }

        /**
         * @brief Default copy assignment operator.
         */
        constexpr pair& operator=(const pair& other) = default;

        /**
         * @brief Assigns other.first to first and other.second to second.
         */
        template<typename OtherType1, typename OtherType2>
        constexpr pair& operator=(const pair<OtherType1, OtherType2>& other)
        {
            first = other.first;
            second = other.second;
            return *this;
        }

        /**
         * @brief Default move assignment operator.
         */
        constexpr pair& operator=(pair&& other) noexcept = default;

        /**
         * @brief Assigns move(other.first) to first and move(other.second) to second.
         */
        template<typename OtherType1, typename OtherType2>
        constexpr pair& operator=(pair<OtherType1, OtherType2>&& other)
        {
            first = move(other.first);
            second = move(other.second);
            return *this;
        }

        /**
         * @brief Exchanges the contents of this pair with those of the other one.
         * @param other pair to exchange the contents with.
         */
        constexpr void swap(pair& other)
        {
            bn::swap(first, other.first);
            bn::swap(second, other.second);
        }

        /**
         * @brief Exchanges the contents of a pair with those of another one.
         * @param a First pair to exchange the contents with.
         * @param b Second pair to exchange the contents with.
         */
        constexpr friend void swap(pair& a, pair& b)
        {
            a.swap(b);
        }

        /**
         * @brief Default three-way comparison operator.
         */
        [[nodiscard]] constexpr friend auto operator<=>(const pair& a, const pair& b) = default;
    };


    /**
     * @brief Creates a pair deducing its types from the types of the arguments.
     * @param x First element to construct the pair from.
     * @param y Second element to construct the pair from.
     * @return pair containing the given values.
     *
     * @ingroup utility
     */
    template<typename Type1, typename Type2>
    constexpr pair<typename decay<Type1>::type, typename decay<Type2>::type> make_pair(Type1&& x, Type2&& y)
    {
        return pair<typename decay<Type1>::type, typename decay<Type2>::type>(forward<Type1>(x), forward<Type2>(y));
    }
}

#endif
