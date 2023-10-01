/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_ASSERT_CALLBACK_TYPE_H
#define BN_ASSERT_CALLBACK_TYPE_H

/**
 * @file
 * bn::assert_callback_type header file.
 *
 * @ingroup assert
 */

namespace bn::assert
{
    using callback_type = void(*)(); //!< Assert callback type alias.
}

#endif
