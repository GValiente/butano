#ifndef BTN_SORTED_SPRITES_H
#define BTN_SORTED_SPRITES_H

#include "btn_vector_fwd.h"
#include "btn_sprites_manager_item.h"

namespace btn::sorted_sprites
{
    using node_type = sprites_manager_node;


    class iterator
    {

    public:
        iterator& operator++()
        {
            _node = _node->next;
            return *this;
        }

        iterator& operator--()
        {
            _node = _node->prev;
            return *this;
        }

        [[nodiscard]] const sprites_manager_item& operator*() const
        {
            return static_cast<sprites_manager_item&>(*_node);
        }

        [[nodiscard]] sprites_manager_item& operator*()
        {
            return static_cast<sprites_manager_item&>(*_node);
        }

        const sprites_manager_item* operator->() const
        {
            return static_cast<sprites_manager_item*>(_node);
        }

        sprites_manager_item* operator->()
        {
            return static_cast<sprites_manager_item*>(_node);
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
        friend class list;
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
        const_iterator(iterator it) :
            _node(it._node)
        {
        }

        const_iterator& operator++()
        {
            _node = _node->next;
            return *this;
        }

        const_iterator& operator--()
        {
            _node = _node->prev;
            return *this;
        }

        [[nodiscard]] const sprites_manager_item& operator*() const
        {
            return static_cast<const sprites_manager_item&>(*_node);
        }

        const sprites_manager_item* operator->() const
        {
            return static_cast<const sprites_manager_item*>(_node);
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
        friend class list;

        const node_type* _node = nullptr;

        explicit const_iterator(const node_type* node) :
            _node(node)
        {
        }
    };


    class list
    {

    public:
        explicit list(unsigned sort_key);

        [[nodiscard]] unsigned sort_key() const
        {
            return _sort_key;
        }

        [[nodiscard]] bool empty() const
        {
            return _size == 0;
        }

        [[nodiscard]] int size() const
        {
            return _size;
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

        void push_front(sprites_manager_item& item)
        {
            insert(begin(), item);
        }

        void push_back(sprites_manager_item& item)
        {
            insert(end(), item);
        }

        iterator insert(iterator position, sprites_manager_item& item);

        iterator erase(sprites_manager_item& item);

    private:
        unsigned _sort_key;
        node_type _first_node;
        node_type _last_node;
        int _size = 0;
    };


    using layers_type = ivector<list*>;


    [[nodiscard]] int items_count();

    [[nodiscard]] layers_type& layers();

    void insert(sprites_manager_item& item);

    void erase(sprites_manager_item& item);
}

#endif
