/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_MEMORY_ALIAS_H
#define BN_MEMORY_ALIAS_H

/**
 * @file
 * Aliases to standard memory functions file.
 *
 * @ingroup memory
 */

#include <memory>

namespace bn
{
    using std::construct_at;
    using std::destroy_at;
}

#endif
