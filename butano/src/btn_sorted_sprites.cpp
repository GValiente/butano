#include "btn_sorted_sprites.h"

#include "btn_vector.h"
#include "btn_config_sprites.h"
#include "btn_sprites_manager_item.h"
#include "../hw/include/btn_hw_bgs.h"

namespace btn::sorted_sprites
{

namespace
{
    static_assert(BTN_CFG_SPRITES_MAX_SORT_LAYERS >= hw::bgs::count() &&
                  BTN_CFG_SPRITES_MAX_SORT_LAYERS <= BTN_CFG_SPRITES_MAX_ITEMS);

    class static_data
    {

    public:
        vector<list, BTN_CFG_SPRITES_MAX_SORT_LAYERS> layers;
        int items_count = 0;
    };

    BTN_DATA_EWRAM static_data data;
}

list::list(unsigned sort_key) :
    _sort_key(sort_key)
{
    _first_node.next = &_last_node;
    _last_node.prev = &_first_node;
}

list::~list()
{
    BTN_ASSERT(empty(), "Clear not supported");
}

list::list([[maybe_unused]] const list& other) :
    list(other._sort_key)
{
    BTN_ASSERT(other.empty(), "Copy not supported");
}

list& list::operator=([[maybe_unused]] const list& other)
{
    BTN_ASSERT(other.empty(), "Copy not supported");

    _sort_key = other._sort_key;
    return *this;
}

list::list(list&& other) :
    list(other._sort_key)
{
    *this = move(other);
}

list& list::operator=(list&& other)
{
    if(this != &other)
    {
        _sort_key = other._sort_key;

        if(other._size)
        {
            _first_node.next = other._first_node.next;
            _first_node.next->prev = &_first_node;
            _last_node.prev = other._last_node.prev;
            _last_node.prev->next = &_last_node;
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

iterator list::insert(iterator position, sprites_manager_item& item)
{
    node_type* new_node = &item;
    node_type* position_node = position._node;
    node_type* prev_node = position_node->prev;
    prev_node->next = new_node;
    new_node->prev = prev_node;
    new_node->next = position_node;
    position_node->prev = new_node;
    ++_size;
    return iterator(new_node);
}

iterator list::erase(sprites_manager_item& item)
{
    node_type* pos_node = &item;
    node_type* prev_node = pos_node->prev;
    node_type* next_node = pos_node->next;
    prev_node->next = next_node;
    next_node->prev = prev_node;
    --_size;
    return iterator(next_node);
}

int items_count()
{
    return data.items_count;
}

layers_type& layers()
{
    return data.layers;
}

void insert(sprites_manager_item& item)
{
    layers_type& layers = data.layers;
    unsigned sort_key = item.sort_key;
    list new_list(sort_key);
    auto layers_end = layers.end();
    auto layer_it = lower_bound(layers.begin(), layers_end, new_list);

    if(layer_it == layers_end)
    {
        BTN_ASSERT(! layers.full(), "No more sprite sort layers available");

        layers.emplace_back(sort_key);
        layer_it = layers_end;
    }
    else if(sort_key != layer_it->sort_key())
    {
        BTN_ASSERT(! layers.full(), "No more sprite sort layers available");

        layer_it = layers.emplace(layer_it, sort_key);
    }

    list& layer = *layer_it;
    layer.push_front(item);
    ++data.items_count;
}

void erase(sprites_manager_item& item)
{
    layers_type& layers = data.layers;
    unsigned sort_key = item.sort_key;
    list new_list(sort_key);
    auto layers_end = layers.end();
    auto layer_it = lower_bound(layers.begin(), layers_end, new_list);
    BTN_ASSERT(layer_it != layers_end, "Sprite sort key not found: ", item.sort_key);
    BTN_ASSERT(sort_key == layer_it->sort_key(), "Sprite sort key not found: ", item.sort_key);

    list& layer = *layer_it;
    layer.erase(item);
    --data.items_count;

    if(layer.empty())
    {
        layers.erase(layer_it);
    }
}

}
