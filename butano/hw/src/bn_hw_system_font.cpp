/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_system_font.h"

#include "../include/bn_hw_tonc.h"

namespace bn
{

system_font::system_font() :
    system_font(vwf_default)
{
}

system_font::system_font(const TFont& tte_font)
{
    static_assert(sizeof(_data) == sizeof(TFont));
    static_assert(alignof(int) == alignof(TFont));

    auto tte_font_data = reinterpret_cast<TFont*>(_data);
    *tte_font_data = tte_font;
}

const TFont& system_font::tte_font() const
{
    return *reinterpret_cast<const TFont*>(_data);
}

}
