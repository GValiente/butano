/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_UTF8_CHARACTERS_MAP_H
#define BN_UTF8_CHARACTERS_MAP_H

/**
 * @file
 * bn::utf8_characters_map header file.
 *
 * @ingroup text
 */

#include "bn_span.h"
#include "bn_utf8_characters_map_ref.h"

namespace bn
{

/**
 * @brief Maps a list of UTF-8 characters to their position in the list.
 *
 * @tparam Utf8Characters UTF-8 characters list to map.
 *
 * @ingroup text
 */
template<const span<const utf8_character>& Utf8Characters>
class utf8_characters_map
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr utf8_characters_map()
    {
        hash<int> hasher;
        uint16_t character_index = 0;

        for(const utf8_character& character : Utf8Characters)
        {
            int data = character.data();
            auto initial_item_index = _item_index(hasher(data));
            int item_index = initial_item_index;

            while(_items[item_index].valid)
            {
                BN_BASIC_ASSERT(data != _items[item_index].data, "Duplicated UTF-8 characters: ", character_index);

                item_index = _item_index(unsigned(item_index + 1));
                BN_BASIC_ASSERT(item_index != initial_item_index, "All items are allocated");
            }

            item_type& item = _items[item_index];
            item.data = data;
            item.index = character_index;
            item.valid = true;
            ++character_index;
        }
    }

    /**
     * @brief Returns a reference to this map without information about the UTF-8 characters input list.
     *
     * This map is not copied but referenced, so it should outlive the utf8_characters_map_ref
     * to avoid dangling references.
     */
    [[nodiscard]] constexpr utf8_characters_map_ref reference() const
    {
        return utf8_characters_map_ref(_items[0], _items_count, Utf8Characters.size());
    }

private:
    using item_type = utf8_characters_map_ref::item_type;

    [[nodiscard]] constexpr static int _calculate_items_count()
    {
        int result = 1;

        while(result < Utf8Characters.size())
        {
            result *= 2;
        }

        return result * 2;
    }

    static constexpr int _items_count = _calculate_items_count();

    item_type _items[_items_count] = {};

    [[nodiscard]] constexpr static int _item_index(unsigned hash)
    {
        return int(hash & (_items_count - 1));
    }
};

}

#endif
