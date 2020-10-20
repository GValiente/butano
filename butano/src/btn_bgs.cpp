/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_bgs.h"

#include "btn_bgs_manager.h"

namespace btn::bgs
{

int used_count()
{
    return bgs_manager::used_count();
}

int available_count()
{
    return bgs_manager::available_count();
}

}
