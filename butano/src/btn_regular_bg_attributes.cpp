#include "btn_regular_bg_attributes.h"

#include "btn_assert.h"
#include "btn_bgs_manager.h"

namespace btn
{

regular_bg_attributes::regular_bg_attributes(int priority, bool mosaic_enabled) :
    _priority(int8_t(priority)),
    _mosaic_enabled(mosaic_enabled)
{
    BTN_ASSERT(priority >= 0 && priority <= bgs_manager::max_priority(), "Invalid priority: ", priority);
}

void regular_bg_attributes::set_priority(int priority)
{
    BTN_ASSERT(priority >= 0 && priority <= bgs_manager::max_priority(), "Invalid priority: ", priority);

    _priority = int8_t(priority);
}

}
