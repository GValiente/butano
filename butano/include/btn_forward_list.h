/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_FORWARD_LIST_H
#define BTN_FORWARD_LIST_H

#include "btn_pool.h"
#include "btn_iterator.h"
#include "btn_algorithm.h"
#include "btn_forward_list_fwd.h"

namespace btn
{

template<typename Type>
class iforward_list
{

protected:
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
        using value_type = iforward_list::value_type;
        using size_type = iforward_list::size_type;
        using reference = iforward_list::reference;
        using const_reference = iforward_list::const_reference;
        using pointer = iforward_list::pointer;
        using const_pointer = iforward_list::const_pointer;
        using iterator_category = forward_iterator_tag;

        iterator& operator++()
        {
            BTN_ASSERT(_node, "Node is null");

            _node = _node->next;
            return *this;
        }

        [[nodiscard]] const_reference operator*() const
        {
            BTN_ASSERT(_node, "Node is null");

            return static_cast<const value_node_type*>(_node)->value;
        }

        [[nodiscard]] reference operator*()
        {
            BTN_ASSERT(_node, "Node is null");

            return static_cast<value_node_type*>(_node)->value;
        }

        const_pointer operator->() const
        {
            BTN_ASSERT(_node, "Node is null");

            return &static_cast<const value_node_type*>(_node)->value;
        }

        pointer operator->()
        {
            BTN_ASSERT(_node, "Node is null");

            return &static_cast<value_node_type*>(_node)->value;
        }

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

    class const_iterator
    {

    public:
        using value_type = iforward_list::value_type;
        using size_type = iforward_list::size_type;
        using reference = iforward_list::reference;
        using const_reference = iforward_list::const_reference;
        using pointer = iforward_list::pointer;
        using const_pointer = iforward_list::const_pointer;
        using iterator_category = forward_iterator_tag;

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

        [[nodiscard]] const_reference operator*() const
        {
            BTN_ASSERT(_node, "Node is null");

            return static_cast<const value_node_type*>(_node)->value;
        }

        const_pointer operator->() const
        {
            BTN_ASSERT(_node, "Node is null");

            return &static_cast<const value_node_type*>(_node)->value;
        }

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

    iforward_list& operator=(const iforward_list& other)
    {
        if(this != &other)
        {
            BTN_ASSERT(other.size() <= max_size(), "Not enough space in list: ", max_size(), " - ", other.size());

            clear();
            _assign(other);
        }

        return *this;
    }

    iforward_list& operator=(iforward_list&& other) noexcept
    {
        if(this != &other)
        {
            BTN_ASSERT(other.size() <= max_size(), "Not enough space in list: ", max_size(), " - ", other.size());

            clear();
            _assign(move(other));
        }

        return *this;
    }

    [[nodiscard]] size_type size() const
    {
        return _ipool->size();
    }

    [[nodiscard]] size_type max_size() const
    {
        return _ipool->max_size();
    }

    [[nodiscard]] bool empty() const
    {
        return _ipool->empty();
    }

    [[nodiscard]] bool full() const
    {
        return _ipool->full();
    }

    [[nodiscard]] size_type available() const
    {
        return _ipool->available();
    }

    [[nodiscard]] const_iterator before_begin() const
    {
        return const_iterator(&_first_node);
    }

    [[nodiscard]] iterator before_begin()
    {
        return iterator(&_first_node);
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
        return const_iterator();
    }

    [[nodiscard]] iterator end()
    {
        return iterator();
    }

    [[nodiscard]] const_iterator cbefore_begin() const
    {
        return const_iterator(&_first_node);
    }

    [[nodiscard]] const_iterator cbegin() const
    {
        return const_iterator(_first_node.next);
    }

    [[nodiscard]] const_iterator cend() const
    {
        return const_iterator();
    }

    [[nodiscard]] const_reference front() const
    {
        BTN_ASSERT(! empty(), "List is empty");

        return static_cast<const value_node_type*>(_first_node.next)->value;
    }

    [[nodiscard]] reference front()
    {
        BTN_ASSERT(! empty(), "List is empty");

        return static_cast<value_node_type*>(_first_node.next)->value;
    }

    void push_front(const_reference value)
    {
        BTN_ASSERT(! full(), "List is full");

        node_type& new_node = _ipool->create(value);
        _insert_after(before_begin(), new_node);
    }

    void push_front(value_type&& value)
    {
        BTN_ASSERT(! full(), "List is full");

        node_type& new_node = _ipool->create(move(value));
        _insert_after(before_begin(), new_node);
    }

    template<typename... Args>
    reference emplace_front(Args&&... args)
    {
        BTN_ASSERT(! full(), "List is full");

        node_type& new_node = _ipool->create(forward<Args>(args)...);
        _insert_after(before_begin(), new_node);
        return static_cast<value_node_type*>(&new_node)->value;
    }

    void pop_front()
    {
        BTN_ASSERT(! empty(), "List is empty");

        _erase_after(before_begin());
    }

    iterator insert_after(const_iterator position, const_reference value)
    {
        BTN_ASSERT(! full(), "List is full");

        node_type& new_node = _ipool->create(value);
        _insert_after(_mutable_iterator(position), new_node);
        return iterator(&new_node);
    }

    iterator insert_after(const_iterator position, value_type&& value)
    {
        BTN_ASSERT(! full(), "List is full");

        node_type& new_node = _ipool->create(move(value));
        _insert_after(_mutable_iterator(position), new_node);
        return iterator(&new_node);
    }

    template<typename... Args>
    iterator emplace_after(const_iterator position, Args&&... args)
    {
        BTN_ASSERT(! full(), "List is full");

        node_type& new_node = _ipool->create(forward<Args>(args)...);
        _insert_after(_mutable_iterator(position), new_node);
        return iterator(&new_node);
    }

    iterator erase_after(const_iterator position)
    {
        BTN_ASSERT(! empty(), "List is empty");

        iterator non_const_position = _mutable_iterator(position);
        _erase_after(non_const_position);
        ++non_const_position;
        return non_const_position;
    }

    iterator erase_after(const_iterator first, const_iterator last)
    {
        iterator erase_first = _mutable_iterator(first);
        iterator erase_last = _mutable_iterator(last);
        iterator erase_it = erase_first;

        while(erase_it != erase_last)
        {
            BTN_ASSERT(! empty(), "List is empty");

            _erase_after(erase_it);
            ++erase_it;
        }

        return erase_it;
    }

    friend size_type erase(iforward_list& list, const_reference value)
    {
        size_type erased_count = 0;
        iterator previous_it = list.before_begin();
        iterator it = list.begin();
        iterator last = list.end();

        while(it != last)
        {
            if(*it == value)
            {
                list._erase_after(previous_it);
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

    template<class Pred>
    friend size_type erase_if(iforward_list& list, const Pred& pred)
    {
        size_type erased_count = 0;
        iterator previous_it = list.before_begin();
        iterator it = list.begin();
        iterator last = list.end();

        while(it != last)
        {
            if(pred(*it))
            {
                list._erase_after(previous_it);
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

    void assign(size_type count, const_reference value)
    {
        BTN_ASSERT(count >= 0 && count <= max_size(), "Invalid count: ", count, " - ", max_size());

        iterator before_first = before_begin();
        clear();

        for(size_type index = 0; index < count; ++index)
        {
            node_type& new_node = _ipool->create(value);
            _insert_after(before_first, new_node);
        }
    }

    template<typename Iterator>
    void assign(const Iterator& first, const Iterator& last)
    {
        iterator before_it = before_begin();
        clear();

        for(Iterator it = first; it != last; ++it)
        {
            BTN_ASSERT(! full(), "List is full");

            node_type& new_node = _ipool->create(*it);
            _insert_after(before_it, new_node);
            ++before_it;
        }
    }

    void swap(iforward_list& other)
    {
        if(this != &other)
        {
            BTN_ASSERT(size() <= other.max_size(), "Invalid size: ", size(), " - ", other.max_size());
            BTN_ASSERT(max_size() <= other.size(), "Invalid max size: ", max_size(), " - ", other.size());

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
                btn::swap(*min_iterator, *max_iterator);
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

    friend void swap(iforward_list& a, iforward_list& b)
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
            _ipool->destroy(static_cast<value_node_type&>(*node));
        }

        _first_node.next = nullptr;
    }

    [[nodiscard]] friend bool operator==(const iforward_list& a, const iforward_list& b)
    {
        if(a.size() != b.size())
        {
            return false;
        }

        return equal(a.begin(), a.end(), b.begin());
    }

    [[nodiscard]] friend bool operator!=(const iforward_list& a, const iforward_list& b)
    {
        return ! (a == b);
    }

    [[nodiscard]] friend bool operator<(const iforward_list& a, const iforward_list& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    [[nodiscard]] friend bool operator>(const iforward_list& a, const iforward_list& b)
    {
        return b < a;
    }

    [[nodiscard]] friend bool operator<=(const iforward_list& a, const iforward_list& b)
    {
        return ! (a > b);
    }

    [[nodiscard]] friend bool operator>=(const iforward_list& a, const iforward_list& b)
    {
        return ! (a < b);
    }

protected:
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
    using value_type = Type;
    using size_type = int;
    using reference = Type&;
    using const_reference = const Type&;
    using pointer = Type*;
    using const_pointer = const Type*;
    using iterator = typename iforward_list<Type>::iterator;
    using const_iterator = typename iforward_list<Type>::const_iterator;

    forward_list() :
        iforward_list<Type>(_pool)
    {
    }

    forward_list(const forward_list& other) :
        forward_list()
    {
        this->_assign(other);
    }

    forward_list(forward_list&& other) noexcept :
        forward_list()
    {
        this->_assign(move(other));
    }

    forward_list(const iforward_list<Type>& other) :
        forward_list()
    {
        BTN_ASSERT(other.size() <= MaxSize, "Not enough space in list: ", MaxSize, " - ", other.size());

        this->_assign(other);
    }

    forward_list(iforward_list<Type>&& other) noexcept :
        forward_list()
    {
        BTN_ASSERT(other.size() <= MaxSize, "Not enough space in list: ", MaxSize, " - ", other.size());

        this->_assign(move(other));
    }

    forward_list& operator=(const forward_list& other)
    {
        if(this != &other)
        {
            this->clear();
            this->_assign(other);
        }

        return *this;
    }

    forward_list& operator=(forward_list&& other) noexcept
    {
        if(this != &other)
        {
            this->clear();
            this->_assign(move(other));
        }

        return *this;
    }

    forward_list& operator=(const iforward_list<Type>& other)
    {
        if(this != &other)
        {
            BTN_ASSERT(other.size() <= MaxSize, "Not enough space in list: ", MaxSize, " - ", other.size());

            this->clear();
            this->_assign(other);
        }

        return *this;
    }

    forward_list& operator=(iforward_list<Type>&& other) noexcept
    {
        if(this != &other)
        {
            BTN_ASSERT(other.size() <= MaxSize, "Not enough space in list: ", MaxSize, " - ", other.size());

            this->clear();
            this->_assign(move(other));
        }

        return *this;
    }

    ~forward_list()
    {
        this->clear();
    }

private:
    pool<typename iforward_list<Type>::value_node_type, MaxSize> _pool;
};

}

#endif
