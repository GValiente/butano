/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CLIP_LINE_H
#define BN_CLIP_LINE_H

/**
 * @file
 * bn::clip_line header file.
 *
 * @ingroup math
 */

#include "bn_array.h"
#include "bn_optional.h"

namespace bn
{

/**
 * @brief Clips a line to a rectangular area.
 * @param x1 Horizontal position of the start point of the line.
 * @param y1 Vertical position of the start point of the line.
 * @param x2 Horizontal position of the end point of the line.
 * @param y2 Vertical position of the end point of the line.
 * @param clip_x Horizontal position of the top-left point of the clipping rectangle.
 * @param clip_y Vertical position of the top-left point of the clipping rectangle.
 * @param clip_width Width of the clipping rectangle (> 0).
 * @param clip_height Height of the clipping rectangle (> 0).
 * @return An array with the clipped line coordinates in the form { x1, y1, x2, y2 } if at least part of the line
 * is inside the rectangular clipping area; bn::nullopt otherwise.
 *
 * Implementation provided by https://github.com/scivision/lineclipping-julia
 *
 * @ingroup math
 */
template<typename Type>
[[nodiscard]] constexpr optional<array<Type, 4>> clip_line(
        Type x1, Type y1, Type x2, Type y2, Type clip_x, Type clip_y, Type clip_width, Type clip_height)
{
    BN_ASSERT(clip_width > 0, "Invalid clip_width: ", clip_width);
    BN_ASSERT(clip_height > 0, "Invalid clip_height: ", clip_height);

    Type xmin = clip_x;
    Type xmax = clip_x + clip_width;
    Type ymin = clip_y;
    Type ymax = clip_y + clip_width;

    auto calculate_code = [&](Type x, Type y)
    {
        int code = 0;

        if(x < xmin)
        {
            code |= (1 << 0);
        }
        else if(x > xmax)
        {
            code |= (1 << 1);
        }

        if(y < ymin)
        {
            code |= (1 << 2);
        }
        else if(y > ymax)
        {
            code |= (1 << 3);
        }

        return code;
    };

    int code_1 = calculate_code(x1, y1);
    int code_2 = calculate_code(x2, y2);
    bool inside = true;

    while(code_1 | code_2)
    {
        if(code_1 & code_2)
        {
            inside = false;
            break;
        }
        else
        {
            Type x, y;
            int code = code_1 ? code_1 : code_2;

            if(code & (1 << 3))
            {
                x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
                y = ymax;
            }
            else if(code & (1 << 2))
            {
                x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
                y = ymin;
            }
            else if(code & (1 << 1))
            {
                y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
                x = xmax;
            }
            else
            {
                y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
                x = xmin;
            }

            if(code == code_1)
            {
                x1 = x;
                y1 = y;
                code_1 = calculate_code(x1, y1);
            }
            else
            {
                x2 = x;
                y2 = y;
                code_2 = calculate_code(x2, y2);
            }
        }
    }

    optional<array<Type, 4>> result;

    if(inside)
    {
        result = { x1, y1, x2, y2 };
    }

    return result;
}

}

#endif
