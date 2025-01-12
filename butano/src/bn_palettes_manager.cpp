/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_palettes_manager.h"

#include "bn_bg_palettes.cpp.h"
#include "bn_bg_palette_ptr.cpp.h"
#include "bn_bg_palette_item.cpp.h"
#include "bn_sprite_palettes.cpp.h"
#include "bn_sprite_palette_ptr.cpp.h"
#include "bn_sprite_palette_item.cpp.h"
#include "bn_palettes_bank.cpp.h"

namespace bn::palettes_manager
{

namespace
{
    class static_data
    {

    public:
        palettes_bank sprite_palettes_bank;
        palettes_bank bg_palettes_bank;
    };

    BN_DATA_EWRAM_BSS static_data data;
}

void init(const optional<color>& transparent_color)
{
    ::new(static_cast<void*>(&data)) static_data();

    bg_palettes_bank().set_transparent_color(transparent_color);
}

palettes_bank& sprite_palettes_bank()
{
    return data.sprite_palettes_bank;
}

palettes_bank& bg_palettes_bank()
{
    return data.bg_palettes_bank;
}

void update()
{
    data.sprite_palettes_bank.update();
    data.bg_palettes_bank.update();
}

void commit(bool use_dma)
{
    palettes_bank::commit_data sprite_commit_data = data.sprite_palettes_bank.retrieve_commit_data();

    if(const color* sprite_colors_ptr = sprite_commit_data.colors_ptr)
    {
        hw::palettes::commit_sprites(sprite_colors_ptr, sprite_commit_data.offset, sprite_commit_data.count, use_dma);
        data.sprite_palettes_bank.reset_commit_data();
    }

    palettes_bank::commit_data bg_commit_data = data.bg_palettes_bank.retrieve_commit_data();

    if(const color* bg_colors_ptr = bg_commit_data.colors_ptr)
    {
        hw::palettes::commit_bgs(bg_colors_ptr, bg_commit_data.offset, bg_commit_data.count, use_dma);
        data.bg_palettes_bank.reset_commit_data();
    }
}

void stop()
{
    data.sprite_palettes_bank.stop();
    data.bg_palettes_bank.stop();
    *hw::palettes::bg_transparent_color_register() = 0;
}

}
