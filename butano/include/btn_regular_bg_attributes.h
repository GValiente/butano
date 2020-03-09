#ifndef BTN_REGULAR_BG_ATTRIBUTES_H
#define BTN_REGULAR_BG_ATTRIBUTES_H

#include "btn_common.h"

namespace btn
{

class regular_bg_attributes
{

public:
    regular_bg_attributes() = default;

    regular_bg_attributes(int priority, bool mosaic_enabled);

    [[nodiscard]] int priority() const
    {
        return _priority;
    }

    void set_priority(int priority);

    [[nodiscard]] bool mosaic_enabled() const
    {
        return _mosaic_enabled;
    }

    void set_mosaic_enabled(bool mosaic_enabled)
    {
        _mosaic_enabled = mosaic_enabled;
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
    int8_t _priority = 3;
    bool _mosaic_enabled = false;
};

}

#endif

