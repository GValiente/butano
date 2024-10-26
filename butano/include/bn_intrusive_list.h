/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_INTRUSIVE_LIST_H
#define BN_INTRUSIVE_LIST_H

/**
 * @file
 * bn::intrusive_list implementation header file.
 *
 * @ingroup intrusive_list
 */

#include "bn_assert.h"
#include "bn_utility.h"
#include "bn_iterator.h"
#include "bn_algorithm.h"
#include "bn_type_traits.h"
#include "bn_intrusive_list_fwd.h"

namespace bn
{

/**
 * @brief To be part of an intrusive_list, values must inherit this class.
 *
 * @ingroup intrusive_list
 */
class intrusive_list_node_type
{

private:
    template<class Type>
    friend class intrusive_list;

    intrusive_list_node_type* prev = nullptr;
    intrusive_list_node_type* next = nullptr;
};


template<class Type>
class intrusive_list
{

private:
    using node_type = intrusive_list_node_type;

    static_assert(is_base_of<node_type, Type>::value);

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
        using value_type = intrusive_list::value_type; //!< Value type alias.
        using size_type = intrusive_list::size_type; //!< Size type alias.
        using difference_type = intrusive_list::difference_type; //!< Difference type alias.
        using reference = intrusive_list::reference; //!< Reference alias.
        using pointer = intrusive_list::pointer; //!< Pointer alias.
        using iterator_category = bidirectional_iterator_tag; //!< Iterator category alias.

        /**
         * @brief Default class constructor.
         */
        iterator() = default;

        /**
         * @brief Class constructor.
         * @param node Intrusive list node.
         */
        explicit iterator(node_type* node) :
            _node(node)
        {
        }

        /**
         * @brief Increments the position.
         * @return Reference to this.
         */
        iterator& operator++()
        {
            _node = _node->next;
            return *this;
        }

        /**
         * @brief Increments the position.
         * @return The iterator before being incremented.
         */
        iterator operator++(int)
        {
            iterator copy(*this);
            _node = _node->next;
            return copy;
        }

        /**
         * @brief Decrements the position.
         * @return Reference to this.
         */
        iterator& operator--()
        {
            _node = _node->prev;
            return *this;
        }

        /**
         * @brief Decrements the position.
         * @return The iterator before being decremented.
         */
        iterator operator--(int)
        {
            iterator copy(*this);
            _node = _node->prev;
            return copy;
        }

        /**
         * @brief Returns a reference to the pointed value.
         */
        [[nodiscard]] reference operator*() const
        {
            return static_cast<reference>(*_node);
        }

        /**
         * @brief Returns a pointer to the pointed value.
         */
        pointer operator->() const
        {
            return static_cast<pointer>(_node);
        }

        /**
         * @brief Default equal operator.
         */
        [[nodiscard]] friend bool operator==(const iterator& a, const iterator& b) = default;

    private:
        friend class intrusive_list;
        friend class const_iterator;

        node_type* _node = nullptr;
    };

    /**
     * @brief Const iterator.
     */
    class const_iterator
    {

    public:
        using value_type = intrusive_list::value_type; //!< Value type alias.
        using size_type = intrusive_list::size_type; //!< Size type alias.
        using difference_type = intrusive_list::difference_type; //!< Difference type alias.
        using reference = intrusive_list::const_reference; //!< Reference type alias.
        using pointer = intrusive_list::const_pointer; //!< Pointer alias.
        using const_pointer = intrusive_list::const_pointer; //!< Const pointer alias.
        using iterator_category = bidirectional_iterator_tag; //!< Iterator category alias.

        /**
         * @brief Default class constructor.
         */
        const_iterator() = default;

        /**
         * @brief Class constructor.
         * @param node Intrusive list node.
         */
        explicit const_iterator(const node_type* node) :
            _node(node)
        {
        }

        /**
         * @brief Public constructor.
         * @param it Non const iterator.
         */
        const_iterator(iterator it) :
            _node(it._node)
        {
        }

        /**
         * @brief Increments the position.
         * @return Reference to this.
         */
        const_iterator& operator++()
        {
            _node = _node->next;
            return *this;
        }

        /**
         * @brief Increments the position.
         * @return The iterator before being incremented.
         */
        const_iterator operator++(int)
        {
            const_iterator copy(*this);
            _node = _node->next;
            return copy;
        }

        /**
         * @brief Decrements the position.
         * @return Reference to this.
         */
        const_iterator& operator--()
        {
            _node = _node->prev;
            return *this;
        }

        /**
         * @brief Decrements the position.
         * @return The iterator before being decremented.
         */
        const_iterator operator--(int)
        {
            const_iterator copy(*this);
            _node = _node->prev;
            return copy;
        }

        /**
         * @brief Returns a const reference to the pointed value.
         */
        [[nodiscard]] const_reference operator*() const
        {
            return static_cast<const_reference>(*_node);
        }

        /**
         * @brief Returns a const pointer to the pointed value.
         */
        const_pointer operator->() const
        {
            return static_cast<const_pointer>(_node);
        }

        /**
         * @brief Default equal operator.
         */
        [[nodiscard]] friend bool operator==(const const_iterator& a, const const_iterator& b) = default;

    private:
        friend class intrusive_list;

        const node_type* _node = nullptr;
    };

    using reverse_iterator = bn::reverse_iterator<iterator>; //!< Reverse iterator alias.
    using const_reverse_iterator = bn::reverse_iterator<const_iterator>; //!< Const reverse iterator alias.

    /**
     * @brief Default constructor.
     */
    intrusive_list()
    {
        _first_node.next = &_last_node;
        _last_node.prev = &_first_node;
    }

    intrusive_list(const intrusive_list& other) = delete;

    /**
     * @brief Move constructor.
     * @param other intrusive_list to move.
     */
    intrusive_list(intrusive_list&& other) noexcept :
        intrusive_list()
    {
        _assign(move(other));
    }

    intrusive_list& operator=(const intrusive_list& other) = delete;

    /**
     * @brief Move assignment operator.
     * @param other intrusive_list to move.
     * @return Reference to this.
     */
    intrusive_list& operator=(intrusive_list&& other) noexcept
    {
        if(this != &other)
        {
            clear();
            _assign(move(other));
        }

        return *this;
    }

    /**
     * @brief Destructor.
     */
    ~intrusive_list() noexcept
    {
        clear();
    }

    /**
     * @brief Returns the current size.
     */
    [[nodiscard]] size_type size() const
    {
        return _size;
    }

    /**
     * @brief Indicates if it doesn't contain any element.
     */
    [[nodiscard]] bool empty() const
    {
        return _size == 0;
    }

    /**
     * @brief Returns a const iterator to the beginning of the intrusive_list.
     */
    [[nodiscard]] const_iterator begin() const
    {
        return const_iterator(_first_node.next);
    }

    /**
     * @brief Returns an iterator to the beginning of the intrusive_list.
     */
    [[nodiscard]] iterator begin()
    {
        return iterator(_first_node.next);
    }

    /**
     * @brief Returns a const iterator to the end of the intrusive_list.
     */
    [[nodiscard]] const_iterator end() const
    {
        return const_iterator(&_last_node);
    }

    /**
     * @brief Returns an iterator to the end of the intrusive_list.
     */
    [[nodiscard]] iterator end()
    {
        return iterator(&_last_node);
    }

    /**
     * @brief Returns a const iterator to the beginning of the intrusive_list.
     */
    [[nodiscard]] const_iterator cbegin() const
    {
        return const_iterator(_first_node.next);
    }

    /**
     * @brief Returns a const iterator to the end of the intrusive_list.
     */
    [[nodiscard]] const_iterator cend() const
    {
        return const_iterator(&_last_node);
    }

    /**
     * @brief Returns a const reverse iterator to the end of the intrusive_list.
     */
    [[nodiscard]] const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }

    /**
     * @brief Returns a reverse iterator to the end of the intrusive_list.
     */
    [[nodiscard]] reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    /**
     * @brief Returns a const reverse iterator to the beginning of the intrusive_list.
     */
    [[nodiscard]] const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }

    /**
     * @brief Returns a reverse iterator to the beginning of the intrusive_list.
     */
    [[nodiscard]] reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    /**
     * @brief Returns a const reverse iterator to the end of the intrusive_list.
     */
    [[nodiscard]] const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(cend());
    }

    /**
     * @brief Returns a const reverse iterator to the beginning of the intrusive_list.
     */
    [[nodiscard]] const_reverse_iterator crend() const
    {
        return const_reverse_iterator(cbegin());
    }

    /**
     * @brief Returns a const reference to the first element.
     */
    [[nodiscard]] const_reference front() const
    {
        BN_BASIC_ASSERT(! empty(), "List is empty");

        return static_cast<const_reference>(*_first_node.next);
    }

    /**
     * @brief Returns a reference to the first element.
     */
    [[nodiscard]] reference front()
    {
        BN_BASIC_ASSERT(! empty(), "List is empty");

        return static_cast<reference>(*_first_node.next);
    }

    /**
     * @brief Returns a const reference to the last element.
     */
    [[nodiscard]] const_reference back() const
    {
        BN_BASIC_ASSERT(! empty(), "List is empty");

        return static_cast<const_reference>(*_last_node.prev);
    }

    /**
     * @brief Returns a reference to the last element.
     */
    [[nodiscard]] reference back()
    {
        BN_BASIC_ASSERT(! empty(), "List is empty");

        return static_cast<reference>(*_last_node.prev);
    }

    /**
     * @brief Inserts a value at the beginning of the intrusive_list.
     * @param value Value to insert.
     */
    void push_front(reference value)
    {
        _insert(begin(), value);
    }

    /**
     * @brief Inserts a value at the end of the intrusive_list.
     * @param value Value to insert.
     */
    void push_back(reference value)
    {
        _insert(end(), value);
    }

    /**
     * @brief Removes the first element of the intrusive_list.
     */
    void pop_front()
    {
        BN_BASIC_ASSERT(! empty(), "List is empty");

        _erase(begin());
    }

    /**
     * @brief Removes the last element of the intrusive_list.
     */
    void pop_back()
    {
        BN_BASIC_ASSERT(! empty(), "List is empty");

        iterator it = end();
        --it;
        _erase(it);
    }

    /**
     * @brief Inserts a value at the specified position.
     * @param position The given value is inserted before this position.
     * @param value Value to insert.
     * @return Iterator pointing to the inserted value.
     */
    iterator insert(const_iterator position, reference value)
    {
        _insert(_mutable_iterator(position), value);
        return iterator(&value);
    }

    /**
     * @brief Inserts a value at the specified position.
     * @param position_value The given value is inserted before the position of this value in the intrusive_list.
     * @param value Value to insert.
     * @return Iterator pointing to the inserted value.
     */
    iterator insert(reference position_value, reference value)
    {
        _insert(iterator(&position_value), value);
        return iterator(&value);
    }

    /**
     * @brief Erases an element.
     * @param position Iterator to the element to erase.
     * @return Iterator following the erased element.
     */
    iterator erase(const_iterator position)
    {
        BN_BASIC_ASSERT(! empty(), "List is empty");

        iterator non_const_position = _mutable_iterator(position);
        iterator next = non_const_position;
        ++next;
        _erase(non_const_position);
        return next;
    }

    /**
     * @brief Erases an element.
     * @param value Element to erase.
     * @return Iterator following the erased element.
     */
    iterator erase(reference value)
    {
        return erase(const_iterator(&value));
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
        iterator erase_first = _mutable_iterator(first);
        iterator erase_last = _mutable_iterator(last);
        iterator erase_it = erase_first;

        while(erase_it != erase_last)
        {
            BN_BASIC_ASSERT(! empty(), "List is empty");

            iterator next = erase_it;
            ++next;
            _erase(erase_it);
            erase_it = next;
        }

        return erase_it;
    }

    /**
     * @brief Erases all elements that are equal to the specified value.
     * @param value Element to erase.
     * @return Number of erased elements.
     */
    size_type remove(const_reference value)
    {
        size_type erased_count = 0;
        iterator it = begin();
        iterator last = end();

        while(it != last)
        {
            if(*it == value)
            {
                iterator next = it;
                ++next;
                _erase(it);
                it = next;
                ++erased_count;
            }
            else
            {
                ++it;
            }
        }

        return erased_count;
    }

    /**
     * @brief Erases all elements that satisfy the specified predicate.
     * @param pred Unary predicate which returns `true` if the element should be erased.
     * @return Number of erased elements.
     */
    template<class Pred>
    size_type remove_if(const Pred& pred)
    {
        size_type erased_count = 0;
        iterator it = begin();
        iterator last = end();

        while(it != last)
        {
            if(pred(*it))
            {
                iterator next = it;
                ++next;
                _erase(it);
                it = next;
                ++erased_count;
            }
            else
            {
                ++it;
            }
        }

        return erased_count;
    }

    /**
     * @brief Assigns values to the intrusive_list, removing the previous ones.
     * @param first Iterator to the first element to insert.
     * @param last Iterator following to the last element to insert.
     */
    template<typename Iterator>
    void assign(const Iterator& first, const Iterator& last)
    {
        iterator input_last = end();
        clear();

        for(Iterator it = first; it != last; ++it)
        {
            reference value = *it;
            _insert(input_last, value);
        }
    }

    /**
     * @brief Removes all elements.
     */
    void clear()
    {
        iterator it = begin();
        iterator last = end();

        while(it != last)
        {
            node_type* node = it._node;
            ++it;
            node->prev = nullptr;
            node->next = nullptr;
        }

        _first_node.next = &_last_node;
        _last_node.prev = &_first_node;
        _size = 0;
    }

    /**
     * @brief Exchanges the contents of this intrusive_list with those of the other one.
     * @param other intrusive_list to exchange the contents with.
     */
    void swap(intrusive_list& other)
    {
        if(this != &other)
        {
            intrusive_list* min_list;
            intrusive_list* max_list;
            size_type this_size = _size;
            size_type other_size = other._size;

            if(this_size < other_size)
            {
                min_list = *this;
                max_list = &other;
            }
            else
            {
                min_list = *other;
                max_list = &this;
            }

            iterator min_iterator = min_list->begin();
            iterator min_end = min_list->end();
            iterator max_iterator = max_list->begin();
            iterator max_end = max_list->end();

            while(min_iterator != min_end)
            {
                bn::swap(*min_iterator, *max_iterator);
                ++min_iterator;
                ++max_iterator;
            }

            while(max_iterator != max_end)
            {
                node_type* max_value_node = max_iterator._node;
                iterator max_next = max_iterator;
                ++max_next;
                max_list->_erase(max_iterator);
                min_list->_insert(min_end, *max_value_node);
                max_iterator = max_next;
            }

            other._size = this_size;
            _size = other_size;
        }
    }

    /**
     * @brief Exchanges the contents of a intrusive_list with those of another one.
     * @param a First intrusive_list to exchange the contents with.
     * @param b Second intrusive_list to exchange the contents with.
     */
    friend void swap(intrusive_list& a, intrusive_list& b)
    {
        a.swap(b);
    }

    /**
     * @brief Equal operator.
     * @param a First intrusive_list to compare.
     * @param b Second intrusive_list to compare.
     * @return `true` if the first intrusive_list is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator==(const intrusive_list& a, const intrusive_list& b)
    {
        if(a.size() != b.size())
        {
            return false;
        }

        return equal(a.begin(), a.end(), b.begin());
    }

    /**
     * @brief Less than operator.
     * @param a First intrusive_list to compare.
     * @param b Second intrusive_list to compare.
     * @return `true` if the first intrusive_list is lexicographically less than the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator<(const intrusive_list& a, const intrusive_list& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    /**
     * @brief Greater than operator.
     * @param a First intrusive_list to compare.
     * @param b Second intrusive_list to compare.
     * @return `true` if the first intrusive_list is lexicographically greater than the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator>(const intrusive_list& a, const intrusive_list& b)
    {
        return b < a;
    }

    /**
     * @brief Less than or equal operator.
     * @param a First intrusive_list to compare.
     * @param b Second intrusive_list to compare.
     * @return `true` if the first intrusive_list is lexicographically less than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator<=(const intrusive_list& a, const intrusive_list& b)
    {
        return ! (a > b);
    }

    /**
     * @brief Greater than or equal operator.
     * @param a First intrusive_list to compare.
     * @param b Second intrusive_list to compare.
     * @return `true` if the first intrusive_list is lexicographically greater than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator>=(const intrusive_list& a, const intrusive_list& b)
    {
        return ! (a < b);
    }

private:
    node_type _first_node;
    node_type _last_node;
    size_type _size = 0;

    [[nodiscard]] static iterator _mutable_iterator(const_iterator const_iterator)
    {
        return iterator(const_cast<node_type*>(const_iterator._node));
    }

    void _assign(intrusive_list&& other)
    {
        iterator last = end();
        iterator other_it = other.begin();
        iterator other_last = other.end();

        while(other_it != other_last)
        {
            node_type* other_node = other_it._node;
            ++other_it;
            _insert(last, *other_node);
        }

        other._first_node.next = &other._last_node;
        other._last_node.prev = &other._first_node;
        other._size = 0;
    }

    void _insert(iterator position, node_type& new_node)
    {
        node_type* position_node = position._node;
        node_type* prev_node = position_node->prev;
        prev_node->next = &new_node;
        new_node.prev = prev_node;
        new_node.next = position_node;
        position_node->prev = &new_node;
        ++_size;
    }

    void _erase(iterator position)
    {
        node_type* position_node = position._node;
        node_type* prev_node = position_node->prev;
        node_type* next_node = position_node->next;
        prev_node->next = next_node;
        next_node->prev = prev_node;
        --_size;
    }
};


/**
 * @brief Erases all elements from a intrusive_list that are equal to the specified value.
 * @param list intrusive_list from which to erase.
 * @param value Element to erase.
 * @return Number of erased elements.
 */
template<typename Type>
typename intrusive_list<Type>::size_type erase(intrusive_list<Type>& list, const Type& value)
{
    return list.remove(value);
}

/**
 * @brief Erases all elements from a intrusive_list that satisfy the specified predicate.
 * @param list intrusive_list from which to erase.
 * @param pred Unary predicate which returns `true` if the element should be erased.
 * @return Number of erased elements.
 */
template<typename Type, class Pred>
typename intrusive_list<Type>::size_type erase_if(intrusive_list<Type>& list, const Pred& pred)
{
    return list.remove_if(pred);
}

}

#endif
