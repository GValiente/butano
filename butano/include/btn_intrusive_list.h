/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_INTRUSIVE_LIST_H
#define BTN_INTRUSIVE_LIST_H

#include "btn_assert.h"
#include "btn_utility.h"
#include "btn_iterator.h"
#include "btn_algorithm.h"
#include "btn_intrusive_list_fwd.h"

namespace btn
{

class intrusive_list_node_type
{

public:
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
    using value_type = Type;
    using size_type = int;
    using reference = Type&;
    using const_reference = const Type&;
    using pointer = Type*;
    using const_pointer = const Type*;

    class iterator
    {

    public:
        using value_type = intrusive_list::value_type;
        using size_type = intrusive_list::size_type;
        using reference = intrusive_list::reference;
        using const_reference = intrusive_list::const_reference;
        using pointer = intrusive_list::pointer;
        using const_pointer = intrusive_list::const_pointer;
        using iterator_category = bidirectional_iterator_tag;

        iterator& operator++()
        {
            BTN_ASSERT(_node, "Node is null");

            _node = _node->next;
            return *this;
        }

        iterator& operator--()
        {
            BTN_ASSERT(_node, "Node is null");

            _node = _node->prev;
            return *this;
        }

        [[nodiscard]] const_reference operator*() const
        {
            BTN_ASSERT(_node, "Node is null");

            return static_cast<const_reference>(*_node);
        }

        [[nodiscard]] reference operator*()
        {
            BTN_ASSERT(_node, "Node is null");

            return static_cast<reference>(*_node);
        }

        const_pointer operator->() const
        {
            BTN_ASSERT(_node, "Node is null");

            return static_cast<const_pointer>(_node);
        }

        pointer operator->()
        {
            BTN_ASSERT(_node, "Node is null");

            return static_cast<pointer>(_node);
        }

        [[nodiscard]] friend bool operator==(const iterator& a, const iterator& b) = default;

    private:
        friend class intrusive_list;
        friend class const_iterator;

        node_type* _node = nullptr;

        explicit iterator(node_type* node) :
            _node(node)
        {
        }
    };

    class const_iterator
    {

    public:
        using value_type = intrusive_list::value_type;
        using size_type = intrusive_list::size_type;
        using reference = intrusive_list::reference;
        using const_reference = intrusive_list::const_reference;
        using pointer = intrusive_list::pointer;
        using const_pointer = intrusive_list::const_pointer;
        using iterator_category = bidirectional_iterator_tag;

        const_iterator(iterator it) :
            _node(it._node)
        {
        }

        const_iterator& operator++()
        {
            BTN_ASSERT(_node, "Node is null");

            _node = _node->next;
            return *this;
        }

        const_iterator& operator--()
        {
            BTN_ASSERT(_node, "Node is null");

            _node = _node->prev;
            return *this;
        }

        [[nodiscard]] const_reference operator*() const
        {
            BTN_ASSERT(_node, "Node is null");

            return static_cast<const_reference>(*_node);
        }

        const_pointer operator->() const
        {
            BTN_ASSERT(_node, "Node is null");

            return static_cast<const_pointer>(_node);
        }

        [[nodiscard]] friend bool operator==(const const_iterator& a, const const_iterator& b) = default;

    private:
        friend class intrusive_list;

        const node_type* _node = nullptr;

        explicit const_iterator(const node_type* node) :
            _node(node)
        {
        }
    };

    using reverse_iterator = btn::reverse_iterator<iterator>;
    using const_reverse_iterator = btn::reverse_iterator<const_iterator>;

    intrusive_list() :
        _size(0)
    {
        _first_node.next = &_last_node;
        _last_node.prev = &_first_node;
    }

    intrusive_list(const intrusive_list& other) = delete;

    intrusive_list(intrusive_list&& other) noexcept :
        intrusive_list()
    {
        _assign(move(other));
    }

    intrusive_list& operator=(const intrusive_list& other) = delete;

    intrusive_list& operator=(intrusive_list&& other) noexcept
    {
        if(this != &other)
        {
            clear();
            _assign(move(other));
        }

        return *this;
    }

    ~intrusive_list()
    {
        clear();
    }

    [[nodiscard]] size_type size() const
    {
        return _size;
    }

    [[nodiscard]] bool empty() const
    {
        return _size == 0;
    }

    [[nodiscard]] const_iterator begin() const
    {
        return const_iterator(_first_node.next);
    }

    [[nodiscard]] iterator begin()
    {
        return iterator(_first_node.next);
    }

    [[nodiscard]] const_iterator end() const
    {
        return const_iterator(&_last_node);
    }

    [[nodiscard]] iterator end()
    {
        return iterator(&_last_node);
    }

    [[nodiscard]] const_iterator cbegin() const
    {
        return const_iterator(_first_node.next);
    }

    [[nodiscard]] const_iterator cend() const
    {
        return const_iterator(&_last_node);
    }

    [[nodiscard]] const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }

    [[nodiscard]] reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    [[nodiscard]] const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }

    [[nodiscard]] reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    [[nodiscard]] const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(cend());
    }

    [[nodiscard]] const_reverse_iterator crend() const
    {
        return const_reverse_iterator(cbegin());
    }

    [[nodiscard]] const_reference front() const
    {
        BTN_ASSERT(! empty(), "List is empty");

        return static_cast<const_reference>(*_first_node.next);
    }

    [[nodiscard]] reference front()
    {
        BTN_ASSERT(! empty(), "List is empty");

        return static_cast<reference>(*_first_node.next);
    }

    [[nodiscard]] const_reference back() const
    {
        BTN_ASSERT(! empty(), "List is empty");

        return static_cast<const_reference>(*_last_node.prev);
    }

    [[nodiscard]] reference back()
    {
        BTN_ASSERT(! empty(), "List is empty");

        return static_cast<reference>(*_last_node.prev);
    }

    void push_front(reference value)
    {
        _insert(begin(), value);
    }

    void push_back(reference value)
    {
        _insert(end(), value);
    }

    void pop_front()
    {
        BTN_ASSERT(! empty(), "List is empty");

        _erase(begin());
    }

    void pop_back()
    {
        BTN_ASSERT(! empty(), "List is empty");

        iterator it = end();
        --it;
        _erase(it);
    }

    iterator insert(const_iterator position, reference value)
    {
        _insert(_mutable_iterator(position), value);
        return iterator(&value);
    }

    iterator erase(const_iterator position)
    {
        BTN_ASSERT(! empty(), "List is empty");

        iterator non_const_position = _mutable_iterator(position);
        iterator next = non_const_position;
        ++next;
        _erase(non_const_position);
        return next;
    }

    iterator erase(reference value)
    {
        BTN_ASSERT(! empty(), "List is empty");

        iterator position(&value);
        iterator next = position;
        ++next;
        _erase(position);
        return next;
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        iterator erase_first = _mutable_iterator(first);
        iterator erase_last = _mutable_iterator(last);
        iterator erase_it = erase_first;

        while(erase_it != erase_last)
        {
            BTN_ASSERT(! empty(), "List is empty");

            iterator next = erase_it;
            ++next;
            _erase(erase_it);
            erase_it = next;
        }

        return erase_it;
    }

    friend void erase(intrusive_list& list, const_reference value)
    {
        iterator it = list.begin();
        iterator last = list.end();

        while(it != last)
        {
            if(*it == value)
            {
                iterator next = it;
                ++next;
                list._erase(it);
                it = next;
            }
            else
            {
                ++it;
            }
        }
    }

    template<class Pred>
    friend void erase_if(intrusive_list& list, const Pred& pred)
    {
        iterator it = list.begin();
        iterator last = list.end();

        while(it != last)
        {
            if(pred(*it))
            {
                iterator next = it;
                ++next;
                list._erase(it);
                it = next;
            }
            else
            {
                ++it;
            }
        }
    }

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
                btn::swap(*min_iterator, *max_iterator);
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

    friend void swap(intrusive_list& a, intrusive_list& b)
    {
        a.swap(b);
    }

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

    [[nodiscard]] friend bool operator==(const intrusive_list& a, const intrusive_list& b)
    {
        if(a.size() != b.size())
        {
            return false;
        }

        return equal(a.begin(), a.end(), b.begin());
    }

    [[nodiscard]] friend bool operator!=(const intrusive_list& a, const intrusive_list& b)
    {
        return ! (a == b);
    }

    [[nodiscard]] friend bool operator<(const intrusive_list& a, const intrusive_list& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    [[nodiscard]] friend bool operator>(const intrusive_list& a, const intrusive_list& b)
    {
        return b < a;
    }

    [[nodiscard]] friend bool operator<=(const intrusive_list& a, const intrusive_list& b)
    {
        return ! (a > b);
    }

    [[nodiscard]] friend bool operator>=(const intrusive_list& a, const intrusive_list& b)
    {
        return ! (a < b);
    }

protected:
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

private:
    node_type _first_node;
    node_type _last_node;
    size_type _size;

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

}

#endif
