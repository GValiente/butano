/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"

#include "bn_hdma.h"
#include "bn_unique_ptr.h"
#include "polygon.h"
#include "polygon_sprite.h"

#include "../../butano/hw/include/bn_hw_sprites.h"

int main()
{
    bn::core::init();

    const bn::fixed_point vertices[] = {
        bn::fixed_point(120, 1),
        bn::fixed_point(120 + 32, 80),
        bn::fixed_point(120, 158),
        bn::fixed_point(120 - 31, 80)
    };

    polygon user_polygon(vertices);
    polygon_sprite user_polygon_sprite(user_polygon, 0, 0);

    const int max_polygon_sprites = 2;
    using hdma_source_array = bn::array<uint16_t, bn::display::height() * 4 * max_polygon_sprites>;
    bn::unique_ptr<hdma_source_array> hdma_source_ptr(new hdma_source_array());
    hdma_source_array& hdma_source = *hdma_source_ptr;
    uint16_t* hdma_source_data = hdma_source.data();
    bn::hdma::start(hdma_source, bn::hw::sprites::vram()[128 - max_polygon_sprites].attr0);

    while(true)
    {
        user_polygon_sprite.update(max_polygon_sprites, hdma_source_data);

        bn::core::update();
    }
}
