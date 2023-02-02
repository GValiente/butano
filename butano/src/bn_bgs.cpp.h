/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_bgs.h"

#include "bn_bgs_manager.h"

namespace bn::bgs
{

int used_items_count()
{
    return bgs_manager::used_count();
}

int available_items_count()
{
    return bgs_manager::available_count();
}

}
