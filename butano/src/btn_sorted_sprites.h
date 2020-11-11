/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SORTED_SPRITES_H
#define BTN_SORTED_SPRITES_H

#include "btn_pool.h"
#include "btn_config_sprites.h"
#include "btn_sprites_manager_item.h"

namespace btn::sorted_sprites
{
    class layer : public intrusive_list_node_type
    {

    public:
        explicit layer(sort_key sort_key) :
            _sort_key(sort_key)
        {
        }

        [[nodiscard]] sort_key layer_sort_key() const
        {
            return _sort_key;
        }

        [[nodiscard]] const intrusive_list<sprites_manager_item>& items() const
        {
            return _items;
        }

        [[nodiscard]] intrusive_list<sprites_manager_item>& items()
        {
            return _items;
        }

    private:
        sort_key _sort_key;
        intrusive_list<sprites_manager_item> _items;
    };


    using layers_type = intrusive_list<layer>;


    class sorter
    {

    public:
        [[nodiscard]] layers_type& layers()
        {
            return _layer_ptrs;
        }

        void insert(sprites_manager_item& item)
        {
            layers_type& layers = _layer_ptrs;
            sort_key item_sort_key = item.sprite_sort_key;
            layers_type::iterator layers_end = layers.end();
            layers_type::iterator layers_it = lower_bound(layers.begin(), layers_end, item_sort_key,
                    [](const layer& layer, sort_key sort_key) {
                        return layer.layer_sort_key() < sort_key;
                    });

            if(layers_it == layers_end)
            {
                BTN_ASSERT(! _layer_pool.full(), "No more sprite sort layers available");

                layer& pool_layer = _layer_pool.create(item_sort_key);
                layers_it = layers.insert(layers_end, pool_layer);
            }
            else if(item_sort_key != layers_it->layer_sort_key())
            {
                BTN_ASSERT(! _layer_pool.full(), "No more sprite sort layers available");

                layer& pool_layer = _layer_pool.create(item_sort_key);
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
                _layer_ptrs.erase(*layer);
                _layer_pool.destroy(*layer);
            }
        }

        [[nodiscard]] static bool put_in_front_of_layer(sprites_manager_item& item)
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

    private:
        pool<layer, BTN_CFG_SPRITES_MAX_SORT_LAYERS> _layer_pool;
        layers_type _layer_ptrs;
    };
}

#endif
