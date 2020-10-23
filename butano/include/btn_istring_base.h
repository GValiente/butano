/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_ISTRING_BASE_H
#define BTN_ISTRING_BASE_H

#include "btn_string_fwd.h"

namespace btn
{

/**
 * @brief Base class of istring.
 *
 * It avoids circular references when including istring's header.
 *
 * @ingroup string
 */
class istring_base
{

public:
    using value_type = char; //!< Value type alias.
    using size_type = int; //!< Size type alias.
    using reference = char&; //!< Reference alias.
    using const_reference = const char&; //!< Const reference alias.
    using pointer = char*; //!< Pointer alias.
    using const_pointer = const char*; //!< Const pointer alias.
    using iterator = char*; //!< Iterator alias.
    using const_iterator = const char*; //!< Const iterator alias.

    /**
     * @brief Constructor.
     * @param char_array Non empty const characters array.
     */
    template<int MaxSize>
    constexpr istring_base(char (&char_array)[MaxSize]) :
        _data(char_array),
        _size(0),
        _max_size(MaxSize - 1)
    {
        static_assert(MaxSize > 1);

        _data[0] = 0;
    }

    /**
     * @brief Returns the number of characters in the string.
     */
    [[nodiscard]] constexpr size_type size() const
    {
        return _size;
    }

    /**
     * @brief Returns the number of characters in the string.
     */
    [[nodiscard]] constexpr size_type length() const
    {
        return _size;
    }

    /**
     * @brief Returns the maximum possible number of characters in the string.
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

protected:
    /// @cond DO_NOT_DOCUMENT

    pointer _data;
    size_type _size;
    size_type _max_size;

    constexpr istring_base(pointer data, size_type size, size_type max_size) :
        _data(data),
        _size(size),
        _max_size(max_size)
    {
    }

    /// @endcond
};

}

#endif
