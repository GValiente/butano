#ifndef BTN_STRING_VIEW_H
#define BTN_STRING_VIEW_H

#include "btn_string.h"
#include "btn_limits.h"
#include "btn_memory.h"
#include "btn_iterator.h"
#include "btn_algorithm.h"

namespace btn
{

class string_view
{

public:
    using value_type = char;
    using size_type = size_t;
    using const_reference = const char&;
    using const_pointer = const char*;
    using pointer = char*;
    using const_iterator = const char*;
    using const_reverse_iterator = reverse_iterator<const_iterator>;

    constexpr static size_type npos = size_type(-1);

    constexpr string_view() = default;

    string_view(const istring& str) :
        _begin(str.begin()),
        _end(str.end())
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

    [[nodiscard]] constexpr const_reference operator[](size_t pos) const
    {
        BTN_CONSTEXPR_ASSERT(pos < size(), "Invalid pos");

        return _begin[pos];
    }

    [[nodiscard]] constexpr const_reference at(size_t pos) const
    {
        BTN_CONSTEXPR_ASSERT(pos < size(), "Invalid pos");

        return _begin[pos];
    }

    constexpr void swap(string_view& other)
    {
        string_view temp = *this;
        *this = other;
        other = temp;
    }

    size_type copy(pointer destination, size_type count, size_type pos = 0) const
    {
        BTN_ASSERT(destination, "Destination is null");

        size_type sz = size();
        size_type n = 0;

        if(pos < sz)
        {
            n = min(count, sz - pos);
            memory::copy(_begin[pos], int(n), *destination);
        }

        return n;
    }

    [[nodiscard]] constexpr string_view substr(size_type pos = 0, size_type count = npos) const
    {
        size_type sz = size();
        string_view view;

        if(pos < sz)
        {
            size_t n = min(count, sz - pos);
            view = string_view(_begin + pos, n);
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

    [[nodiscard]] constexpr int compare(const string_view& other) const
    {
        return *this == other ?
                    0 :
                    *this > other ?
                        1 : -1;
    }

    [[nodiscard]] constexpr int compare(size_type pos, size_type count, const string_view& other) const
    {
        return substr(pos, count).compare(other);
    }

    [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, const string_view& other, size_type pos2,
                                        size_type count2) const
    {
        return substr(pos1, count1).compare(other.substr(pos2, count2));
    }

    [[nodiscard]] constexpr bool starts_with(const string_view& other) const
    {
        return size() >= other.size() && compare(0, other.size(), other) == 0;
    }

    [[nodiscard]] constexpr bool starts_with(char c) const
    {
        return ! empty() && front() == c;
    }

    [[nodiscard]] constexpr bool ends_with(const string_view& other) const
    {
        return size() >= other.size() && compare(size() - other.size(), npos, other) == 0;
    }

    [[nodiscard]] constexpr bool ends_with(char c) const
    {
        return ! empty() && back() == c;
    }

    [[nodiscard]] constexpr bool operator==(const string_view& other) const
    {
        return size() == other.size() && _equal_characters(_begin, other._begin);
    }

    [[nodiscard]] constexpr bool operator!=(const string_view& other) const
    {
        return ! (*this == other);
    }

    [[nodiscard]] constexpr bool operator<(const string_view& other) const
    {
        return _lexicographical_compare(begin(), end(), other.begin(), other.end());
    }

    [[nodiscard]] constexpr bool operator>(const string_view& other) const
    {
        return other < *this;
    }

    [[nodiscard]] constexpr bool operator<=(const string_view& other) const
    {
        return ! (*this > other);
    }

    [[nodiscard]] constexpr bool operator>=(const string_view& other) const
    {
        return ! (*this < other);
    }

private:
    const_pointer _begin = nullptr;
    const_pointer _end = nullptr;

    [[nodiscard]] static constexpr bool _equal_characters(const_pointer a, const_pointer b)
    {
        return *a == *b && (*a == '\0' || _equal_characters(a + 1, b + 1));
    }

    [[nodiscard]] static constexpr bool _lexicographical_compare(const_pointer first1, const_pointer last1,
                                                                 const_pointer first2, const_pointer last2)
    {
        for(; first1 != last1 && first2 != last2; ++first1, ++first2)
        {
            if(*first1 < *first2)
            {
                return true;
            }

            if(*first2 < *first1)
            {
                return false;
            }
        }

        return first1 == last1 && first2 != last2;
    }
};

template<>
struct hash<string_view>
{
    [[nodiscard]] constexpr size_t operator()(const string_view& value) const
    {
        return array_hash(value.data(), value.size());
    }
};

}

#endif
