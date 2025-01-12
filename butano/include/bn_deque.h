/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DEQUE_H
#define BN_DEQUE_H

/**
 * @file
 * bn::ideque and bn::deque implementation header file.
 *
 * @ingroup deque
 */

#include <new>
#include "bn_assert.h"
#include "bn_utility.h"
#include "bn_iterator.h"
#include "bn_algorithm.h"
#include "bn_type_traits.h"
#include "bn_power_of_two.h"
#include "bn_deque_fwd.h"

namespace bn
{

template<typename Type>
class ideque
{

public:
    using value_type = Type; //!< Value type alias.
    using size_type = int; //!< Size type alias.
    using difference_type = int; //!< Difference type alias.
    using reference = Type&; //!< Reference alias.
    using const_reference = const Type&; //!< Const reference alias.
    using pointer = Type*; //!< Pointer alias.
    using const_pointer = const Type*; //!< Const pointer alias.

    /**
     * @brief Non const iterator.
     */
    class iterator
    {

    public:
        using value_type = ideque::value_type; //!< Value type alias.
        using size_type = ideque::size_type; //!< Size type alias.
        using difference_type = ideque::difference_type; //!< Difference type alias.
        using reference = ideque::reference; //!< Reference alias.
        using pointer = ideque::pointer; //!< Pointer alias.
        using iterator_category = random_access_iterator_tag; //!< Iterator category alias.

        /**
         * @brief Default constructor.
         */
        iterator() = default;

        /**
         * @brief Increments the position.
         * @return Reference to this.
         */
        iterator& operator++()
        {
            ++_index;
            return *this;
        }

        /**
         * @brief Increments the position.
         * @return The iterator before being incremented.
         */
        iterator operator++(int)
        {
            iterator copy(*this);
            ++_index;
            return copy;
        }

        /**
         * @brief Increments the position the given number of times.
         * @param value Number of positions to increment.
         * @return Reference to this.
         */
        iterator& operator+=(size_type value)
        {
            _index += value;
            return *this;
        }

        /**
         * @brief Decrements the position.
         * @return Reference to this.
         */
        iterator& operator--()
        {
            --_index;
            return *this;
        }

        /**
         * @brief Decrements the position.
         * @return The iterator before being decremented.
         */
        iterator operator--(int)
        {
            iterator copy(*this);
            --_index;
            return copy;
        }

        /**
         * @brief Decrements the position the given number of times.
         * @param value Number of positions to decrement.
         * @return Reference to this.
         */
        iterator& operator-=(size_type value)
        {
            _index -= value;
            return *this;
        }

        /**
         * @brief Returns a reference to the pointed value.
         */
        [[nodiscard]] reference operator*() const
        {
            return _deque->_value(_index);
        }

        /**
         * @brief Returns a pointer to the pointed value.
         */
        pointer operator->() const
        {
            return &_deque->_value(_index);
        }

        /**
         * @brief Returns a reference to the pointed value by this iterator incremented offset times.
         */
        [[nodiscard]] reference operator[](size_type offset) const
        {
            iterator result(*this);
            result += offset;
            return *result;
        }

        /**
         * @brief Returns the iterator it incremented offset times.
         */
        [[nodiscard]] friend iterator operator+(const iterator& it, size_type offset)
        {
            return iterator(&it._deque, it._index + offset);
        }

        /**
         * @brief Returns the iterator it incremented offset times.
         */
        [[nodiscard]] friend iterator operator+(size_type offset, const iterator& it)
        {
            return iterator(&it._deque, it._index + offset);
        }

        /**
         * @brief Returns the iterator it decremented offset times.
         */
        [[nodiscard]] friend iterator operator-(const iterator& it, size_type offset)
        {
            return iterator(&it._deque, it._index - offset);
        }

        /**
         * @brief Returns the iterator it decremented offset times.
         */
        [[nodiscard]] friend iterator operator-(size_type offset, const iterator& it)
        {
            return iterator(&it._deque, it._index - offset);
        }

        /**
         * @brief Returns the sum of a and b.
         */
        [[nodiscard]] friend size_type operator+(const iterator& a, const iterator& b)
        {
            return a._index + b._index;
        }

        /**
         * @brief Returns b subtracted from a.
         */
        [[nodiscard]] friend size_type operator-(const iterator& a, const iterator& b)
        {
            return a._index - b._index;
        }

        /**
         * @brief Equal operator.
         * @param a First iterator to compare.
         * @param b Second iterator to compare.
         * @return `true` if the first iterator is equal to the second one, otherwise `false`.
         */
        [[nodiscard]] friend bool operator==(const iterator& a, const iterator& b)
        {
            return a._index == b._index;
        }

        /**
         * @brief Not equal operator.
         * @param a First iterator to compare.
         * @param b Second iterator to compare.
         * @return `true` if the first iterator is not equal to the second one, otherwise `false`.
         */
        [[nodiscard]] friend bool operator!=(const iterator& a, const iterator& b)
        {
            return a._index != b._index;
        }

        /**
         * @brief Less than operator.
         * @param a First iterator to compare.
         * @param b Second iterator to compare.
         * @return `true` if the first iterator is lexicographically less than the second one,
         * otherwise `false`.
         */
        [[nodiscard]] friend bool operator<(const iterator& a, const iterator& b)
        {
            return a._index < b._index;
        }

        /**
         * @brief Greater than operator.
         * @param a First iterator to compare.
         * @param b Second iterator to compare.
         * @return `true` if the first iterator is lexicographically greater than the second one,
         * otherwise `false`.
         */
        [[nodiscard]] friend bool operator>(const iterator& a, const iterator& b)
        {
            return a._index > b._index;
        }

        /**
         * @brief Less than or equal operator.
         * @param a First iterator to compare.
         * @param b Second iterator to compare.
         * @return `true` if the first iterator is lexicographically less than or equal to the second one,
         * otherwise `false`.
         */
        [[nodiscard]] friend bool operator<=(const iterator& a, const iterator& b)
        {
            return a._index <= b._index;
        }

        /**
         * @brief Greater than or equal operator.
         * @param a First iterator to compare.
         * @param b Second iterator to compare.
         * @return `true` if the first iterator is lexicographically greater than or equal to the second one,
         * otherwise `false`.
         */
        [[nodiscard]] friend bool operator>=(const iterator& a, const iterator& b)
        {
            return a._index >= b._index;
        }

    private:
        friend class ideque;
        friend class const_iterator;

        ideque* _deque = nullptr;
        size_type _index = 0;

        iterator(ideque& deque, size_type index) :
            _deque(&deque),
            _index(index)
        {
        }
    };

    /**
     * @brief Const iterator.
     */
    class const_iterator
    {

    public:
        using value_type = ideque::value_type; //!< Value type alias.
        using size_type = ideque::size_type; //!< Size type alias.
        using difference_type = ideque::difference_type; //!< Difference type alias.
        using reference = ideque::const_reference; //!< Reference alias.
        using pointer = ideque::const_pointer; //!< Pointer alias.
        using iterator_category = random_access_iterator_tag; //!< Iterator category alias.

        /**
         * @brief Default constructor.
         */
        const_iterator() = default;

        /**
         * @brief Constructor.
         * @param it Non const iterator.
         */
        const_iterator(const iterator& it) :
            _deque(it._deque),
            _index(it._index)
        {
        }

        /**
         * @brief Increments the position.
         * @return Reference to this.
         */
        const_iterator& operator++()
        {
            ++_index;
            return *this;
        }

        /**
         * @brief Increments the position.
         * @return The iterator before being incremented.
         */
        const_iterator operator++(int)
        {
            const_iterator copy(*this);
            ++_index;
            return copy;
        }

        /**
         * @brief Increments the position the given number of times.
         * @param value Number of positions to increment.
         * @return Reference to this.
         */
        const_iterator& operator+=(size_type value)
        {
            _index += value;
            return *this;
        }

        /**
         * @brief Decrements the position.
         * @return Reference to this.
         */
        const_iterator& operator--()
        {
            --_index;
            return *this;
        }

        /**
         * @brief Decrements the position.
         * @return The iterator before being decremented.
         */
        const_iterator operator--(int)
        {
            const_iterator copy(*this);
            --_index;
            return copy;
        }

        /**
         * @brief Decrements the position the given number of times.
         * @param value Number of positions to decrement.
         * @return Reference to this.
         */
        const_iterator& operator-=(size_type value)
        {
            _index -= value;
            return *this;
        }

        /**
         * @brief Returns a const reference to the pointed value.
         */
        [[nodiscard]] const_reference operator*() const
        {
            return _deque->_value(_index);
        }

        /**
         * @brief Returns a const pointer to the pointed value.
         */
        const_pointer operator->() const
        {
            return &_deque->_value(_index);
        }

        /**
         * @brief Returns a const reference to the pointed value by this iterator incremented offset times.
         */
        [[nodiscard]] const_reference operator[](size_type offset) const
        {
            const_iterator result(*this);
            result += offset;
            return *result;
        }

        /**
         * @brief Returns the iterator it incremented offset times.
         */
        [[nodiscard]] friend const_iterator operator+(const const_iterator& it, size_type offset)
        {
            return const_iterator(&it._deque, it._index + offset);
        }

        /**
         * @brief Returns the iterator it incremented offset times.
         */
        [[nodiscard]] friend const_iterator operator+(size_type offset, const const_iterator& it)
        {
            return const_iterator(&it._deque, it._index + offset);
        }

        /**
         * @brief Returns the iterator it decremented offset times.
         */
        [[nodiscard]] friend const_iterator operator-(const const_iterator& it, size_type offset)
        {
            return const_iterator(&it._deque, it._index - offset);
        }

        /**
         * @brief Returns the iterator it decremented offset times.
         */
        [[nodiscard]] friend const_iterator operator-(size_type offset, const const_iterator& it)
        {
            return const_iterator(&it._deque, it._index - offset);
        }

        /**
         * @brief Returns the sum of a and b.
         */
        [[nodiscard]] friend size_type operator+(const const_iterator& a, const const_iterator& b)
        {
            return a._index + b._index;
        }

        /**
         * @brief Returns b subtracted from a.
         */
        [[nodiscard]] friend size_type operator-(const const_iterator& a, const const_iterator& b)
        {
            return a._index - b._index;
        }

        /**
         * @brief Equal operator.
         * @param a First const_iterator to compare.
         * @param b Second const_iterator to compare.
         * @return `true` if the first const_iterator is equal to the second one, otherwise `false`.
         */
        [[nodiscard]] friend bool operator==(const const_iterator& a, const const_iterator& b)
        {
            return a._index == b._index;
        }

        /**
         * @brief Not equal operator.
         * @param a First const_iterator to compare.
         * @param b Second const_iterator to compare.
         * @return `true` if the first const_iterator is not equal to the second one, otherwise `false`.
         */
        [[nodiscard]] friend bool operator!=(const const_iterator& a, const const_iterator& b)
        {
            return a._index != b._index;
        }

        /**
         * @brief Less than operator.
         * @param a First const_iterator to compare.
         * @param b Second const_iterator to compare.
         * @return `true` if the first const_iterator is lexicographically less than the second one,
         * otherwise `false`.
         */
        [[nodiscard]] friend bool operator<(const const_iterator& a, const const_iterator& b)
        {
            return a._index < b._index;
        }

        /**
         * @brief Greater than operator.
         * @param a First const_iterator to compare.
         * @param b Second const_iterator to compare.
         * @return `true` if the first const_iterator is lexicographically greater than the second one,
         * otherwise `false`.
         */
        [[nodiscard]] friend bool operator>(const const_iterator& a, const const_iterator& b)
        {
            return a._index > b._index;
        }

        /**
         * @brief Less than or equal operator.
         * @param a First const_iterator to compare.
         * @param b Second const_iterator to compare.
         * @return `true` if the first const_iterator is
         * lexicographically less than or equal to the second one, otherwise `false`.
         */
        [[nodiscard]] friend bool operator<=(const const_iterator& a, const const_iterator& b)
        {
            return a._index <= b._index;
        }

        /**
         * @brief Greater than or equal operator.
         * @param a First const_iterator to compare.
         * @param b Second const_iterator to compare.
         * @return `true` if the first const_iterator is
         * lexicographically greater than or equal to the second one, otherwise `false`.
         */
        [[nodiscard]] friend bool operator>=(const const_iterator& a, const const_iterator& b)
        {
            return a._index >= b._index;
        }

    private:
        friend class ideque;

        const ideque* _deque = nullptr;
        size_type _index = 0;

        const_iterator(const ideque& deque, size_type index) :
            _deque(&deque),
            _index(index)
        {
        }
    };

    using reverse_iterator = bn::reverse_iterator<iterator>; //!< Reverse iterator alias.
    using const_reverse_iterator = bn::reverse_iterator<const_iterator>; //!< Const reverse iterator alias.

    ideque(const ideque& other) = delete;

    /**
     * @brief Destructor.
     */
    ~ideque() noexcept = default;

    /**
     * @brief Destructor.
     */
    ~ideque() noexcept
    requires(! is_trivially_destructible_v<Type>)
    {
        clear();
    }

    /**
     * @brief Copy assignment operator.
     * @param other ideque to copy.
     * @return Reference to this.
     */
    ideque& operator=(const ideque& other)
    {
        if(this != &other)
        {
            BN_ASSERT(other._size <= max_size(), "Not enough space: ", max_size(), " - ", other._size);

            clear();
            _assign(other);
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other ideque to move.
     * @return Reference to this.
     */
    ideque& operator=(ideque&& other) noexcept
    {
        if(this != &other)
        {
            BN_ASSERT(other._size <= max_size(), "Not enough space: ", max_size(), " - ", other._size);

            clear();
            _assign(move(other));
        }

        return *this;
    }

    /**
     * @brief Returns the current elements count.
     */
    [[nodiscard]] size_type size() const
    {
        return _size;
    }

    /**
     * @brief Returns the maximum possible elements count.
     */
    [[nodiscard]] size_type max_size() const
    {
        return _max_size_minus_one + 1;
    }

    /**
     * @brief Returns the remaining element capacity.
     */
    [[nodiscard]] size_type available() const
    {
        return max_size() - _size;
    }

    /**
     * @brief Indicates if it doesn't contain any element.
     */
    [[nodiscard]] bool empty() const
    {
        return _size == 0;
    }

    /**
     * @brief Indicates if it can't contain any more elements.
     */
    [[nodiscard]] bool full() const
    {
        return _size == max_size();
    }

    /**
     * @brief Returns a const iterator to the beginning of the ideque.
     */
    [[nodiscard]] const_iterator begin() const
    {
        return const_iterator(*this, 0);
    }

    /**
     * @brief Returns an iterator to the beginning of the ideque.
     */
    [[nodiscard]] iterator begin()
    {
        return iterator(*this, 0);
    }

    /**
     * @brief Returns a const iterator to the end of the ideque.
     */
    [[nodiscard]] const_iterator end() const
    {
        return const_iterator(*this, _size);
    }

    /**
     * @brief Returns an iterator to the end of the ideque.
     */
    [[nodiscard]] iterator end()
    {
        return iterator(*this, _size);
    }

    /**
     * @brief Returns a const iterator to the beginning of the ideque.
     */
    [[nodiscard]] const_iterator cbegin() const
    {
        return const_iterator(*this, 0);
    }

    /**
     * @brief Returns a const iterator to the end of the ideque.
     */
    [[nodiscard]] const_iterator cend() const
    {
        return const_iterator(*this, _size);
    }

    /**
     * @brief Returns a const reverse iterator to the end of the ideque.
     */
    [[nodiscard]] const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }

    /**
     * @brief Returns a reverse iterator to the end of the ideque.
     */
    [[nodiscard]] reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    /**
     * @brief Returns a const reverse iterator to the beginning of the ideque.
     */
    [[nodiscard]] const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }

    /**
     * @brief Returns a reverse iterator to the beginning of the ideque.
     */
    [[nodiscard]] reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    /**
     * @brief Returns a const reverse iterator to the end of the ideque.
     */
    [[nodiscard]] const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(cend());
    }

    /**
     * @brief Returns a const reverse iterator to the beginning of the ideque.
     */
    [[nodiscard]] const_reverse_iterator crend() const
    {
        return const_reverse_iterator(cbegin());
    }

    /**
     * @brief Returns a const reference to the value stored at the specified index.
     */
    [[nodiscard]] const_reference operator[](size_type index) const
    {
        BN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _value(index);
    }

    /**
     * @brief Returns a reference to the value stored at the specified index.
     */
    [[nodiscard]] reference operator[](size_type index)
    {
        BN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _value(index);
    }

    /**
     * @brief Returns a const reference to the value stored at the specified index.
     */
    [[nodiscard]] const_reference at(size_type index) const
    {
        BN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _value(index);
    }

    /**
     * @brief Returns a reference to the value stored at the specified index.
     */
    [[nodiscard]] reference at(size_type index)
    {
        BN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _value(index);
    }

    /**
     * @brief Returns a const reference to the first element.
     */
    [[nodiscard]] const_reference front() const
    {
        BN_BASIC_ASSERT(_size, "Deque is empty");

        return _data[_begin];
    }

    /**
     * @brief Returns a reference to the first element.
     */
    [[nodiscard]] reference front()
    {
        BN_BASIC_ASSERT(_size, "Deque is empty");

        return _data[_begin];
    }

    /**
     * @brief Returns a const reference to the last element.
     */
    [[nodiscard]] const_reference back() const
    {
        BN_BASIC_ASSERT(_size, "Deque is empty");

        return _value(_size - 1);
    }

    /**
     * @brief Returns a reference to the last element.
     */
    [[nodiscard]] reference back()
    {
        BN_BASIC_ASSERT(_size, "Deque is empty");

        return _value(_size - 1);
    }

    /**
     * @brief Inserts a copy of a value at the beginning of the ideque.
     * @param value Value to insert.
     */
    void push_front(const_reference value)
    {
        BN_BASIC_ASSERT(! full(), "Deque is full");

        _push_front();
        ::new(static_cast<void*>(_data + _begin)) value_type(value);
    }

    /**
     * @brief Inserts a moved value at the beginning of the ideque.
     * @param value Value to insert.
     */
    void push_front(value_type&& value)
    {
        BN_BASIC_ASSERT(! full(), "Deque is full");

        _push_front();
        ::new(static_cast<void*>(_data + _begin)) value_type(move(value));
    }

    /**
     * @brief Constructs and inserts a value at the beginning of the ideque.
     * @param args Parameters of the value to insert.
     * @return Reference to the new value.
     */
    template<typename... Args>
    reference emplace_front(Args&&... args)
    {
        BN_BASIC_ASSERT(! full(), "Deque is full");

        _push_front();

        Type* result = _data + _begin;
        ::new(static_cast<void*>(result)) value_type(forward<Args>(args)...);
        return *result;
    }

    /**
     * @brief Inserts a copy of a value at the end of the ideque.
     * @param value Value to insert.
     */
    void push_back(const_reference value)
    {
        BN_BASIC_ASSERT(! full(), "Deque is full");

        ::new(static_cast<void*>(_data + _real_index(_size))) value_type(value);
        ++_size;
    }

    /**
     * @brief Inserts a moved value at the end of the ideque.
     * @param value Value to insert.
     */
    void push_back(value_type&& value)
    {
        BN_BASIC_ASSERT(! full(), "Deque is full");

        ::new(static_cast<void*>(_data + _real_index(_size))) value_type(move(value));
        ++_size;
    }

    /**
     * @brief Constructs and inserts a value at the end of the ideque.
     * @param args Parameters of the value to insert.
     * @return Reference to the new value.
     */
    template<typename... Args>
    reference emplace_back(Args&&... args)
    {
        BN_BASIC_ASSERT(! full(), "Deque is full");

        Type* result = _data + _real_index(_size);
        ::new(static_cast<void*>(result)) value_type(forward<Args>(args)...);
        ++_size;
        return *result;
    }

    /**
     * @brief Removes the first element of the ideque.
     */
    void pop_front()
    {
        BN_BASIC_ASSERT(_size, "Deque is empty");

        _pop_front();
    }

    /**
     * @brief Removes the last element of the ideque.
     */
    void pop_back()
    {
        BN_BASIC_ASSERT(_size, "Deque is empty");

        --_size;
        _value(_size).~value_type();
    }

    /**
     * @brief Inserts a copy of a value at the specified position.
     * @param position The given value is inserted before this position.
     * @param value Value to insert.
     * @return Iterator pointing to the inserted value.
     */
    iterator insert(const const_iterator& position, const_reference value)
    {
        size_type index = position._index;

        if(index == 0)
        {
            ::new(static_cast<void*>(_data + _begin)) value_type(value);
            _push_front();
        }
        else
        {
            BN_ASSERT(index > 0 && index <= _size, "Invalid position: ", index, " - ", _size);
            BN_BASIC_ASSERT(! full(), "Deque is full");

            pointer data = _data;
            size_type last = _size;
            size_type last_real_index = _real_index(last);
            ::new(static_cast<void*>(data + last_real_index)) value_type(value);
            reference last_value = data[last_real_index];

            for(; index != last; ++index)
            {
                bn::swap(data[_real_index(index)], last_value);
            }

            ++_size;
        }

        return _mutable_iterator(position);
    }

    /**
     * @brief Inserts a moved value at the specified position.
     * @param position The given value is inserted before this position.
     * @param value Value to insert.
     * @return Iterator pointing to the inserted value.
     */
    iterator insert(const const_iterator& position, value_type&& value)
    {
        size_type index = position._index;

        if(index == 0)
        {
            _push_front();
            ::new(static_cast<void*>(_data + _begin)) value_type(move(value));
        }
        else
        {
            BN_ASSERT(index > 0 && index <= _size, "Invalid position: ", index, " - ", _size);
            BN_BASIC_ASSERT(! full(), "Deque is full");

            pointer data = _data;
            size_type last = _size;
            size_type last_real_index = _real_index(last);
            ::new(static_cast<void*>(data + last_real_index)) value_type(move(value));
            reference last_value = data[last_real_index];

            for(; index != last; ++index)
            {
                bn::swap(data[_real_index(index)], last_value);
            }

            ++_size;
        }

        return _mutable_iterator(position);
    }

    /**
     * @brief Constructs and inserts a value at the specified position.
     * @param position The new value is inserted before this position.
     * @param args Parameters of the value to insert.
     * @return Iterator pointing to the new value.
     */
    template<typename... Args>
    iterator emplace(const const_iterator& position, Args&&... args)
    {
        size_type index = position._index;

        if(index == 0)
        {
            _push_front();
            ::new(static_cast<void*>(_data + _begin)) value_type(forward<Args>(args)...);
        }
        else
        {
            BN_ASSERT(index > 0 && index <= _size, "Invalid position: ", index, " - ", _size);
            BN_BASIC_ASSERT(! full(), "Deque is full");

            pointer data = _data;
            size_type last = _size;
            size_type last_real_index = _real_index(last);
            ::new(static_cast<void*>(_data + last_real_index)) value_type(forward<Args>(args)...);
            reference last_value = data[last_real_index];

            for(; index != last; ++index)
            {
                bn::swap(data[_real_index(index)], last_value);
            }

            ++_size;
        }

        return _mutable_iterator(position);
    }

    /**
     * @brief Erases an element.
     * @param position Iterator to the element to erase.
     * @return Iterator following the erased element.
     */
    iterator erase(const const_iterator& position)
    {
        size_type index = position._index;

        if(index == 0)
        {
            _pop_front();
        }
        else
        {
            BN_ASSERT(index > 0 && index < _size, "Invalid position: ", index, " - ", _size);

            --_size;

            pointer data = _data;
            size_type last = _size;
            size_type real_index = _real_index(index);

            for(; index != last; ++index)
            {
                size_type real_next_index = _real_index(index + 1);
                data[real_index] = move(data[real_next_index]);
                real_index = real_next_index;
            }

            data[real_index].~value_type();
        }

        return _mutable_iterator(position);
    }

    /**
     * @brief Erases a range of elements.
     *
     * The range includes all the elements between first and last, including the
     * element pointed by first, but not the one pointed by last.
     *
     * @param first Iterator to the first element to erase.
     * @param last Iterator to the last element to erase.
     * @return Iterator following the last erased element.
     */
    iterator erase(const const_iterator& first, const const_iterator& last)
    {
        size_type first_index = first._index;
        size_type size = _size;
        BN_ASSERT(first_index >= 0, "Invalid first: ", first_index, " - ", size);

        size_type last_index = last._index;
        BN_ASSERT(last_index <= size, "Invalid last: ", last_index, " - ", size);

        size_type delete_count = last_index - first_index;
        BN_ASSERT(delete_count >= 0 && delete_count <= size, "Invalid delete count: ", delete_count, " - ", size);

        if(delete_count)
        {
            if(delete_count == size)
            {
                clear();
            }
            else if(first_index == 0)
            {
                while(delete_count)
                {
                    _pop_front();
                    --delete_count;
                }
            }
            else
            {
                pointer data = _data;
                size_type index = first_index;
                size_type next_index = first_index + delete_count;

                while(next_index != size)
                {
                    data[_real_index(index)] = move(data[_real_index(next_index)]);
                    ++index;
                    ++next_index;
                }

                while(index != size)
                {
                    data[_real_index(index)].~value_type();
                    ++index;
                }

                _size -= delete_count;
            }
        }

        return _mutable_iterator(first);
    }

    /**
     * @brief Resizes the ideque.
     * @param count New size.
     */
    void resize(size_type count)
    {
        BN_ASSERT(count >= 0 && count <= max_size(), "Invalid count: ", count, " - ", max_size());

        pointer data = _data;
        size_type size = _size;
        _size = count;

        if(count < size)
        {
            for(size_type index = count; index < size; ++index)
            {
                data[_real_index(index)].~value_type();
            }
        }
        else
        {
            for(size_type index = size; index < count; ++index)
            {
                ::new(static_cast<void*>(data + _real_index(index))) value_type();
            }
        }
    }

    /**
     * @brief Resizes the ideque.
     * @param count New size.
     * @param value Value to fill new elements with.
     */
    void resize(size_type count, const_reference value)
    {
        BN_ASSERT(count >= 0 && count <= max_size(), "Invalid count: ", count, " - ", max_size());

        pointer data = _data;
        size_type size = _size;
        _size = count;

        if(count < size)
        {
            for(size_type index = count; index < size; ++index)
            {
                data[_real_index(index)].~value_type();
            }
        }
        else
        {
            for(size_type index = size; index < count; ++index)
            {
                ::new(static_cast<void*>(data + _real_index(index))) value_type(value);
            }
        }
    }

    /**
     * @brief Resizes the ideque to a size less or equal than the previous one.
     * @param count New size.
     */
    void shrink(size_type count)
    {
        BN_ASSERT(count >= 0 && count <= _size, "Invalid count: ", count, " - ", _size);

        pointer data = _data;

        for(size_type index = count, limit = _size; index < limit; ++index)
        {
            data[_real_index(index)].~value_type();
        }

        _size = count;
    }

    /**
     * @brief Assigns values to the ideque, removing the previous ones.
     * @param count Number of elements to insert.
     * @param value Value to fill new elements with.
     */
    void assign(size_type count, const_reference value)
    {
        BN_ASSERT(count >= 0 && count <= max_size(), "Invalid count: ", count, " - ", max_size());

        pointer data = _data;
        clear();
        _size = count;

        for(size_type index = 0; index < count; ++index)
        {
            ::new(static_cast<void*>(data + index)) value_type(value);
        }
    }

    /**
     * @brief Assigns values to the ideque, removing the previous ones.
     * @param first Iterator to the first element to insert.
     * @param last Iterator following to the last element to insert.
     */
    template<typename Iterator>
    void assign(const Iterator& first, const Iterator& last)
    {
        size_type count = last - first;
        BN_ASSERT(count >= 0 && count <= max_size(), "Invalid count: ", count, " - ", max_size());

        pointer data = _data;
        clear();
        _size = count;

        for(size_type index = 0; index < count; ++index)
        {
            ::new(static_cast<void*>(data + index)) value_type(*first);
            ++first;
        }
    }

    /**
     * @brief Removes all elements.
     */
    void clear()
    {
        _size = 0;
        _begin = 0;
    }

    /**
     * @brief Removes all elements.
     */
    void clear()
    requires(! is_trivially_destructible_v<Type>)
    {
        pointer data = _data;

        for(size_type index = 0, size = _size; index < size; ++index)
        {
            data[_real_index(index)].~value_type();
        }

        _size = 0;
        _begin = 0;
    }

    /**
     * @brief Exchanges the contents of this ideque with those of the other one.
     * @param other ideque to exchange the contents with.
     */
    void swap(ideque& other)
    {
        if(_data != other._data)
        {
            BN_ASSERT(_size <= other.max_size(), "Invalid size: ", _size, " - ", other.max_size());
            BN_ASSERT(max_size() <= other._size, "Invalid max size: ", max_size(), " - ", other._size);

            ideque* min_deque;
            ideque* max_deque;

            if(_size < other._size)
            {
                min_deque = this;
                max_deque = &other;
            }
            else
            {
                min_deque = &other;
                max_deque = this;
            }

            pointer min_data = min_deque->_data;
            pointer max_data = max_deque->_data;
            size_type min_size = min_deque->_size;
            size_type max_size = max_deque->_size;

            for(size_type index = 0; index < min_size; ++index)
            {
                bn::swap(min_data[min_deque->_real_index(index)], max_data[max_deque->_real_index(index)]);
            }

            for(size_type index = min_size; index < max_size; ++index)
            {
                size_type max_real_index = max_deque->_real_index(index);
                min_data[min_deque->_real_index(index)] = move(max_data[max_real_index]);
                max_data[max_real_index].~value_type();
            }

            bn::swap(_size, other._size);
        }
    }

    /**
     * @brief Exchanges the contents of a ideque with those of another one.
     * @param a First ideque to exchange the contents with.
     * @param b Second ideque to exchange the contents with.
     */
    friend void swap(ideque& a, ideque& b)
    {
        a.swap(b);
    }

    /**
     * @brief Equal operator.
     * @param a First ideque to compare.
     * @param b Second ideque to compare.
     * @return `true` if the first ideque is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator==(const ideque& a, const ideque& b)
    {
        if(a.size() != b.size())
        {
            return false;
        }

        if(a._data == b._data)
        {
            return true;
        }

        return equal(a.begin(), a.end(), b.begin());
    }

    /**
     * @brief Not equal operator.
     * @param a First ideque to compare.
     * @param b Second ideque to compare.
     * @return `true` if the first ideque is not equal to the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator!=(const ideque& a, const ideque& b)
    {
        return ! (a == b);
    }

    /**
     * @brief Less than operator.
     * @param a First ideque to compare.
     * @param b Second ideque to compare.
     * @return `true` if the first ideque is lexicographically less than the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator<(const ideque& a, const ideque& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    /**
     * @brief Greater than operator.
     * @param a First ideque to compare.
     * @param b Second ideque to compare.
     * @return `true` if the first ideque is lexicographically greater than the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator>(const ideque& a, const ideque& b)
    {
        return b < a;
    }

    /**
     * @brief Less than or equal operator.
     * @param a First ideque to compare.
     * @param b Second ideque to compare.
     * @return `true` if the first ideque is lexicographically less than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator<=(const ideque& a, const ideque& b)
    {
        return ! (a > b);
    }

    /**
     * @brief Greater than or equal operator.
     * @param a First ideque to compare.
     * @param b Second ideque to compare.
     * @return `true` if the first ideque is lexicographically greater than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator>=(const ideque& a, const ideque& b)
    {
        return ! (a < b);
    }

protected:
    /// @cond DO_NOT_DOCUMENT

    ideque(reference data, size_type max_size) :
        _data(&data),
        _max_size_minus_one(max_size - 1)
    {
    }

    void _assign(const ideque& other)
    {
        pointer data = _data;
        const_pointer other_data = other._data;
        size_type other_size = other._size;
        _size = other_size;

        for(size_type index = 0; index < other_size; ++index)
        {
            ::new(static_cast<void*>(data + index)) value_type(other_data[other._real_index(index)]);
        }
    }

    void _assign(ideque&& other)
    {
        pointer data = _data;
        pointer other_data = other._data;
        size_type other_size = other._size;
        _size = other_size;

        for(size_type index = 0; index < other_size; ++index)
        {
            ::new(static_cast<void*>(data + index)) value_type(move(other_data[other._real_index(index)]));
        }

        other._size = 0;
        other._begin = 0;
    }

    void _assign(size_type count)
    {
        pointer data = _data;
        size_type size = _size;
        _size = count;

        for(size_type index = size; index < count; ++index)
        {
            ::new(static_cast<void*>(data + index)) value_type();
        }
    }

    void _assign(size_type count, const_reference value)
    {
        pointer data = _data;
        _size = count;

        for(size_type index = 0; index < count; ++index)
        {
            ::new(static_cast<void*>(data + index)) value_type(value);
        }
    }

    /// @endcond

private:
    pointer _data;
    size_type _size = 0;
    size_type _max_size_minus_one;
    size_type _begin = 0;

    [[nodiscard]] static iterator _mutable_iterator(const const_iterator& const_iterator)
    {
        return iterator(const_cast<ideque&>(*const_iterator._deque), const_iterator._index);
    }

    [[nodiscard]] size_type _real_index(size_type index) const
    {
        return (_begin + index) & _max_size_minus_one;
    }

    [[nodiscard]] const_reference _value(size_type index) const
    {
        return _data[_real_index(index)];
    }

    [[nodiscard]] reference _value(size_type index)
    {
        return _data[_real_index(index)];
    }

    void _push_front()
    {
        _begin = (_begin - 1) & _max_size_minus_one;
        ++_size;
    }

    void _pop_front()
    {
        _data[_begin].~value_type();
        _begin = (_begin + 1) & _max_size_minus_one;
        --_size;
    }
};


template<typename Type, int MaxSize>
class deque : public ideque<Type>
{
    static_assert(power_of_two(MaxSize));

public:
    using value_type = Type; //!< Value type alias.
    using size_type = int; //!< Size type alias.
    using difference_type = int; //!< Difference type alias.
    using reference = Type&; //!< Reference alias.
    using const_reference = const Type&; //!< Const reference alias.
    using pointer = Type*; //!< Pointer alias.
    using const_pointer = const Type*; //!< Const pointer alias.
    using iterator = typename ideque<Type>::iterator; //!< Iterator alias.
    using const_iterator = typename ideque<Type>::const_iterator; //!< Const iterator alias.
    using reverse_iterator = typename ideque<Type>::reverse_iterator; //!< Reverse iterator alias.
    using const_reverse_iterator = typename ideque<Type>::const_reverse_iterator; //!< Const reverse iterator alias.

    /**
     * @brief Default constructor.
     */
    deque() :
        ideque<Type>(*reinterpret_cast<pointer>(_storage_buffer), MaxSize)
    {
    }

    /**
     * @brief Copy constructor.
     * @param other deque to copy.
     */
    deque(const deque& other) :
        deque()
    {
        this->_assign(other);
    }

    /**
     * @brief Move constructor.
     * @param other deque to move.
     */
    deque(deque&& other) noexcept :
        deque()
    {
        this->_assign(move(other));
    }

    /**
     * @brief Copy constructor.
     * @param other ideque to copy.
     */
    deque(const ideque<Type>& other) :
        deque()
    {
        BN_ASSERT(other.size() <= MaxSize, "Not enough space: ", MaxSize, " - ", other.size());

        this->_assign(other);
    }

    /**
     * @brief Move constructor.
     * @param other ideque to move.
     */
    deque(ideque<Type>&& other) noexcept :
        deque()
    {
        BN_ASSERT(other.size() <= MaxSize, "Not enough space: ", MaxSize, " - ", other.size());

        this->_assign(move(other));
    }

    /**
     * @brief Size constructor.
     * @param count Initial size of the deque.
     */
    explicit deque(size_type count) :
        deque()
    {
        BN_ASSERT(count >= 0 && count <= MaxSize, "Invalid count: ", count, " - ", MaxSize);

        this->_assign(count);
    }

    /**
     * @brief Size constructor.
     * @param count Initial size of the deque.
     * @param value Value to fill the deque with.
     */
    deque(size_type count, const_reference value) :
        deque()
    {
        BN_ASSERT(count >= 0 && count <= MaxSize, "Invalid count: ", count, " - ", MaxSize);

        this->_assign(count, value);
    }

    /**
     * @brief Copy assignment operator.
     * @param other deque to copy.
     * @return Reference to this.
     */
    deque& operator=(const deque& other)
    {
        if(this != &other)
        {
            this->clear();
            this->_assign(other);
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other deque to move.
     * @return Reference to this.
     */
    deque& operator=(deque&& other) noexcept
    {
        if(this != &other)
        {
            this->clear();
            this->_assign(move(other));
        }

        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param other ideque to copy.
     * @return Reference to this.
     */
    deque& operator=(const ideque<Type>& other)
    {
        if(this != &other)
        {
            BN_ASSERT(other.size() <= MaxSize, "Not enough space: ", MaxSize, " - ", other.size());

            this->clear();
            this->_assign(other);
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other ideque to move.
     * @return Reference to this.
     */
    deque& operator=(ideque<Type>&& other) noexcept
    {
        if(this != &other)
        {
            BN_ASSERT(other.size() <= MaxSize, "Not enough space: ", MaxSize, " - ", other.size());

            this->clear();
            this->_assign(move(other));
        }

        return *this;
    }

private:
    static constexpr unsigned _alignment = alignof(value_type) > alignof(int) ? alignof(value_type) : alignof(int);

    alignas(_alignment) char _storage_buffer[sizeof(value_type) * MaxSize];
};


/**
 * @brief Erases all elements from a ideque that are equal to the specified value.
 * @param deque ideque from which to erase.
 * @param value Element to erase.
 * @return Number of erased elements.
 */
template<typename Type>
typename ideque<Type>::size_type erase(ideque<Type>& deque, const Type& value)
{
    auto old_size = deque.size();
    deque.erase(remove(deque.begin(), deque.end(), value), deque.end());
    return old_size - deque.size();
}

/**
 * @brief Erases all elements from a ideque that satisfy the specified predicate.
 * @param deque ideque from which to erase.
 * @param pred Unary predicate which returns `true` if the element should be erased.
 * @return Number of erased elements.
 */
template<typename Type, class Pred>
typename ideque<Type>::size_type erase_if(ideque<Type>& deque, const Pred& pred)
{
    auto old_size = deque.size();
    deque.erase(remove_if(deque.begin(), deque.end(), pred), deque.end());
    return old_size - deque.size();
}

}

#endif
