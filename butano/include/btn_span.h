#ifndef BTN_SPAN_H
#define BTN_SPAN_H

#include "btn_array.h"
#include "btn_span_fwd.h"

namespace btn
{

template<typename Type>
class span
{

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

    constexpr span() = default;

    constexpr span(pointer ptr, size_type size) :
        _begin(ptr),
        _end(_build_end(ptr, size))
    {
    }

    constexpr span(pointer first, pointer last) :
        _begin(first),
        _end(last)
    {
        BTN_CONSTEXPR_ASSERT((! first && ! last) || (first && last), "Both pointers must be null or not null");
        BTN_CONSTEXPR_ASSERT(first <= last, "Last is before first");
    }

    template<size_type ArraySize>
    constexpr span(value_type (&array)[ArraySize]) :
        _begin(array),
        _end(array + ArraySize)
    {
    }

    template<class OtherType, size_type ArraySize>
    constexpr span(const array<OtherType, ArraySize>& array) :
        _begin(array.begin()),
        _end(array.end())
    {
    }

    template<class OtherType, size_type ArraySize>
    constexpr span(array<OtherType, ArraySize>& array) :
        _begin(array.begin()),
        _end(array.end())
    {
    }

    template<class OtherType>
    constexpr span(const span<OtherType>& other) :
        _begin(other.begin()),
        _end(other.end())
    {
    }

    [[nodiscard]] constexpr const_iterator begin() const
    {
        return _begin;
    }

    [[nodiscard]] constexpr iterator begin()
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

    [[nodiscard]] constexpr iterator end()
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

    [[nodiscard]] constexpr reverse_iterator rbegin()
    {
        return reverse_iterator(_end);
    }

    [[nodiscard]] constexpr const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(_end);
    }

    [[nodiscard]] constexpr const_reverse_iterator rend() const
    {
        return const_reverse_iterator(_begin);
    }

    [[nodiscard]] constexpr reverse_iterator rend()
    {
        return reverse_iterator(_begin);
    }

    [[nodiscard]] constexpr const_reverse_iterator crend() const
    {
        return const_reverse_iterator(_begin);
    }

    [[nodiscard]] constexpr const_reference front() const
    {
        BTN_CONSTEXPR_ASSERT(! empty(), "Is empty");

        return *_begin;
    }

    [[nodiscard]] constexpr reference front()
    {
        BTN_CONSTEXPR_ASSERT(! empty(), "Is empty");

        return *_begin;
    }

    [[nodiscard]] constexpr const_reference back() const
    {
        BTN_CONSTEXPR_ASSERT(! empty(), "Is empty");

        return *(_end - 1);
    }

    [[nodiscard]] constexpr reference back()
    {
        BTN_CONSTEXPR_ASSERT(! empty(), "Is empty");

        return *(_end - 1);
    }

    [[nodiscard]] constexpr reference operator[](size_type index)
    {
        BTN_CONSTEXPR_ASSERT(index >= 0 && index < size(), "Invalid index");

        return _begin[index];
    }

    [[nodiscard]] constexpr const_reference operator[](size_type index) const
    {
        BTN_CONSTEXPR_ASSERT(index >= 0 && index < size(), "Invalid index");

        return _begin[index];
    }

    [[nodiscard]] constexpr const_pointer data() const
    {
        return _begin;
    }

    [[nodiscard]] constexpr pointer data()
    {
        return _begin;
    }

    [[nodiscard]] constexpr bool empty() const
    {
        return _begin == _end;
    }

    [[nodiscard]] constexpr size_type size() const
    {
        return _end - _begin;
    }

    [[nodiscard]] constexpr size_type size_bytes() const
    {
        return (_end - _begin) * sizeof(value_type);
    }

    [[nodiscard]] constexpr span first(size_type count) const
    {
        BTN_CONSTEXPR_ASSERT(count >= 0 && count <= size(), "Invalid count");

        return span(_begin, _begin + count);
    }

    [[nodiscard]] constexpr span last(size_type count) const
    {
        BTN_CONSTEXPR_ASSERT(count >= 0 && count <= size(), "Invalid count");

        return span(_end - count, _end);
    }

    [[nodiscard]] constexpr span subspan(size_type offset) const
    {
        BTN_CONSTEXPR_ASSERT(offset >= 0 && offset <= size(), "Invalid offset");

        return span(_begin + offset, _end);
    }

    [[nodiscard]] constexpr span subspan(size_type offset, size_type count) const
    {
        BTN_CONSTEXPR_ASSERT(offset >= 0, "Invalid offset");
        BTN_CONSTEXPR_ASSERT(count >= 0, "Invalid count");
        BTN_CONSTEXPR_ASSERT(offset + count <= size(), "Invalid offset or count");

        pointer new_begin = _begin + offset;
        return span(new_begin, new_begin + count);
    }

    constexpr void swap(span& other)
    {
        btn::swap(_begin, other._begin);
        btn::swap(_end, other._end);
    }

    constexpr friend void swap(span& a, span& b)
    {
        a.swap(b);
    }

    [[nodiscard]] constexpr friend bool operator==(const span& a, const span& b)
    {
        if(a.size() != b.size())
        {
            return false;
        }

        return equal(a.begin(), a.end(), b.begin());
    }

    [[nodiscard]] constexpr friend bool operator!=(const span& a, const span& b)
    {
        return ! (a == b);
    }

    [[nodiscard]] constexpr friend bool operator<(const span& a, const span& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    [[nodiscard]] constexpr friend bool operator>(const span& a, const span& b)
    {
        return b < a;
    }

    [[nodiscard]] constexpr friend bool operator<=(const span& a, const span& b)
    {
        return ! (a > b);
    }

    [[nodiscard]] constexpr friend bool operator>=(const span& a, const span& b)
    {
        return ! (a < b);
    }

private:
    pointer _begin = nullptr;
    pointer _end = nullptr;

    [[nodiscard]] static constexpr pointer _build_end(pointer ptr, size_type size)
    {
        BTN_CONSTEXPR_ASSERT(size >= 0, "Invalid size");
        BTN_CONSTEXPR_ASSERT(ptr || ! size, "Pointer is null and size is not zero");

        return ptr + size;
    }
};

}

#endif
