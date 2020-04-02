#include "btn_sorted_sprites.h"

#include "btn_pool.h"
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
        pool<list, BTN_CFG_SPRITES_MAX_SORT_LAYERS> layer_pool;
        vector<list*, BTN_CFG_SPRITES_MAX_SORT_LAYERS> layer_ptrs;
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
    return data.layer_ptrs;
}

void insert(sprites_manager_item& item)
{
    layers_type& layers = data.layer_ptrs;
    unsigned sort_key = item.sort_key;
    list new_list(sort_key);
    auto layers_end = layers.end();
    auto layers_it = lower_bound(layers.begin(), layers_end, &new_list, [](const list* a, const list* b) {
        return a->sort_key() < b->sort_key();
    });

    if(layers_it == layers_end)
    {
        BTN_ASSERT(! layers.full(), "No more sprite sort layers available");

        list& pool_list = data.layer_pool.create(sort_key);
        layers.push_back(&pool_list);
        layers_it = layers_end;
    }
    else if(sort_key != (*layers_it)->sort_key())
    {
        BTN_ASSERT(! layers.full(), "No more sprite sort layers available");

        list& pool_list = data.layer_pool.create(sort_key);
        layers_it = layers.insert(layers_it, &pool_list);
    }

    list* layer = *layers_it;
    layer->push_front(item);
    ++data.items_count;
}

void erase(sprites_manager_item& item)
{
    layers_type& layers = data.layer_ptrs;
    unsigned sort_key = item.sort_key;
    list new_list(sort_key);
    auto layers_end = layers.end();
    auto layers_it = lower_bound(layers.begin(), layers_end, &new_list, [](const list* a, const list* b) {
        return a->sort_key() < b->sort_key();
    });

    BTN_ASSERT(layers_it != layers_end, "Sprite sort key not found: ", item.sort_key);
    BTN_ASSERT(sort_key == (*layers_it)->sort_key(), "Sprite sort key not found: ", item.sort_key);

    list* layer = *layers_it;
    layer->erase(item);
    --data.items_count;

    if(layer->empty())
    {
        layers.erase(layers_it);
        data.layer_pool.destroy(*layer);
    }
}

}
