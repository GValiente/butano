#ifndef BTN_HASH_SET_FWD_H
#define BTN_HASH_SET_FWD_H

#include "btn_functional.h"

namespace btn
{
    template<typename Key, typename KeyHash = hash<Key>, typename KeyEqual = equal_to<Key>>
    class ihash_set;

    template<typename Key, size_t MaxSize, typename KeyHash = hash<Key>, typename KeyEqual = equal_to<Key>>
    class hash_set;
}

#endif
