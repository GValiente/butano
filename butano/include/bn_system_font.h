/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SYSTEM_FONT_H
#define BN_SYSTEM_FONT_H

/**
 * @file
 * bn::system_font header file.
 *
 * @ingroup core
 */

#include "bn_common.h"

extern "C"
{
    struct TFont;
}

namespace bn
{

/**
 * @brief Contains information of the font used to show assert and profiling messages.
 *
 * @ingroup core
 */
class system_font
{

public:
    /**
     * @brief Default constructor.
     */
    system_font();

    /**
     * @brief Constructor.
     * @param tte_font Instance of
     * <a href="https://www.coranac.com/man/tonclib/structTFont.htm">TFont</a> struct.
     */
    system_font(const TFont& tte_font);

    /**
     * @brief Returns an instance of
     * <a href="https://www.coranac.com/man/tonclib/structTFont.htm">TFont</a> struct.
     */
    [[nodiscard]] const TFont& tte_font() const;

private:
    alignas(int) uint8_t _data[24];
};

}

#endif
