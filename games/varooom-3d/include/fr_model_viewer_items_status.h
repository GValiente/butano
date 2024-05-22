/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_MODEL_VIEWER_ITEMS_STATUS_H
#define FR_MODEL_VIEWER_ITEMS_STATUS_H

#include "bn_assert.h"

namespace fr
{

class model_viewer_items_status
{

public:
    [[nodiscard]] constexpr static int items_count()
    {
        return sizeof(_items) / sizeof(_items[0]);
    }

    constexpr void init()
    {
        _items[0] = 1;
    }

    [[nodiscard]] constexpr int unlocked_items_count() const
    {
        int result = 0;

        for(int item : _items)
        {
            if(item > 0)
            {
                ++result;
            }
        }

        return result;
    }

    [[nodiscard]] constexpr bool unlocked_item(int item_index) const
    {
        BN_ASSERT(item_index >= 0 && item_index < items_count(), "Invalid item index: ", item_index);

        return _items[item_index] > 0;
    }

    [[nodiscard]] constexpr bool unlock_item(int item_index)
    {
        BN_ASSERT(item_index >= 0 && item_index < items_count(), "Invalid item index: ", item_index);

        int8_t& item = _items[item_index];

        if(item == 0)
        {
            item = 1;
            return true;
        }

        return false;
    }

    [[nodiscard]] constexpr bool all_items_viewed() const
    {
        for(int item : _items)
        {
            if(item == 1)
            {
                return false;
            }
        }

        return true;
    }

    [[nodiscard]] constexpr bool viewed_item(int item_index) const
    {
        BN_ASSERT(item_index >= 0 && item_index < items_count(), "Invalid item index: ", item_index);

        return _items[item_index] == 2;
    }

    constexpr void view_item(int item_index)
    {
        BN_ASSERT(item_index >= 0 && item_index < items_count(), "Invalid item index: ", item_index);
        BN_ASSERT(unlocked_item(item_index), "Locked item index: ", item_index);

        _items[item_index] = 2;
    }

private:
    int8_t _items[30] = {};
};

}

#endif
