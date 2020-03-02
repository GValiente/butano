#include "btn_regular_bg_attributes.h"

#include "btn_bgs_manager.h"

namespace btn
{

regular_bg_attributes::regular_bg_attributes(optional<int> priority, optional<bool> mosaic_enabled) :
    _priority(move(priority)),
    _mosaic_enabled(move(mosaic_enabled))
{
    BTN_ASSERT(! _priority || (*_priority >= 0 && *_priority <= bgs_manager::max_priority()),
               "Invalid priority: ", *_priority);
}

void regular_bg_attributes::set_priority(optional<int> priority)
{
    BTN_ASSERT(! priority || (*priority >= 0 && *priority <= bgs_manager::max_priority()),
               "Invalid priority: ", *priority);

    _priority = move(priority);
}

}
