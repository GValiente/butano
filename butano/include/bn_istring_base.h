/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_ISTRING_BASE_H
#define BN_ISTRING_BASE_H

/**
 * @file
 * bn::istring_base header file.
 *
 * @ingroup string
 */

#include "bn_algorithm.h"
#include "bn_functional.h"
#include "bn_string_fwd.h"

namespace bn
{

/**
 * @brief Base class of bn::istring.
 *
 * It avoids circular references when including bn::istring header.
 *
 * @ingroup string
 */
class istring_base
{

public:
    using value_type = char; //!< Value type alias.
    using size_type = int; //!< Size type alias.
    using difference_type = int; //!< Difference type alias.
    using reference = char&; //!< Reference alias.
    using const_reference = const char&; //!< Const reference alias.
    using pointer = char*; //!< Pointer alias.
    using const_pointer = const char*; //!< Const pointer alias.
    using iterator = char*; //!< Iterator alias.
    using const_iterator = const char*; //!< Const iterator alias.

    /**
     * @brief Constructor.
     * @param char_array_ref Non empty const characters array.
     *
     * The characters are not copied but referenced, so they should outlive the istring_base
     * to avoid dangling references.
     */
    template<int MaxSize>
    constexpr istring_base(char (&char_array_ref)[MaxSize]) :
        _data(char_array_ref),
        _max_size(MaxSize - 1)
    {
        static_assert(MaxSize > 1);

        _data[0] = 0;
    }

    /**
     * @brief Returns the number of characters in the istring_base.
     */
    [[nodiscard]] constexpr size_type size() const
    {
        return _size;
    }

    /**
     * @brief Returns the number of characters in the istring_base.
     */
    [[nodiscard]] constexpr size_type length() const
    {
        return _size;
    }

    /**
     * @brief Returns the maximum possible number of characters in the istring_base.
     */
    [[nodiscard]] constexpr size_type max_size() const
    {
        return _max_size;
    }

    /**
     * @brief Returns the remaining characters capacity.
     */
    [[nodiscard]] constexpr size_type available() const
    {
        return _max_size - _size;
    }

    /**
     * @brief Indicates if it doesn't contain any character.
     */
    [[nodiscard]] constexpr bool empty() const
    {
        return _size == 0;
    }

    /**
     * @brief Indicates if it can't contain any more characters.
     */
    [[nodiscard]] constexpr bool full() const
    {
        return _size == _max_size;
    }

    /**
     * @brief Returns a const pointer to the beginning of the characters data.
     */
    [[nodiscard]] constexpr const_pointer data() const
    {
        return _data;
    }

    /**
     * @brief Returns a const pointer to the beginning of the characters data.
     */
    [[nodiscard]] constexpr pointer data()
    {
        return _data;
    }

    /**
     * @brief Returns a const pointer to the beginning of the characters data (it is null-terminated).
     */
    [[nodiscard]] constexpr const_pointer c_str() const
    {
        return _data;
    }

    /**
     * @brief Returns a const iterator to the beginning of the characters data.
     */
    [[nodiscard]] constexpr const_iterator begin() const
    {
        return _data;
    }

    /**
     * @brief Returns an iterator to the beginning of the characters data.
     */
    [[nodiscard]] constexpr iterator begin()
    {
        return _data;
    }

    /**
     * @brief Returns a const iterator to the end of the characters data.
     */
    [[nodiscard]] constexpr iterator end()
    {
        return _data + _size;
    }

    /**
     * @brief Returns an iterator to the end of the characters data.
     */
    [[nodiscard]] constexpr const_iterator end() const
    {
        return _data + _size;
    }

    /**
     * @brief Returns a const iterator to the beginning of the characters data.
     */
    [[nodiscard]] constexpr const_iterator cbegin() const
    {
        return _data;
    }

    /**
     * @brief Returns a const iterator to the end of the characters data.
     */
    [[nodiscard]] constexpr const_iterator cend() const
    {
        return _data + _size;
    }

    /**
     * @brief Removes all characters.
     */
    constexpr void clear()
    {
        _data[0] = 0;
        _size = 0;
    }

    /**
     * @brief Equal operator.
     * @param a First istring_base to compare.
     * @param b Second istring_base to compare.
     * @return `true` if the first istring_base is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator==(const istring_base& a, const istring_base& b)
    {
        size_type a_size = a.size();

        if(a_size != b.size())
        {
            return false;
        }

        const_pointer a_data = a.data();
        const_pointer b_data = b.data();

        if(a_data == b_data)
        {
            return true;
        }

        return equal(a_data, a_data + a_size, b_data);
    }

    /**
     * @brief Less than operator.
     * @param a First istring_base to compare.
     * @param b Second istring_base to compare.
     * @return `true` if the first istring_base is lexicographically less than the second one,
     * otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator<(const istring_base& a, const istring_base& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    /**
     * @brief Greater than operator.
     * @param a First istring_base to compare.
     * @param b Second istring_base to compare.
     * @return `true` if the first istring_base is lexicographically greater than the second one,
     * otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator>(const istring_base& a, const istring_base& b)
    {
        return b < a;
    }

    /**
     * @brief Less than or equal operator.
     * @param a First istring_base to compare.
     * @param b Second istring_base to compare.
     * @return `true` if the first istring_base is lexicographically less than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator<=(const istring_base& a, const istring_base& b)
    {
        return ! (a > b);
    }

    /**
     * @brief Greater than or equal operator.
     * @param a First istring_base to compare.
     * @param b Second istring_base to compare.
     * @return `true` if the first istring_base is lexicographically greater than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator>=(const istring_base& a, const istring_base& b)
    {
        return ! (a < b);
    }

protected:
    /// @cond DO_NOT_DOCUMENT

    pointer _data;
    size_type _size = 0;
    size_type _max_size;

    constexpr istring_base(pointer data, size_type size, size_type max_size) :
        _data(data),
        _size(size),
        _max_size(max_size)
    {
    }

    /// @endcond
};


/**
 * @brief Hash support for istring_base.
 *
 * @ingroup string
 * @ingroup functional
 */
template<>
struct hash<istring_base>
{
    /**
     * @brief Returns the hash of the given istring_base.
     */
    [[nodiscard]] constexpr unsigned operator()(const istring_base& value) const
    {
        return array_hash(value.data(), value.size());
    }
};

}

#endif
