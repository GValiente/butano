#ifndef BTN_HASH_MAP_FWD_H
#define BTN_HASH_MAP_FWD_H

#include "btn_functional.h"

namespace btn
{
    template<typename Key, typename Value, typename KeyHash = hash<Key>, typename KeyEqual = equal_to<Key>>
    class ihash_map;

    template<typename Key, typename Value, int MaxSize, typename KeyHash = hash<Key>, typename KeyEqual = equal_to<Key>>
    class hash_map;
}

#endif
