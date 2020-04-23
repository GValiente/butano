#include "btn_third_sprite_attributes.h"

#include "btn_sprites.h"
#include "btn_assert.h"

namespace btn
{

third_sprite_attributes::third_sprite_attributes(sprite_tiles_ptr tiles_ptr, sprite_palette_ptr palette_ptr,
                                                 int bg_priority) :
    _tiles_ptr(move(tiles_ptr)),
    _palette_ptr(move(palette_ptr)),
    _bg_priority(int8_t(bg_priority))
{
    BTN_ASSERT(bg_priority >= 0 && bg_priority <= sprites::max_bg_priority(), "Invalid bg priority: ", bg_priority);
}

void third_sprite_attributes::set_bg_priority(int bg_priority)
{
    BTN_ASSERT(bg_priority >= 0 && bg_priority <= sprites::max_bg_priority(), "Invalid bg priority: ", bg_priority);

    _bg_priority = int8_t(bg_priority);
}

}
