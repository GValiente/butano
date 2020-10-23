/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_STRING_H
#define BTN_STRING_H

#include "btn_sstream.h"
#include "btn_string_view.h"

namespace btn
{

/**
 * @brief Base class of string.
 *
 * Can be used as a reference type for all string containers.
 *
 * @ingroup string
 */
class istring : public istring_base
{

public:
    using reverse_iterator = btn::reverse_iterator<iterator>; //!< Reverse iterator alias.
    using const_reverse_iterator = btn::reverse_iterator<const_iterator>; //!< Const reverse iterator alias.

public:
    /**
     * @brief Copy assignment operator.
     * @param other istring to copy.
     * @return Reference to this.
     */
    constexpr istring& operator=(const istring& other)
    {
        return assign(other);
    }

    /**
     * @brief Copy assignment operator.
     * @param view string_view to copy.
     * @return Reference to this.
     */
    constexpr istring& operator=(const string_view& view)
    {
        return assign(view);
    }

    /**
     * @brief Copy assignment operator.
     * @param char_array_ptr Pointer to null-terminated characters array.
     * @return Reference to this.
     */
    constexpr istring& operator=(const_pointer char_array_ptr)
    {
        return assign(char_array_ptr);
    }

    /**
     * @brief Returns a non-modifiable string_view into the entire string.
     *
     * It is not ensured that the resulting string_view does not outlive the string.
     */
    [[nodiscard]] constexpr operator string_view() const
    {
        return string_view(_data, _size);
    }

    /**
     * @brief Returns a const reverse iterator to the end of the characters data.
     */
    [[nodiscard]] constexpr const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }

    /**
     * @brief Returns a reverse iterator to the end of the characters data.
     */
    [[nodiscard]] constexpr reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    /**
     * @brief Returns a const reverse iterator to the beginning of the characters data.
     */
    [[nodiscard]] constexpr const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }

    /**
     * @brief Returns a reverse iterator to the beginning of the characters data.
     */
    [[nodiscard]] constexpr reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    /**
     * @brief Returns a const reverse iterator to the end of the characters data.
     */
    [[nodiscard]] constexpr const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(cend());
    }

    /**
     * @brief Returns a const reverse iterator to the beginning of the characters data.
     */
    [[nodiscard]] constexpr const_reverse_iterator crend() const
    {
        return const_reverse_iterator(cbegin());
    }

    /**
     * @brief Returns a const reference to the character stored at the specified index.
     */
    [[nodiscard]] constexpr const_reference operator[](size_type index) const
    {
        BTN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _data[index];
    }

    /**
     * @brief Returns a reference to the character stored at the specified index.
     */
    [[nodiscard]] constexpr reference operator[](size_type index)
    {
        BTN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _data[index];
    }

    /**
     * @brief Returns a const reference to the character stored at the specified index.
     */
    [[nodiscard]] constexpr const_reference at(size_type index) const
    {
        BTN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _data[index];
    }

    /**
     * @brief Returns a reference to the character stored at the specified index.
     */
    [[nodiscard]] constexpr reference at(size_type index)
    {
        BTN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _data[index];
    }

    /**
     * @brief Returns a const reference to the first character.
     */
    [[nodiscard]] constexpr const_reference front() const
    {
        BTN_ASSERT(! empty(), "String is empty");

        return _data[0];
    }

    /**
     * @brief Returns a reference to the first character.
     */
    [[nodiscard]] constexpr reference front()
    {
        BTN_ASSERT(! empty(), "String is empty");

        return _data[0];
    }

    /**
     * @brief Returns a const reference to the last character.
     */
    [[nodiscard]] constexpr const_reference back() const
    {
        BTN_ASSERT(! empty(), "String is empty");

        return _data[_size - 1];
    }

    /**
     * @brief Returns a reference to the last character.
     */
    [[nodiscard]] constexpr reference back()
    {
        BTN_ASSERT(! empty(), "String is empty");

        return _data[_size - 1];
    }

    /**
     * @brief Replaces the contents of the string.
     * @param other string replacement.
     * @return Reference to this.
     */
    constexpr istring& assign(const istring& other)
    {
        BTN_ASSERT(other._size <= _max_size, "Not enough space in string: ", other._size, " - ", _max_size);

        btn::copy(other.begin(), other.end(), begin());
        _size = other._size;
        _data[_size] = 0;
        return *this;
    }

    /**
     * @brief Replaces the contents of the string.
     * @param value Character replacement.
     * @return Reference to this.
     */
    constexpr istring& assign(value_type value)
    {
        clear();
        append(value);
        return *this;
    }

    /**
     * @brief Replaces the contents of the string.
     * @param view string_view replacement.
     * @return Reference to this.
     */
    constexpr istring& assign(const string_view& view)
    {
        clear();
        append(view);
        return *this;
    }

    /**
     * @brief Replaces the contents of the string.
     * @param char_array_ptr Pointer to null-terminated characters array replacement.
     * @return Reference to this.
     */
    constexpr istring& assign(const_pointer char_array_ptr)
    {
        clear();
        append(char_array_ptr);
        return *this;
    }

    /**
     * @brief Replaces the contents of the string with count copies of character value.
     * @param count New size.
     * @param value Character replacement.
     * @return Reference to this.
     */
    constexpr istring& assign(size_type count, value_type value)
    {
        clear();
        append(count, value);
        return *this;
    }

    /**
     * @brief Replaces the contents of the string with the characters in the range [first, last).
     * @param first First element of the range.
     * @param last Last element of the range.
     * @return Reference to this.
     */
    constexpr istring& assign(const_iterator first, const_iterator last)
    {
        clear();
        append(first, last);
        return *this;
    }

    /**
     * @brief Inserts a character at the end of the string.
     * @param value Character to insert.
     */
    constexpr void push_back(value_type value)
    {
        BTN_ASSERT(! full(), "String is full");

        pointer data = _data + _size;
        data[0] = value;
        data[1] = 0;
        ++_size;
    }

    /**
     * @brief Removes the last character of the string.
     */
    constexpr void pop_back()
    {
        BTN_ASSERT(! empty(), "String is empty");

        --_size;
        _data[_size] = 0;
    }

    /**
     * @brief Appends additional characters to the string.
     * @param other string to append.
     * @return Reference to this.
     */
    constexpr istring& append(const istring& other)
    {
        append(other.begin(), other.end());
        return *this;
    }

    /**
     * @brief Appends an additional character to the string.
     * @param value Character to append.
     * @return Reference to this.
     */
    constexpr istring& append(value_type value)
    {
        push_back(value);
        return *this;
    }

    /**
     * @brief Appends additional characters to the string.
     * @param view string_view to append.
     * @return Reference to this.
     */
    constexpr istring& append(const string_view& view)
    {
        append(view.begin(), view.end());
        return *this;
    }

    /**
     * @brief Appends additional characters to the string.
     * @param char_array_ptr Pointer to null-terminated characters array.
     * @return Reference to this.
     */
    constexpr istring& append(const_pointer char_array_ptr)
    {
        append(string_view(char_array_ptr));
        return *this;
    }

    /**
     * @brief Appends additional characters to the string.
     * @param char_array_ptr Pointer to characters array.
     * @param char_array_size Characters count of the characters array.
     * @return Reference to this.
     */
    constexpr istring& append(const_pointer char_array_ptr, size_type char_array_size)
    {
        append(string_view(char_array_ptr, char_array_size));
        return *this;
    }

    /**
     * @brief Appends additional characters to the string.
     * @param count Number of characters to append.
     * @param value Character to append.
     * @return Reference to this.
     */
    constexpr istring& append(size_type count, value_type value)
    {
        BTN_ASSERT(count >= 0, "Invalid count: ", count);
        BTN_ASSERT(_size + count <= _max_size, "Not enough space in string: ", _size + count, " - ", _max_size);

        iterator append_position = end();
        fill(append_position, append_position + count, value);
        _size += count;
        _data[_size] = 0;
        return *this;
    }

    /**
     * @brief Appends additional characters to the string.
     * @param first First element of the range to append.
     * @param last Last element of the range to append.
     * @return Reference to this.
     */
    constexpr istring& append(const_iterator first, const_iterator last)
    {
        size_type count = last - first;
        BTN_ASSERT(count >= 0, "Invalid range");
        BTN_ASSERT(_size + count <= _max_size, "Not enough space in string: ", _size + count, " - ", _max_size);

        iterator append_position = end();

        if(append_position <= first)
        {
            btn::copy(first, last, append_position);
        }
        else
        {
            copy_backward(first, last, append_position + count);
        }

        _size += count;
        _data[_size] = 0;
        return *this;
    }

    /**
     * @brief Erases a character.
     * @param position Iterator to the character to erase.
     * @return Iterator following the erased character.
     */
    constexpr iterator erase(const_iterator position)
    {
        BTN_ASSERT(! empty(), "String is empty");

        auto erase_position = const_cast<iterator>(position);
        btn::copy(erase_position + 1, end(), erase_position);
        --_size;
        _data[_size] = 0;
        return erase_position;
    }

    /**
     * @brief Erases a range of characters.
     *
     * The range includes all the characters between first and last, including the
     * character pointed by first, but not the one pointed by last.
     *
     * @param first Iterator to the first character to erase.
     * @param last Iterator to the last character to erase.
     * @return Iterator following the last erased character.
     */
    constexpr iterator erase(const_iterator first, const_iterator last)
    {
        size_type count = last - first;
        BTN_ASSERT(count >= 0, "Invalid range");
        BTN_ASSERT(count <= _size, "Invalid range: ", count, " - ", _size);

        auto erase_first = const_cast<iterator>(first);
        auto erase_last = const_cast<iterator>(last);
        btn::copy(erase_last, end(), erase_first);
        _size -= count;
        _data[_size] = 0;
        return erase_first;
    }

    /**
     * @brief Erases all characters that are equal to the specified one.
     * @param string string from which to erase.
     * @param value Character to erase.
     * @return Number of erased characters.
     */
    constexpr friend size_type erase(istring& string, value_type value)
    {
        size_type old_size = string.size();
        string.erase(remove(string.begin(), string.end(), value), string.end());
        return old_size - string.size();
    }

    /**
     * @brief Erases all characters that satisfy the specified predicate.
     * @param string string from which to erase.
     * @param pred Unary predicate which returns <b>true</b> if the character should be erased.
     * @return Number of erased characters.
     */
    template<class Pred>
    constexpr friend size_type erase_if(istring& string, const Pred& pred)
    {
        size_type old_size = string.size();
        string.erase(remove_if(string.begin(), string.end(), pred), string.end());
        return old_size - string.size();
    }

    /**
     * @brief Checks if the referenced string begins with the given prefix.
     * @param value Single character.
     * @return <b>true</b> if the referenced string begins with the given prefix, <b>false</b> otherwise.
     */
    [[nodiscard]] constexpr bool starts_with(value_type value) const
    {
        return ! empty() && _data[0] == value;
    }

    /**
     * @brief Checks if the referenced string begins with the given prefix.
     * @param other Another string_view.
     * @return <b>true</b> if the referenced string begins with the given prefix, <b>false</b> otherwise.
     */
    [[nodiscard]] constexpr bool starts_with(const string_view& other) const
    {
        size_type other_size = other.size();

        if(size() < other_size)
        {
            return false;
        }

        return equal(_data, _data + other_size, other.data());
    }

    /**
     * @brief Checks if the referenced string begins with the given prefix.
     * @param char_array_ptr Pointer to null-terminated characters array.
     * @return <b>true</b> if the referenced string begins with the given prefix, <b>false</b> otherwise.
     */
    [[nodiscard]] constexpr bool starts_with(const_pointer char_array_ptr) const
    {
        if(! char_array_ptr)
        {
            return true;
        }

        const_pointer this_char_array_ptr = _data;

        for(size_type index = 0, limit = size(); index < limit; ++index)
        {
            if(*this_char_array_ptr != *char_array_ptr)
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
     * @return <b>true</b> if the referenced string ends with the given prefix, <b>false</b> otherwise.
     */
    [[nodiscard]] constexpr bool ends_with(value_type value) const
    {
        return ! empty() && _data[_size - 1] == value;
    }

    /**
     * @brief Checks if the referenced string ends with the given prefix.
     * @param other Another string_view.
     * @return <b>true</b> if the referenced string ends with the given prefix, <b>false</b> otherwise.
     */
    [[nodiscard]] constexpr bool ends_with(const string_view& other) const
    {
        size_type this_size = size();
        size_type other_size = other.size();

        if(this_size < other_size)
        {
            return false;
        }

        return equal(_data + this_size - other_size, _data + this_size, other.data());
    }

    /**
     * @brief Exchanges the contents of this string with those of the other one.
     * @param other string to exchange the contents with.
     */
    constexpr void swap(istring& other)
    {
        if(_data != other._data)
        {
            BTN_ASSERT(_size <= other._max_size, "Invalid size: ", _size, " - ", other._max_size);
            BTN_ASSERT(_max_size <= other._size, "Invalid max size: ", _max_size, " - ", other._size);

            pointer min_data = nullptr;
            pointer max_data = nullptr;
            size_type min_size = 0;
            size_type max_size = 0;

            if(_size < other._size)
            {
                min_data = _data;
                max_data = other._data;
                min_size = _size;
                max_size = other._size;
            }
            else
            {
                min_data = other._data;
                max_data = _data;
                min_size = other._size;
                max_size = _size;
            }

            for(size_type index = 0; index < min_size; ++index)
            {
                btn::swap(min_data[index], max_data[index]);
            }

            for(size_type index = min_size; index < max_size; ++index)
            {
                min_data[index] = max_data[index];
            }

            btn::swap(_size, other._size);
            _data[_size] = 0;
            other._data[other._size] = 0;
        }
    }

    /**
     * @brief Exchanges the contents of a string with those of another one.
     * @param a First string to exchange the contents with.
     * @param b Second string to exchange the contents with.
     */
    constexpr friend void swap(istring& a, istring& b)
    {
        a.swap(b);
    }

    /**
     * @brief Appends additional characters to the string.
     * @param other string to append.
     * @return Reference to this.
     */
    constexpr istring& operator+=(const istring& other)
    {
        append(other);
        return *this;
    }

    /**
     * @brief Appends an additional character to the string.
     * @param value Character to append.
     * @return Reference to this.
     */
    constexpr istring& operator+=(value_type value)
    {
        append(value);
        return *this;
    }

    /**
     * @brief Appends additional characters to the string.
     * @param view string_view to append.
     * @return Reference to this.
     */
    constexpr istring& operator+=(const string_view& view)
    {
        append(view);
        return *this;
    }

    /**
     * @brief Appends additional characters to the string.
     * @param char_array_ptr Pointer to null-terminated characters array.
     * @return Reference to this.
     */
    constexpr istring& operator+=(const_pointer char_array_ptr)
    {
        append(char_array_ptr);
        return *this;
    }

    /**
     * @brief Equal operator.
     * @param a First string to compare.
     * @param b Second string to compare.
     * @return <b>true</b> if the first string is equal to the second one, otherwise <b>false</b>.
     */
    [[nodiscard]] constexpr friend bool operator==(const istring& a, const istring& b)
    {
        if(a._size != b._size)
        {
            return false;
        }

        return equal(a.begin(), a.end(), b.begin());
    }

    /**
     * @brief Not equal operator.
     * @param a First string to compare.
     * @param b Second string to compare.
     * @return <b>true</b> if the first string is not equal to the second one, otherwise <b>false</b>.
     */
    [[nodiscard]] constexpr friend bool operator!=(const istring& a, const istring& b)
    {
        return ! (a == b);
    }

    /**
     * @brief Less than operator.
     * @param a First string to compare.
     * @param b Second string to compare.
     * @return <b>true</b> if the first string is lexicographically less than the second one, otherwise <b>false</b>.
     */
    [[nodiscard]] constexpr friend bool operator<(const istring& a, const istring& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    /**
     * @brief Greater than operator.
     * @param a First string to compare.
     * @param b Second string to compare.
     * @return <b>true</b> if the first string is lexicographically greater than the second one,
     * otherwise <b>false</b>.
     */
    [[nodiscard]] constexpr friend bool operator>(const istring& a, const istring& b)
    {
        return b < a;
    }

    /**
     * @brief Less than or equal operator.
     * @param a First string to compare.
     * @param b Second string to compare.
     * @return <b>true</b> if the first string is lexicographically less than or equal to the second one,
     * otherwise <b>false</b>.
     */
    [[nodiscard]] constexpr friend bool operator<=(const istring& a, const istring& b)
    {
        return ! (a > b);
    }

    /**
     * @brief Greater than or equal operator.
     * @param a First string to compare.
     * @param b Second string to compare.
     * @return <b>true</b> if the first string is lexicographically greater than or equal to the second one,
     * otherwise <b>false</b>.
     */
    [[nodiscard]] constexpr friend bool operator>=(const istring& a, const istring& b)
    {
        return ! (a < b);
    }

protected:
    /// @cond DO_NOT_DOCUMENT

    constexpr istring(pointer data, size_type size, size_type max_size) :
        istring_base(data, size, max_size)
    {
    }

    /// @endcond
};


template<int MaxSize>
class string : public istring
{
    static_assert(MaxSize > 0);

public:
    /**
     * @brief Default constructor.
     */
    constexpr string() :
        istring(_buffer, 0, MaxSize)
    {
        _data[0] = 0;
    }

    /**
     * @brief Copy constructor.
     * @param other string to copy.
     */
    constexpr string(const string& other) :
        string()
    {
        append(other);
    }

    /**
     * @brief Copy constructor.
     * @param other Base string to copy.
     */
    constexpr string(const istring& other) :
        string()
    {
        append(other);
    }

    /**
     * @brief Copy constructor.
     * @param char_array_ptr Pointer to null-terminated characters array.
     */
    constexpr string(const_pointer char_array_ptr) :
        string()
    {
        append(char_array_ptr);
    }

    /**
     * @brief Copy constructor.
     * @param char_array_ptr Pointer to characters array.
     * @param char_array_size Characters count of the characters array.
     */
    constexpr string(const_pointer char_array_ptr, size_type char_array_size) :
        string()
    {
        append(char_array_ptr, char_array_size);
    }

    /**
     * @brief Constructs a string with count copies of character value.
     * @param count Number of characters to append.
     * @param value Character to append.
     */
    constexpr string(size_type count, value_type value) :
        string()
    {
        append(count, value);
    }

    /**
     * @brief Constructs a string with the characters in the range [first, last).
     * @param first First element of the range.
     * @param last Last element of the range.
     */
    constexpr string(const_iterator first, const_iterator last) :
        string()
    {
        append(first, last);
    }

    /**
     * @brief Copy assignment operator.
     * @param other string to copy.
     * @return Reference to this.
     */
    constexpr string& operator=(const string& other)
    {
        istring::operator=(other);
        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param other Base string to copy.
     * @return Reference to this.
     */
    constexpr string& operator=(const istring& other)
    {
        istring::operator=(other);
        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param char_array_ptr Pointer to null-terminated characters array.
     * @return Reference to this.
     */
    constexpr string& operator=(const_pointer char_array_ptr)
    {
        istring::operator=(char_array_ptr);
        return *this;
    }

    /**
     * @brief Concatenates two strings.
     * @param a First string to concatenate.
     * @param b Second string to concatenate.
     * @return string containing characters from a followed by the characters from b.
     */
    [[nodiscard]] constexpr friend string operator+(const string& a, const istring& b)
    {
        string result = a;
        result.append(b);
        return result;
    }

    /**
     * @brief Concatenates a string and a character.
     * @param a First string to concatenate.
     * @param b Second character to concatenate.
     * @return string containing characters from a followed by b.
     */
    [[nodiscard]] constexpr friend string operator+(const string& a, value_type b)
    {
        string result = a;
        result.append(b);
        return result;
    }

    /**
     * @brief Concatenates two strings.
     * @param a First string to concatenate.
     * @param b Second string_view to concatenate.
     * @return string containing characters from a followed by the characters from b.
     */
    [[nodiscard]] constexpr friend string operator+(const string& a, const string_view& b)
    {
        string result = a;
        result.append(b);
        return result;
    }

    /**
     * @brief Concatenates two strings.
     * @param a First string to concatenate.
     * @param b Second pointer to null-terminated characters array to concatenate.
     * @return string containing characters from a followed by the characters from b.
     */
    [[nodiscard]] constexpr friend string operator+(const string& a, const_pointer b)
    {
        string result = a;
        result.append(b);
        return result;
    }

private:
    alignas(int) char _buffer[MaxSize + 1];
};


/**
 * @brief Hash support for string.
 *
 * @ingroup string
 */
template<>
struct hash<istring>
{
    /**
     * @brief Returns the hash of the given string.
     */
    [[nodiscard]] constexpr unsigned operator()(const istring& value) const
    {
        return array_hash(value.data(), value.size());
    }
};


/**
 * @brief Creates a string deducing its size from the size of the argument.
 * @param char_array Non empty const characters array.
 * @return string containing the given char array.
 *
 * @ingroup string
 */
template<int MaxSize>
string<MaxSize - 1> make_string(const char (&char_array)[MaxSize])
{
    return string<MaxSize - 1>(char_array, MaxSize - 1);
}


/**
 * @brief Converts the given value to a string.
 * @tparam MaxSize Maximum number of characters that can be stored in the output string.
 * @param value Value to print in the string.
 * @return string containing the representation of the given value.
 *
 * @ingroup string
 */
template<int MaxSize, typename Type>
string<MaxSize> to_string(const Type& value)
{
    string<MaxSize> result;
    ostringstream stream(result);
    stream << value;
    return result;
}

}

#endif
