/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_VECTOR_H
#define BN_VECTOR_H

/**
 * @file
 * bn::ivector and bn::vector implementation header file.
 *
 * @ingroup vector
 */

#include <new>
#include "bn_assert.h"
#include "bn_utility.h"
#include "bn_iterator.h"
#include "bn_algorithm.h"
#include "bn_type_traits.h"
#include "bn_vector_fwd.h"

namespace bn
{

template<typename Type>
class ivector
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

    ivector(const ivector& other) = delete;

    /**
     * @brief Destructor.
     */
    ~ivector() noexcept = default;

    /**
     * @brief Destructor.
     */
    ~ivector() noexcept
    requires(! is_trivially_destructible_v<Type>)
    {
        clear();
    }

    /**
     * @brief Copy assignment operator.
     * @param other ivector to copy.
     * @return Reference to this.
     */
    ivector& operator=(const ivector& other)
    {
        if(this != &other)
        {
            BN_ASSERT(other._size <= _max_size, "Not enough space: ", _max_size, " - ", other._size);

            clear();
            _assign(other);
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other ivector to move.
     * @return Reference to this.
     */
    ivector& operator=(ivector&& other) noexcept
    {
        if(this != &other)
        {
            BN_ASSERT(other._size <= _max_size, "Not enough space: ", _max_size, " - ", other._size);

            clear();
            _assign(move(other));
        }

        return *this;
    }

    /**
     * @brief Returns a const pointer to the beginning of the ivector data.
     */
    [[nodiscard]] const_pointer data() const
    {
        return _data;
    }

    /**
     * @brief Returns a pointer to the beginning of the ivector data.
     */
    [[nodiscard]] pointer data()
    {
        return _data;
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
        return _max_size;
    }

    /**
     * @brief Returns the remaining element capacity.
     */
    [[nodiscard]] size_type available() const
    {
        return _max_size - _size;
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
        return _size == _max_size;
    }

    /**
     * @brief Returns a const iterator to the beginning of the ivector.
     */
    [[nodiscard]] const_iterator begin() const
    {
        return _data;
    }

    /**
     * @brief Returns an iterator to the beginning of the ivector.
     */
    [[nodiscard]] iterator begin()
    {
        return _data;
    }

    /**
     * @brief Returns a const iterator to the end of the ivector.
     */
    [[nodiscard]] const_iterator end() const
    {
        return _data + _size;
    }

    /**
     * @brief Returns an iterator to the end of the ivector.
     */
    [[nodiscard]] iterator end()
    {
        return _data + _size;
    }

    /**
     * @brief Returns a const iterator to the beginning of the ivector.
     */
    [[nodiscard]] const_iterator cbegin() const
    {
        return _data;
    }

    /**
     * @brief Returns a const iterator to the end of the ivector.
     */
    [[nodiscard]] const_iterator cend() const
    {
        return _data + _size;
    }

    /**
     * @brief Returns a const reverse iterator to the end of the ivector.
     */
    [[nodiscard]] const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }

    /**
     * @brief Returns a reverse iterator to the end of the ivector.
     */
    [[nodiscard]] reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    /**
     * @brief Returns a const reverse iterator to the beginning of the ivector.
     */
    [[nodiscard]] const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }

    /**
     * @brief Returns a reverse iterator to the beginning of the ivector.
     */
    [[nodiscard]] reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    /**
     * @brief Returns a const reverse iterator to the end of the ivector.
     */
    [[nodiscard]] const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(cend());
    }

    /**
     * @brief Returns a const reverse iterator to the beginning of the ivector.
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

        return _data[index];
    }

    /**
     * @brief Returns a reference to the value stored at the specified index.
     */
    [[nodiscard]] reference operator[](size_type index)
    {
        BN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _data[index];
    }

    /**
     * @brief Returns a const reference to the value stored at the specified index.
     */
    [[nodiscard]] const_reference at(size_type index) const
    {
        BN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _data[index];
    }

    /**
     * @brief Returns a reference to the value stored at the specified index.
     */
    [[nodiscard]] reference at(size_type index)
    {
        BN_ASSERT(index >= 0 && index < _size, "Invalid index: ", index, " - ", _size);

        return _data[index];
    }

    /**
     * @brief Returns a const reference to the first element.
     */
    [[nodiscard]] const_reference front() const
    {
        BN_BASIC_ASSERT(_size, "Vector is empty");

        return _data[0];
    }

    /**
     * @brief Returns a reference to the first element.
     */
    [[nodiscard]] reference front()
    {
        BN_BASIC_ASSERT(_size, "Vector is empty");

        return _data[0];
    }

    /**
     * @brief Returns a const reference to the last element.
     */
    [[nodiscard]] const_reference back() const
    {
        BN_BASIC_ASSERT(_size, "Vector is empty");

        return _data[_size - 1];
    }

    /**
     * @brief Returns a reference to the last element.
     */
    [[nodiscard]] reference back()
    {
        BN_BASIC_ASSERT(_size, "Vector is empty");

        return _data[_size - 1];
    }

    /**
     * @brief Inserts a copy of a value at the end of the ivector.
     * @param value Value to insert.
     */
    void push_back(const_reference value)
    {
        BN_BASIC_ASSERT(! full(), "Vector is full");

        ::new(static_cast<void*>(_data + _size)) value_type(value);
        ++_size;
    }

    /**
     * @brief Inserts a moved value at the end of the ivector.
     * @param value Value to insert.
     */
    void push_back(value_type&& value)
    {
        BN_BASIC_ASSERT(! full(), "Vector is full");

        ::new(static_cast<void*>(_data + _size)) value_type(move(value));
        ++_size;
    }

    /**
     * @brief Constructs and inserts a value at the end of the ivector.
     * @param args Parameters of the value to insert.
     * @return Reference to the new value.
     */
    template<typename... Args>
    reference emplace_back(Args&&... args)
    {
        BN_BASIC_ASSERT(! full(), "Vector is full");

        Type* result = _data + _size;
        ::new(static_cast<void*>(result)) value_type(forward<Args>(args)...);
        ++_size;
        return *result;
    }

    /**
     * @brief Removes the last element of the ivector.
     */
    void pop_back()
    {
        BN_BASIC_ASSERT(_size, "Vector is empty");

        --_size;
        _data[_size].~value_type();
    }

    /**
     * @brief Inserts a copy of a value at the specified position.
     * @param position The given value is inserted before this position.
     * @param value Value to insert.
     * @return Iterator pointing to the inserted value.
     */
    iterator insert(const_iterator position, const_reference value)
    {
        BN_ASSERT(position >= begin() && position <= end(), "Invalid position");
        BN_BASIC_ASSERT(! full(), "Vector is full");

        auto non_const_position = const_cast<iterator>(position);
        iterator last = end();
        ::new(static_cast<void*>(_data + _size)) value_type(value);
        ++_size;

        for(iterator it = non_const_position; it != last; ++it)
        {
            bn::swap(*it, *last);
        }

        return non_const_position;
    }

    /**
     * @brief Inserts a moved value at the specified position.
     * @param position The given value is inserted before this position.
     * @param value Value to insert.
     * @return Iterator pointing to the inserted value.
     */
    iterator insert(const_iterator position, value_type&& value)
    {
        BN_ASSERT(position >= begin() && position <= end(), "Invalid position");
        BN_BASIC_ASSERT(! full(), "Vector is full");

        auto non_const_position = const_cast<iterator>(position);
        iterator last = end();
        ::new(static_cast<void*>(_data + _size)) value_type(move(value));
        ++_size;

        for(iterator it = non_const_position; it != last; ++it)
        {
            bn::swap(*it, *last);
        }

        return non_const_position;
    }

    /**
     * @brief Constructs and inserts a value at the specified position.
     * @param position The new value is inserted before this position.
     * @param args Parameters of the value to insert.
     * @return Iterator pointing to the new value.
     */
    template<typename... Args>
    iterator emplace(const_iterator position, Args&&... args)
    {
        BN_ASSERT(position >= begin() && position <= end(), "Invalid position");
        BN_BASIC_ASSERT(! full(), "Vector is full");

        auto non_const_position = const_cast<iterator>(position);
        iterator last = end();
        ::new(static_cast<void*>(_data + _size)) value_type(forward<Args>(args)...);
        ++_size;

        for(iterator it = non_const_position; it != last; ++it)
        {
            bn::swap(*it, *last);
        }

        return non_const_position;
    }

    /**
     * @brief Erases an element.
     * @param position Iterator to the element to erase.
     * @return Iterator following the erased element.
     */
    iterator erase(const_iterator position)
    {
        BN_BASIC_ASSERT(_size, "Vector is empty");
        BN_ASSERT(position >= begin() && position < end(), "Invalid position");

        auto non_const_position = const_cast<iterator>(position);
        iterator it = non_const_position;
        --_size;

        iterator last = end();

        while(it != last)
        {
            iterator next = it + 1;
            *it = move(*next);
            it = next;
        }

        _data[_size].~value_type();
        return non_const_position;
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
    iterator erase(const_iterator first, const_iterator last)
    {
        BN_ASSERT(first >= begin(), "Invalid first");
        BN_ASSERT(last <= end(), "Invalid last");

        size_type delete_count = last - first;
        BN_ASSERT(delete_count >= 0 && delete_count <= _size, "Invalid delete count: ", delete_count, " - ", _size);

        if(delete_count)
        {
            if(delete_count == _size)
            {
                clear();
            }
            else
            {
                iterator erase_it =  const_cast<iterator>(first);
                iterator erase_next = erase_it + delete_count;
                iterator erase_last = end();
                _size -= delete_count;

                while(erase_next != erase_last)
                {
                    *erase_it = move(*erase_next);
                    ++erase_it;
                    ++erase_next;
                }

                while(erase_it != erase_last)
                {
                    erase_it->~value_type();
                    ++erase_it;
                }
            }
        }

        return const_cast<iterator>(first);
    }

    /**
     * @brief Resizes the ivector.
     * @param count New size.
     */
    void resize(size_type count)
    {
        BN_ASSERT(count >= 0 && count <= _max_size, "Invalid count: ", count, " - ", _max_size);

        pointer data = _data;
        size_type size = _size;
        _size = count;

        if(count < size)
        {
            for(size_type index = count; index < size; ++index)
            {
                data[index].~value_type();
            }
        }
        else
        {
            for(size_type index = size; index < count; ++index)
            {
                ::new(static_cast<void*>(data + index)) value_type();
            }
        }
    }

    /**
     * @brief Resizes the ivector.
     * @param count New size.
     * @param value Value to fill new elements with.
     */
    void resize(size_type count, const_reference value)
    {
        BN_ASSERT(count >= 0 && count <= _max_size, "Invalid count: ", count, " - ", _max_size);

        pointer data = _data;
        size_type size = _size;
        _size = count;

        if(count < size)
        {
            for(size_type index = count; index < size; ++index)
            {
                data[index].~value_type();
            }
        }
        else
        {
            for(size_type index = size; index < count; ++index)
            {
                ::new(static_cast<void*>(data + index)) value_type(value);
            }
        }
    }

    /**
     * @brief Resizes the ivector to a size less or equal than the previous one.
     * @param count New size.
     */
    void shrink(size_type count)
    {
        BN_ASSERT(count >= 0 && count <= _size, "Invalid count: ", count, " - ", _size);

        pointer data = _data;

        for(size_type index = count, limit = _size; index < limit; ++index)
        {
            data[index].~value_type();
        }

        _size = count;
    }

    /**
     * @brief Assigns values to the ivector, removing the previous ones.
     * @param count Number of elements to insert.
     * @param value Value to fill new elements with.
     */
    void assign(size_type count, const_reference value)
    {
        BN_ASSERT(count >= 0 && count <= _max_size, "Invalid count: ", count, " - ", _max_size);

        pointer data = _data;
        clear();
        _size = count;

        for(size_type index = 0; index < count; ++index)
        {
            ::new(static_cast<void*>(data + index)) value_type(value);
        }
    }

    /**
     * @brief Assigns values to the ivector, removing the previous ones.
     * @param first Iterator to the first element to insert.
     * @param last Iterator following to the last element to insert.
     */
    template<typename Iterator>
    void assign(const Iterator& first, const Iterator& last)
    {
        size_type count = last - first;
        BN_ASSERT(count >= 0 && count <= _max_size, "Invalid count: ", count, " - ", _max_size);

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
            data[index].~value_type();
        }

        _size = 0;
    }

    /**
     * @brief Exchanges the contents of this ivector with those of the other one.
     * @param other ivector to exchange the contents with.
     */
    void swap(ivector& other)
    {
        if(_data != other._data)
        {
            BN_ASSERT(_size <= other._max_size, "Invalid size: ", _size, " - ", other._max_size);
            BN_ASSERT(_max_size <= other._size, "Invalid max size: ", _max_size, " - ", other._size);

            pointer min_data;
            pointer max_data;
            size_type min_size;
            size_type max_size;

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
                min_data[index] = move(max_data[index]);
                max_data[index].~value_type();
            }

            bn::swap(_size, other._size);
        }
    }

    /**
     * @brief Exchanges the contents of a ivector with those of another one.
     * @param a First ivector to exchange the contents with.
     * @param b Second ivector to exchange the contents with.
     */
    friend void swap(ivector& a, ivector& b)
    {
        a.swap(b);
    }

    /**
     * @brief Equal operator.
     * @param a First ivector to compare.
     * @param b Second ivector to compare.
     * @return `true` if the first ivector is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator==(const ivector& a, const ivector& b)
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
     * @param a First ivector to compare.
     * @param b Second ivector to compare.
     * @return `true` if the first ivector is lexicographically less than the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator<(const ivector& a, const ivector& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    /**
     * @brief Greater than operator.
     * @param a First ivector to compare.
     * @param b Second ivector to compare.
     * @return `true` if the first ivector is lexicographically greater than the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator>(const ivector& a, const ivector& b)
    {
        return b < a;
    }

    /**
     * @brief Less than or equal operator.
     * @param a First ivector to compare.
     * @param b Second ivector to compare.
     * @return `true` if the first ivector is lexicographically less than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator<=(const ivector& a, const ivector& b)
    {
        return ! (a > b);
    }

    /**
     * @brief Greater than or equal operator.
     * @param a First ivector to compare.
     * @param b Second ivector to compare.
     * @return `true` if the first ivector is lexicographically greater than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator>=(const ivector& a, const ivector& b)
    {
        return ! (a < b);
    }

protected:
    /// @cond DO_NOT_DOCUMENT

    ivector(reference data, size_type max_size) :
        _data(&data),
        _max_size(max_size)
    {
    }

    void _assign(const ivector& other)
    {
        pointer data = _data;
        const_pointer other_data = other._data;
        size_type other_size = other._size;
        _size = other_size;

        for(size_type index = 0; index < other_size; ++index)
        {
            ::new(static_cast<void*>(data + index)) value_type(other_data[index]);
        }
    }

    void _assign(ivector&& other)
    {
        pointer data = _data;
        pointer other_data = other._data;
        size_type other_size = other._size;
        _size = other_size;

        for(size_type index = 0; index < other_size; ++index)
        {
            ::new(static_cast<void*>(data + index)) value_type(move(other_data[index]));
        }

        other.clear();
    }

    void _assign(size_type count)
    {
        pointer data = _data;
        _size = count;

        for(size_type index = 0; index < count; ++index)
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
    size_type _max_size;
};


template<typename Type, int MaxSize>
class vector : public ivector<Type>
{
    static_assert(MaxSize > 0);

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
     * @brief Default constructor.
     */
    vector() :
        ivector<Type>(*reinterpret_cast<pointer>(_storage_buffer), MaxSize)
    {
    }

    /**
     * @brief Copy constructor.
     * @param other vector to copy.
     */
    vector(const vector& other) :
        vector()
    {
        this->_assign(other);
    }

    /**
     * @brief Move constructor.
     * @param other vector to move.
     */
    vector(vector&& other) noexcept :
        vector()
    {
        this->_assign(move(other));
    }

    /**
     * @brief Copy constructor.
     * @param other ivector to copy.
     */
    vector(const ivector<Type>& other) :
        vector()
    {
        BN_ASSERT(other.size() <= MaxSize, "Not enough space: ", MaxSize, " - ", other.size());

        this->_assign(other);
    }

    /**
     * @brief Move constructor.
     * @param other ivector to move.
     */
    vector(ivector<Type>&& other) noexcept :
        vector()
    {
        BN_ASSERT(other.size() <= MaxSize, "Not enough space: ", MaxSize, " - ", other.size());

        this->_assign(move(other));
    }

    /**
     * @brief Size constructor.
     * @param count Initial size of the vector.
     */
    explicit vector(size_type count) :
        vector()
    {
        BN_ASSERT(count >= 0 && count <= MaxSize, "Invalid count: ", count, " - ", MaxSize);

        this->_assign(count);
    }

    /**
     * @brief Size constructor.
     * @param count Initial size of the vector.
     * @param value Value to fill the vector with.
     */
    vector(size_type count, const_reference value) :
        vector()
    {
        BN_ASSERT(count >= 0 && count <= MaxSize, "Invalid count: ", count, " - ", MaxSize);

        this->_assign(count, value);
    }

    /**
     * @brief Copy assignment operator.
     * @param other vector to copy.
     * @return Reference to this.
     */
    vector& operator=(const vector& other)
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
     * @param other vector to move.
     * @return Reference to this.
     */
    vector& operator=(vector&& other) noexcept
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
     * @param other ivector to copy.
     * @return Reference to this.
     */
    vector& operator=(const ivector<Type>& other)
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
     * @param other ivector to move.
     * @return Reference to this.
     */
    vector& operator=(ivector<Type>&& other) noexcept
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
 * @brief Erases all elements from a ivector that are equal to the specified value.
 * @param vector ivector from which to erase.
 * @param value Element to erase.
 * @return Number of erased elements.
 */
template<typename Type>
typename ivector<Type>::size_type erase(ivector<Type>& vector, const Type& value)
{
    auto old_size = vector.size();
    vector.erase(remove(vector.begin(), vector.end(), value), vector.end());
    return old_size - vector.size();
}

/**
 * @brief Erases all elements from a ivector that satisfy the specified predicate.
 * @param vector ivector from which to erase.
 * @param pred Unary predicate which returns `true` if the element should be erased.
 * @return Number of erased elements.
 */
template<typename Type, class Pred>
typename ivector<Type>::size_type erase_if(ivector<Type>& vector, const Pred& pred)
{
    auto old_size = vector.size();
    vector.erase(remove_if(vector.begin(), vector.end(), pred), vector.end());
    return old_size - vector.size();
}

}

#endif
