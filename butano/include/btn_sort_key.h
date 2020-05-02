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

    [[nodiscard]] constexpr static int max_z_order()
    {
        return numeric_limits<int16_t>::max() - 1;
    }

    [[nodiscard]] constexpr static int min_z_order()
    {
        return -max_z_order();
    }

    [[nodiscard]] constexpr static int z_orders()
    {
        return numeric_limits<uint16_t>::max();
    }

    constexpr sort_key() = default;

    constexpr sort_key(int priority, int z_order) :
        _value((priority << 16) + unsigned(z_order + max_z_order()))
    {
        BTN_CONSTEXPR_ASSERT(priority >= 0 && priority <= max_priority(), "Invalid priority");
        BTN_CONSTEXPR_ASSERT(z_order >= min_z_order() && z_order <= max_z_order(), "Invalid z order");
    }

    [[nodiscard]] constexpr int priority() const
    {
        return _value >> 16;
    }

    constexpr void set_priority(int priority)
    {
        BTN_CONSTEXPR_ASSERT(priority >= 0 && priority <= max_priority(), "Invalid priority");

        unsigned z_order_part = _value & unsigned(z_orders());
        _value = (priority << 16) + z_order_part;
    }

    [[nodiscard]] constexpr int z_order() const
    {
        unsigned z_order_part = _value & unsigned(z_orders());
        return int(z_order_part) - max_z_order();
    }

    constexpr void set_z_order(int z_order)
    {
        BTN_CONSTEXPR_ASSERT(z_order >= min_z_order() && z_order <= max_z_order(), "Invalid z order");

        unsigned z_order_part = _value & unsigned(z_orders());
        _value -= z_order_part;
        _value += unsigned(z_order + max_z_order());
    }

    [[nodiscard]] constexpr friend bool operator==(sort_key a, sort_key b)
    {
        return a._value == b._value;
    }

    [[nodiscard]] constexpr friend bool operator!=(sort_key a, sort_key b)
    {
        return a._value != b._value;
    }

    [[nodiscard]] constexpr friend bool operator<(sort_key a, sort_key b)
    {
        return a._value < b._value;
    }

    [[nodiscard]] constexpr friend bool operator<=(sort_key a, sort_key b)
    {
        return a._value <= b._value;
    }

    [[nodiscard]] constexpr friend bool operator>(sort_key a, sort_key b)
    {
        return a._value > b._value;
    }

    [[nodiscard]] constexpr friend bool operator>=(sort_key a, sort_key b)
    {
        return a._value >= b._value;
    }

private:
    unsigned _value = 0;
};

}

#endif
