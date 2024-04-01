/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_FORWARD_LIST_H
#define BN_FORWARD_LIST_H

/**
 * @file
 * bn::iforward_list and bn::forward_list implementation header file.
 *
 * @ingroup forward_list
 */

#include "bn_pool.h"
#include "bn_iterator.h"
#include "bn_algorithm.h"
#include "bn_forward_list_fwd.h"

namespace bn
{

template<typename Type>
class iforward_list
{

protected:
    /// @cond DO_NOT_DOCUMENT

    class node_type
    {

    public:
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
        using value_type = iforward_list::value_type; //!< Value type alias.
        using size_type = iforward_list::size_type; //!< Size type alias.
        using difference_type = iforward_list::difference_type; //!< Difference type alias.
        using reference = iforward_list::reference; //!< Reference alias.
        using const_reference = iforward_list::const_reference; //!< Const reference alias.
        using pointer = iforward_list::pointer; //!< Pointer alias.
        using const_pointer = iforward_list::const_pointer; //!< Const pointer alias.
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
            return static_cast<const value_node_type*>(_node)->value;
        }

        /**
         * @brief Returns a reference to the pointed value.
         */
        [[nodiscard]] reference operator*()
        {
            return static_cast<value_node_type*>(_node)->value;
        }

        /**
         * @brief Returns a const pointer to the pointed value.
         */
        const_pointer operator->() const
        {
            return &static_cast<const value_node_type*>(_node)->value;
        }

        /**
         * @brief Returns a pointer to the pointed value.
         */
        pointer operator->()
        {
            return &static_cast<value_node_type*>(_node)->value;
        }

        /**
         * @brief Default equal operator.
         */
        [[nodiscard]] friend bool operator==(const iterator& a, const iterator& b) = default;

    private:
        friend class iforward_list;
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
        using value_type = iforward_list::value_type; //!< Value type alias.
        using size_type = iforward_list::size_type; //!< Size type alias.
        using difference_type = iforward_list::difference_type; //!< Difference type alias.
        using reference = iforward_list::reference; //!< Reference alias.
        using const_reference = iforward_list::const_reference; //!< Const reference alias.
        using pointer = iforward_list::pointer; //!< Pointer alias.
        using const_pointer = iforward_list::const_pointer; //!< Const pointer alias.
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
        friend class iforward_list;

        const node_type* _node = nullptr;

        const_iterator() = default;

        explicit const_iterator(const node_type* node) :
            _node(node)
        {
        }
    };

    iforward_list(const iforward_list& other) = delete;

    /**
     * @brief Copy assignment operator.
     * @param other iforward_list to copy.
     * @return Reference to this.
     */
    iforward_list& operator=(const iforward_list& other)
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
     * @param other iforward_list to move.
     * @return Reference to this.
     */
    iforward_list& operator=(iforward_list&& other) noexcept
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
     * @brief Returns a const iterator to the position before the beginning of the iforward_list.
     */
    [[nodiscard]] const_iterator before_begin() const
    {
        return const_iterator(&_first_node);
    }

    /**
     * @brief Returns a const iterator to the beginning of the iforward_list.
     */
    [[nodiscard]] iterator before_begin()
    {
        return iterator(&_first_node);
    }

    /**
     * @brief Returns a const iterator to the beginning of the iforward_list.
     */
    [[nodiscard]] const_iterator begin() const
    {
        return const_iterator(_first_node.next);
    }

    /**
     * @brief Returns an iterator to the beginning of the iforward_list.
     */
    [[nodiscard]] iterator begin()
    {
        return iterator(_first_node.next);
    }

    /**
     * @brief Returns a const iterator to the end of the iforward_list.
     */
    [[nodiscard]] const_iterator end() const
    {
        return const_iterator();
    }

    /**
     * @brief Returns an iterator to the end of the iforward_list.
     */
    [[nodiscard]] iterator end()
    {
        return iterator();
    }

    /**
     * @brief Returns a const iterator to the position before the beginning of the iforward_list.
     */
    [[nodiscard]] const_iterator cbefore_begin() const
    {
        return const_iterator(&_first_node);
    }

    /**
     * @brief Returns a const iterator to the beginning of the iforward_list.
     */
    [[nodiscard]] const_iterator cbegin() const
    {
        return const_iterator(_first_node.next);
    }

    /**
     * @brief Returns a const iterator to the end of the iforward_list.
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
     * @brief Inserts a copy of a value at the beginning of the iforward_list.
     * @param value Value to insert.
     */
    void push_front(const_reference value)
    {
        node_type& new_node = _ipool->create(value);
        _insert_after(before_begin(), new_node);
    }

    /**
     * @brief Inserts a moved value at the beginning of the iforward_list.
     * @param value Value to insert.
     */
    void push_front(value_type&& value)
    {
        node_type& new_node = _ipool->create(move(value));
        _insert_after(before_begin(), new_node);
    }

    /**
     * @brief Constructs and inserts a value at the beginning of the iforward_list.
     * @param args Parameters of the value to insert.
     * @return Reference to the new value.
     */
    template<typename... Args>
    reference emplace_front(Args&&... args)
    {
        node_type& new_node = _ipool->create(forward<Args>(args)...);
        _insert_after(before_begin(), new_node);
        return static_cast<value_node_type*>(&new_node)->value;
    }

    /**
     * @brief Removes the first element of the iforward_list.
     */
    void pop_front()
    {
        BN_BASIC_ASSERT(! empty(), "List is empty");

        _erase_after(before_begin());
    }

    /**
     * @brief Inserts a copy of a value at the specified position.
     * @param position The given value is inserted after this position.
     * @param value Value to insert.
     * @return Iterator pointing to the inserted value.
     */
    iterator insert_after(const_iterator position, const_reference value)
    {
        node_type& new_node = _ipool->create(value);
        _insert_after(_mutable_iterator(position), new_node);
        return iterator(&new_node);
    }

    /**
     * @brief Inserts a moved value at the specified position.
     * @param position The given value is inserted after this position.
     * @param value Value to insert.
     * @return Iterator pointing to the inserted value.
     */
    iterator insert_after(const_iterator position, value_type&& value)
    {
        node_type& new_node = _ipool->create(move(value));
        _insert_after(_mutable_iterator(position), new_node);
        return iterator(&new_node);
    }

    /**
     * @brief Constructs and inserts a value at the specified position.
     * @param position The new value is inserted after this position.
     * @param args Parameters of the value to insert.
     * @return Iterator pointing to the new value.
     */
    template<typename... Args>
    iterator emplace_after(const_iterator position, Args&&... args)
    {
        node_type& new_node = _ipool->create(forward<Args>(args)...);
        _insert_after(_mutable_iterator(position), new_node);
        return iterator(&new_node);
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
     * @brief Assigns values to the iforward_list, removing the previous ones.
     * @param count Number of elements to insert.
     * @param value Value to fill new elements with.
     */
    void assign(size_type count, const_reference value)
    {
        BN_ASSERT(count >= 0 && count <= max_size(), "Invalid count: ", count, " - ", max_size());

        iterator before_first = before_begin();
        clear();

        for(size_type index = 0; index < count; ++index)
        {
            node_type& new_node = _ipool->create(value);
            _insert_after(before_first, new_node);
        }
    }

    /**
     * @brief Assigns values to the iforward_list, removing the previous ones.
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
            node_type& new_node = _ipool->create(*it);
            _insert_after(before_it, new_node);
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
            _ipool->destroy(static_cast<value_node_type&>(*node));
        }

        _first_node.next = nullptr;
    }

    /**
     * @brief Exchanges the contents of this iforward_list with those of the other one.
     * @param other iforward_list to exchange the contents with.
     */
    void swap(iforward_list& other)
    {
        if(this != &other)
        {
            BN_ASSERT(size() <= other.max_size(), "Invalid size: ", size(), " - ", other.max_size());
            BN_ASSERT(max_size() <= other.size(), "Invalid max size: ", max_size(), " - ", other.size());

            iforward_list* min_list;
            iforward_list* max_list;

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
                node_type& min_new_node = min_list->_ipool->create(move(*max_iterator));
                min_list->_insert_after(min_previous_iterator, min_new_node);
                ++min_previous_iterator;

                ++max_iterator;
                max_list->_erase_after(max_previous_iterator);
                ++max_previous_iterator;
            }
        }
    }

    /**
     * @brief Exchanges the contents of a iforward_list with those of another one.
     * @param a First iforward_list to exchange the contents with.
     * @param b Second iforward_list to exchange the contents with.
     */
    friend void swap(iforward_list& a, iforward_list& b)
    {
        a.swap(b);
    }

    /**
     * @brief Equal operator.
     * @param a First iforward_list to compare.
     * @param b Second iforward_list to compare.
     * @return `true` if the first iforward_list is equal to the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator==(const iforward_list& a, const iforward_list& b)
    {
        if(a.size() != b.size())
        {
            return false;
        }

        return equal(a.begin(), a.end(), b.begin());
    }

    /**
     * @brief Not equal operator.
     * @param a First iforward_list to compare.
     * @param b Second iforward_list to compare.
     * @return `true` if the first iforward_list is not equal to the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator!=(const iforward_list& a, const iforward_list& b)
    {
        return ! (a == b);
    }

    /**
     * @brief Less than operator.
     * @param a First iforward_list to compare.
     * @param b Second iforward_list to compare.
     * @return `true` if the first iforward_list is lexicographically less than the second one, otherwise `false`.
     */
    [[nodiscard]] friend bool operator<(const iforward_list& a, const iforward_list& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    /**
     * @brief Greater than operator.
     * @param a First iforward_list to compare.
     * @param b Second iforward_list to compare.
     * @return `true` if the first iforward_list is lexicographically greater than the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator>(const iforward_list& a, const iforward_list& b)
    {
        return b < a;
    }

    /**
     * @brief Less than or equal operator.
     * @param a First iforward_list to compare.
     * @param b Second iforward_list to compare.
     * @return `true` if the first iforward_list is lexicographically less than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator<=(const iforward_list& a, const iforward_list& b)
    {
        return ! (a > b);
    }

    /**
     * @brief Greater than or equal operator.
     * @param a First iforward_list to compare.
     * @param b Second iforward_list to compare.
     * @return `true` if the first iforward_list is lexicographically greater than or equal to the second one,
     * otherwise `false`.
     */
    [[nodiscard]] friend bool operator>=(const iforward_list& a, const iforward_list& b)
    {
        return ! (a < b);
    }

protected:
    /// @cond DO_NOT_DOCUMENT

    explicit iforward_list(ipool<value_node_type>& ipool) :
        _ipool(&ipool)
    {
    }

    void _assign(const iforward_list& other)
    {
        iterator before_it = before_begin();

        for(const_reference value : other)
        {
            node_type& new_node = _ipool->create(move(value));
            _insert_after(before_it, new_node);
            ++before_it;
        }
    }

    void _assign(iforward_list&& other)
    {
        iterator before_it = before_begin();

        for(reference value : other)
        {
            node_type& new_node = _ipool->create(move(value));
            _insert_after(before_it, new_node);
            ++before_it;
        }

        other.clear();
    }

    /// @endcond

private:
    ipool<value_node_type>* _ipool;
    node_type _first_node;

    [[nodiscard]] static iterator _mutable_iterator(const_iterator const_iterator)
    {
        return iterator(const_cast<node_type*>(const_iterator._node));
    }

    void _insert_after(iterator position, node_type& new_node)
    {
        node_type* position_node = position._node;
        node_type* next_node = position_node->next;
        position_node->next = &new_node;
        new_node.next = next_node;
    }

    void _erase_after(iterator position)
    {
        node_type* position_node = position._node;
        node_type* next_node = position_node->next;
        node_type* next_next_node = next_node->next;
        position_node->next = next_next_node;
        _ipool->destroy(static_cast<value_node_type&>(*next_node));
    }
};


template<typename Type, int MaxSize>
class forward_list : public iforward_list<Type>
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
    using iterator = typename iforward_list<Type>::iterator; //!< Iterator alias.
    using const_iterator = typename iforward_list<Type>::const_iterator; //!< Const iterator alias.

    /**
     * @brief Default constructor.
     */
    forward_list() :
        iforward_list<Type>(_pool)
    {
    }

    /**
     * @brief Copy constructor.
     * @param other forward_list to copy.
     */
    forward_list(const forward_list& other) :
        forward_list()
    {
        this->_assign(other);
    }

    /**
     * @brief Move constructor.
     * @param other forward_list to move.
     */
    forward_list(forward_list&& other) noexcept :
        forward_list()
    {
        this->_assign(move(other));
    }

    /**
     * @brief Copy constructor.
     * @param other iforward_list to copy.
     */
    forward_list(const iforward_list<Type>& other) :
        forward_list()
    {
        BN_ASSERT(other.size() <= MaxSize, "Not enough space: ", MaxSize, " - ", other.size());

        this->_assign(other);
    }

    /**
     * @brief Move constructor.
     * @param other iforward_list to move.
     */
    forward_list(iforward_list<Type>&& other) noexcept :
        forward_list()
    {
        BN_ASSERT(other.size() <= MaxSize, "Not enough space: ", MaxSize, " - ", other.size());

        this->_assign(move(other));
    }

    /**
     * @brief Size constructor.
     * @param count Initial size of the forward_list.
     */
    forward_list(size_type count) :
        forward_list()
    {
        this->assign(count, Type());
    }

    /**
     * @brief Size constructor.
     * @param count Initial size of the forward_list.
     * @param value Value to fill the forward_list with.
     */
    forward_list(size_type count, const_reference value) :
        forward_list()
    {
        this->assign(count, value);
    }

    /**
     * @brief Copy assignment operator.
     * @param other forward_list to copy.
     * @return Reference to this.
     */
    forward_list& operator=(const forward_list& other)
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
     * @param other forward_list to move.
     * @return Reference to this.
     */
    forward_list& operator=(forward_list&& other) noexcept
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
     * @param other iforward_list to copy.
     * @return Reference to this.
     */
    forward_list& operator=(const iforward_list<Type>& other)
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
     * @param other iforward_list to move.
     * @return Reference to this.
     */
    forward_list& operator=(iforward_list<Type>&& other) noexcept
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
    ~forward_list() noexcept = default;

    /**
     * @brief Destructor.
     */
    ~forward_list() noexcept
    requires(! is_trivially_destructible_v<Type>)
    {
        this->clear();
    }

private:
    pool<typename iforward_list<Type>::value_node_type, MaxSize> _pool;
};


/**
 * @brief Erases all elements from a iforward_list that are equal to the specified value.
 * @param list iforward_list from which to erase.
 * @param value Element to erase.
 * @return Number of erased elements.
 */
template<typename Type>
typename iforward_list<Type>::size_type erase(iforward_list<Type>& list, const Type& value)
{
    return list.remove(value);
}

/**
 * @brief Erases all elements from a iforward_list that satisfy the specified predicate.
 * @param list iforward_list from which to erase.
 * @param pred Unary predicate which returns `true` if the element should be erased.
 * @return Number of erased elements.
 */
template<typename Type, class Pred>
typename iforward_list<Type>::size_type erase_if(iforward_list<Type>& list, const Pred& pred)
{
    return list.remove_if(pred);
}

}

#endif
