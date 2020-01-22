#include "btn_sorted_sprites.h"

#include "btn_pool.h"
#include "btn_assert.h"
#include "btn_config_sprites.h"
#include "btn_sprites_manager_item.h"
#include "../hw/include/btn_hw_bgs.h"

namespace btn::sorted_sprites
{

static_assert(BTN_CFG_SPRITES_MAX_SORT_LAYERS >= hw::bgs::count() &&
              BTN_CFG_SPRITES_MAX_SORT_LAYERS <= BTN_CFG_SPRITES_MAX_ITEMS);

namespace
{
    class static_data
    {

    public:
        pool<node, BTN_CFG_SPRITES_MAX_ITEMS> items_pool;
        etl::map<unsigned, list, BTN_CFG_SPRITES_MAX_SORT_LAYERS> layers;
    };

    BTN_DATA_EWRAM static_data data;
}

list::list()
{
    _first_node.next = &_last_node;
    _last_node.prev = &_first_node;
}

list::list(const list& other) :
    list()
{
    for(const sprites_manager_item& item : other)
    {
        push_back(const_cast<sprites_manager_item&>(item));
    }
}

list& list::operator=(const list& other)
{
    if(this != &other)
    {
        clear();

        for(const sprites_manager_item& item : other)
        {
            push_back(const_cast<sprites_manager_item&>(item));
        }
    }

    return *this;
}

list::list(list&& other)
{
    *this = std::move(other);
}

list& list::operator=(list&& other)
{
    if(this != &other)
    {
        if(other._size)
        {
            _first_node.next = other._first_node.next;
            _last_node.prev = other._last_node.prev;
            _size = other._size;

            other._first_node.next = &other._last_node;
            other._last_node.prev = &other._first_node;
            other._size = 0;
        }
        else
        {
            _first_node.next = &_last_node;
            _last_node.prev = &_first_node;
        }
    }

    return *this;
}

iterator list::insert(iterator pos, sprites_manager_item& item)
{
    node* new_node = data.items_pool.create<node>();
    new_node->item = &item;

    node* pos_node = pos._node;
    node* prev_node = pos_node->prev;
    prev_node->next = new_node;
    new_node->prev = prev_node;
    new_node->next = pos_node;
    pos_node->prev = new_node;
    ++_size;
    return iterator(new_node);
}

iterator list::erase(iterator pos)
{
    node* pos_node = pos._node;
    node* prev_node = pos_node->prev;
    node* next_node = pos_node->next;
    prev_node->next = next_node;
    next_node->prev = prev_node;
    data.items_pool.destroy<node>(pos_node);
    --_size;
    return iterator(next_node);
}

void list::clear()
{
    node* current_node = _first_node.next;
    node* last_node = &_last_node;

    while(current_node != last_node)
    {
        node* current_node_copy = current_node;
        current_node = current_node->next;
        data.items_pool.destroy<node>(current_node_copy);
    }

    _size = 0;
}

void insert(sprites_manager_item& item)
{
    auto layer_it = data.layers.find(item.sort_key);

    if(layer_it == data.layers.end())
    {
        BTN_ASSERT(! data.layers.full(), "No more sprite sort layers available");

        layer_it = data.layers.insert(layer_it, make_pair(item.sort_key, list()));
    }

    list& layer = layer_it->second;
    item.sort_iterator = layer.push_back(item);
}

void erase(const sprites_manager_item& item)
{
    auto layer_it = data.layers.find(item.sort_key);
    BTN_ASSERT(layer_it != data.layers.end(), "Sprite sort key not found: ", item.sort_key);

    list& layer = layer_it->second;
    layer.erase(item.sort_iterator);

    if(layer.size() == 0)
    {
        data.layers.erase(layer_it);
    }
}

layers_type& layers()
{
    return data.layers;
}

}
