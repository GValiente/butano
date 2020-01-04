#include "btn_sprite_builder.h"

#include "btn_sprite_ptr.h"
#include "btn_sprites_manager.h"

namespace btn
{

sprite_builder::sprite_builder(const sprite_item& item, int graphics_id) :
    _item(item),
    _shape(item.shape()),
    _size(item.size()),
    _graphics_id(graphics_id)
{
    BTN_ASSERT(graphics_id >= 0, "Invalid graphics_id: ", graphics_id);
    BTN_ASSERT(graphics_id < item.tiles_item().graphics(), "Invalid graphics_id: ", graphics_id, " - ",
               item.tiles_item().graphics());
}

sprite_builder::sprite_builder(sprite_shape shape, sprite_size size, sprite_tiles_ptr tiles_ptr,
                               sprite_palette_ptr palette_ptr) :
    _tiles_ptr(move(tiles_ptr)),
    _palette_ptr(move(palette_ptr)),
    _shape(shape),
    _size(size),
    _graphics_id(0)
{
    BTN_ASSERT(_tiles_ptr->tiles_count() == shape_size().tiles(_palette_ptr->colors_count() > 16),
               "Invalid tiles_ptr size: ", _tiles_ptr->tiles_count(), " - ",
               shape_size().tiles(_palette_ptr->colors_count() > 16));
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

sprite_ptr sprite_builder::build_and_release()
{
    return sprite_ptr::create(move(*this));
}

sprite_tiles_ptr sprite_builder::tiles() const
{
    if(_tiles_ptr)
    {
        return *_tiles_ptr;
    }

    BTN_ASSERT(_item, "Sprite item is null");

    return _item->tiles_item().tiles_ptr(_graphics_id, _tiles_create_mode);
}

sprite_palette_ptr sprite_builder::palette() const
{
    if(_palette_ptr)
    {
        return *_palette_ptr;
    }

    BTN_ASSERT(_item, "Sprite item is null");

    return _item->palette_item().palette_ptr(_palette_create_mode);
}

sprite_tiles_ptr sprite_builder::release_tiles()
{
    if(_tiles_ptr)
    {
        sprite_tiles_ptr result = move(*_tiles_ptr);
        _tiles_ptr.reset();
        return result;
    }

    BTN_ASSERT(_item, "Sprite item is null");

    return _item->tiles_item().tiles_ptr(_graphics_id, _tiles_create_mode);
}

sprite_palette_ptr sprite_builder::release_palette()
{
    if(_palette_ptr)
    {
        sprite_palette_ptr result = move(*_palette_ptr);
        _palette_ptr.reset();
        return result;
    }

    BTN_ASSERT(_item, "Sprite item is null");

    return _item->palette_item().palette_ptr(_palette_create_mode);
}

}
