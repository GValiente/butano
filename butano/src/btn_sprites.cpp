#include "btn_sprites.h"

#include "btn_sprites_manager.h"

namespace btn::sprites
{

int used_items_count()
{
    return sprites_manager::used_items_count();
}

int available_items_count()
{
    return sprites_manager::available_items_count();
}

}
