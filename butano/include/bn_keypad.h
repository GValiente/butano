/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_KEYPAD_H
#define BN_KEYPAD_H

/**
 * @file
 * bn::keypad header file.
 *
 * @ingroup keypad
 */

#include "bn_common.h"

/**
 * @brief Keypad related functions.
 *
 * @ingroup keypad
 */
namespace bn::keypad
{
    /**
     * @brief Available key types.
     */
    enum class key_type : uint16_t
    {
        A =         0x0001, //!< `A` key.
        B =         0x0002, //!< `B` key.
        SELECT =    0x0004, //!< `Select` key.
        START =     0x0008, //!< `Start` key.
        RIGHT =     0x0010, //!< `Right` key.
        LEFT =      0x0020, //!< `Left` key.
        UP =        0x0040, //!< `Up` key.
        DOWN =      0x0080, //!< `Down` key.
        R =         0x0100, //!< `R` key.
        L =         0x0200  //!< `L` key.
    };

    /**
     * @brief Indicates if the given key is held or not.
     */
    [[nodiscard]] bool held(key_type key);

    /**
     * @brief Indicates if the given key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool pressed(key_type key);

    /**
     * @brief Indicates if the given key has been released in the current frame or not.
     */
    [[nodiscard]] bool released(key_type key);

    /**
     * @brief Indicates if the `A` key is held or not.
     */
    [[nodiscard]] bool a_held();

    /**
     * @brief Indicates if the `A` key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool a_pressed();

    /**
     * @brief Indicates if the `A` key has been released in the current frame or not.
     */
    [[nodiscard]] bool a_released();

    /**
     * @brief Indicates if the `B` key is held or not.
     */
    [[nodiscard]] bool b_held();

    /**
     * @brief Indicates if the `B` key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool b_pressed();

    /**
     * @brief Indicates if the `B` key has been released in the current frame or not.
     */
    [[nodiscard]] bool b_released();

    /**
     * @brief Indicates if the `Select` key is held or not.
     */
    [[nodiscard]] bool select_held();

    /**
     * @brief Indicates if the `Select` key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool select_pressed();

    /**
     * @brief Indicates if the `Select` key has been released in the current frame or not.
     */
    [[nodiscard]] bool select_released();

    /**
     * @brief Indicates if the `Start` key is held or not.
     */
    [[nodiscard]] bool start_held();

    /**
     * @brief Indicates if the `Start` key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool start_pressed();

    /**
     * @brief Indicates if the `Start` key has been released in the current frame or not.
     */
    [[nodiscard]] bool start_released();

    /**
     * @brief Indicates if the `Right` key is held or not.
     */
    [[nodiscard]] bool right_held();

    /**
     * @brief Indicates if the `Right` key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool right_pressed();

    /**
     * @brief Indicates if the `Right` key has been released in the current frame or not.
     */
    [[nodiscard]] bool right_released();

    /**
     * @brief Indicates if the `Left` key is held or not.
     */
    [[nodiscard]] bool left_held();

    /**
     * @brief Indicates if the `Left` key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool left_pressed();

    /**
     * @brief Indicates if the `Left` key has been released in the current frame or not.
     */
    [[nodiscard]] bool left_released();

    /**
     * @brief Indicates if the `Up` key is held or not.
     */
    [[nodiscard]] bool up_held();

    /**
     * @brief Indicates if the `Up` key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool up_pressed();

    /**
     * @brief Indicates if the `Up` key has been released in the current frame or not.
     */
    [[nodiscard]] bool up_released();

    /**
     * @brief Indicates if the `Down` key is held or not.
     */
    [[nodiscard]] bool down_held();

    /**
     * @brief Indicates if the `Down` key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool down_pressed();

    /**
     * @brief Indicates if the `Down` key has been released in the current frame or not.
     */
    [[nodiscard]] bool down_released();

    /**
     * @brief Indicates if the `R` key is held or not.
     */
    [[nodiscard]] bool r_held();

    /**
     * @brief Indicates if the `R` key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool r_pressed();

    /**
     * @brief Indicates if the `R` key has been released in the current frame or not.
     */
    [[nodiscard]] bool r_released();

    /**
     * @brief Indicates if the `L` key is held or not.
     */
    [[nodiscard]] bool l_held();

    /**
     * @brief Indicates if the `L` key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool l_pressed();

    /**
     * @brief Indicates if the `L` key has been released in the current frame or not.
     */
    [[nodiscard]] bool l_released();

    /**
     * @brief Indicates if any key is held or not.
     */
    [[nodiscard]] bool any_held();

    /**
     * @brief Indicates if any key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool any_pressed();

    /**
     * @brief Indicates if any key has been released in the current frame or not.
     */
    [[nodiscard]] bool any_released();
}

#endif
