/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_STRING_VIEW_H
#define BN_STRING_VIEW_H

/**
 * @file
 * bn::string_view header file.
 *
 * @ingroup string
 */

#include "bn_assert.h"
#include "bn_utility.h"
#include "bn_iterator.h"
#include "bn_algorithm.h"
#include "bn_functional.h"
#include "bn_istring_base.h"

namespace bn
{

/**
 * @brief Refers to a constant contiguous sequence of char values.
 *
 * It doesn't store the char data like a string does.
 *
 * @ingroup string
 */
class string_view
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
    using reverse_iterator = bn::reverse_iterator<iterator>; //!< Reverse iterator alias.
    using const_reverse_iterator = bn::reverse_iterator<const_iterator>; //!< Const reverse iterator alias.

    /**
     * @brief Default constructor.
     */
    constexpr string_view() = default;

    /**
     * @brief Constructor.
     * @param string Base string to reference.
     */
    constexpr string_view(const istring_base& string) :
        _begin(string.data()),
        _end(string.data() + string.size())
    {
    }

    /**
     * @brief Constructor.
     * @param char_array_ptr Pointer to null-terminated characters array.
     */
    constexpr string_view(const_pointer char_array_ptr) :
        _begin(char_array_ptr),
        _end(char_array_ptr)
    {
        if(char_array_ptr)
        {
            while(*char_array_ptr++)
            {
                ++_end;
            }
        }
    }

    /**
     * @brief Constructor.
     * @param char_array_ptr Pointer to characters array.
     * @param char_array_size Characters count of the characters array.
     */
    constexpr string_view(const_pointer char_array_ptr, size_type char_array_size) :
        _begin(char_array_ptr),
        _end(char_array_ptr + char_array_size)
    {
        BN_ASSERT(char_array_size >= 0, "Invalid char array size: ", char_array_size);
    }

    /**
     * @brief Returns a const reference to the first character.
     */
    [[nodiscard]] constexpr const_reference front() const
    {
        BN_BASIC_ASSERT(! empty(), "String view is empty");

        return *_begin;
    }

    /**
     * @brief Returns a const reference to the last character.
     */
    [[nodiscard]] constexpr const_reference back() const
    {
        BN_BASIC_ASSERT(! empty(), "String view is empty");

        return *(_end - 1);
    }

    /**
     * @brief Returns a const pointer to the first character of the internal storage.
     */
    [[nodiscard]] constexpr const_pointer data() const
    {
        return _begin;
    }

    /**
     * @brief Returns a const iterator to the beginning of the string_view.
     */
    [[nodiscard]] constexpr const_iterator begin() const
    {
        return _begin;
    }

    /**
     * @brief Returns a const iterator to the beginning of the string_view.
     */
    [[nodiscard]] constexpr const_iterator cbegin() const
    {
        return _begin;
    }

    /**
     * @brief Returns a const iterator to the end of the string_view.
     */
    [[nodiscard]] constexpr const_iterator end() const
    {
        return _end;
    }

    /**
     * @brief Returns a const iterator to the end of the string_view.
     */
    [[nodiscard]] constexpr const_iterator cend() const
    {
        return _end;
    }

    /**
     * @brief Returns a const reverse iterator to the end of the string_view.
     */
    [[nodiscard]] constexpr const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(_end);
    }

    /**
     * @brief Returns a const reverse iterator to the end of the string_view.
     */
    [[nodiscard]] constexpr const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(_end);
    }

    /**
     * @brief Returns a const reverse iterator to the beginning of the string_view.
     */
    [[nodiscard]] constexpr const_reverse_iterator rend() const
    {
        return const_reverse_iterator(_begin);
    }

    /**
     * @brief Returns a const reverse iterator to the beginning of the string_view.
     */
    [[nodiscard]] constexpr const_reverse_iterator crend() const
    {
        return const_reverse_iterator(_begin);
    }

    /**
     * @brief Indicates if it doesn't contain any character.
     */
    [[nodiscard]] constexpr bool empty() const
    {
        return _begin == _end;
    }

    /**
     * @brief Returns the number of characters in the string_view.
     */
    [[nodiscard]] constexpr size_type size() const
    {
        return _end - _begin;
    }

    /**
     * @brief Returns the number of characters in the string_view.
     */
    [[nodiscard]] constexpr size_type length() const
    {
        return size();
    }

    /**
     * @brief Returns a const reference to the character at the specified location.
     * @param position Character's position index.
     * @return Const reference to the character at the specified location.
     */
    [[nodiscard]] constexpr const_reference operator[](size_type position) const
    {
        BN_ASSERT(position >= 0 && position < size(), "Invalid position: ", position, " - ", size());

        return _begin[position];
    }

    /**
     * @brief Returns a const reference to the character at the specified location.
     * @param position Character's position index.
     * @return Const reference to the character at the specified location.
     */
    [[nodiscard]] constexpr const_reference at(size_type position) const
    {
        BN_ASSERT(position >= 0 && position < size(), "Invalid position: ", position, " - ", size());

        return _begin[position];
    }

    /**
     * @brief Returns a copy of this.
     */
    [[nodiscard]] constexpr string_view substr() const
    {
        return *this;
    }

    /**
     * @brief Returns a string_view of the substring [position, size() - position).
     */
    [[nodiscard]] constexpr string_view substr(size_type position) const
    {
        BN_ASSERT(position >= 0, "Invalid position: ", position);

        size_type sz = size();
        string_view view;

        if(position < sz)
        {
            view = string_view(_begin + position, sz - position);
        }

        return view;
    }

    /**
     * @brief Returns a string_view of the substring [position, position + count).
     */
    [[nodiscard]] constexpr string_view substr(size_type position, size_type count) const
    {
        BN_ASSERT(count >= 0, "Invalid count: ", count);
        BN_ASSERT(position >= 0, "Invalid position: ", position);

        size_type sz = size();
        string_view view;

        if(position < sz)
        {
            size_type n = min(count, sz - position);
            view = string_view(_begin + position, n);
        }

        return view;
    }

    /**
     * @brief Shrinks the string_view by moving its start forward by n characters.
     */
    constexpr void remove_prefix(size_type n)
    {
        BN_ASSERT(n <= size(), "Invalid n: ", n, " - ", size());

        _begin += n;
    }

    /**
     * @brief Shrinks the string_view by moving its end backward by n characters.
     */
    constexpr void remove_suffix(size_type n)
    {
        BN_ASSERT(n <= size(), "Invalid n: ", n, " - ", size());

        _end -= n;
    }

    /**
     * @brief Checks if the referenced string begins with the given prefix.
     * @param value Single character.
     * @return `true` if the referenced string begins with the given prefix; `false` otherwise.
     */
    [[nodiscard]] constexpr bool starts_with(value_type value) const
    {
        return ! empty() && *_begin == value;
    }

    /**
     * @brief Checks if the referenced string begins with the given prefix.
     * @param other Another string_view.
     * @return `true` if the referenced string begins with the given prefix; `false` otherwise.
     */
    [[nodiscard]] constexpr bool starts_with(const string_view& other) const
    {
        size_type other_size = other.size();

        if(size() < other_size)
        {
            return false;
        }

        const_pointer this_data = data();
        const_pointer other_data = other.data();

        if(this_data == other_data)
        {
            return true;
        }

        return equal(this_data, this_data + other_size, other_data);
    }

    /**
     * @brief Checks if the referenced string begins with the given prefix.
     * @param char_array_ptr Pointer to null-terminated characters array.
     * @return `true` if the referenced string begins with the given prefix; `false` otherwise.
     */
    [[nodiscard]] constexpr bool starts_with(const_pointer char_array_ptr) const
    {
        if(! char_array_ptr)
        {
            return true;
        }

        const_pointer this_char_array_ptr = _begin;

        if(this_char_array_ptr == char_array_ptr)
        {
            return true;
        }

        for(size_type index = 0, limit = size(); index < limit; ++index)
        {
            value_type character = *char_array_ptr;

            if(character == 0)
            {
                return true;
            }

            if(character != *this_char_array_ptr)
            {
                return false;
            }

            ++this_char_array_ptr;
            ++char_array_ptr;
        }

        return *char_array_ptr == 0;
    }

    /**
     * @brief Checks if the referenced string ends with the given prefix.
     * @param value Single character.
     * @return `true` if the referenced string ends with the given prefix; `false` otherwise.
     */
    [[nodiscard]] constexpr bool ends_with(value_type value) const
    {
        return ! empty() && *(_end - 1) == value;
    }

    /**
     * @brief Checks if the referenced string ends with the given prefix.
     * @param other Another string_view.
     * @return `true` if the referenced string ends with the given prefix; `false` otherwise.
     */
    [[nodiscard]] constexpr bool ends_with(const string_view& other) const
    {
        size_type this_size = size();
        size_type other_size = other.size();

        if(this_size < other_size)
        {
            return false;
        }

        return equal(_begin + this_size - other_size, _end, other._begin);
    }

    /**
     * @brief Exchanges the contents of this string_view with those of the other one.
     * @param other string_view to exchange the contents with.
     */
    constexpr void swap(string_view& other)
    {
        bn::swap(_begin, other._begin);
        bn::swap(_end, other._end);
    }

    /**
     * @brief Exchanges the contents of a string_view with those of another one.
     * @param a First string_view to exchange the contents with.
     * @param b Second string_view to exchange the contents with.
     */
    constexpr friend void swap(string_view& a, string_view& b)
    {
        a.swap(b);
    }

    /**
     * @brief Equal operator.
     * @param a First string_view to compare.
     * @param b Second string_view to compare.
     * @return `true` if the first string_view is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator==(const string_view& a, const string_view& b)
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
     * @brief Not equal operator.
     * @param a First string_view to compare.
     * @param b Second string_view to compare.
     * @return `true` if the first string_view is not equal to the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator!=(const string_view& a, const string_view& b)
    {
        return ! (a == b);
    }

    /**
     * @brief Less than operator.
     * @param a First string_view to compare.
     * @param b Second string_view to compare.
     * @return `true` if the first string_view is lexicographically less than the second one,
     * otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator<(const string_view& a, const string_view& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    /**
     * @brief Greater than operator.
     * @param a First string_view to compare.
     * @param b Second string_view to compare.
     * @return `true` if the first string_view is lexicographically greater than the second one,
     * otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator>(const string_view& a, const string_view& b)
    {
        return b < a;
    }

    /**
     * @brief Less than or equal operator.
     * @param a First string_view to compare.
     * @param b Second string_view to compare.
     * @return `true` if the first string_view is lexicographically less than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator<=(const string_view& a, const string_view& b)
    {
        return ! (a > b);
    }

    /**
     * @brief Greater than or equal operator.
     * @param a First string_view to compare.
     * @param b Second string_view to compare.
     * @return `true` if the first string_view is lexicographically greater than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator>=(const string_view& a, const string_view& b)
    {
        return ! (a < b);
    }

private:
    const_pointer _begin = nullptr;
    const_pointer _end = nullptr;
};


/**
 * @brief Hash support for string_view.
 *
 * @ingroup string
 * @ingroup functional
 */
template<>
struct hash<string_view>
{
    /**
     * @brief Returns the hash of the given string_view.
     */
    [[nodiscard]] constexpr unsigned operator()(const string_view& value) const
    {
        return array_hash(value.data(), value.size());
    }
};

}

#endif
