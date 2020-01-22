#ifndef BTN_SORTED_SPRITES_H
#define BTN_SORTED_SPRITES_H

#include "../3rd_party/etl/include/etl/map.h"

namespace btn
{
    class sprites_manager_item;
}

namespace btn::sorted_sprites
{
    class node
    {

    public:
        node* prev = nullptr;
        sprites_manager_item* item = nullptr;
        node* next = nullptr;
    };


    class iterator
    {

    public:
        iterator() = default;

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

        [[nodiscard]] sprites_manager_item& operator*()
        {
            return *_node->item;
        }

        [[nodiscard]] const sprites_manager_item& operator*() const
        {
            return *_node->item;
        }

        sprites_manager_item* operator->()
        {
            return _node->item;
        }

        const sprites_manager_item* operator->() const
        {
            return _node->item;
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

        node* _node = nullptr;

        explicit iterator(node* node) :
            _node(node)
        {
        }
    };


    class const_iterator
    {

    public:
        const_iterator() = default;

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
            return *_node->item;
        }

        const sprites_manager_item* operator->() const
        {
            return _node->item;
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

        const node* _node = nullptr;

        explicit const_iterator(const node* node) :
            _node(node)
        {
        }
    };


    class list
    {

    public:
        list();

        list(const list& other);

        list& operator=(const list& other);

        list(list&& other);

        list& operator=(list&& other);

        ~list()
        {
            clear();
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

        iterator push_back(sprites_manager_item& item)
        {
            return insert(iterator(&_last_node), item);
        }

        iterator insert(iterator pos, sprites_manager_item& item);

        iterator erase(iterator pos);

        void clear();

    private:
        node _first_node;
        node _last_node;
        int _size = 0;
    };

    using layers_type = etl::imap<unsigned, list>;

    void insert(sprites_manager_item& item);

    void erase(const sprites_manager_item& item);

    layers_type& layers();
}

#endif
