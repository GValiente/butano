/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_palettes_manager.h"

#include "btn_bg_palettes.cpp.h"
#include "btn_bg_palette_ptr.cpp.h"
#include "btn_bg_palette_item.cpp.h"
#include "btn_sprite_palettes.cpp.h"
#include "btn_sprite_palette_ptr.cpp.h"
#include "btn_sprite_palette_item.cpp.h"
#include "btn_palettes_bank.cpp.h"

namespace btn::palettes_manager
{

namespace
{
    class static_data
    {

    public:
        palettes_bank sprite_palettes_bank;
        palettes_bank bg_palettes_bank;
    };

    BTN_DATA_EWRAM static_data data;
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

void commit()
{
    if(optional<palettes_bank::commit_data> commit_data = data.sprite_palettes_bank.retrieve_commit_data())
    {
        hw::palettes::commit_sprites(commit_data->colors_ptr, commit_data->offset, commit_data->count);
        data.sprite_palettes_bank.reset_commit_data();
    }

    if(optional<palettes_bank::commit_data> commit_data = data.bg_palettes_bank.retrieve_commit_data())
    {
        hw::palettes::commit_bgs(commit_data->colors_ptr, commit_data->offset, commit_data->count);
        data.bg_palettes_bank.reset_commit_data();
    }
}

void stop()
{
    *hw::palettes::bg_transparent_color_register() = 0;
}

}
