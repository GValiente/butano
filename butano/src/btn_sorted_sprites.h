/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SORTED_SPRITES_H
#define BTN_SORTED_SPRITES_H

#include "btn_intrusive_list.h"
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


    [[nodiscard]] layers_type& layers();

    void insert(sprites_manager_item& item);

    void erase(sprites_manager_item& item);

    [[nodiscard]] bool put_in_front_of_layer(sprites_manager_item& item);
}

#endif
