#include "btn_regular_bg_attributes.h"

#include "btn_bgs.h"
#include "btn_assert.h"
#include "btn_bg_tiles_ptr.h"

namespace btn
{

regular_bg_attributes::regular_bg_attributes(const regular_bg_map_ptr& map, int priority, bool mosaic_enabled) :
    _map(map),
    _priority(int8_t(priority)),
    _mosaic_enabled(mosaic_enabled)
{
    BTN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);
}

regular_bg_attributes::regular_bg_attributes(regular_bg_map_ptr&& map, int priority, bool mosaic_enabled) :
    _map(move(map)),
    _priority(int8_t(priority)),
    _mosaic_enabled(mosaic_enabled)
{
    BTN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);
}

void regular_bg_attributes::set_priority(int priority)
{
    BTN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);

    _priority = int8_t(priority);
}

}
