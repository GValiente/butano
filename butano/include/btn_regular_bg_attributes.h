/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_REGULAR_BG_ATTRIBUTES_H
#define BTN_REGULAR_BG_ATTRIBUTES_H

#include "btn_regular_bg_map_ptr.h"

namespace btn
{

class regular_bg_attributes
{

public:
    regular_bg_attributes(const regular_bg_map_ptr& map, int priority, bool mosaic_enabled);

    regular_bg_attributes(regular_bg_map_ptr&& map, int priority, bool mosaic_enabled);

    [[nodiscard]] const regular_bg_map_ptr& map() const
    {
        return _map;
    }

    void set_map(const regular_bg_map_ptr& map)
    {
        _map = map;
    }

    void set_map(regular_bg_map_ptr&& map)
    {
        _map = move(map);
    }

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

    [[nodiscard]] friend bool operator==(const regular_bg_attributes& a, const regular_bg_attributes& b) = default;

private:
    regular_bg_map_ptr _map;
    int8_t _priority;
    bool _mosaic_enabled;
};

}

#endif

