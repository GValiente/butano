/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SORTED_SPRITES_H
#define BN_SORTED_SPRITES_H

#include "bn_pool.h"
#include "bn_config_sprites.h"
#include "bn_sprites_manager_item.h"

namespace bn::sorted_sprites
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
            layers_type& layer_ptrs = _layer_ptrs;
            sort_key item_sort_key = item.sprite_sort_key;
            layers_type::iterator layers_end = layer_ptrs.end();
            layers_type::iterator layers_it = lower_bound(layer_ptrs.begin(), layers_end, item_sort_key,
                    [](const layer& layer, sort_key sort_key) {
                        return layer.layer_sort_key() < sort_key;
                    });

            if(layers_it == layers_end)
            {
                BN_BASIC_ASSERT(! _layer_pool.full(), "No more sprite sort layers available");

                layer& pool_layer = _layer_pool.create(item_sort_key);
                layers_it = layer_ptrs.insert(layers_end, pool_layer);
            }
            else if(item_sort_key != layers_it->layer_sort_key())
            {
                BN_BASIC_ASSERT(! _layer_pool.full(), "No more sprite sort layers available");

                layer& pool_layer = _layer_pool.create(item_sort_key);
                layers_it = layer_ptrs.insert(layers_it, pool_layer);
            }

            layer& layer_ref = *layers_it;
            layer_ref.items().push_front(item);

            int diff = &layer_ref - reinterpret_cast<layer*>(&layer_ptrs);
            item.sort_layer_ptr_diff = int16_t(diff);
        }

        void erase(sprites_manager_item& item)
        {
            layer* layer = _layer_ptr(item.sort_layer_ptr_diff);
            intrusive_list<sprites_manager_item>& layer_items = layer->items();
            layer_items.erase(item);

            if(layer_items.empty())
            {
                _layer_ptrs.erase(*layer);
                _layer_pool.destroy(*layer);
            }
        }

        [[nodiscard]] bool put_in_front_of_layer(sprites_manager_item& item)
        {
            layer* layer = _layer_ptr(item.sort_layer_ptr_diff);
            intrusive_list<sprites_manager_item>& layer_items = layer->items();
            bool sort = &layer_items.front() != &item;

            if(sort)
            {
                layer_items.erase(item);
                layer_items.push_front(item);
            }

            return sort;
        }

        [[nodiscard]] bool put_in_back_of_layer(sprites_manager_item& item)
        {
            layer* layer = _layer_ptr(item.sort_layer_ptr_diff);
            intrusive_list<sprites_manager_item>& layer_items = layer->items();
            bool sort = &layer_items.back() != &item;

            if(sort)
            {
                layer_items.erase(item);
                layer_items.push_back(item);
            }

            return sort;
        }

    private:
        pool<layer, BN_CFG_SPRITES_MAX_SORT_LAYERS> _layer_pool;
        layers_type _layer_ptrs;

        [[nodiscard]] layer* _layer_ptr(int diff)
        {
            return reinterpret_cast<layer*>(&_layer_ptrs) + diff;
        }
    };
}

#endif
