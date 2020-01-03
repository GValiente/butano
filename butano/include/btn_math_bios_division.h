#ifndef BTN_MATH_BIOS_DIVISION_H
#define BTN_MATH_BIOS_DIVISION_H

#include "btn_common.h"

namespace btn
{
    /**
     * @brief Integer division (numerator / denominator).
     * @param numerator Division numerator.
     * @param denominator Division denominator (must be != 0).
     * @return The result of the division.
     */
    [[nodiscard]] int bios_division(int numerator, int denominator);

    /**
     * @brief Integer division (numerator / denominator).
     * @param numerator Division numerator.
     * @param denominator Division denominator (must be != 0).
     * @param remainder Stores the remainder of the division.
     * @return The result of the division.
     */
    [[nodiscard]] int bios_division(int numerator, int denominator, int& remainder);

    /**
     * @brief Integer division remainder (numerator % denominator).
     * @param numerator Division numerator.
     * @param denominator Division denominator (must be != 0).
     * @return The remainder of the division.
     */
    [[nodiscard]] int bios_remainder(int numerator, int denominator);
}

#endif
