/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_ARRAY_H
#define BN_ARRAY_H

/**
 * @file
 * bn::array implementation header file.
 *
 * @ingroup array
 */

#include "bn_assert.h"
#include "bn_utility.h"
#include "bn_iterator.h"
#include "bn_algorithm.h"
#include "bn_type_traits.h"
#include "bn_array_fwd.h"

namespace bn
{

template<typename Type, int Size>
class array
{
    static_assert(Size > 0);

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
     * @brief Returns a const pointer to the beginning of the array.
     */
    [[nodiscard]] constexpr const_pointer data() const
    {
        return &_data[0];
    }

    /**
     * @brief Returns a pointer to the beginning of the array.
     */
    [[nodiscard]] constexpr pointer data()
    {
        return &_data[0];
    }

    /**
     * @brief Returns the elements count.
     */
    [[nodiscard]] constexpr size_type size() const
    {
        return Size;
    }

    /**
     * @brief Returns the maximum possible elements count.
     */
    [[nodiscard]] constexpr size_type max_size() const
    {
        return Size;
    }

    /**
     * @brief Returns the remaining element capacity.
     */
    [[nodiscard]] constexpr size_type available() const
    {
        return 0;
    }

    /**
     * @brief Indicates if it doesn't contain any element.
     */
    [[nodiscard]] constexpr bool empty() const
    {
        return false;
    }

    /**
     * @brief Indicates if it can't contain any more elements.
     */
    [[nodiscard]] constexpr bool full() const
    {
        return true;
    }

    /**
     * @brief Returns a const iterator to the beginning of the array.
     */
    [[nodiscard]] constexpr const_iterator begin() const
    {
        return &_data[0];
    }

    /**
     * @brief Returns an iterator to the beginning of the array.
     */
    [[nodiscard]] constexpr iterator begin()
    {
        return &_data[0];
    }

    /**
     * @brief Returns a const iterator to the end of the array.
     */
    [[nodiscard]] constexpr const_iterator end() const
    {
        return &_data[Size];
    }

    /**
     * @brief Returns an iterator to the end of the array.
     */
    [[nodiscard]] constexpr iterator end()
    {
        return &_data[Size];
    }

    /**
     * @brief Returns a const iterator to the beginning of the array.
     */
    [[nodiscard]] constexpr const_iterator cbegin() const
    {
        return begin();
    }

    /**
     * @brief Returns a const iterator to the end of the array.
     */
    [[nodiscard]] constexpr const_iterator cend() const
    {
        return end();
    }

    /**
     * @brief Returns a const reverse iterator to the end of the array.
     */
    [[nodiscard]] constexpr const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }

    /**
     * @brief Returns a reverse iterator to the end of the array.
     */
    [[nodiscard]] constexpr reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    /**
     * @brief Returns a const reverse iterator to the beginning of the array.
     */
    [[nodiscard]] constexpr const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }

    /**
     * @brief Returns a reverse iterator to the beginning of the array.
     */
    [[nodiscard]] constexpr reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    /**
     * @brief Returns a const reverse iterator to the end of the array.
     */
    [[nodiscard]] constexpr const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(end());
    }

    /**
     * @brief Returns a const reverse iterator to the beginning of the array.
     */
    [[nodiscard]] constexpr const_reverse_iterator crend() const
    {
        return const_reverse_iterator(begin());
    }

    /**
     * @brief Returns a const reference to the value stored at the specified index.
     */
    [[nodiscard]] constexpr const_reference operator[](size_type index) const
    {
        BN_ASSERT(index >= 0 && index < Size, "Invalid index: ", index);

        return _data[index];
    }

    /**
     * @brief Returns a reference to the value stored at the specified index.
     */
    [[nodiscard]] constexpr reference operator[](size_type index)
    {
        BN_ASSERT(index >= 0 && index < Size, "Invalid index: ", index);

        return _data[index];
    }

    /**
     * @brief Returns a const reference to the value stored at the specified index.
     */
    [[nodiscard]] constexpr const_reference at(size_type index) const
    {
        BN_ASSERT(index >= 0 && index < Size, "Invalid index: ", index);

        return _data[index];
    }

    /**
     * @brief Returns a reference to the value stored at the specified index.
     */
    [[nodiscard]] constexpr reference at(size_type index)
    {
        BN_ASSERT(index >= 0 && index < Size, "Invalid index: ", index);

        return _data[index];
    }

    /**
     * @brief Returns a const reference to the first element.
     */
    [[nodiscard]] constexpr const_reference front() const
    {
        return _data[0];
    }

    /**
     * @brief Returns a reference to the first element.
     */
    [[nodiscard]] constexpr reference front()
    {
        return _data[0];
    }

    /**
     * @brief Returns a const reference to the last element.
     */
    [[nodiscard]] constexpr const_reference back() const
    {
        return _data[Size - 1];
    }

    /**
     * @brief Returns a reference to the last element.
     */
    [[nodiscard]] constexpr reference back()
    {
        return _data[Size - 1];
    }

    /**
     * @brief Fills the array with the given value.
     */
    constexpr void fill(const_reference value)
    {
        bn::fill(begin(), end(), value);
    }

    /**
     * @brief Exchanges the contents of this array with those of the other one.
     * @param other array to exchange the contents with.
     */
    constexpr void swap(array& other)
    {
        pointer data = _data;
        pointer other_data = other._data;

        for(size_type index = 0; index < Size; ++index)
        {
            bn::swap(data[index], other_data[index]);
        }
    }

    /**
     * @brief Exchanges the contents of an array with those of another one.
     * @param a First array to exchange the contents with.
     * @param b Second array to exchange the contents with.
     */
    constexpr friend void swap(array& a, array& b)
    {
        a.swap(b);
    }

    /**
     * @brief Equal operator.
     * @param a First array to compare.
     * @param b Second array to compare.
     * @return `true` if the first array is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator==(const array& a, const array& b)
    {
        if(a.size() != b.size())
        {
            return false;
        }

        return equal(a.begin(), a.end(), b.begin());
    }

    /**
     * @brief Less than operator.
     * @param a First array to compare.
     * @param b Second array to compare.
     * @return `true` if the first array is lexicographically less than the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator<(const array& a, const array& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    /**
     * @brief Greater than operator.
     * @param a First array to compare.
     * @param b Second array to compare.
     * @return `true` if the first array is lexicographically greater than the second one,
     * otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator>(const array& a, const array& b)
    {
        return b < a;
    }

    /**
     * @brief Less than or equal operator.
     * @param a First array to compare.
     * @param b Second array to compare.
     * @return `true` if the first array is lexicographically less than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator<=(const array& a, const array& b)
    {
        return ! (a > b);
    }

    /**
     * @brief Greater than or equal operator.
     * @param a First array to compare.
     * @param b Second array to compare.
     * @return `true` if the first array is lexicographically greater than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator>=(const array& a, const array& b)
    {
        return ! (a < b);
    }

    Type _data[Size]; //!< (Not so) internal data.
};


/**
 * @brief Empty array template specialization.
 *
 * @tparam Type Element type.
 */
template<typename Type>
class array<Type, 0>
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
     * @brief Returns a const pointer to the beginning of the array.
     */
    [[nodiscard]] constexpr const_pointer data() const
    {
        return nullptr;
    }

    /**
     * @brief Returns a pointer to the beginning of the array.
     */
    [[nodiscard]] constexpr pointer data()
    {
        return nullptr;
    }

    /**
     * @brief Returns the elements count.
     */
    [[nodiscard]] constexpr size_type size() const
    {
        return 0;
    }

    /**
     * @brief Returns the maximum possible elements count.
     */
    [[nodiscard]] constexpr size_type max_size() const
    {
        return 0;
    }

    /**
     * @brief Returns the remaining element capacity.
     */
    [[nodiscard]] constexpr size_type available() const
    {
        return 0;
    }

    /**
     * @brief Indicates if it doesn't contain any element.
     */
    [[nodiscard]] constexpr bool empty() const
    {
        return true;
    }

    /**
     * @brief Indicates if it can't contain any more elements.
     */
    [[nodiscard]] constexpr bool full() const
    {
        return true;
    }

    /**
     * @brief Returns a const iterator to the beginning of the array.
     */
    [[nodiscard]] constexpr const_iterator begin() const
    {
        return nullptr;
    }

    /**
     * @brief Returns an iterator to the beginning of the array.
     */
    [[nodiscard]] constexpr iterator begin()
    {
        return nullptr;
    }

    /**
     * @brief Returns a const iterator to the end of the array.
     */
    [[nodiscard]] constexpr const_iterator end() const
    {
        return nullptr;
    }

    /**
     * @brief Returns an iterator to the end of the array.
     */
    [[nodiscard]] constexpr iterator end()
    {
        return nullptr;
    }

    /**
     * @brief Returns a const iterator to the beginning of the array.
     */
    [[nodiscard]] constexpr const_iterator cbegin() const
    {
        return begin();
    }

    /**
     * @brief Returns a const iterator to the end of the array.
     */
    [[nodiscard]] constexpr const_iterator cend() const
    {
        return end();
    }

    /**
     * @brief Returns a const reverse iterator to the end of the array.
     */
    [[nodiscard]] constexpr const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }

    /**
     * @brief Returns a reverse iterator to the end of the array.
     */
    [[nodiscard]] constexpr reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    /**
     * @brief Returns a const reverse iterator to the beginning of the array.
     */
    [[nodiscard]] constexpr const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }

    /**
     * @brief Returns a reverse iterator to the beginning of the array.
     */
    [[nodiscard]] constexpr reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    /**
     * @brief Returns a const reverse iterator to the end of the array.
     */
    [[nodiscard]] constexpr const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(end());
    }

    /**
     * @brief Returns a const reverse iterator to the beginning of the array.
     */
    [[nodiscard]] constexpr const_reverse_iterator crend() const
    {
        return const_reverse_iterator(begin());
    }

    /**
     * @brief Fills the array with the given value.
     */
    constexpr void fill([[maybe_unused]] const_reference value)
    {
    }

    /**
     * @brief Exchanges the contents of this array with those of the other one.
     * @param other array to exchange the contents with.
     */
    constexpr void swap([[maybe_unused]] array& other)
    {
    }

    /**
     * @brief Exchanges the contents of an array with those of another one.
     * @param a First array to exchange the contents with.
     * @param b Second array to exchange the contents with.
     */
    constexpr friend void swap([[maybe_unused]] array& a, [[maybe_unused]] array& b)
    {
    }

    /**
     * @brief Equal operator.
     * @param a First array to compare.
     * @param b Second array to compare.
     * @return `true` if the first array is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator==([[maybe_unused]] const array& a, [[maybe_unused]] const array& b)
    {
        return true;
    }

    /**
     * @brief Less than operator.
     * @param a First array to compare.
     * @param b Second array to compare.
     * @return `true` if the first array is lexicographically less than the second one, otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator<(const array& a, const array& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    /**
     * @brief Greater than operator.
     * @param a First array to compare.
     * @param b Second array to compare.
     * @return `true` if the first array is lexicographically greater than the second one,
     * otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator>(const array& a, const array& b)
    {
        return b < a;
    }

    /**
     * @brief Less than or equal operator.
     * @param a First array to compare.
     * @param b Second array to compare.
     * @return `true` if the first array is lexicographically less than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator<=(const array& a, const array& b)
    {
        return ! (a > b);
    }

    /**
     * @brief Greater than or equal operator.
     * @param a First array to compare.
     * @param b Second array to compare.
     * @return `true` if the first array is lexicographically greater than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] constexpr friend bool operator>=(const array& a, const array& b)
    {
        return ! (a < b);
    }
};

}


/// @cond DO_NOT_DOCUMENT

namespace _bn
{
    template<typename Type, int Size, unsigned... Indexes>
    constexpr bn::array<bn::remove_cv_t<Type>, Size>
        to_array_impl(Type (&base_array)[Size], bn::index_sequence<Indexes...>)
    {
        return { { base_array[Indexes]...} };
    }

    template<typename Type, int Size, unsigned... Indexes>
    constexpr bn::array<bn::remove_cv_t<Type>, Size>
        to_array_impl(Type (&&base_array)[Size], bn::index_sequence<Indexes...>)
    {
        return { { bn::move(base_array[Indexes])... } };
    }
}

/// @endcond


namespace bn
{
    /**
     * @brief Creates a bn::array object from the given built-in array.
     *
     * @ingroup array
     */
    template<typename Type, int Size>
    constexpr array<remove_cv_t<Type>, Size> to_array(Type (&base_array)[Size])
    {
        return _bn::to_array_impl(base_array, make_index_sequence<unsigned(Size)>{});
    }

    /**
     * @brief Creates a bn::array object from the given built-in array.
     *
     * @ingroup array
     */
    template<typename Type, int Size>
    constexpr array<remove_cv_t<Type>, Size> to_array(Type (&&base_array)[Size])
    {
        return _bn::to_array_impl(move(base_array), make_index_sequence<unsigned(Size)>{});
    }
}

#endif
