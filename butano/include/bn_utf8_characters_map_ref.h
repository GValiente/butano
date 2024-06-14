/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_UTF8_CHARACTERS_MAP_REF_H
#define BN_UTF8_CHARACTERS_MAP_REF_H

/**
 * @file
 * bn::utf8_characters_map header file.
 *
 * @ingroup text
 */

#include "bn_functional.h"
#include "bn_power_of_two.h"
#include "bn_utf8_character.h"

namespace bn
{

/**
 * @brief Maps a list of UTF-8 characters to their position in the list.
 *
 * @ingroup text
 */
class utf8_characters_map_ref
{

public:
    /**
     * @brief Map item info.
     */
    struct item_type
    {
        int data = 0; //!< UTF-8 character data.
        unsigned index: 31 = 0; //!< Position of the UTF-8 character in the input list.
        bool valid: 1 = false; //!< Indicates if the item is valid or not.
    };

    /**
     * @brief Default class constructor.
     */
    constexpr utf8_characters_map_ref() = default;

    /**
     * @brief Class constructor.
     * @param items_ref Reference to a list of map items.
     *
     * Map items are not copied but referenced, so they should outlive the utf8_characters_map_ref
     * to avoid dangling references.
     *
     * @param items_count Referenced map items count.
     * @param characters_count Mapped UTF-8 characters count.
     */
    constexpr utf8_characters_map_ref(const item_type& items_ref, int items_count, int characters_count) :
        _items(&items_ref),
        _items_count_minus_one(items_count - 1),
        _characters_count(characters_count)
    {
        BN_ASSERT(power_of_two(items_count), "Items count must be a power of two: ", items_count);
        BN_ASSERT(characters_count >= 0 && characters_count <= items_count,
                  "Invalid characters count: ", characters_count, " - ", items_count);
    }

    /**
     * @brief Returns the mapped UTF-8 characters count.
     */
    [[nodiscard]] constexpr int size() const
    {
        return _characters_count;
    }

    /**
     * @brief Indicates if it doesn't contain any mapped UTF-8 character.
     */
    [[nodiscard]] constexpr bool empty() const
    {
        return _characters_count == 0;
    }

    /**
     * @brief Searches for the position of a given UTF-8 character in the input list.
     * @param character UTF-8 character to search for.
     * @return Position of the UTF-8 character in the input list.
     */
    [[nodiscard]] constexpr int index(const utf8_character& character) const
    {
        hash<int> hasher;
        int data = character.data();
        int index = _item_index(hasher(data));
        int its = 0;
        int items_count = _items_count_minus_one + 1;

        while(its < items_count && _items[index].valid)
        {
            if(data == _items[index].data)
            {
                return int(_items[index].index);
            }

            index = _item_index(unsigned(index + 1));
            ++its;
        }

        BN_ERROR("UTF-8 character not found: ", data);
        return 0;
    }

private:
    const item_type* _items = nullptr;
    int _items_count_minus_one = -1;
    int _characters_count = 0;

    [[nodiscard]] constexpr int _item_index(unsigned hash) const
    {
        return int(hash & unsigned(_items_count_minus_one));
    }
};

}

#endif
