#include "btn_sprite_builder.h"

#include "btn_sprite_ptr.h"
#include "btn_sprite_item.h"
#include "btn_sprites_manager.h"
#include "btn_sprite_tiles_ptr.h"
#include "btn_sprite_palette_ptr.h"

namespace btn
{

sprite_builder::sprite_builder(const sprite_item& item_ref, int graphics_id) :
    _item_ref(&item_ref),
    _tiles_ptr_ref(nullptr),
    _palette_ptr_ref(nullptr),
    _shape(item_ref.shape()),
    _size(item_ref.size()),
    _graphics_id(graphics_id)
{
    BTN_ASSERT(graphics_id >= 0, "Invalid graphics_id: ", graphics_id);
    BTN_ASSERT(graphics_id < item_ref.tiles_item().graphics(), "Invalid graphics_id: ",
               graphics_id, " - ", item_ref.tiles_item().graphics());
}

sprite_builder::sprite_builder(sprite_shape shape, sprite_size size, const sprite_tiles_ptr& tiles_ptr_ref,
                               const sprite_palette_ptr& palette_ptr_ref) :
    _item_ref(nullptr),
    _tiles_ptr_ref(&tiles_ptr_ref),
    _palette_ptr_ref(&palette_ptr_ref),
    _shape(shape),
    _size(size),
    _graphics_id(0)
{
    BTN_ASSERT(tiles_ptr_ref.tiles_count() == shape_size().tiles(palette_ptr_ref.colors_count() > 16),
               "Invalid tiles_ptr size: ", tiles_ptr_ref.tiles_count(), " - ",
               shape_size().tiles(palette_ptr_ref.colors_count() > 16));
}

void sprite_builder::set_bg_priority(int bg_priority)
{
    BTN_ASSERT(bg_priority >= 0 && bg_priority <= sprites_manager::max_bg_priority(), "Invalid bg_priority: ",
               bg_priority);

    _bg_priority = bg_priority;
}

void sprite_builder::set_z_order(int z_order)
{
    BTN_ASSERT(z_order >= 0 && z_order < int(sprites_manager::z_orders()), "Invalid z_order: ", z_order);

    _z_order = z_order;
}

sprite_ptr sprite_builder::build() const
{
    return sprite_ptr::create(*this);
}

sprite_tiles_ptr sprite_builder::tiles_ptr() const
{
    if(_tiles_ptr_ref)
    {
        return *_tiles_ptr_ref;
    }

    return _item_ref->tiles_item().tiles_ptr(_graphics_id, _tiles_create_mode);
}

sprite_palette_ptr sprite_builder::palette_ptr() const
{
    if(_palette_ptr_ref)
    {
        return *_palette_ptr_ref;
    }

    return _item_ref->palette_item().palette_ptr(_palette_create_mode);
}

}
