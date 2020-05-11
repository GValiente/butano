#ifndef BTN_STRING_VIEW_H
#define BTN_STRING_VIEW_H

#include "btn_assert.h"
#include "btn_limits.h"
#include "btn_utility.h"
#include "btn_iterator.h"
#include "btn_algorithm.h"
#include "btn_functional.h"
#include "btn_istring_base.h"

namespace btn
{

class string_view
{

public:
    using value_type = char;
    using size_type = int;
    using const_reference = const char&;
    using const_pointer = const char*;
    using pointer = char*;
    using const_iterator = const char*;
    using const_reverse_iterator = reverse_iterator<const_iterator>;

    constexpr static size_type npos = numeric_limits<size_type>::max();

    constexpr string_view() = default;

    constexpr string_view(const istring_base& str) :
        _begin(str.data()),
        _end(str.data() + str.size())
    {
    }

    constexpr string_view(const_pointer str) :
        _begin(str),
        _end(str)
    {
        if(str)
        {
            while(*str++)
            {
                ++_end;
            }
        }
    }

    constexpr string_view(const_pointer str, size_type size) :
        _begin(str),
        _end(str + size)
    {
        BTN_CONSTEXPR_ASSERT(size >= 0, "Invalid size");
    }

    [[nodiscard]] constexpr const_reference front() const
    {
        BTN_CONSTEXPR_ASSERT(! empty(), "String view is empty");

        return *_begin;
    }

    [[nodiscard]] constexpr const_reference back() const
    {
        BTN_CONSTEXPR_ASSERT(! empty(), "String view is empty");

        return *(_end - 1);
    }

    [[nodiscard]] constexpr const_pointer data() const
    {
        return _begin;
    }

    [[nodiscard]] constexpr const_iterator begin() const
    {
        return _begin;
    }

    [[nodiscard]] constexpr const_iterator cbegin() const
    {
        return _begin;
    }

    [[nodiscard]] constexpr const_iterator end() const
    {
        return _end;
    }

    [[nodiscard]] constexpr const_iterator cend() const
    {
        return _end;
    }

    [[nodiscard]] constexpr const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(_end);
    }

    [[nodiscard]] constexpr const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(_end);
    }

    [[nodiscard]] constexpr const_reverse_iterator rend() const
    {
        return const_reverse_iterator(_begin);
    }

    [[nodiscard]] constexpr const_reverse_iterator crend() const
    {
        return const_reverse_iterator(_begin);
    }

    [[nodiscard]] constexpr bool empty() const
    {
        return _begin == _end;
    }

    [[nodiscard]] constexpr size_type size() const
    {
        return size_type(_end - _begin);
    }

    [[nodiscard]] constexpr size_type length() const
    {
        return size();
    }

    [[nodiscard]] constexpr const_reference operator[](size_type position) const
    {
        BTN_CONSTEXPR_ASSERT(position >= 0 && position < size(), "Invalid position");

        return _begin[position];
    }

    [[nodiscard]] constexpr const_reference at(size_type position) const
    {
        BTN_CONSTEXPR_ASSERT(position >= 0 && position < size(), "Invalid position");

        return _begin[position];
    }

    constexpr size_type copy(pointer destination, size_type count) const
    {
        BTN_CONSTEXPR_ASSERT(destination, "Destination is null");
        BTN_CONSTEXPR_ASSERT(count >= 0, "Invalid count");

        size_type n = 0;

        if(size_type sz = size())
        {
            n = min(count, sz);
            btn::copy(_begin, _begin + n, destination);
        }

        return n;
    }

    constexpr size_type copy(pointer destination, size_type count, size_type position) const
    {
        BTN_CONSTEXPR_ASSERT(destination, "Destination is null");
        BTN_CONSTEXPR_ASSERT(count >= 0, "Invalid count");
        BTN_CONSTEXPR_ASSERT(position >= 0, "Invalid position");

        size_type sz = size();
        size_type n = 0;

        if(position < sz)
        {
            n = min(count, sz - position);
            btn::copy(_begin + position, _begin + position + n, destination);
        }

        return n;
    }

    [[nodiscard]] constexpr string_view substr() const
    {
        return *this;
    }

    [[nodiscard]] constexpr string_view substr(size_type position) const
    {
        BTN_CONSTEXPR_ASSERT(position >= 0, "Invalid position");

        size_type sz = size();
        string_view view;

        if(position < sz)
        {
            view = string_view(_begin + position, sz - position);
        }

        return view;
    }

    [[nodiscard]] constexpr string_view substr(size_type position, size_type count) const
    {
        BTN_CONSTEXPR_ASSERT(count >= 0, "Invalid count");
        BTN_CONSTEXPR_ASSERT(position >= 0, "Invalid position");

        size_type sz = size();
        string_view view;

        if(position < sz)
        {
            size_type n = min(count, sz - position);
            view = string_view(_begin + position, n);
        }

        return view;
    }

    constexpr void remove_prefix(size_type n)
    {
        BTN_CONSTEXPR_ASSERT(n <= size(), "Invalid n");

        if(n)
        {
            _begin += n;
        }
    }

    constexpr void remove_suffix(size_type n)
    {
        BTN_CONSTEXPR_ASSERT(n <= size(), "Invalid n");

        if(n)
        {
            _end -= n;
        }
    }

    [[nodiscard]] constexpr size_type compare(const string_view& other) const
    {
        return *this == other ?
                    0 :
                    *this > other ?
                        1 : -1;
    }

    [[nodiscard]] constexpr size_type compare(size_type position, size_type count, const string_view& other) const
    {
        return substr(position, count).compare(other);
    }

    [[nodiscard]] constexpr size_type compare(size_type position1, size_type count1, const string_view& other,
                                        size_type position2, size_type count2) const
    {
        return substr(position1, count1).compare(other.substr(position2, count2));
    }

    [[nodiscard]] constexpr bool starts_with(const string_view& other) const
    {
        return size() >= other.size() && compare(0, other.size(), other) == 0;
    }

    [[nodiscard]] constexpr bool starts_with(value_type value) const
    {
        return ! empty() && front() == value;
    }

    [[nodiscard]] constexpr bool ends_with(const string_view& other) const
    {
        return size() >= other.size() && compare(size() - other.size(), npos, other) == 0;
    }

    [[nodiscard]] constexpr bool ends_with(value_type value) const
    {
        return ! empty() && back() == value;
    }

    constexpr void swap(string_view& other)
    {
        btn::swap(_begin, other._begin);
        btn::swap(_end, other._end);
    }

    constexpr friend void swap(string_view& a, string_view& b)
    {
        a.swap(b);
    }

    [[nodiscard]] constexpr friend bool operator==(const string_view& a, const string_view& b)
    {
        if(a.size() != b.size())
        {
            return false;
        }

        return equal(a.begin(), a.end(), b.begin());
    }

    [[nodiscard]] constexpr friend bool operator!=(const string_view& a, const string_view& b)
    {
        return ! (a == b);
    }

    [[nodiscard]] constexpr friend bool operator<(const string_view& a, const string_view& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    [[nodiscard]] constexpr friend bool operator>(const string_view& a, const string_view& b)
    {
        return b < a;
    }

    [[nodiscard]] constexpr friend bool operator<=(const string_view& a, const string_view& b)
    {
        return ! (a > b);
    }

    [[nodiscard]] constexpr friend bool operator>=(const string_view& a, const string_view& b)
    {
        return ! (a < b);
    }

private:
    const_pointer _begin = nullptr;
    const_pointer _end = nullptr;
};


template<>
struct hash<string_view>
{
    [[nodiscard]] constexpr unsigned operator()(const string_view& value) const
    {
        return array_hash(value.data(), value.size());
    }
};

}

#endif
