#include "btn_bg_tiles_item.h"

#include "btn_optional.h"
#include "btn_bg_tiles_ptr.h"

namespace btn
{

optional<bg_tiles_ptr> bg_tiles_item::find_tiles() const
{
    return bg_tiles_ptr::find(_tiles_ref);
}

bg_tiles_ptr bg_tiles_item::create_tiles() const
{
    return bg_tiles_ptr::find_or_create(_tiles_ref);
}

bg_tiles_ptr bg_tiles_item::create_new_tiles() const
{
    return bg_tiles_ptr::create(_tiles_ref);
}

optional<bg_tiles_ptr> bg_tiles_item::create_tiles_optional() const
{
    return bg_tiles_ptr::find_or_create_optional(_tiles_ref);
}

optional<bg_tiles_ptr> bg_tiles_item::create_new_tiles_optional() const
{
    return bg_tiles_ptr::create_optional(_tiles_ref);
}

}
