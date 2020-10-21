/*
 * Copyright (c) 2014 jwellbelove Embedded Template Library https://www.etlcpp.com
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_STRING_H
#define BTN_STRING_H

#include "btn_assert.h"
#include "btn_limits.h"
#include "btn_sstream.h"
#include "btn_utility.h"
#include "btn_iterator.h"
#include "btn_algorithm.h"
#include "btn_functional.h"

namespace btn
{

class istring : public istring_base
{

public:
    using value_type = char;
    using size_type = int;
    using reference = char&;
    using const_reference = const char&;
    using pointer = char*;
    using const_pointer = const char*;
    using iterator = char*;
    using const_iterator = const char*;
    using reverse_iterator = btn::reverse_iterator<iterator>;
    using const_reverse_iterator = btn::reverse_iterator<const_iterator>;

public:
    constexpr static size_type npos = numeric_limits<size_type>::max();

    constexpr istring& operator=(const istring& other)
    {
        BTN_ASSERT(other._size <= _max_size, "Not enough space in string: ", other._size, " - ", _max_size);

        btn::copy(other.begin(), other.end(), begin());
        _size = other._size;
        _data[_size] = 0;
        return *this;
    }

    constexpr istring& operator=(const_pointer str)
    {
        BTN_ASSERT(str, "Str is null");

        size_type length = _strlen(str);
        BTN_ASSERT(length <= _max_size, "Not enough space in string: ", length, " - ", _max_size);

        btn::copy(str, str + length, begin());
        _size = length;
        _data[_size] = 0;
        return *this;
    }

    [[nodiscard]] constexpr const_pointer c_str() const
    {
        return _data;
    }

    [[nodiscard]] constexpr iterator begin()
    {
        return _data;
    }

    [[nodiscard]] constexpr const_iterator begin() const
    {
        return _data;
    }

    [[nodiscard]] constexpr iterator end()
    {
        return _data + _size;
    }

    [[nodiscard]] constexpr const_iterator end() const
    {
        return _data + _size;
    }

    [[nodiscard]] constexpr const_iterator cbegin() const
    {
        return _data;
    }

    [[nodiscard]] constexpr const_iterator cend() const
    {
        return _data + _size;
    }

    [[nodiscard]] constexpr reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    [[nodiscard]] constexpr const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }

    [[nodiscard]] constexpr reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    [[nodiscard]] constexpr const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }

    [[nodiscard]] constexpr const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(cend());
    }

    [[nodiscard]] constexpr const_reverse_iterator crend() const
    {
        return const_reverse_iterator(cbegin());
    }

    [[nodiscard]] constexpr const_reference operator[](size_type index) const
    {
        BTN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _data[index];
    }

    [[nodiscard]] constexpr reference operator[](size_type index)
    {
        BTN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _data[index];
    }

    [[nodiscard]] constexpr const_reference at(size_type index) const
    {
        BTN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _data[index];
    }

    [[nodiscard]] constexpr reference at(size_type index)
    {
        BTN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _data[index];
    }

    [[nodiscard]] constexpr const_reference front() const
    {
        BTN_ASSERT(! empty(), "String is empty");

        return _data[0];
    }

    [[nodiscard]] constexpr reference front()
    {
        BTN_ASSERT(! empty(), "String is empty");

        return _data[0];
    }

    [[nodiscard]] constexpr const_reference back() const
    {
        BTN_ASSERT(! empty(), "String is empty");

        return _data[_size - 1];
    }

    [[nodiscard]] constexpr reference back()
    {
        BTN_ASSERT(! empty(), "String is empty");

        return _data[_size - 1];
    }

    constexpr void clear()
    {
        _data[0] = 0;
        _size = 0;
    }

    constexpr istring& assign(const istring& other)
    {
        clear();
        append(other);
        return *this;
    }

    constexpr istring& assign(const istring& other, size_type subposition)
    {
        clear();
        append(other, subposition);
        return *this;
    }

    constexpr istring& assign(const istring& other, size_type subposition, size_type sublength)
    {
        clear();
        append(other, subposition, sublength);
        return *this;
    }

    constexpr istring& assign(value_type value)
    {
        clear();
        append(value);
        return *this;
    }

    constexpr istring& assign(const_pointer str)
    {
        clear();
        append(str);
        return *this;
    }

    constexpr istring& assign(const_pointer str, size_type str_size)
    {
        clear();
        append(str, str_size);
        return *this;
    }

    constexpr istring& assign(size_type count, value_type value)
    {
        clear();
        append(count, value);
        return *this;
    }

    constexpr istring& assign(const_iterator first, const_iterator last)
    {
        clear();
        append(first, last);
        return *this;
    }

    constexpr void push_back(value_type value)
    {
        BTN_ASSERT(! full(), "String is full");

        pointer data = _data + _size;
        data[0] = value;
        data[1] = 0;
        ++_size;
    }

    constexpr void pop_back()
    {
        BTN_ASSERT(! empty(), "String is empty");

        --_size;
        _data[_size] = 0;
    }

    constexpr istring& append(const istring& other)
    {
        append(other.begin(), other.end());
        return *this;
    }

    constexpr istring& append(const istring& other, size_type subposition)
    {
        BTN_ASSERT(subposition >= 0 && subposition <= other._size, "Invalid subposition: ",
                   subposition, " - ", other._size);

        size_type sublength = other._size - subposition;
        append(other.begin() + subposition, other.begin() + subposition + sublength);
        return *this;
    }

    constexpr istring& append(const istring& other, size_type subposition, size_type sublength)
    {
        BTN_ASSERT(subposition >= 0, "Invalid subposition: ", subposition);
        BTN_ASSERT(sublength >= 0, "Invalid sublength: ", sublength);
        BTN_ASSERT(subposition + sublength <= other._size, "Invalid subposition or sublength: ",
                   subposition, " - ", sublength, " - ", other._size);

        append(other.begin() + subposition, other.begin() + subposition + sublength);
        return *this;
    }

    constexpr istring& append(value_type value)
    {
        push_back(value);
        return *this;
    }

    constexpr istring& append(const_pointer str)
    {
        BTN_ASSERT(str, "Str is null");

        append(str, str + _strlen(str));
        return *this;
    }

    constexpr istring& append(const_pointer str, size_type str_size)
    {
        BTN_ASSERT(str, "Str is null");
        BTN_ASSERT(str_size >= 0, "Invalid str size: ", str_size);

        append(str, str + str_size);
        return *this;
    }

    constexpr istring& append(size_type count, value_type value)
    {
        BTN_ASSERT(count >= 0, "Invalid count: ", count);

        auto append_position = end();
        size_type size = _size;
        size_type to_position = size + count;
        BTN_ASSERT(to_position <= _max_size, "Not enough space in string: ", to_position, " - ", _max_size);

        if(to_position == _max_size)
        {
            _size = _max_size;
            fill(append_position, end(), value);
        }
        else
        {
            _size += count;
            copy_backward(append_position, append_position, begin() + to_position);
            fill(append_position, append_position + count, value);
        }

        _data[_size] = 0;
        return *this;
    }

    constexpr istring& append(const_iterator first, const_iterator last)
    {
        size_type count = last - first;
        BTN_ASSERT(count >= 0, "Invalid range");

        iterator append_position = end();
        size_type size = _size;
        size_type to_position = size + count;
        BTN_ASSERT(to_position <= _max_size, "Not enough space in string: ", to_position, " - ", _max_size);

        if(to_position == _max_size)
        {
            _size = _max_size;

            iterator end_it = end();

            while(append_position != end_it)
            {
                *append_position++ = *first++;
            }
        }
        else
        {
            _size += count;
            copy_backward(append_position, append_position, begin() + to_position);

            while(first != last)
            {
                *append_position++ = *first++;
            }
        }

        _data[_size] = 0;
        return *this;
    }

    constexpr iterator insert(const_iterator position, value_type value)
    {
        BTN_ASSERT(! full(), "String is full");

        auto insert_position = const_cast<iterator>(position);
        iterator last = end();

        if(position != last)
        {
            ++_size;

            copy_backward(insert_position, last - 1, last);
            *insert_position = value;
        }
        else
        {
            *insert_position = value;
            ++_size;
        }

        _data[_size] = 0;
        return insert_position;
    }

    constexpr void insert(const_iterator position, size_type count, value_type value)
    {
        BTN_ASSERT(count >= 0, "Invalid count: ", count);

        auto insert_position = const_cast<iterator>(position);
        size_type start = position - begin();
        BTN_ASSERT(start < _max_size, "Not enough space in string: ", start, " - ", _max_size);

        if(start + count >= _max_size)
        {
            BTN_ASSERT(_size + count <= _max_size, "Not enough space in string: ",
                       _size, " - ", count, " - ", _max_size);

            _size = _max_size;
            fill(insert_position, end(), value);
        }
        else
        {
            size_type shift_amount = count;
            size_type to_position = start + shift_amount;
            size_type remaining_characters = _size - start;
            size_type max_shift_characters = _max_size - start - shift_amount;
            size_type characters_to_shift = min(max_shift_characters, remaining_characters);
            BTN_ASSERT(start + shift_amount + remaining_characters <= _max_size, "Not enough space in string: ",
                       start, " - ", shift_amount, " - ", remaining_characters, " - ", _max_size);

            _size += shift_amount;
            copy_backward(insert_position, insert_position + characters_to_shift,
                          begin() + to_position + characters_to_shift);
            fill(insert_position, insert_position + shift_amount, value);
        }

        _data[_size] = 0;
    }

    constexpr void insert(const_iterator position, const_iterator first, const_iterator last)
    {
        size_type count = last - first;
        BTN_ASSERT(count >= 0, "Invalid range");

        auto insert_position = const_cast<iterator>(position);
        size_type start = position - begin();
        BTN_ASSERT(start < _max_size, "Not enough space in string: ", start, " - ", _max_size);

        if(start + count >= _max_size)
        {
            BTN_ASSERT(_size + count <= _max_size, "Not enough space in string: ",
                       _size, " - ", count, " - ", _max_size);

            _size = _max_size;

            iterator end_it = end();

            while(insert_position != end_it)
            {
                *insert_position++ = *first++;
            }
        }
        else
        {
            size_type shift_amount = count;
            size_type to_position = start + shift_amount;
            size_type remaining_characters = _size - start;
            size_type max_shift_characters = _max_size - start - shift_amount;
            size_type characters_to_shift = min(max_shift_characters, remaining_characters);
            BTN_ASSERT(start + shift_amount + remaining_characters <= _max_size, "Not enough space in string: ",
                       start, " - ", shift_amount, " - ", remaining_characters, " - ", _max_size);

            _size += shift_amount;
            copy_backward(position, position + characters_to_shift, begin() + to_position + characters_to_shift);

            while(first != last)
            {
                *insert_position++ = *first++;
            }
        }

        _data[_size] = 0;
    }

    constexpr istring& insert(size_type position, const istring& other)
    {
        BTN_ASSERT(position >= 0, "Invalid position: ", position);

        insert(begin() + position, other.begin(), other.end());
        return *this;
    }

    constexpr istring& insert(size_type position, const istring& other, size_type subposition)
    {
        BTN_ASSERT(position >= 0, "Invalid position: ", position);
        BTN_ASSERT(subposition >= 0 && subposition <= other._size, "Invalid subposition: ",
                   subposition, " - ", other._size);

        size_type sublength = other._size - subposition;
        insert(begin() + position, other.begin() + subposition, other.begin() + subposition + sublength);
        return *this;
    }

    constexpr istring& insert(size_type position, const istring& other, size_type subposition, size_type sublength)
    {
        BTN_ASSERT(position >= 0, "Invalid position: ", position);
        BTN_ASSERT(subposition >= 0, "Invalid subposition: ", subposition);
        BTN_ASSERT(sublength >= 0, "Invalid sublength: ", sublength);
        BTN_ASSERT(subposition + sublength <= other._size, "Invalid subposition or sublength: ",
                   subposition, " - ", sublength, " - ", other._size);

        insert(begin() + position, other.begin() + subposition, other.begin() + subposition + sublength);
        return *this;
    }

    constexpr istring& insert(size_type position, const_pointer str)
    {
        BTN_ASSERT(position >= 0, "Invalid position: ", position);
        BTN_ASSERT(str, "Str is null");

        insert(begin() + position, str, str + _strlen(str));
        return *this;
    }

    constexpr istring& insert(size_type position, const_pointer str, size_type str_size)
    {
        BTN_ASSERT(position >= 0, "Invalid position: ", position);
        BTN_ASSERT(str, "Str is null");
        BTN_ASSERT(str_size >= 0, "Invalid str size: ", str_size);

        insert(begin() + position, str, str + str_size);
        return *this;
    }

    constexpr istring& insert(size_type position, size_type count, value_type value)
    {
        BTN_ASSERT(position >= 0, "Invalid position: ", position);

        insert(begin() + position, count, value);
        return *this;
    }

    constexpr istring& erase(size_type position)
    {
        BTN_ASSERT(position >= 0, "Invalid position: ", position);

        size_type length = _size - position;
        erase(begin() + position, begin() + position + length);
        return *this;
    }

    constexpr istring& erase(size_type position, size_type length)
    {
        BTN_ASSERT(position >= 0, "Invalid position: ", position);
        BTN_ASSERT(length >= 0, "Invalid length: ", length);

        erase(begin() + position, begin() + position + length);
        return *this;
    }

    constexpr iterator erase(const_iterator position)
    {
        BTN_ASSERT(! empty(), "String is empty");

        auto erase_position = const_cast<iterator>(position);
        btn::copy(erase_position + 1, end(), erase_position);
        --_size;
        _data[_size] = 0;
        return erase_position;
    }

    constexpr iterator erase(const_iterator first, const_iterator last)
    {
        size_type delete_count = last - first;
        BTN_ASSERT(delete_count <= _size, "Invalid range: ", delete_count, " - ", _size);

        auto erase_first = const_cast<iterator>(first);
        auto erase_last = const_cast<iterator>(last);
        btn::copy(erase_last, end(), erase_first);

        _size -= delete_count;
        _data[_size] = 0;
        return erase_first;
    }

    constexpr friend size_type erase(istring& string, value_type value)
    {
        size_type old_size = string.size();
        string.erase(remove(string.begin(), string.end(), value), string.end());
        return old_size - string.size();
    }

    template<class Pred>
    constexpr friend size_type erase_if(istring& string, const Pred& pred)
    {
        size_type old_size = string.size();
        string.erase(remove_if(string.begin(), string.end(), pred), string.end());
        return old_size - string.size();
    }

    constexpr size_type copy(pointer str, size_type length)
    {
        BTN_ASSERT(str, "Str is null");
        BTN_ASSERT(length >= 0 && length <= _size, "Invalid length: ", length, " - ", _size);

        for(size_type index = 0; index < length; ++index)
        {
            *str++ = _data[index];
        }

        return length;
    }

    constexpr size_type copy(pointer str, size_type length, size_type position)
    {
        BTN_ASSERT(str, "Str is null");
        BTN_ASSERT(length >= 0, "Invalid length: ", length);
        BTN_ASSERT(position >= 0, "Invalid position: ", position);

        size_type end = position + length;
        BTN_ASSERT(end <= _size, "Invalid position or length: ", end, " - ", _size);

        for(size_type index = position; index < end; ++index)
        {
            *str++ = _data[index];
        }

        return end - position;
    }

    constexpr istring& replace(size_type position, size_type length, const istring& other)
    {
        BTN_ASSERT(position >= 0 && position <= _size, "Invalid position: ", position, " - ", _size);
        BTN_ASSERT(length >= 0 && length <= _size - position, "Invalid length: ",
                   length, " - ", _size, " - ", position);

        erase(position, length);
        insert(position, other);
        return *this;
    }

    constexpr istring& replace(const_iterator first, const_iterator last, const istring& other)
    {
        iterator replace_first = const_cast<iterator>(first);
        iterator replace_last = const_cast<iterator>(last);
        erase(replace_first, replace_last);
        insert(replace_first, other.begin(), other.end());
        return *this;
    }

    constexpr istring& replace(size_type position, size_type length, const istring& other, size_type subposition)
    {
        BTN_ASSERT(position >= 0 && position <= _size, "Invalid position: ", position, " - ", _size);
        BTN_ASSERT(length >= 0 && length <= _size - position, "Invalid length: ",
                   length, " - ", _size, " - ", position);
        BTN_ASSERT(subposition >= 0 && subposition <= other._size, "Invalid subposition: ",
                   subposition, " - ", other._size);

        size_type sublength = other._size - subposition;
        erase(position, length);
        insert(position, other, subposition, sublength);
        return *this;
    }

    constexpr istring& replace(size_type position, size_type length, const istring& other, size_type subposition,
                               size_type sublength)
    {
        BTN_ASSERT(position >= 0 && position <= _size, "Invalid position: ", position, " - ", _size);
        BTN_ASSERT(length >= 0 && length <= _size - position, "Invalid length: ",
                   length, " - ", _size, " - ", position);
        BTN_ASSERT(subposition >= 0 && subposition <= other._size, "Invalid subposition: ",
                   subposition, " - ", other._size);
        BTN_ASSERT(sublength >= 0 && sublength <= other._size - subposition, "Invalid sublength: ",
                   sublength, " - ", other._size, " - ", subposition);

        erase(position, length);
        insert(position, other, subposition, sublength);
        return *this;
    }

    constexpr istring& replace(size_type position, size_type length, const_pointer str)
    {
        BTN_ASSERT(position >= 0 && position <= _size, "Invalid position: ", position, " - ", _size);
        BTN_ASSERT(length >= 0 && length <= _size - position, "Invalid length: ",
                   length, " - ", _size, " - ", position);
        BTN_ASSERT(str, "Str is null");

        erase(position, length);
        insert(position, str, _strlen(str));
        return *this;
    }

    constexpr istring& replace(const_iterator first, const_iterator last, const_pointer str)
    {
        BTN_ASSERT(str, "Str is null");

        auto replace_first = const_cast<iterator>(first);
        auto replace_last = const_cast<iterator>(last);
        erase(replace_first, replace_last);
        insert(replace_first, str, str + _strlen(str));
        return *this;
    }

    constexpr istring& replace(size_type position, size_type length, const_pointer str, size_type str_size)
    {
        BTN_ASSERT(position >= 0 && position <= _size, "Invalid position: ", position, " - ", _size);
        BTN_ASSERT(length >= 0 && length <= _size - position, "Invalid length: ",
                   length, " - ", _size, " - ", position);
        BTN_ASSERT(str, "Str is null");
        BTN_ASSERT(str_size >= 0, "Invalid str size: ", str_size);

        erase(position, length);
        insert(position, str, str_size);
        return *this;
    }

    constexpr istring& replace(const_iterator first, const_iterator last, const_pointer str, size_type str_size)
    {
        BTN_ASSERT(str, "Str is null");
        BTN_ASSERT(str_size >= 0, "Invalid str size: ", str_size);

        auto replace_first = const_cast<iterator>(first);
        auto replace_last = const_cast<iterator>(last);
        erase(replace_first, replace_last);
        insert(replace_first, str, str + str_size);
        return *this;
    }

    constexpr istring& replace(size_type position, size_type length, size_type count, value_type value)
    {
        BTN_ASSERT(position >= 0 && position <= _size, "Invalid position: ", position, " - ", _size);
        BTN_ASSERT(length >= 0 && length <= _size - position, "Invalid length: ",
                   length, " - ", _size, " - ", position);
        BTN_ASSERT(count >= 0, "Invalid count: ", count);

        erase(position, length);
        insert(position, count, value);
        return *this;
    }

    constexpr istring& replace(const_iterator first, const_iterator last, size_type count, value_type value)
    {
        BTN_ASSERT(count >= 0, "Invalid count: ", count);

        auto replace_first = const_cast<iterator>(first);
        auto replace_last = const_cast<iterator>(last);
        erase(replace_first, replace_last);
        insert(replace_first, count, value);
        return *this;
    }

    constexpr istring& replace(const_iterator first, const_iterator last, const_iterator first_replace,
                               const_iterator last_replace)
    {
        auto replace_first = const_cast<iterator>(first);
        auto replace_last = const_cast<iterator>(last);
        erase(replace_first, replace_last);
        insert(replace_first, first_replace, last_replace);
        return *this;
    }

    [[nodiscard]] constexpr size_type compare(const istring& other) const
    {
        return _compare(_data, _data + _size, other._data, other._data + other._size);
    }

    [[nodiscard]] constexpr size_type compare(size_type position, size_type length, const istring& other) const
    {
        BTN_ASSERT(position >= 0 && position <= _size, "Invalid position: ", position, " - ", _size);
        BTN_ASSERT(length >= 0 && length <= _size - position, "Invalid length: ",
                   length, " - ", _size, " - ", position);

        return _compare(_data + position, _data + position + length, other._data, other._data + other._size);
    }

    [[nodiscard]] constexpr size_type compare(size_type position, size_type length, const istring& other,
                                              size_type subposition) const
    {
        BTN_ASSERT(position >= 0 && position <= _size, "Invalid position: ", position, " - ", _size);
        BTN_ASSERT(length >= 0 && length <= _size - position, "Invalid length: ",
                   length, " - ", _size, " - ", position);
        BTN_ASSERT(subposition >= 0 && subposition <= other._size, "Invalid subposition: ",
                   subposition, " - ", other._size);

        size_type sublength = other._size - subposition;
        return _compare(_data + position, _data + position + length,
                        other._data + subposition, other._data + subposition + sublength);
    }

    [[nodiscard]] constexpr size_type compare(size_type position, size_type length, const istring& other,
                                              size_type subposition, size_type sublength) const
    {
        BTN_ASSERT(position >= 0 && position <= _size, "Invalid position: ", position, " - ", _size);
        BTN_ASSERT(length >= 0 && length <= _size - position, "Invalid length: ",
                   length, " - ", _size, " - ", position);
        BTN_ASSERT(subposition >= 0 && subposition <= other._size, "Invalid subposition: ",
                   subposition, " - ", other._size);
        BTN_ASSERT(sublength >= 0 && sublength <= other._size - subposition, "Invalid sublength: ",
                   sublength, " - ", other._size, " - ", subposition);

        return _compare(_data + position, _data + position + length,
                        other._data + subposition, other._data + subposition + sublength);
    }

    [[nodiscard]] constexpr size_type compare(const_pointer str) const
    {
        BTN_ASSERT(str, "Str is null");

        return _compare(_data, _data + _size, str, str + _strlen(str));
    }

    [[nodiscard]] size_type compare(size_type position, size_type length, const_pointer str) const
    {
        BTN_ASSERT(position >= 0 && position <= _size, "Invalid position: ", position, " - ", _size);
        BTN_ASSERT(length >= 0 && length <= _size - position, "Invalid length: ",
                   length, " - ", _size, " - ", position);
        BTN_ASSERT(str, "Str is null");

        return _compare(_data + position, _data + position + length, str, str + _strlen(str));
    }

    [[nodiscard]] constexpr size_type compare(size_type position, size_type length, const_pointer str,
                                              size_type str_size) const
    {
        BTN_ASSERT(position >= 0 && position <= _size, "Invalid position: ", position, " - ", _size);
        BTN_ASSERT(length >= 0 && length <= _size - position, "Invalid length: ",
                   length, " - ", _size, " - ", position);
        BTN_ASSERT(str, "Str is null");
        BTN_ASSERT(str_size >= 0, "Invalid str size: ", str_size);

        return _compare(_data + position, _data + position + length,
                        str, str + str_size);
    }

    [[nodiscard]] constexpr bool starts_with(const istring& other) const
    {
        return size() >= other.size() && compare(0, other.size(), other) == 0;
    }

    [[nodiscard]] constexpr bool starts_with(value_type value) const
    {
        return ! empty() && front() == value;
    }

    [[nodiscard]] constexpr bool ends_with(const istring& other) const
    {
        return size() >= other.size() && compare(size() - other.size(), npos, other) == 0;
    }

    [[nodiscard]] constexpr bool ends_with(value_type value) const
    {
        return ! empty() && back() == value;
    }

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
        }
    }

    constexpr friend void swap(istring& a, istring& b)
    {
        a.swap(b);
    }

    constexpr istring& operator+=(const istring& other)
    {
        append(other);
        return *this;
    }

    constexpr istring& operator+=(const_pointer str)
    {
        append(str);
        return *this;
    }

    constexpr istring& operator+=(value_type value)
    {
        append(value);
        return *this;
    }

    [[nodiscard]] constexpr friend bool operator==(const istring& a, const istring& b)
    {
        if(a._size != b._size)
        {
            return false;
        }

        return equal(a.begin(), a.end(), b.begin());
    }

    [[nodiscard]] constexpr friend bool operator!=(const istring& a, const istring& b)
    {
        return ! (a == b);
    }

    [[nodiscard]] constexpr friend bool operator<(const istring& a, const istring& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    [[nodiscard]] constexpr friend bool operator>(const istring& a, const istring& b)
    {
        return b < a;
    }

    [[nodiscard]] constexpr friend bool operator<=(const istring& a, const istring& b)
    {
        return ! (a > b);
    }

    [[nodiscard]] constexpr friend bool operator>=(const istring& a, const istring& b)
    {
        return ! (a < b);
    }

protected:
    constexpr istring(pointer data, size_type size, size_type max_size) :
        istring_base(data, size, max_size)
    {
    }

private:
    [[nodiscard]] constexpr static size_type _strlen(const_pointer str)
    {
        size_type result = 0;

        while(*str++)
        {
            ++result;
        }

        return result;
    }

    [[nodiscard]] constexpr static size_type _compare(const_pointer first1, const_pointer last1,
                                                      const_pointer first2, const_pointer last2)
    {
        while(first1 != last1 && first2 != last2)
        {
            if(*first1 < *first2)
            {
                return -1;
            }
            else if(*first1 > *first2)
            {
                return 1;
            }

            ++first1;
            ++first2;
        }

        if(first1 == last1 && first2 == last2)
        {
            return 0;
        }

        if(first1 == last1)
        {
            return -1;
        }

        return 1;
    }
};


template<int MaxSize>
class string : public istring
{
    static_assert(MaxSize > 0);

public:
    constexpr string() :
        istring(_buffer, 0, MaxSize)
    {
        _data[0] = 0;
    }

    constexpr string(const string& other) :
        string()
    {
        append(other);
    }

    constexpr string(const istring& other) :
        string()
    {
        append(other);
    }

    constexpr string(const istring& other, size_type position, size_type length) :
        string()
    {
        append(other, position, length);
    }

    constexpr string(const_pointer str) :
        string()
    {
        append(str);
    }

    constexpr string(const_pointer str, size_type str_size) :
        string()
    {
        append(str, str_size);
    }

    constexpr string(size_type count, value_type value) :
        string()
    {
        append(count, value);
    }

    constexpr string(const_iterator first, const_iterator last) :
        string()
    {
        append(first, last);
    }

    constexpr string& operator=(const string& other)
    {
        btn::copy(other.begin(), other.end(), begin());
        _size = other.size();
        _data[_size] = 0;
        return *this;
    }

    constexpr string& operator=(const istring& other)
    {
        istring::operator=(other);
        return *this;
    }

    constexpr string& operator=(const_pointer str)
    {
        istring::operator=(str);
        return *this;
    }

    [[nodiscard]] constexpr string substr(size_type position) const
    {
        string result;
        result.append(_data + position, end());
        return result;
    }

    [[nodiscard]] constexpr string substr(size_type position, size_type length) const
    {
        string result;
        result.append(_data + position, _data + position + length);
        return result;
    }

    [[nodiscard]] constexpr friend string operator+(const string& a, const istring& b)
    {
        string result = a;
        result.append(b);
        return result;
    }

    [[nodiscard]] constexpr friend string operator+=(const string& a, const_pointer b)
    {
        string result = a;
        result.append(b);
        return result;
    }

    [[nodiscard]] constexpr friend string operator+=(const string& a, value_type b)
    {
        string result = a;
        result.append(b);
        return result;
    }

private:
    alignas(int) char _buffer[MaxSize + 1];
};


template<>
struct hash<istring>
{
    [[nodiscard]] constexpr unsigned operator()(const istring& value) const
    {
        return array_hash(value.data(), value.size());
    }
};


template<int MaxSize>
string<MaxSize - 1> make_string(const char (&text)[MaxSize])
{
    return string<MaxSize - 1>(text, MaxSize - 1);
}


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
