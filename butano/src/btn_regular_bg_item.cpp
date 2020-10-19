#include "btn_regular_bg_item.h"

#include "btn_fixed.h"
#include "btn_optional.h"
#include "btn_regular_bg_ptr.h"
#include "btn_regular_bg_map_ptr.h"

namespace btn
{

regular_bg_ptr regular_bg_item::create_bg(fixed x, fixed y) const
{
    return regular_bg_ptr::create(x, y, *this);
}

regular_bg_ptr regular_bg_item::create_bg(const fixed_point& position) const
{
    return regular_bg_ptr::create(position, *this);
}

optional<regular_bg_ptr> regular_bg_item::create_bg_optional(fixed x, fixed y) const
{
    return regular_bg_ptr::create_optional(x, y, *this);
}

optional<regular_bg_ptr> regular_bg_item::create_bg_optional(const fixed_point& position) const
{
    return regular_bg_ptr::create_optional(position, *this);
}

regular_bg_map_ptr regular_bg_item::create_map() const
{
    return regular_bg_map_ptr::create(*this);
}

optional<regular_bg_map_ptr> regular_bg_item::create_map_optional() const
{
    return regular_bg_map_ptr::create_optional(*this);
}

}
