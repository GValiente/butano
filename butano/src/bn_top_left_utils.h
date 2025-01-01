/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_TOP_LEFT_UTILS_H
#define BN_TOP_LEFT_UTILS_H

#include "bn_display.h"
#include "bn_fixed_point.h"

namespace bn
{

[[nodiscard]] constexpr fixed to_top_left_x(fixed x, int width)
{
    return x + ((display::width() - width) / 2);
}

[[nodiscard]] constexpr fixed from_top_left_x(fixed top_left_x, int width)
{
    return top_left_x - ((display::width() - width) / 2);
}

[[nodiscard]] constexpr fixed to_top_left_y(fixed y, int height)
{
    return y + ((display::height() - height) / 2);
}

[[nodiscard]] constexpr fixed from_top_left_y(fixed top_left_y, int height)
{
    return top_left_y - ((display::height() - height) / 2);
}

[[nodiscard]] constexpr fixed_point to_top_left_position(const fixed_point& position, int width, int height)
{
    return fixed_point(to_top_left_x(position.x(), width),
                       to_top_left_y(position.y(), height));
}

[[nodiscard]] constexpr fixed_point to_top_left_position(const fixed_point& position, const size& dimensions)
{
    return fixed_point(to_top_left_x(position.x(), dimensions.width()),
                       to_top_left_y(position.y(), dimensions.height()));
}

[[nodiscard]] constexpr fixed_point from_top_left_position(
        const fixed_point& top_left_position, int width, int height)
{
    return fixed_point(from_top_left_x(top_left_position.x(), width),
                       from_top_left_y(top_left_position.y(), height));
}

[[nodiscard]] constexpr fixed_point from_top_left_position(
        const fixed_point& top_left_position, const size& dimensions)
{
    return fixed_point(from_top_left_x(top_left_position.x(), dimensions.width()),
                       from_top_left_y(top_left_position.y(), dimensions.height()));
}

}

#endif
