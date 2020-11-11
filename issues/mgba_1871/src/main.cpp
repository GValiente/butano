/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_core.h"

#include "polygon.h"
#include "polygon_sprite.h"

#include "../../butano/hw/include/btn_hw_hdma.h"
#include "../../butano/hw/include/btn_hw_sprites.h"

int main()
{
    btn::core::init();

    const btn::fixed_point vertices[] = {
        btn::fixed_point(120, 1),
        btn::fixed_point(120 + 32, 80),
        btn::fixed_point(120, 158),
        btn::fixed_point(120 - 31, 80)
    };

    polygon user_polygon(vertices);
    polygon_sprite user_polygon_sprite(user_polygon, 0, 0);

    const int max_polygon_sprites = 2;
    using hdma_source_array = btn::array<uint16_t, btn::display::height() * 4 * max_polygon_sprites>;
    btn::unique_ptr<hdma_source_array> hdma_source(new hdma_source_array());
    uint16_t* hdma_source_data = hdma_source->data();

    while(true)
    {
        user_polygon_sprite.update(max_polygon_sprites, hdma_source_data);

        btn::core::update();
        btn::hw::hdma::start(hdma_source_data, 4 * max_polygon_sprites,
                             &btn::hw::sprites::vram()[128 - max_polygon_sprites].attr0);
    }
}
