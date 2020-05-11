#ifndef BTN_INTRUSIVE_FORWARD_LIST_H
#define BTN_INTRUSIVE_FORWARD_LIST_H

#include "btn_assert.h"
#include "btn_utility.h"
#include "btn_algorithm.h"
#include "btn_forward_list_fwd.h"

namespace btn
{

class intrusive_forward_list_node_type
{

public:
    intrusive_forward_list_node_type* next = nullptr;
};


template<typename Type>
class intrusive_forward_list
{

private:
    using node_type = intrusive_forward_list_node_type;

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
        iterator& operator++()
        {
            BTN_ASSERT(_node, "Node is null");

            _node = _node->next;
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

        [[nodiscard]] friend bool operator==(const iterator& a, const iterator& b)
        {
            return a._node == b._node;
        }

        [[nodiscard]] friend bool operator!=(const iterator& a, const iterator& b)
        {
            return ! (a == b);
        }

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

    class const_iterator
    {

    public:
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

            return static_cast<const_reference>(*_node);
        }

        const_pointer operator->() const
        {
            BTN_ASSERT(_node, "Node is null");

            return static_cast<const_pointer>(_node);
        }

        [[nodiscard]] friend bool operator==(const const_iterator& a, const const_iterator& b)
        {
            return a._node == b._node;
        }

        [[nodiscard]] friend bool operator!=(const const_iterator& a, const const_iterator& b)
        {
            return ! (a == b);
        }

    private:
        friend class intrusive_forward_list;

        const node_type* _node = nullptr;

        const_iterator() = default;

        explicit const_iterator(const node_type* node) :
            _node(node)
        {
        }
    };

    intrusive_forward_list() = default;

    intrusive_forward_list(const intrusive_forward_list& other) = delete;

    intrusive_forward_list(intrusive_forward_list&& other) noexcept :
        intrusive_forward_list()
    {
        _assign(move(other));
    }

    intrusive_forward_list& operator=(const intrusive_forward_list& other) = delete;

    intrusive_forward_list& operator=(intrusive_forward_list&& other) noexcept
    {
        if(this != &other)
        {
            clear();
            _assign(move(other));
        }

        return *this;
    }

    ~intrusive_forward_list()
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

        return static_cast<const_reference>(*_first_node.next);
    }

    [[nodiscard]] reference front()
    {
        BTN_ASSERT(! empty(), "List is empty");

        return static_cast<reference>(_first_node.next)->value;
    }

    void push_front(reference value)
    {
        _insert_after(before_begin(), value);
    }

    void pop_front()
    {
        BTN_ASSERT(! empty(), "List is empty");

        _erase_after(before_begin());
    }

    iterator insert_after(const_iterator position, reference value)
    {
        _insert_after(_mutable_iterator(position), value);
        return iterator(&value);
    }

    iterator erase_after(const_iterator position)
    {
        BTN_ASSERT(! empty(), "List is empty");

        iterator non_const_position = _mutable_iterator(position);
        _erase_after(non_const_position);
        ++non_const_position;
        return non_const_position;
    }

    iterator erase_after(reference value)
    {
        BTN_ASSERT(! empty(), "List is empty");

        iterator position(&value);
        _erase_after(position);
        ++position;
        return position;
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

    friend void erase(intrusive_forward_list& list, const_reference value)
    {
        iterator previous_it = list.before_begin();
        iterator it = list.begin();
        iterator last = list.end();

        while(it != last)
        {
            if(*it == value)
            {
                list._erase_after(previous_it);
                it = previous_it;
            }
            else
            {
                previous_it = it;
            }

            ++it;
        }
    }

    template<class Pred>
    friend void erase_if(intrusive_forward_list& list, const Pred& pred)
    {
        iterator previous_it = list.before_begin();
        iterator it = list.begin();
        iterator last = list.end();

        while(it != last)
        {
            if(pred(*it))
            {
                list._erase_after(previous_it);
                it = previous_it;
            }
            else
            {
                previous_it = it;
            }

            ++it;
        }
    }

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
                btn::swap(*min_iterator, *max_iterator);
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

    friend void swap(intrusive_forward_list& a, intrusive_forward_list& b)
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
            node->next = nullptr;
        }

        _first_node.next = nullptr;
        _size = 0;
    }

    [[nodiscard]] friend bool operator==(const intrusive_forward_list& a, const intrusive_forward_list& b)
    {
        if(a.size() != b.size())
        {
            return false;
        }

        return equal(a.begin(), a.end(), b.begin());
    }

    [[nodiscard]] friend bool operator!=(const intrusive_forward_list& a, const intrusive_forward_list& b)
    {
        return ! (a == b);
    }

    [[nodiscard]] friend bool operator<(const intrusive_forward_list& a, const intrusive_forward_list& b)
    {
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    [[nodiscard]] friend bool operator>(const intrusive_forward_list& a, const intrusive_forward_list& b)
    {
        return b < a;
    }

    [[nodiscard]] friend bool operator<=(const intrusive_forward_list& a, const intrusive_forward_list& b)
    {
        return ! (a > b);
    }

    [[nodiscard]] friend bool operator>=(const intrusive_forward_list& a, const intrusive_forward_list& b)
    {
        return ! (a < b);
    }

protected:
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

private:
    node_type _first_node;
    size_type _size = 0;

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

}

#endif
