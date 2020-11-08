/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_sorted_sprites.h"

#include "btn_pool.h"
#include "btn_list.h"
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
        pool<layer, BTN_CFG_SPRITES_MAX_SORT_LAYERS> layer_pool;
        layers_type layer_ptrs;
    };

    BTN_DATA_EWRAM static_data data;

    [[nodiscard]] layers_type::iterator _find_layers_it(sort_key key)
    {
        layers_type& layers = data.layer_ptrs;
        return lower_bound(layers.begin(), layers.end(), key, [](const layer& layer, sort_key sort_key) {
            return layer.layer_sort_key() < sort_key;
        });
    }
}

layers_type& layers()
{
    return data.layer_ptrs;
}

void insert(sprites_manager_item& item)
{
    layers_type& layers = data.layer_ptrs;
    sort_key sort_key = item.sprite_sort_key;
    layers_type::iterator layers_it = _find_layers_it(sort_key);
    layers_type::iterator layers_end = layers.end();

    if(layers_it == layers_end)
    {
        BTN_ASSERT(! data.layer_pool.full(), "No more sprite sort layers available");

        layer& pool_layer = data.layer_pool.create(sort_key);
        layers_it = layers.insert(layers_end, pool_layer);
    }
    else if(sort_key != layers_it->layer_sort_key())
    {
        BTN_ASSERT(! data.layer_pool.full(), "No more sprite sort layers available");

        layer& pool_layer = data.layer_pool.create(sort_key);
        layers_it = layers.insert(layers_it, pool_layer);
    }

    layer& layer = *layers_it;
    layer.items().push_front(item);
    item.sort_layer_ptr = &layer;
}

void erase(sprites_manager_item& item)
{
    layer* layer = item.sort_layer_ptr;
    intrusive_list<sprites_manager_item>& layer_items = layer->items();
    layer_items.erase(item);

    if(layer_items.empty())
    {
        data.layer_ptrs.erase(*layer);
        data.layer_pool.destroy(*layer);
    }
}

bool put_in_front_of_layer(sprites_manager_item& item)
{
    layer* layer = item.sort_layer_ptr;
    intrusive_list<sprites_manager_item>& layer_items = layer->items();
    bool sort = &layer_items.front() != &item;

    if(sort)
    {
        layer_items.erase(item);
        layer_items.push_front(item);
    }

    return sort;
}

}
