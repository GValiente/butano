#include "btn_bg_tiles_item.h"

#include "btn_optional.h"
#include "btn_bg_tiles_ptr.h"

namespace btn
{

bg_tiles_ptr bg_tiles_item::create_tiles() const
{
    return bg_tiles_ptr::find_or_create(_tiles_ref);
}

bg_tiles_ptr bg_tiles_item::force_create_tiles() const
{
    return bg_tiles_ptr::create(_tiles_ref);
}

optional<bg_tiles_ptr> bg_tiles_item::optional_create_tiles() const
{
    return bg_tiles_ptr::optional_find_or_create(_tiles_ref);
}

optional<bg_tiles_ptr> bg_tiles_item::optional_force_create_tiles() const
{
    return bg_tiles_ptr::optional_create(_tiles_ref);
}

}
