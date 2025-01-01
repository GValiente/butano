/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_hbe_ptr.h"

#include "bn_hblank_effects_manager.h"

namespace bn
{

hbe_ptr::~hbe_ptr()
{
    if(_id >= 0)
    {
        hblank_effects_manager::decrease_usages(_id);
    }
}

bool hbe_ptr::visible() const
{
    return hblank_effects_manager::visible(_id);
}

void hbe_ptr::set_visible(bool visible)
{
    hblank_effects_manager::set_visible(_id, visible);
}

hbe_ptr::hbe_ptr(const hbe_ptr& other) :
    hbe_ptr(other._id)
{
    hblank_effects_manager::increase_usages(_id);
}

hbe_ptr& hbe_ptr::operator=(const hbe_ptr& other)
{
    if(_id != other._id)
    {
        if(_id >= 0)
        {
            hblank_effects_manager::decrease_usages(_id);
        }

        _id = other._id;
        hblank_effects_manager::increase_usages(_id);
    }

    return *this;
}

}
