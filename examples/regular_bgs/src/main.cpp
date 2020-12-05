#include "bn_core.h"
#include "bn_regular_bg_tiles_ptr.h"

#include "bn_regular_bg_items_red.h"

int main()
{
    bn::core::init();

    bn::regular_bg_tiles_ptr red_tiles = bn::regular_bg_items::red.tiles_item().create_tiles();

    while(true)
    {
        bn::core::update();
    }
}
