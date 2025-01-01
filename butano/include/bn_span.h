/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPAN_H
#define BN_SPAN_H

/**
 * @file
 * bn::span implementation header file.
 *
 * @ingroup span
 */

#include "bn_array.h"
#include "bn_span_fwd.h"

namespace bn
{

template<typename Type>
class span
{

public:
    using value_type = Type; //!< Value type alias.
    using size_type = int; //!< Size type alias.
    using difference_type = int; //!< Difference type alias.
    using reference = Type&; //!< Reference alias.
    using const_reference = const Type&; //!< Const reference alias.
    using pointer = Type*; //!< Pointer alias.
    using const_pointer = const Type*; //!< Const pointer alias.
    using iterator = Type*; //!< Iterator alias.
    using const_iterator = const Type*; //!< Const iterator alias.
    using reverse_iterator = bn::reverse_iterator<iterator>; //!< Reverse iterator alias.
    using const_reverse_iterator = bn::reverse_iterator<const_iterator>; //!< Const reverse iterator alias.

    /**
     * @brief Constructs an empty span.
     */
    constexpr span() = default;

    /**
     * @brief Constructs a span that is a view over the range [ptr, ptr + size).
     * @param ptr Pointer to the first element.
     * @param size Elements count.
     */
    constexpr span(pointer ptr, size_type size) :
        _begin(ptr),
        _end(_build_end(ptr, size))
    {
    }

    /**
     * @brief Constructs a span that is a view over the range [first, last).
     * @param first Pointer to the first element.
     * @param last Pointer to the element after the last.
     */
    constexpr span(pointer first, pointer last) :
        _begin(first),
        _end(last)
    {
        BN_ASSERT((! first && ! last) || (first && last),
                   "Both pointers must be null or not null: ", first, " - ", last);
        BN_ASSERT(first <= last, "Last is before first: ", first, " - ", last);
    }

    /**
     * @brief Constructs a span that is a view over the given array.
     */
    template<size_type ArraySize>
    constexpr span(value_type (&array)[ArraySize]) :
        _begin(array),
        _end(array + ArraySize)
    {
    }

    /**
     * @brief Constructs a span that is a const view over the given bn::array.
     */
    template<class OtherType, size_type ArraySize>
    constexpr span(const array<OtherType, ArraySize>& array) :
        _begin(array.begin()),
        _end(array.end())
    {
    }

    /**
     * @brief Constructs a span that is a non-const view over the given bn::array.
     */
    template<class OtherType, size_type ArraySize>
    constexpr span(array<OtherType, ArraySize>& array) :
        _begin(array.begin()),
        _end(array.end())
    {
    }

    /**
     * @brief Constructs a span that is a view over a span of another type.
     */
    template<class OtherType>
    constexpr span(const span<OtherType>& other) :
        _begin(other.begin()),
        _end(other.end())
    {
    }

    /**
     * @brief Returns a const pointer to the beginning of the referenced data.
     */
    [[nodiscard]] constexpr const_pointer data() const
    {
        return _begin;
    }

    /**
     * @brief Returns a pointer to the beginning of the referenced data.
     */
    [[nodiscard]] constexpr pointer data()
    {
        return _begin;
    }

    /**
     * @brief Returns the number of referenced elements.
     */
    [[nodiscard]] constexpr size_type size() const
    {
        return _end - _begin;
    }

    /**
     * @brief Returns the number of referenced bytes.
     */
    [[nodiscard]] constexpr size_type size_bytes() const
    {
        return size() * size_type(sizeof(value_type));
    }

    /**
     * @brief Indicates if it doesn't reference any element.
     */
    [[nodiscard]] constexpr bool empty() const
    {
        return _begin == _end;
    }

    /**
     * @brief Returns a const iterator to the beginning of the span.
     */
    [[nodiscard]] constexpr const_iterator begin() const
    {
        return _begin;
    }

    /**
     * @brief Returns an iterator to the beginning of the span.
     */
    [[nodiscard]] constexpr iterator begin()
    {
        return _begin;
    }

    /**
     * @brief Returns a const iterator to the beginning of the span.
     */
    [[nodiscard]] constexpr const_iterator cbegin() const
    {
        return _begin;
    }

    /**
     * @brief Returns a const iterator to the end of the span.
     */
    [[nodiscard]] constexpr const_iterator end() const
    {
        return _end;
    }

    /**
     * @brief Returns an iterator to the end of the span.
     */
    [[nodiscard]] constexpr iterator end()
    {
        return _end;
    }

    /**
     * @brief Returns a const iterator to the end of the span.
     */
    [[nodiscard]] constexpr const_iterator cend() const
    {
        return _end;
    }

    /**
     * @brief Returns a const reverse iterator to the end of the span.
     */
    [[nodiscard]] constexpr const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(_end);
    }

    /**
     * @brief Returns a reverse iterator to the end of the span.
     */
    [[nodiscard]] constexpr reverse_iterator rbegin()
    {
        return reverse_iterator(_end);
    }

    /**
     * @brief Returns a const reverse iterator to the end of the span.
     */
    [[nodiscard]] constexpr const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(_end);
    }

    /**
     * @brief Returns a const reverse iterator to the begin of the span.
     */
    [[nodiscard]] constexpr const_reverse_iterator rend() const
    {
        return const_reverse_iterator(_begin);
    }

    /**
     * @brief Returns a reverse iterator to the begin of the span.
     */
    [[nodiscard]] constexpr reverse_iterator rend()
    {
        return reverse_iterator(_begin);
    }

    /**
     * @brief Returns a const reverse iterator to the begin of the span.
     */
    [[nodiscard]] constexpr const_reverse_iterator crend() const
    {
        return const_reverse_iterator(_begin);
    }

    /**
     * @brief Returns a const reference to the first element.
     */
    [[nodiscard]] constexpr const_reference front() const
    {
        BN_BASIC_ASSERT(! empty(), "Is empty");

        return *_begin;
    }

    /**
     * @brief Returns a reference to the first element.
     */
    [[nodiscard]] constexpr reference front()
    {
        BN_BASIC_ASSERT(! empty(), "Is empty");

        return *_begin;
    }

    /**
     * @brief Returns a const reference to the last element.
     */
    [[nodiscard]] constexpr const_reference back() const
    {
        BN_BASIC_ASSERT(! empty(), "Is empty");

        return *(_end - 1);
    }

    /**
     * @brief Returns a reference to the last element.
     */
    [[nodiscard]] constexpr reference back()
    {
        BN_BASIC_ASSERT(! empty(), "Is empty");

        return *(_end - 1);
    }

    /**
     * @brief Returns a const reference to the value stored at the specified index.
     */
    [[nodiscard]] constexpr const_reference operator[](size_type index) const
    {
        BN_ASSERT(index >= 0 && index < size(), "Invalid index: ", index);

        return _begin[index];
    }

    /**
     * @brief Returns a reference to the value stored at the specified index.
     */
    [[nodiscard]] constexpr reference operator[](size_type index)
    {
        BN_ASSERT(index >= 0 && index < size(), "Invalid index: ", index);

        return _begin[index];
    }

    /**
     * @brief Returns a const reference to the value stored at the specified index.
     */
    [[nodiscard]] constexpr const_reference at(size_type index) const
    {
        BN_ASSERT(index >= 0 && index < size(), "Invalid index: ", index);

        return _begin[index];
    }

    /**
     * @brief Returns a reference to the value stored at the specified index.
     */
    [[nodiscard]] constexpr reference at(size_type index)
    {
        BN_ASSERT(index >= 0 && index < size(), "Invalid index: ", index);

        return _begin[index];
    }

    /**
     * @brief Returns a span that is a view over the first count elements of this span.
     */
    [[nodiscard]] constexpr span first(size_type count) const
    {
        BN_ASSERT(count >= 0 && count <= size(), "Invalid count: ", count);

        return span(_begin, _begin + count);
    }

    /**
     * @brief Returns a span that is a view over the last count elements of this span.
     */
    [[nodiscard]] constexpr span last(size_type count) const
    {
        BN_ASSERT(count >= 0 && count <= size(), "Invalid count: ", count);

        return span(_end - count, _end);
    }

    /**
     * @brief Returns a span that is a view over the elements of this span starting at offset.
     */
    [[nodiscard]] constexpr span subspan(size_type offset) const
    {
        BN_ASSERT(offset >= 0 && offset <= size(), "Invalid offset: ", offset);

        return span(_begin + offset, _end);
    }

    /**
     * @brief Obtains a span that is a view over the count elements of this span starting at offset.
     */
    [[nodiscard]] constexpr span subspan(size_type offset, size_type count) const
    {
        BN_ASSERT(offset >= 0, "Invalid offset: ", offset);
        BN_ASSERT(count >= 0, "Invalid count: ", count);
        BN_ASSERT(offset + count <= size(), "Invalid offset or count: ", offset, " - ", offset);

        pointer new_begin = _begin + offset;
        return span(new_begin, new_begin + count);
    }

    /**
     * @brief Exchanges the contents of this span with those of the other one.
     * @param other span to exchange the contents with.
     */
    constexpr void swap(span& other)
    {
        bn::swap(_begin, other._begin);
        bn::swap(_end, other._end);
    }

    /**
     * @brief Exchanges the contents of a span with those of another one.
     * @param a First span to exchange the contents with.
     * @param b Second span to exchange the contents with.
     */
    constexpr friend void swap(span& a, span& b)
    {
        a.swap(b);
    }

    /**
     * @brief Equal operator.
     * @param a First span to compare.
     * @param b Second span to compare.
     * @return `true` if the first span is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator==(const span& a, const span& b)
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
     * @param a First span to compare.
     * @param b Second span to compare.
     * @return `true` if the first span is lexicographically less than the second one,
     * otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator<(const span& a, const span& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    /**
     * @brief Greater than operator.
     * @param a First span to compare.
     * @param b Second span to compare.
     * @return `true` if the first span is lexicographically greater than the second one,
     * otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator>(const span& a, const span& b)
    {
        return b < a;
    }

    /**
     * @brief Less than or equal operator.
     * @param a First span to compare.
     * @param b Second span to compare.
     * @return `true` if the first span is lexicographically less than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator<=(const span& a, const span& b)
    {
        return ! (a > b);
    }

    /**
     * @brief Greater than or equal operator.
     * @param a First span to compare.
     * @param b Second span to compare.
     * @return `true` if the first span is lexicographically greater than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator>=(const span& a, const span& b)
    {
        return ! (a < b);
    }

private:
    pointer _begin = nullptr;
    pointer _end = nullptr;

    [[nodiscard]] static constexpr pointer _build_end(pointer ptr, size_type size)
    {
        BN_ASSERT(size >= 0, "Invalid size: ", size);
        BN_BASIC_ASSERT(ptr || ! size, "Pointer is null and size is not zero: ", size);

        return ptr + size;
    }
};

}

#endif
