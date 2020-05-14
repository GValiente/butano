#ifndef BTN_SORT_KEY_H
#define BTN_SORT_KEY_H

#include "btn_assert.h"
#include "btn_limits.h"

namespace btn
{

class sort_key
{

public:
    [[nodiscard]] constexpr static int max_priority()
    {
        return numeric_limits<uint16_t>::max();
    }

    [[nodiscard]] constexpr static int min_z_order()
    {
        return numeric_limits<int16_t>::min();
    }

    [[nodiscard]] constexpr static int max_z_order()
    {
        return numeric_limits<int16_t>::max();
    }

    [[nodiscard]] constexpr static int z_orders()
    {
        return numeric_limits<uint16_t>::max();
    }

    constexpr sort_key() = default;

    constexpr sort_key(int priority, int z_order) :
        _fields({ uint16_t(priority), int16_t(z_order) })
    {
        BTN_CONSTEXPR_ASSERT(priority >= 0 && priority <= max_priority(), "Invalid priority");
        BTN_CONSTEXPR_ASSERT(z_order >= min_z_order() && z_order <= max_z_order(), "Invalid z order");
    }

    [[nodiscard]] constexpr int priority() const
    {
        return _fields.priority;
    }

    constexpr void set_priority(int priority)
    {
        BTN_CONSTEXPR_ASSERT(priority >= 0 && priority <= max_priority(), "Invalid priority");

        _fields.priority = priority;
    }

    [[nodiscard]] constexpr int z_order() const
    {
        return _fields.z_order;
    }

    constexpr void set_z_order(int z_order)
    {
        BTN_CONSTEXPR_ASSERT(z_order >= min_z_order() && z_order <= max_z_order(), "Invalid z order");

        _fields.z_order = z_order;
    }

    [[nodiscard]] constexpr friend bool operator==(sort_key a, sort_key b)
    {
        return a._data == b._data;
    }

    [[nodiscard]] constexpr friend bool operator!=(sort_key a, sort_key b)
    {
        return a._data != b._data;
    }

    [[nodiscard]] constexpr friend bool operator<(sort_key a, sort_key b)
    {
        return a._data < b._data;
    }

    [[nodiscard]] constexpr friend bool operator<=(sort_key a, sort_key b)
    {
        return a._data <= b._data;
    }

    [[nodiscard]] constexpr friend bool operator>(sort_key a, sort_key b)
    {
        return a._data > b._data;
    }

    [[nodiscard]] constexpr friend bool operator>=(sort_key a, sort_key b)
    {
        return a._data >= b._data;
    }

private:
    union
    {
       struct
       {
          uint16_t priority;
          int16_t z_order;
       } _fields;
       unsigned _data = 0;
    };
};

}

#endif
