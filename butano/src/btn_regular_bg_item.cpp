#include "btn_regular_bg_item.h"

#include "btn_fixed.h"
#include "btn_optional.h"
#include "btn_regular_bg_ptr.h"

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

optional<regular_bg_ptr> regular_bg_item::optional_create_bg(fixed x, fixed y) const
{
    return regular_bg_ptr::optional_create(x, y, *this);
}

optional<regular_bg_ptr> regular_bg_item::optional_create_bg(const fixed_point& position) const
{
    return regular_bg_ptr::optional_create(position, *this);
}

}
