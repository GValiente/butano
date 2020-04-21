#include "btn_regular_bg_attributes.h"

#include "btn_bgs.h"
#include "btn_assert.h"
#include "btn_bg_tiles_ptr.h"

namespace btn
{

regular_bg_attributes::regular_bg_attributes(const regular_bg_map_ptr& map_ptr, int priority, bool mosaic_enabled) :
    _map_ptr(map_ptr),
    _priority(int8_t(priority)),
    _mosaic_enabled(mosaic_enabled)
{
    BTN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);

    _update_map_info();
}

regular_bg_attributes::regular_bg_attributes(regular_bg_map_ptr&& map_ptr, int priority, bool mosaic_enabled) :
    _map_ptr(move(map_ptr)),
    _priority(int8_t(priority)),
    _mosaic_enabled(mosaic_enabled)
{
    BTN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);

    _update_map_info();
}

void regular_bg_attributes::set_map(const regular_bg_map_ptr& map_ptr)
{
    _map_ptr = map_ptr;
    _update_map_info();
}

void regular_bg_attributes::set_map(regular_bg_map_ptr&& map_ptr)
{
    _map_ptr = move(map_ptr);
    _update_map_info();
}

void regular_bg_attributes::set_priority(int priority)
{
    BTN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);

    _priority = int8_t(priority);
}

void regular_bg_attributes::_update_map_info()
{
    _tiles_cbb = int8_t(_map_ptr.tiles().cbb());
    _bpp_mode = int8_t(_map_ptr.bpp_mode());
}

}
