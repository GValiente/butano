#ifndef BTN_UNORDERED_SET_FWD_H
#define BTN_UNORDERED_SET_FWD_H

#include "btn_functional.h"

namespace btn
{
    template<typename Key, typename KeyHash = hash<Key>, typename KeyEqual = equal_to<Key>>
    class iunordered_set;

    template<typename Key, int MaxSize, typename KeyHash = hash<Key>, typename KeyEqual = equal_to<Key>>
    class unordered_set;
}

#endif
