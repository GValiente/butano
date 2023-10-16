/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_STRING_H
#define BN_STRING_H

/**
 * @file
 * bn::istring and bn::string implementation header file.
 *
 * @ingroup string
 */

#include "bn_sstream.h"
#include "bn_string_view.h"

namespace bn
{

/**
 * @brief Base class of bn::string.
 *
 * Can be used as a reference type for all bn::string containers.
 *
 * @ingroup string
 */
class istring : public istring_base
{

public:
    using reverse_iterator = bn::reverse_iterator<iterator>; //!< Reverse iterator alias.
    using const_reverse_iterator = bn::reverse_iterator<const_iterator>; //!< Const reverse iterator alias.

    /**
     * @brief Copy assignment operator.
     * @param other istring_base to copy.
     * @return Reference to this.
     */
    constexpr istring& operator=(const istring_base& other)
    {
        assign(other);
        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param view string_view to copy.
     * @return Reference to this.
     */
    constexpr istring& operator=(const string_view& view)
    {
        assign(view);
        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param char_array_ptr Pointer to null-terminated characters array.
     * @return Reference to this.
     */
    constexpr istring& operator=(const_pointer char_array_ptr)
    {
        assign(char_array_ptr);
        return *this;
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
        BN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _data[index];
    }

    /**
     * @brief Returns a reference to the character stored at the specified index.
     */
    [[nodiscard]] constexpr reference operator[](size_type index)
    {
        BN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _data[index];
    }

    /**
     * @brief Returns a const reference to the character stored at the specified index.
     */
    [[nodiscard]] constexpr const_reference at(size_type index) const
    {
        BN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _data[index];
    }

    /**
     * @brief Returns a reference to the character stored at the specified index.
     */
    [[nodiscard]] constexpr reference at(size_type index)
    {
        BN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _data[index];
    }

    /**
     * @brief Returns a const reference to the first character.
     */
    [[nodiscard]] constexpr const_reference front() const
    {
        BN_BASIC_ASSERT(! empty(), "String is empty");

        return _data[0];
    }

    /**
     * @brief Returns a reference to the first character.
     */
    [[nodiscard]] constexpr reference front()
    {
        BN_BASIC_ASSERT(! empty(), "String is empty");

        return _data[0];
    }

    /**
     * @brief Returns a const reference to the last character.
     */
    [[nodiscard]] constexpr const_reference back() const
    {
        BN_BASIC_ASSERT(! empty(), "String is empty");

        return _data[_size - 1];
    }

    /**
     * @brief Returns a reference to the last character.
     */
    [[nodiscard]] constexpr reference back()
    {
        BN_BASIC_ASSERT(! empty(), "String is empty");

        return _data[_size - 1];
    }

    /**
     * @brief Checks if the referenced string begins with the given prefix.
     * @param value Single character.
     * @return `true` if the referenced string begins with the given prefix; `false` otherwise.
     */
    [[nodiscard]] constexpr bool starts_with(value_type value) const
    {
        return ! empty() && _data[0] == value;
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

        const_pointer this_char_array_ptr = _data;

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
        return ! empty() && _data[_size - 1] == value;
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

        return equal(_data + this_size - other_size, _data + this_size, other.data());
    }

    /**
     * @brief Replaces the contents of the istring.
     * @param other istring_base replacement.
     * @return Reference to this.
     */
    constexpr istring& assign(const istring_base& other)
    {
        _assign(other.data(), other.size());
        return *this;
    }

    /**
     * @brief Replaces the contents of the istring.
     * @param value Character replacement.
     * @return Reference to this.
     */
    constexpr istring& assign(value_type value)
    {
        pointer data = _data;
        data[0] = value;
        data[1] = 0;
        _size = 1;
        return *this;
    }

    /**
     * @brief Replaces the contents of the istring.
     * @param view string_view replacement.
     * @return Reference to this.
     */
    constexpr istring& assign(const string_view& view)
    {
        _assign(view.data(), view.size());
        return *this;
    }

    /**
     * @brief Replaces the contents of the istring.
     * @param char_array_ptr Pointer to null-terminated characters array replacement.
     * @return Reference to this.
     */
    constexpr istring& assign(const_pointer char_array_ptr)
    {
        return assign(string_view(char_array_ptr));
    }

    /**
     * @brief Replaces the contents of the istring.
     * @param char_array_ptr Pointer to null-terminated characters array replacement.
     * @param char_array_size Characters count of the characters array.
     * @return Reference to this.
     */
    constexpr istring& assign(const_pointer char_array_ptr, size_type char_array_size)
    {
        return assign(string_view(char_array_ptr, char_array_size));
    }

    /**
     * @brief Replaces the contents of the istring with count copies of character value.
     * @param count New size.
     * @param value Character replacement.
     * @return Reference to this.
     */
    constexpr istring& assign(size_type count, value_type value)
    {
        BN_ASSERT(count >= 0, "Invalid count: ", count);
        BN_ASSERT(count <= _max_size, "Not enough space: ", count, " - ", _max_size);

        pointer data = _data;
        bn::fill(data, data + count, value);
        data[count] = 0;
        _size = count;
        return *this;
    }

    /**
     * @brief Replaces the contents of the istring with the characters in the range [first, last).
     * @param first First element of the range.
     * @param last Last element of the range.
     * @return Reference to this.
     */
    constexpr istring& assign(const_iterator first, const_iterator last)
    {
        return assign(string_view(first, last - first));
    }

    /**
     * @brief Inserts a character at the end of the istring.
     * @param value Character to insert.
     */
    constexpr void push_back(value_type value)
    {
        BN_BASIC_ASSERT(! full(), "String is full");

        size_type size = _size;
        pointer data = _data + size;
        data[0] = value;
        data[1] = 0;
        _size = size + 1;
    }

    /**
     * @brief Removes the last character of the istring.
     */
    constexpr void pop_back()
    {
        size_type size = _size;
        BN_BASIC_ASSERT(size, "String is empty");

        --size;
        _data[size] = 0;
        _size = size;
    }

    /**
     * @brief Appends additional characters to the istring.
     * @param other istring_base to append.
     * @return Reference to this.
     */
    constexpr istring& append(const istring_base& other)
    {
        _append(other.data(), other.size());
        return *this;
    }

    /**
     * @brief Appends an additional character to the istring.
     * @param value Character to append.
     * @return Reference to this.
     */
    constexpr istring& append(value_type value)
    {
        push_back(value);
        return *this;
    }

    /**
     * @brief Appends additional characters to the istring.
     * @param view string_view to append.
     * @return Reference to this.
     */
    constexpr istring& append(const string_view& view)
    {
        _append(view.data(), view.size());
        return *this;
    }

    /**
     * @brief Appends additional characters to the istring.
     * @param char_array_ptr Pointer to null-terminated characters array.
     * @return Reference to this.
     */
    constexpr istring& append(const_pointer char_array_ptr)
    {
        append(string_view(char_array_ptr));
        return *this;
    }

    /**
     * @brief Appends additional characters to the istring.
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
     * @brief Appends additional characters to the istring.
     * @param count Number of characters to append.
     * @param value Character to append.
     * @return Reference to this.
     */
    constexpr istring& append(size_type count, value_type value)
    {
        BN_ASSERT(count >= 0, "Invalid count: ", count);
        BN_ASSERT(_size + count <= _max_size, "Not enough space: ", _size + count, " - ", _max_size);

        iterator append_position = end();
        bn::fill(append_position, append_position + count, value);
        _size += count;
        _data[_size] = 0;
        return *this;
    }

    /**
     * @brief Appends additional characters to the istring.
     * @param first First element of the range to append.
     * @param last Last element of the range to append.
     * @return Reference to this.
     */
    constexpr istring& append(const_iterator first, const_iterator last)
    {
        return assign(string_view(first, last - first));
    }

    /**
     * @brief Erases a character.
     * @param position Iterator to the character to erase.
     * @return Iterator following the erased character.
     */
    constexpr iterator erase(const_iterator position)
    {
        BN_BASIC_ASSERT(! empty(), "String is empty");

        auto erase_position = const_cast<iterator>(position);
        bn::copy(erase_position + 1, end(), erase_position);
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
        BN_ASSERT(count >= 0, "Invalid range");
        BN_ASSERT(count <= _size, "Invalid range: ", count, " - ", _size);

        auto erase_first = const_cast<iterator>(first);
        auto erase_last = const_cast<iterator>(last);
        bn::copy(erase_last, end(), erase_first);
        _size -= count;
        _data[_size] = 0;
        return erase_first;
    }

    /**
     * @brief Resizes the istring.
     * @param count New size.
     */
    constexpr void resize(size_type count)
    {
        resize(count, 0);
    }

    /**
     * @brief Resizes the istring.
     * @param count New size.
     * @param value Character to fill new elements with.
     */
    constexpr void resize(size_type count, value_type value)
    {
        BN_ASSERT(count >= 0 && count <= _max_size, "Invalid count: ", count, " - ", _max_size);

        pointer data = _data;
        size_type size = _size;
        _size = count;

        if(size < count)
        {
            bn::fill(data + size, data + size + count, value);
        }

        data[count] = 0;
    }

    /**
     * @brief Resizes the istring to a size less or equal than the previous one.
     * @param count New size.
     */
    constexpr void shrink(size_type count)
    {
        BN_ASSERT(count >= 0 && count <= _size, "Invalid count: ", count, " - ", _size);

        _data[count] = 0;
        _size = count;
    }

    /**
     * @brief Exchanges the contents of this istring with those of the other one.
     * @param other istring to exchange the contents with.
     */
    constexpr void swap(istring& other)
    {
        if(_data != other._data)
        {
            BN_ASSERT(_size <= other._max_size, "Invalid size: ", _size, " - ", other._max_size);
            BN_ASSERT(_max_size <= other._size, "Invalid max size: ", _max_size, " - ", other._size);

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
                bn::swap(min_data[index], max_data[index]);
            }

            for(size_type index = min_size; index < max_size; ++index)
            {
                min_data[index] = max_data[index];
            }

            bn::swap(_size, other._size);
            _data[_size] = 0;
            other._data[other._size] = 0;
        }
    }

    /**
     * @brief Exchanges the contents of a istring with those of another one.
     * @param a First istring to exchange the contents with.
     * @param b Second istring to exchange the contents with.
     */
    constexpr friend void swap(istring& a, istring& b)
    {
        a.swap(b);
    }

    /**
     * @brief Appends additional characters to the istring.
     * @param other istring_base to append.
     * @return Reference to this.
     */
    constexpr istring& operator+=(const istring_base& other)
    {
        append(other);
        return *this;
    }

    /**
     * @brief Appends an additional character to the istring.
     * @param value Character to append.
     * @return Reference to this.
     */
    constexpr istring& operator+=(value_type value)
    {
        append(value);
        return *this;
    }

    /**
     * @brief Appends additional characters to the istring.
     * @param view string_view to append.
     * @return Reference to this.
     */
    constexpr istring& operator+=(const string_view& view)
    {
        append(view);
        return *this;
    }

    /**
     * @brief Appends additional characters to the istring.
     * @param char_array_ptr Pointer to null-terminated characters array.
     * @return Reference to this.
     */
    constexpr istring& operator+=(const_pointer char_array_ptr)
    {
        append(char_array_ptr);
        return *this;
    }

protected:
    /// @cond DO_NOT_DOCUMENT

    constexpr istring(pointer data, size_type size, size_type max_size) :
        istring_base(data, size, max_size)
    {
    }

    /// @endcond

private:
    constexpr void _assign(const_pointer other_data, size_type other_size)
    {
        BN_ASSERT(other_size <= _max_size, "Not enough space: ", other_size, " - ", _max_size);

        pointer data = _data;
        bn::copy(other_data, other_data + other_size, data);
        data[other_size] = 0;
        _size = other_size;
    }

    constexpr void _append(const_pointer other_data, size_type other_size)
    {
        size_type new_size = _size + other_size;
        BN_ASSERT(new_size <= _max_size, "Not enough space: ", new_size, " - ", _max_size);

        iterator append_position = end();

        if(append_position <= other_data)
        {
            bn::copy(other_data, other_data + other_size, append_position);
        }
        else
        {
            bn::copy_backward(other_data, other_data + other_size, append_position + other_size);
        }

        _data[new_size] = 0;
        _size = new_size;
    }
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
        assign(other);
    }

    /**
     * @brief Copy constructor.
     * @param other istring_base to copy.
     */
    constexpr string(const istring_base& other) :
        string()
    {
        assign(other);
    }

    /**
     * @brief Copy constructor.
     * @param view string_view to copy.
     */
    constexpr string(const string_view& view) :
        string()
    {
        assign(view);
    }

    /**
     * @brief Copy constructor.
     * @param char_array_ptr Pointer to null-terminated characters array.
     */
    constexpr string(const_pointer char_array_ptr) :
        string()
    {
        assign(char_array_ptr);
    }

    /**
     * @brief Copy constructor.
     * @param char_array_ptr Pointer to characters array.
     * @param char_array_size Characters count of the characters array.
     */
    constexpr string(const_pointer char_array_ptr, size_type char_array_size) :
        string()
    {
        assign(char_array_ptr, char_array_size);
    }

    /**
     * @brief Constructs a string with count copies of character value.
     * @param count Number of characters to assign.
     * @param value Character to assign.
     */
    constexpr string(size_type count, value_type value) :
        string()
    {
        assign(count, value);
    }

    /**
     * @brief Constructs a string with the characters in the range [first, last).
     * @param first First element of the range.
     * @param last Last element of the range.
     */
    constexpr string(const_iterator first, const_iterator last) :
        string()
    {
        assign(first, last);
    }

    /**
     * @brief Copy assignment operator.
     * @param other string to copy.
     * @return Reference to this.
     */
    constexpr string& operator=(const string& other)
    {
        assign(other);
        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param other istring_base to copy.
     * @return Reference to this.
     */
    constexpr string& operator=(const istring_base& other)
    {
        assign(other);
        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param view string_view to copy.
     * @return Reference to this.
     */
    constexpr string& operator=(const string_view& view)
    {
        assign(view);
        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param char_array_ptr Pointer to null-terminated characters array.
     * @return Reference to this.
     */
    constexpr string& operator=(const_pointer char_array_ptr)
    {
        assign(char_array_ptr);
        return *this;
    }

    /**
     * @brief Concatenates a string and a istring_base.
     * @param a First string to concatenate.
     * @param b Second istring_base to concatenate.
     * @return string containing characters from a followed by the characters from b.
     */
    [[nodiscard]] constexpr friend string operator+(const string& a, const istring_base& b)
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
 * @brief Creates a string deducing its size from the size of the argument.
 * @param char_array Non empty const characters array.
 * @return string containing the given char array.
 *
 * @ingroup string
 */
template<int MaxSize>
[[nodiscard]] constexpr string<MaxSize - 1> make_string(const char (&char_array)[MaxSize])
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
[[nodiscard]] string<MaxSize> to_string(const Type& value)
{
    string<MaxSize> result;
    ostringstream stream(result);
    stream << value;
    return result;
}


/**
 * @brief Hash support for istring.
 *
 * @ingroup string
 * @ingroup functional
 */
template<>
struct hash<istring>
{
    /**
     * @brief Returns the hash of the given istring.
     */
    [[nodiscard]] constexpr unsigned operator()(const istring& value) const
    {
        return hash<istring_base>()(value);
    }
};


/**
 * @brief Hash support for string.
 * @tparam MaxSize Maximum number of characters that can be stored in the string.
 *
 * @ingroup string
 * @ingroup functional
 */
template<int MaxSize>
struct hash<string<MaxSize>>
{
    /**
     * @brief Returns the hash of the given string.
     */
    [[nodiscard]] constexpr unsigned operator()(const string<MaxSize>& value) const
    {
        return hash<istring_base>()(value);
    }
};

/**
 * @brief Erases all characters from a istring that are equal to the specified one.
 * @param string istring from which to erase.
 * @param value Character to erase.
 * @return Number of erased characters.
 */
constexpr istring::size_type erase(istring& string, char value)
{
    auto old_size = string.size();
    string.erase(remove(string.begin(), string.end(), value), string.end());
    return old_size - string.size();
}

/**
 * @brief Erases all characters from a istring that satisfy the specified predicate.
 * @param string istring from which to erase.
 * @param pred Unary predicate which returns `true` if the character should be erased.
 * @return Number of erased characters.
 */
template<class Pred>
constexpr istring::size_type erase_if(istring& string, const Pred& pred)
{
    auto old_size = string.size();
    string.erase(remove_if(string.begin(), string.end(), pred), string.end());
    return old_size - string.size();
}

}

#endif
