#include "btn_sprite_tiles.h"

#include "btn_sprite_tiles_manager.h"

namespace btn::sprite_tiles
{

int used_tiles_count()
{
    return sprite_tiles_manager::used_tiles_count();
}

int available_tiles_count()
{
    return sprite_tiles_manager::available_tiles_count();
}

int used_items_count()
{
    return sprite_tiles_manager::used_items_count();
}

int available_items_count()
{
    return sprite_tiles_manager::available_items_count();
}

}
