/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_LIST_H
#define BN_LIST_H

/**
 * @file
 * bn::ilist and bn::list implementation header file.
 *
 * @ingroup list
 */

#include "bn_pool.h"
#include "bn_iterator.h"
#include "bn_algorithm.h"
#include "bn_list_fwd.h"

namespace bn
{

template<typename Type>
class ilist
{

protected:
    /// @cond DO_NOT_DOCUMENT

    class node_type
    {

    public:
        node_type* prev = nullptr;
        node_type* next = nullptr;
    };

    class value_node_type : public node_type
    {

    public:
        Type value;

        template<typename... Args>
        value_node_type(Args&&... args) :
            value(forward<Args>(args)...)
        {
        }
    };

    /// @endcond

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
        using value_type = ilist::value_type; //!< Value type alias.
        using size_type = ilist::size_type; //!< Size type alias.
        using difference_type = ilist::difference_type; //!< Difference type alias.
        using reference = ilist::reference; //!< Reference alias.
        using pointer = ilist::pointer; //!< Pointer alias.
        using iterator_category = bidirectional_iterator_tag; //!< Iterator category alias.

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
            return static_cast<value_node_type*>(_node)->value;
        }

        /**
         * @brief Returns a pointer to the pointed value.
         */
        pointer operator->() const
        {
            return &static_cast<value_node_type*>(_node)->value;
        }

        /**
         * @brief Default equal operator.
         */
        [[nodiscard]] friend bool operator==(const iterator& a, const iterator& b) = default;

    private:
        friend class ilist;
        friend class const_iterator;

        node_type* _node = nullptr;

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
        using value_type = ilist::value_type; //!< Value type alias.
        using size_type = ilist::size_type; //!< Size type alias.
        using difference_type = ilist::difference_type; //!< Difference type alias.
        using reference = ilist::const_reference; //!< Reference type alias.
        using pointer = ilist::const_pointer; //!< Pointer alias.
        using iterator_category = bidirectional_iterator_tag; //!< Iterator category alias.

        /**
         * @brief Default constructor.
         */
        const_iterator() = default;

        /**
         * @brief Constructor.
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
            return static_cast<const value_node_type*>(_node)->value;
        }

        /**
         * @brief Returns a const pointer to the pointed value.
         */
        const_pointer operator->() const
        {
            return &static_cast<const value_node_type*>(_node)->value;
        }

        /**
         * @brief Default equal operator.
         */
        [[nodiscard]] friend bool operator==(const const_iterator& a, const const_iterator& b) = default;

    private:
        friend class ilist;

        const node_type* _node = nullptr;

        explicit const_iterator(const node_type* node) :
            _node(node)
        {
        }
    };

    using reverse_iterator = bn::reverse_iterator<iterator>; //!< Reverse iterator alias.
    using const_reverse_iterator = bn::reverse_iterator<const_iterator>; //!< Const reverse iterator alias.

    ilist(const ilist& other) = delete;

    /**
     * @brief Copy assignment operator.
     * @param other ilist to copy.
     * @return Reference to this.
     */
    ilist& operator=(const ilist& other)
    {
        if(this != &other)
        {
            BN_ASSERT(other.size() <= max_size(), "Not enough space: ", max_size(), " - ", other.size());

            clear();
            _assign(other);
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other ilist to move.
     * @return Reference to this.
     */
    ilist& operator=(ilist&& other) noexcept
    {
        if(this != &other)
        {
            BN_ASSERT(other.size() <= max_size(), "Not enough space: ", max_size(), " - ", other.size());

            clear();
            _assign(move(other));
        }

        return *this;
    }

    /**
     * @brief Returns the current size.
     */
    [[nodiscard]] size_type size() const
    {
        return _ipool->size();
    }

    /**
     * @brief Returns the maximum possible size.
     */
    [[nodiscard]] size_type max_size() const
    {
        return _ipool->max_size();
    }

    /**
     * @brief Returns the remaining capacity.
     */
    [[nodiscard]] size_type available() const
    {
        return _ipool->available();
    }

    /**
     * @brief Indicates if it doesn't contain any element.
     */
    [[nodiscard]] bool empty() const
    {
        return _ipool->empty();
    }

    /**
     * @brief Indicates if it can't contain any more elements.
     */
    [[nodiscard]] bool full() const
    {
        return _ipool->full();
    }

    /**
     * @brief Returns a const iterator to the beginning of the ilist.
     */
    [[nodiscard]] const_iterator begin() const
    {
        return const_iterator(_first_node.next);
    }

    /**
     * @brief Returns an iterator to the beginning of the ilist.
     */
    [[nodiscard]] iterator begin()
    {
        return iterator(_first_node.next);
    }

    /**
     * @brief Returns a const iterator to the end of the ilist.
     */
    [[nodiscard]] const_iterator end() const
    {
        return const_iterator(&_last_node);
    }

    /**
     * @brief Returns an iterator to the end of the ilist.
     */
    [[nodiscard]] iterator end()
    {
        return iterator(&_last_node);
    }

    /**
     * @brief Returns a const iterator to the beginning of the ilist.
     */
    [[nodiscard]] const_iterator cbegin() const
    {
        return const_iterator(_first_node.next);
    }

    /**
     * @brief Returns a const iterator to the end of the ilist.
     */
    [[nodiscard]] const_iterator cend() const
    {
        return const_iterator(&_last_node);
    }

    /**
     * @brief Returns a const reverse iterator to the end of the ilist.
     */
    [[nodiscard]] const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }

    /**
     * @brief Returns a reverse iterator to the end of the ilist.
     */
    [[nodiscard]] reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    /**
     * @brief Returns a const reverse iterator to the beginning of the ilist.
     */
    [[nodiscard]] const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }

    /**
     * @brief Returns a reverse iterator to the beginning of the ilist.
     */
    [[nodiscard]] reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    /**
     * @brief Returns a const reverse iterator to the end of the ilist.
     */
    [[nodiscard]] const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(cend());
    }

    /**
     * @brief Returns a const reverse iterator to the beginning of the ilist.
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

        return static_cast<const value_node_type*>(_first_node.next)->value;
    }

    /**
     * @brief Returns a reference to the first element.
     */
    [[nodiscard]] reference front()
    {
        BN_BASIC_ASSERT(! empty(), "List is empty");

        return static_cast<value_node_type*>(_first_node.next)->value;
    }

    /**
     * @brief Returns a const reference to the last element.
     */
    [[nodiscard]] const_reference back() const
    {
        BN_BASIC_ASSERT(! empty(), "List is empty");

        return static_cast<const value_node_type*>(_last_node.prev)->value;
    }

    /**
     * @brief Returns a reference to the last element.
     */
    [[nodiscard]] reference back()
    {
        BN_BASIC_ASSERT(! empty(), "List is empty");

        return static_cast<value_node_type*>(_last_node.prev)->value;
    }

    /**
     * @brief Inserts a copy of a value at the beginning of the ilist.
     * @param value Value to insert.
     */
    void push_front(const_reference value)
    {
        node_type& new_node = _ipool->create(value);
        _insert(begin(), new_node);
    }

    /**
     * @brief Inserts a moved value at the beginning of the ilist.
     * @param value Value to insert.
     */
    void push_front(value_type&& value)
    {
        node_type& new_node = _ipool->create(move(value));
        _insert(begin(), new_node);
    }

    /**
     * @brief Constructs and inserts a value at the beginning of the ilist.
     * @param args Parameters of the value to insert.
     * @return Reference to the new value.
     */
    template<typename... Args>
    reference emplace_front(Args&&... args)
    {
        node_type& new_node = _ipool->create(forward<Args>(args)...);
        _insert(begin(), new_node);
        return static_cast<value_node_type*>(&new_node)->value;
    }

    /**
     * @brief Inserts a copy of a value at the end of the ilist.
     * @param value Value to insert.
     */
    void push_back(const_reference value)
    {
        node_type& new_node = _ipool->create(value);
        _insert(end(), new_node);
    }

    /**
     * @brief Inserts a moved value at the end of the ilist.
     * @param value Value to insert.
     */
    void push_back(value_type&& value)
    {
        node_type& new_node = _ipool->create(move(value));
        _insert(end(), new_node);
    }

    /**
     * @brief Constructs and inserts a value at the end of the ilist.
     * @param args Parameters of the value to insert.
     * @return Reference to the new value.
     */
    template<typename... Args>
    reference emplace_back(Args&&... args)
    {
        node_type& new_node = _ipool->create(forward<Args>(args)...);
        _insert(end(), new_node);
        return static_cast<value_node_type*>(&new_node)->value;
    }

    /**
     * @brief Removes the first element of the ilist.
     */
    void pop_front()
    {
        BN_BASIC_ASSERT(! empty(), "List is empty");

        _erase(begin());
    }

    /**
     * @brief Removes the last element of the ilist.
     */
    void pop_back()
    {
        BN_BASIC_ASSERT(! empty(), "List is empty");

        iterator it = end();
        --it;
        _erase(it);
    }

    /**
     * @brief Inserts a copy of a value at the specified position.
     * @param position The given value is inserted before this position.
     * @param value Value to insert.
     * @return Iterator pointing to the inserted value.
     */
    iterator insert(const_iterator position, const_reference value)
    {
        node_type& new_node = _ipool->create(value);
        _insert(_mutable_iterator(position), new_node);
        return iterator(&new_node);
    }

    /**
     * @brief Inserts a moved value at the specified position.
     * @param position The given value is inserted before this position.
     * @param value Value to insert.
     * @return Iterator pointing to the inserted value.
     */
    iterator insert(const_iterator position, value_type&& value)
    {
        node_type& new_node = _ipool->create(move(value));
        _insert(_mutable_iterator(position), new_node);
        return iterator(&new_node);
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
        node_type& new_node = _ipool->create(forward<Args>(args)...);
        _insert(_mutable_iterator(position), new_node);
        return iterator(&new_node);
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
     * @brief Assigns values to the ilist, removing the previous ones.
     * @param count Number of elements to insert.
     * @param value Value to fill new elements with.
     */
    void assign(size_type count, const_reference value)
    {
        BN_ASSERT(count >= 0 && count <= max_size(), "Invalid count: ", count, " - ", max_size());

        iterator last = end();
        clear();

        for(size_type index = 0; index < count; ++index)
        {
            node_type& new_node = _ipool->create(value);
            _insert(last, new_node);
        }
    }

    /**
     * @brief Assigns values to the ilist, removing the previous ones.
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
            node_type& new_node = _ipool->create(*it);
            _insert(input_last, new_node);
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
            _ipool->destroy(static_cast<value_node_type&>(*node));
        }

        _first_node.next = &_last_node;
        _last_node.prev = &_first_node;
    }

    /**
     * @brief Exchanges the contents of this ilist with those of the other one.
     * @param other ilist to exchange the contents with.
     */
    void swap(ilist& other)
    {
        if(this != &other)
        {
            BN_ASSERT(size() <= other.max_size(), "Invalid size: ", size(), " - ", other.max_size());
            BN_ASSERT(max_size() <= other.size(), "Invalid max size: ", max_size(), " - ", other.size());

            ilist* min_list;
            ilist* max_list;

            if(size() < other.size())
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
                node_type& min_new_node = min_list->_ipool->create(move(*max_iterator));
                min_list->_insert(min_end, min_new_node);

                iterator max_next = max_iterator;
                ++max_next;
                max_list->_erase(max_iterator);
                max_iterator = max_next;
            }
        }
    }

    /**
     * @brief Exchanges the contents of a ilist with those of another one.
     * @param a First ilist to exchange the contents with.
     * @param b Second ilist to exchange the contents with.
     */
    friend void swap(ilist& a, ilist& b)
    {
        a.swap(b);
    }

    /**
     * @brief Equal operator.
     * @param a First ilist to compare.
     * @param b Second ilist to compare.
     * @return `true` if the first ilist is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator==(const ilist& a, const ilist& b)
    {
        if(a.size() != b.size())
        {
            return false;
        }

        return equal(a.begin(), a.end(), b.begin());
    }

    /**
     * @brief Less than operator.
     * @param a First ilist to compare.
     * @param b Second ilist to compare.
     * @return `true` if the first ilist is lexicographically less than the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator<(const ilist& a, const ilist& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    /**
     * @brief Greater than operator.
     * @param a First ilist to compare.
     * @param b Second ilist to compare.
     * @return `true` if the first ilist is lexicographically greater than the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator>(const ilist& a, const ilist& b)
    {
        return b < a;
    }

    /**
     * @brief Less than or equal operator.
     * @param a First ilist to compare.
     * @param b Second ilist to compare.
     * @return `true` if the first ilist is lexicographically less than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator<=(const ilist& a, const ilist& b)
    {
        return ! (a > b);
    }

    /**
     * @brief Greater than or equal operator.
     * @param a First ilist to compare.
     * @param b Second ilist to compare.
     * @return `true` if the first ilist is lexicographically greater than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator>=(const ilist& a, const ilist& b)
    {
        return ! (a < b);
    }

protected:
    /// @cond DO_NOT_DOCUMENT

    explicit ilist(ipool<value_node_type>& ipool) :
        _ipool(&ipool)
    {
        _first_node.next = &_last_node;
        _last_node.prev = &_first_node;
    }

    void _assign(const ilist& other)
    {
        iterator last = end();

        for(const_reference value : other)
        {
            node_type& new_node = _ipool->create(move(value));
            _insert(last, new_node);
        }
    }

    void _assign(ilist&& other) noexcept
    {
        iterator last = end();

        for(reference value : other)
        {
            node_type& new_node = _ipool->create(move(value));
            _insert(last, new_node);
        }

        other.clear();
    }

    /// @endcond

private:
    ipool<value_node_type>* _ipool;
    node_type _first_node;
    node_type _last_node;

    [[nodiscard]] static iterator _mutable_iterator(const_iterator const_iterator)
    {
        return iterator(const_cast<node_type*>(const_iterator._node));
    }

    void _insert(iterator position, node_type& new_node)
    {
        node_type* position_node = position._node;
        node_type* prev_node = position_node->prev;
        prev_node->next = &new_node;
        new_node.prev = prev_node;
        new_node.next = position_node;
        position_node->prev = &new_node;
    }

    void _erase(iterator position)
    {
        node_type* position_node = position._node;
        node_type* prev_node = position_node->prev;
        node_type* next_node = position_node->next;
        prev_node->next = next_node;
        next_node->prev = prev_node;
        _ipool->destroy(static_cast<value_node_type&>(*position_node));
    }
};


template<typename Type, int MaxSize>
class list : public ilist<Type>
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
    using iterator = typename ilist<Type>::iterator; //!< Iterator alias.
    using const_iterator = typename ilist<Type>::const_iterator; //!< Const iterator alias.
    using reverse_iterator = typename ilist<Type>::reverse_iterator; //!< Reverse iterator alias.
    using const_reverse_iterator = typename ilist<Type>::const_reverse_iterator; //!< Const reverse iterator alias.

    /**
     * @brief Default constructor.
     */
    list() :
        ilist<Type>(_pool)
    {
    }

    /**
     * @brief Copy constructor.
     * @param other list to copy.
     */
    list(const list& other) :
        list()
    {
        this->_assign(other);
    }

    /**
     * @brief Move constructor.
     * @param other list to move.
     */
    list(list&& other) noexcept :
        list()
    {
        this->_assign(move(other));
    }

    /**
     * @brief Copy constructor.
     * @param other ilist to copy.
     */
    list(const ilist<Type>& other) :
        list()
    {
        BN_ASSERT(other.size() <= MaxSize, "Not enough space: ", MaxSize, " - ", other.size());

        this->_assign(other);
    }

    /**
     * @brief Move constructor.
     * @param other ilist to move.
     */
    list(ilist<Type>&& other) noexcept :
        list()
    {
        BN_ASSERT(other.size() <= MaxSize, "Not enough space: ", MaxSize, " - ", other.size());

        this->_assign(move(other));
    }

    /**
     * @brief Size constructor.
     * @param count Initial size of the list.
     */
    list(size_type count) :
        list()
    {
        this->assign(count, Type());
    }

    /**
     * @brief Size constructor.
     * @param count Initial size of the list.
     * @param value Value to fill the list with.
     */
    list(size_type count, const_reference value) :
        list()
    {
        this->assign(count, value);
    }

    /**
     * @brief Copy assignment operator.
     * @param other list to copy.
     * @return Reference to this.
     */
    list& operator=(const list& other)
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
     * @param other list to move.
     * @return Reference to this.
     */
    list& operator=(list&& other) noexcept
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
     * @param other ilist to copy.
     * @return Reference to this.
     */
    list& operator=(const ilist<Type>& other)
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
     * @param other ilist to move.
     * @return Reference to this.
     */
    list& operator=(ilist<Type>&& other) noexcept
    {
        if(this != &other)
        {
            BN_ASSERT(other.size() <= MaxSize, "Not enough space: ", MaxSize, " - ", other.size());

            this->clear();
            this->_assign(move(other));
        }

        return *this;
    }

    /**
     * @brief Destructor.
     */
    ~list() noexcept = default;

    /**
     * @brief Destructor.
     */
    ~list() noexcept
    requires(! is_trivially_destructible_v<Type>)
    {
        this->clear();
    }

private:
    pool<typename ilist<Type>::value_node_type, MaxSize> _pool;
};


/**
 * @brief Erases all elements from a ilist that are equal to the specified value.
 * @param list ilist from which to erase.
 * @param value Element to erase.
 * @return Number of erased elements.
 */
template<typename Type>
typename ilist<Type>::size_type erase(ilist<Type>& list, const Type& value)
{
    return list.remove(value);
}

/**
 * @brief Erases all elements from a ilist that satisfy the specified predicate.
 * @param list ilist from which to erase.
 * @param pred Unary predicate which returns `true` if the element should be erased.
 * @return Number of erased elements.
 */
template<typename Type, class Pred>
typename ilist<Type>::size_type erase_if(ilist<Type>& list, const Pred& pred)
{
    return list.remove_if(pred);
}

}

#endif
