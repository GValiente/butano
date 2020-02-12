#ifndef BTN_ARRAY_H
#define BTN_ARRAY_H

#include "btn_assert.h"
#include "btn_utility.h"
#include "btn_iterator.h"
#include "btn_algorithm.h"
#include "btn_array_fwd.h"

namespace btn
{

template<typename Type, int Size>
class array
{
    static_assert(Size > 0);

public:
    using value_type = Type;
    using size_type = int;
    using reference = Type&;
    using const_reference = const Type&;
    using pointer = Type*;
    using const_pointer = const Type*;
    using iterator = Type*;
    using const_iterator = const Type*;
    using reverse_iterator = btn::reverse_iterator<iterator>;
    using const_reverse_iterator = btn::reverse_iterator<const_iterator>;

    [[nodiscard]] constexpr pointer data()
    {
        return &_data[0];
    }

    [[nodiscard]] constexpr const_pointer data() const
    {
        return &_data[0];
    }

    [[nodiscard]] constexpr size_type size() const
    {
        return Size;
    }

    [[nodiscard]] constexpr const_reference operator[](size_type index) const
    {
        BTN_CONSTEXPR_ASSERT(index >= 0 && index < Size, "Invalid index");

        return _data[index];
    }

    [[nodiscard]] constexpr reference operator[](size_type index)
    {
        BTN_CONSTEXPR_ASSERT(index >= 0 && index < Size, "Invalid index");

        return _data[index];
    }

    [[nodiscard]] constexpr const_reference at(size_type index) const
    {
        BTN_CONSTEXPR_ASSERT(index >= 0 && index < Size, "Invalid index");

        return _data[index];
    }

    [[nodiscard]] constexpr reference at(size_type index)
    {
        BTN_CONSTEXPR_ASSERT(index >= 0 && index < Size, "Invalid index");

        return _data[index];
    }

    [[nodiscard]] constexpr const_reference front() const
    {
        return _data[0];
    }

    [[nodiscard]] constexpr reference front()
    {
        return _data[0];
    }

    [[nodiscard]] constexpr const_reference back() const
    {
        return _data[Size - 1];
    }

    [[nodiscard]] constexpr reference back()
    {
        return _data[Size - 1];
    }

    [[nodiscard]] constexpr iterator begin()
    {
        return &_data[0];
    }

    [[nodiscard]] constexpr const_iterator begin() const
    {
        return &_data[0];
    }

    [[nodiscard]] constexpr const_iterator cbegin() const
    {
        return begin();
    }

    [[nodiscard]] constexpr iterator end()
    {
        return &_data[Size];
    }

    [[nodiscard]] constexpr const_iterator end() const
    {
        return &_data[Size];
    }

    [[nodiscard]] constexpr const_iterator cend() const
    {
        return &_data[Size];
    }

    [[nodiscard]] constexpr reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    [[nodiscard]] constexpr const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }

    [[nodiscard]] constexpr const_reverse_iterator crbegin() const
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

    [[nodiscard]] constexpr const_reverse_iterator crend() const
    {
        return const_reverse_iterator(begin());
    }

    constexpr void fill(const_reference value)
    {
        btn::fill(begin(), end(), value);
    }

    constexpr void swap(array& other)
    {
        pointer data = _data;
        pointer other_data = other._data;

        for(size_type index = 0; index < Size; ++index)
        {
            btn::swap(data[index], other_data[index]);
        }
    }

    constexpr friend void swap(array& a, array& b)
    {
        a.swap(b);
    }

    [[nodiscard]] constexpr friend bool operator==(const array& a, const array& b)
    {
        if(a.size() != b.size())
        {
            return false;
        }

        return equal(a.begin(), a.end(), b.begin());
    }

    [[nodiscard]] constexpr friend bool operator!=(const array& a, const array& b)
    {
        return ! (a == b);
    }

    [[nodiscard]] constexpr friend bool operator<(const array& a, const array& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    [[nodiscard]] constexpr friend bool operator>(const array& a, const array& b)
    {
        return b < a;
    }

    [[nodiscard]] constexpr friend bool operator<=(const array& a, const array& b)
    {
        return ! (a > b);
    }

    [[nodiscard]] constexpr friend bool operator>=(const array& a, const array& b)
    {
        return ! (a < b);
    }

    template<size_type Index>
    [[nodiscard]] constexpr const_reference get(const array& array)
    {
        static_assert(Index >= 0 && Index < Size);

        return array._data[Index];
    }

    template<size_type Index>
    [[nodiscard]] constexpr reference get(array& array)
    {
        static_assert(Index >= 0 && Index < Size);

        return array._data[Index];
    }

    Type _data[Size];
};

}

#endif
