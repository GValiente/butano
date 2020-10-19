#include "btn_bg_tiles_item.h"

#include "btn_optional.h"
#include "btn_bg_tiles_ptr.h"

namespace btn
{

optional<bg_tiles_ptr> bg_tiles_item::find_tiles() const
{
    return bg_tiles_ptr::find(*this);
}

bg_tiles_ptr bg_tiles_item::create_tiles() const
{
    return bg_tiles_ptr::find_or_create(*this);
}

bg_tiles_ptr bg_tiles_item::create_new_tiles() const
{
    return bg_tiles_ptr::create(*this);
}

optional<bg_tiles_ptr> bg_tiles_item::create_tiles_optional() const
{
    return bg_tiles_ptr::find_or_create_optional(*this);
}

optional<bg_tiles_ptr> bg_tiles_item::create_new_tiles_optional() const
{
    return bg_tiles_ptr::create_optional(*this);
}

}
