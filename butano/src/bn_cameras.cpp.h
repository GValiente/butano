/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_cameras.h"

#include "bn_cameras_manager.h"

namespace bn::cameras
{

int used_items_count()
{
    return cameras_manager::used_items_count();
}

int available_items_count()
{
    return cameras_manager::available_items_count();
}

}
