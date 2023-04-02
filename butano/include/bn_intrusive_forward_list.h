/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_INTRUSIVE_FORWARD_LIST_H
#define BN_INTRUSIVE_FORWARD_LIST_H

/**
 * @file
 * bn::intrusive_forward_list implementation header file.
 *
 * @ingroup intrusive_forward_list
 */

#include "bn_assert.h"
#include "bn_utility.h"
#include "bn_iterator.h"
#include "bn_algorithm.h"
#include "bn_forward_list_fwd.h"

namespace bn
{

/**
 * @brief To be part of an intrusive_forward_list, values must inherit this class.
 *
 * @ingroup intrusive_forward_list
 */
class intrusive_forward_list_node_type
{

private:
    template<class Type>
    friend class intrusive_forward_list;

    intrusive_forward_list_node_type* next = nullptr;
};


template<typename Type>
class intrusive_forward_list
{

private:
    using node_type = intrusive_forward_list_node_type;

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
        using value_type = intrusive_forward_list::value_type; //!< Value type alias.
        using size_type = intrusive_forward_list::size_type; //!< Size type alias.
        using difference_type = intrusive_forward_list::difference_type; //!< Difference type alias.
        using reference = intrusive_forward_list::reference; //!< Reference alias.
        using const_reference = intrusive_forward_list::const_reference; //!< Const reference alias.
        using pointer = intrusive_forward_list::pointer; //!< Pointer alias.
        using const_pointer = intrusive_forward_list::const_pointer; //!< Const pointer alias.
        using iterator_category = forward_iterator_tag; //!< Iterator category alias.

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
         * @brief Returns a const reference to the pointed value.
         */
        [[nodiscard]] const_reference operator*() const
        {
            return static_cast<const_reference>(*_node);
        }

        /**
         * @brief Returns a reference to the pointed value.
         */
        [[nodiscard]] reference operator*()
        {
            return static_cast<reference>(*_node);
        }

        /**
         * @brief Returns a const pointer to the pointed value.
         */
        const_pointer operator->() const
        {
            return static_cast<const_pointer>(_node);
        }

        /**
         * @brief Returns a pointer to the pointed value.
         */
        pointer operator->()
        {
            return static_cast<pointer>(_node);
        }

        /**
         * @brief Default equal operator.
         */
        [[nodiscard]] friend bool operator==(const iterator& a, const iterator& b) = default;

    private:
        friend class intrusive_forward_list;
        friend class const_iterator;

        node_type* _node = nullptr;

        iterator() = default;

        explicit iterator(node_type* node) :
            _node(node)
        {
        }
    };

    /**
     * @brief Const iterator.
     */
    class const_iterator
    {

    public:
        using value_type = intrusive_forward_list::value_type; //!< Value type alias.
        using size_type = intrusive_forward_list::size_type; //!< Size type alias.
        using difference_type = intrusive_forward_list::difference_type; //!< Difference type alias.
        using reference = intrusive_forward_list::reference; //!< Reference alias.
        using const_reference = intrusive_forward_list::const_reference; //!< Const reference alias.
        using pointer = intrusive_forward_list::pointer; //!< Pointer alias.
        using const_pointer = intrusive_forward_list::const_pointer; //!< Const pointer alias.
        using iterator_category = forward_iterator_tag; //!< Iterator category alias.

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
        friend class intrusive_forward_list;

        const node_type* _node = nullptr;

        const_iterator() = default;

        explicit const_iterator(const node_type* node) :
            _node(node)
        {
        }
    };

    /**
     * @brief Default constructor.
     */
    intrusive_forward_list() = default;

    intrusive_forward_list(const intrusive_forward_list& other) = delete;

    /**
     * @brief Move constructor.
     * @param other intrusive_forward_list to move.
     */
    intrusive_forward_list(intrusive_forward_list&& other) noexcept :
        intrusive_forward_list()
    {
        _assign(move(other));
    }

    intrusive_forward_list& operator=(const intrusive_forward_list& other) = delete;

    /**
     * @brief Move assignment operator.
     * @param other intrusive_forward_list to move.
     * @return Reference to this.
     */
    intrusive_forward_list& operator=(intrusive_forward_list&& other) noexcept
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
    ~intrusive_forward_list() noexcept
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
     * @brief Returns a const iterator to the position before the beginning of the intrusive_forward_list.
     */
    [[nodiscard]] const_iterator before_begin() const
    {
        return const_iterator(&_first_node);
    }

    /**
     * @brief Returns an iterator to the position before the beginning of the intrusive_forward_list.
     */
    [[nodiscard]] iterator before_begin()
    {
        return iterator(&_first_node);
    }

    /**
     * @brief Returns a const iterator to the beginning of the intrusive_forward_list.
     */
    [[nodiscard]] const_iterator begin() const
    {
        return const_iterator(_first_node.next);
    }

    /**
     * @brief Returns an iterator to the beginning of the intrusive_forward_list.
     */
    [[nodiscard]] iterator begin()
    {
        return iterator(_first_node.next);
    }

    /**
     * @brief Returns a const iterator to the end of the intrusive_forward_list.
     */
    [[nodiscard]] const_iterator end() const
    {
        return const_iterator();
    }

    /**
     * @brief Returns an iterator to the end of the intrusive_forward_list.
     */
    [[nodiscard]] iterator end()
    {
        return iterator();
    }

    /**
     * @brief Returns a const iterator to the position before the beginning of the intrusive_forward_list.
     */
    [[nodiscard]] const_iterator cbefore_begin() const
    {
        return const_iterator(&_first_node);
    }

    /**
     * @brief Returns a const iterator to the beginning of the intrusive_forward_list.
     */
    [[nodiscard]] const_iterator cbegin() const
    {
        return const_iterator(_first_node.next);
    }

    /**
     * @brief Returns a const iterator to the end of the intrusive_forward_list.
     */
    [[nodiscard]] const_iterator cend() const
    {
        return const_iterator();
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
     * @brief Inserts a value at the beginning of the intrusive_forward_list.
     * @param value Value to insert.
     */
    void push_front(reference value)
    {
        _insert_after(before_begin(), value);
    }

    /**
     * @brief Removes the first element of the intrusive_forward_list.
     */
    void pop_front()
    {
        BN_BASIC_ASSERT(! empty(), "List is empty");

        _erase_after(before_begin());
    }

    /**
     * @brief Inserts a value at the specified position.
     * @param position The given value is inserted after this position.
     * @param value Value to insert.
     * @return Iterator pointing to the inserted value.
     */
    iterator insert_after(const_iterator position, reference value)
    {
        _insert_after(_mutable_iterator(position), value);
        return iterator(&value);
    }

    /**
     * @brief Inserts a value at the specified position.
     * @param position_value The given value is inserted after the position of this value
     * in the intrusive_forward_list.
     * @param value Value to insert.
     * @return Iterator pointing to the inserted value.
     */
    iterator insert_after(reference position_value, reference value)
    {
        _insert_after(iterator(&position_value), value);
        return iterator(&value);
    }

    /**
     * @brief Erases an element.
     * @param position Iterator to the previous element to the one to erase.
     * @return Iterator following the erased element.
     */
    iterator erase_after(const_iterator position)
    {
        BN_BASIC_ASSERT(! empty(), "List is empty");

        iterator non_const_position = _mutable_iterator(position);
        _erase_after(non_const_position);
        ++non_const_position;
        return non_const_position;
    }

    /**
     * @brief Erases an element.
     * @param value Previous element to the one to erase.
     * @return Iterator following the erased element.
     */
    iterator erase_after(reference value)
    {
        BN_BASIC_ASSERT(! empty(), "List is empty");

        iterator position(&value);
        _erase_after(position);
        ++position;
        return position;
    }

    /**
     * @brief Erases a range of elements.
     *
     * The range includes all the elements between first and last, including the
     * element pointed by last, but not the one pointed by first.
     *
     * @param first Iterator to the first element to erase.
     * @param last Iterator to the last element to erase.
     * @return Iterator following the last erased element.
     */
    iterator erase_after(const_iterator first, const_iterator last)
    {
        iterator erase_first = _mutable_iterator(first);
        iterator erase_last = _mutable_iterator(last);
        iterator erase_it = erase_first;

        while(erase_it != erase_last)
        {
            BN_BASIC_ASSERT(! empty(), "List is empty");

            _erase_after(erase_it);
            ++erase_it;
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
        iterator previous_it = before_begin();
        iterator it = begin();
        iterator last = end();

        while(it != last)
        {
            if(*it == value)
            {
                _erase_after(previous_it);
                it = previous_it;
                ++erased_count;
            }
            else
            {
                previous_it = it;
            }

            ++it;
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
        iterator previous_it = before_begin();
        iterator it = begin();
        iterator last = end();

        while(it != last)
        {
            if(pred(*it))
            {
                _erase_after(previous_it);
                it = previous_it;
                ++erased_count;
            }
            else
            {
                previous_it = it;
            }

            ++it;
        }

        return erased_count;
    }

    /**
     * @brief Assigns values to the intrusive_forward_list, removing the previous ones.
     * @param first Iterator to the first element to insert.
     * @param last Iterator following to the last element to insert.
     */
    template<typename Iterator>
    void assign(const Iterator& first, const Iterator& last)
    {
        iterator before_it = before_begin();
        clear();

        for(Iterator it = first; it != last; ++it)
        {
            reference value = *it;
            _insert_after(before_it, value);
            ++before_it;
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
            node->next = nullptr;
        }

        _first_node.next = nullptr;
        _size = 0;
    }

    /**
     * @brief Exchanges the contents of this intrusive_forward_list with those of the other one.
     * @param other intrusive_forward_list to exchange the contents with.
     */
    void swap(intrusive_forward_list& other)
    {
        if(this != &other)
        {
            intrusive_forward_list* min_list;
            intrusive_forward_list* max_list;
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

            iterator min_previous_iterator = min_list->before_begin();
            iterator min_iterator = min_list->begin();
            iterator min_end = min_list->end();
            iterator max_previous_iterator = max_list->before_begin();
            iterator max_iterator = max_list->begin();
            iterator max_end = max_list->end();

            while(min_iterator != min_end)
            {
                bn::swap(*min_iterator, *max_iterator);
                min_previous_iterator = min_iterator;
                ++min_iterator;
                max_previous_iterator = max_iterator;
                ++max_iterator;
            }

            while(max_iterator != max_end)
            {
                node_type* max_value_node = max_iterator._node;
                iterator max_next = max_iterator;
                ++max_next;
                max_list->_erase_after(max_previous_iterator);
                min_list->_insert_after(min_previous_iterator, max_value_node);
                ++min_previous_iterator;
                max_iterator = max_next;
                ++max_previous_iterator;
            }

            other._size = this_size;
            _size = other_size;
        }
    }

    /**
     * @brief Exchanges the contents of a intrusive_forward_list with those of another one.
     * @param a First intrusive_forward_list to exchange the contents with.
     * @param b Second intrusive_forward_list to exchange the contents with.
     */
    friend void swap(intrusive_forward_list& a, intrusive_forward_list& b)
    {
        a.swap(b);
    }

    /**
     * @brief Equal operator.
     * @param a First intrusive_forward_list to compare.
     * @param b Second intrusive_forward_list to compare.
     * @return `true` if the first intrusive_forward_list is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator==(const intrusive_forward_list& a, const intrusive_forward_list& b)
    {
        if(a.size() != b.size())
        {
            return false;
        }

        return equal(a.begin(), a.end(), b.begin());
    }

    /**
     * @brief Not equal operator.
     * @param a First intrusive_forward_list to compare.
     * @param b Second intrusive_forward_list to compare.
     * @return `true` if the first intrusive_forward_list is not equal to the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator!=(const intrusive_forward_list& a, const intrusive_forward_list& b)
    {
        return ! (a == b);
    }

    /**
     * @brief Less than operator.
     * @param a First intrusive_forward_list to compare.
     * @param b Second intrusive_forward_list to compare.
     * @return `true` if the first intrusive_forward_list is lexicographically less than the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator<(const intrusive_forward_list& a, const intrusive_forward_list& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    /**
     * @brief Greater than operator.
     * @param a First intrusive_forward_list to compare.
     * @param b Second intrusive_forward_list to compare.
     * @return `true` if the first intrusive_forward_list is lexicographically greater than the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator>(const intrusive_forward_list& a, const intrusive_forward_list& b)
    {
        return b < a;
    }

    /**
     * @brief Less than or equal operator.
     * @param a First intrusive_forward_list to compare.
     * @param b Second intrusive_forward_list to compare.
     * @return `true` if the first intrusive_forward_list is
     * lexicographically less than or equal to the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator<=(const intrusive_forward_list& a, const intrusive_forward_list& b)
    {
        return ! (a > b);
    }

    /**
     * @brief Greater than or equal operator.
     * @param a First intrusive_forward_list to compare.
     * @param b Second intrusive_forward_list to compare.
     * @return `true` if the first intrusive_forward_list is
     * lexicographically greater than or equal to the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator>=(const intrusive_forward_list& a, const intrusive_forward_list& b)
    {
        return ! (a < b);
    }

private:
    node_type _first_node;
    size_type _size = 0;

    [[nodiscard]] static iterator _mutable_iterator(const_iterator const_iterator)
    {
        return iterator(const_cast<node_type*>(const_iterator._node));
    }

    void _assign(intrusive_forward_list&& other)
    {
        iterator before_it = before_begin();
        iterator other_it = other.begin();
        iterator other_last = other.end();

        while(other_it != other_last)
        {
            node_type* other_node = other_it._node;
            ++other_it;
            _insert_after(before_it, *other_node);
            ++before_it;
        }

        other._first_node.next = nullptr;
        other._size = 0;
    }

    void _insert_after(iterator position, node_type& new_node)
    {
        node_type* position_node = position._node;
        node_type* next_node = position_node->next;
        position_node->next = &new_node;
        new_node.next = next_node;
        ++_size;
    }

    void _erase_after(iterator position)
    {
        node_type* position_node = position._node;
        node_type* next_node = position_node->next;
        node_type* next_next_node = next_node->next;
        position_node->next = next_next_node;
        --_size;
    }
};


/**
 * @brief Erases all elements from a intrusive_forward_list that are equal to the specified value.
 * @param list intrusive_forward_list from which to erase.
 * @param value Element to erase.
 * @return Number of erased elements.
 */
template<typename Type>
typename intrusive_forward_list<Type>::size_type erase(intrusive_forward_list<Type>& list, const Type& value)
{
    return list.remove(value);
}

/**
 * @brief Erases all elements from a intrusive_forward_list that satisfy the specified predicate.
 * @param list intrusive_forward_list from which to erase.
 * @param pred Unary predicate which returns `true` if the element should be erased.
 * @return Number of erased elements.
 */
template<typename Type, class Pred>
typename intrusive_forward_list<Type>::size_type erase_if(intrusive_forward_list<Type>& list, const Pred& pred)
{
    return list.remove_if(pred);
}

}

#endif
