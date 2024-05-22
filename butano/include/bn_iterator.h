/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_ITERATOR_H
#define BN_ITERATOR_H

/**
 * @file
 * Standard library iterator aliases header file.
 *
 * @ingroup std
 */

#include <iterator>
#include "bn_common.h"

namespace bn
{
    using std::forward_iterator_tag;

    using std::bidirectional_iterator_tag;

    using std::random_access_iterator_tag;

    using std::reverse_iterator;

    using std::distance;
}

#endif
