#ifndef BTN_REGULAR_BG_ATTRIBUTES_H
#define BTN_REGULAR_BG_ATTRIBUTES_H

#include "btn_optional.h"

namespace btn
{

class regular_bg_attributes
{

public:
    regular_bg_attributes() = default;

    regular_bg_attributes(optional<int> priority, optional<bool> mosaic_enabled);

    [[nodiscard]] const optional<int>& priority() const
    {
        return _priority;
    }

    void set_priority(optional<int> priority);

    [[nodiscard]] const optional<bool>& mosaic_enabled() const
    {
        return _mosaic_enabled;
    }

    void set_mosaic_enabled(optional<bool> mosaic_enabled)
    {
        _mosaic_enabled = move(mosaic_enabled);
    }

    [[nodiscard]] friend bool operator==(const regular_bg_attributes& a, const regular_bg_attributes& b)
    {
        return a._priority == b._priority && a._mosaic_enabled == b._mosaic_enabled;
    }

    [[nodiscard]] friend bool operator!=(const regular_bg_attributes& a, const regular_bg_attributes& b)
    {
        return ! (a == b);
    }

private:
    optional<int> _priority;
    optional<bool> _mosaic_enabled;
};

}

#endif

