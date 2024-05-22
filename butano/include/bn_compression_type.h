/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_COMPRESSION_TYPE_H
#define BN_COMPRESSION_TYPE_H

/**
 * @file
 * bn::compression_type header file.
 *
 * @ingroup tool
 */

#include "bn_common.h"

namespace bn
{

/**
 * @brief Specifies the available compression types.
 *
 * @ingroup tool
 */
enum class compression_type : uint8_t
{
    NONE, //!< Uncompressed data.
    LZ77, //!< LZ77 compressed data.
    RUN_LENGTH, //!< Run-length compressed data.
    HUFFMAN //!< Huffman compressed data.
};

}

#endif

