/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_KEYPAD_H
#define BTN_KEYPAD_H

#include "btn_common.h"

/**
 * @brief Keypad related functions.
 *
 * @ingroup keypad
 */
namespace btn::keypad
{
    /**
     * @brief Available key types.
     */
    enum class key_type
    {
        A =         0x0001, //!< <b>A</b> key.
        B =         0x0002, //!< <b>B</b> key.
        SELECT =    0x0004, //!< <b>Select</b> key.
        START =     0x0008, //!< <b>Start</b> key.
        RIGHT =     0x0010, //!< <b>Right</b> key.
        LEFT =      0x0020, //!< <b>Left</b> key.
        UP =        0x0040, //!< <b>Up</b> key.
        DOWN =      0x0080, //!< <b>Down</b> key.
        R =         0x0100, //!< <b>R</b> key.
        L =         0x0200  //!< <b>L</b> key.
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
     * @brief Indicates if the <b>A</b> key is held or not.
     */
    [[nodiscard]] bool a_held();

    /**
     * @brief Indicates if the <b>A</b> key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool a_pressed();

    /**
     * @brief Indicates if the <b>A</b> key has been released in the current frame or not.
     */
    [[nodiscard]] bool a_released();

    /**
     * @brief Indicates if the <b>B</b> key is held or not.
     */
    [[nodiscard]] bool b_held();

    /**
     * @brief Indicates if the <b>B</b> key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool b_pressed();

    /**
     * @brief Indicates if the <b>B</b> key has been released in the current frame or not.
     */
    [[nodiscard]] bool b_released();

    /**
     * @brief Indicates if the <b>Select</b> key is held or not.
     */
    [[nodiscard]] bool select_held();

    /**
     * @brief Indicates if the <b>Select</b> key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool select_pressed();

    /**
     * @brief Indicates if the <b>Select</b> key has been released in the current frame or not.
     */
    [[nodiscard]] bool select_released();

    /**
     * @brief Indicates if the <b>Start</b> key is held or not.
     */
    [[nodiscard]] bool start_held();

    /**
     * @brief Indicates if the <b>Start</b> key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool start_pressed();

    /**
     * @brief Indicates if the <b>Start</b> key has been released in the current frame or not.
     */
    [[nodiscard]] bool start_released();

    /**
     * @brief Indicates if the <b>Right</b> key is held or not.
     */
    [[nodiscard]] bool right_held();

    /**
     * @brief Indicates if the <b>Right</b> key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool right_pressed();

    /**
     * @brief Indicates if the <b>Right</b> key has been released in the current frame or not.
     */
    [[nodiscard]] bool right_released();

    /**
     * @brief Indicates if the <b>Left</b> key is held or not.
     */
    [[nodiscard]] bool left_held();

    /**
     * @brief Indicates if the <b>Left</b> key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool left_pressed();

    /**
     * @brief Indicates if the <b>Left</b> key has been released in the current frame or not.
     */
    [[nodiscard]] bool left_released();

    /**
     * @brief Indicates if the <b>Up</b> key is held or not.
     */
    [[nodiscard]] bool up_held();

    /**
     * @brief Indicates if the <b>Up</b> key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool up_pressed();

    /**
     * @brief Indicates if the <b>Up</b> key has been released in the current frame or not.
     */
    [[nodiscard]] bool up_released();

    /**
     * @brief Indicates if the <b>Down</b> key is held or not.
     */
    [[nodiscard]] bool down_held();

    /**
     * @brief Indicates if the <b>Down</b> key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool down_pressed();

    /**
     * @brief Indicates if the <b>Down</b> key has been released in the current frame or not.
     */
    [[nodiscard]] bool down_released();

    /**
     * @brief Indicates if the <b>R</b> key is held or not.
     */
    [[nodiscard]] bool r_held();

    /**
     * @brief Indicates if the <b>R</b> key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool r_pressed();

    /**
     * @brief Indicates if the <b>R</b> key has been released in the current frame or not.
     */
    [[nodiscard]] bool r_released();

    /**
     * @brief Indicates if the <b>L</b> key is held or not.
     */
    [[nodiscard]] bool l_held();

    /**
     * @brief Indicates if the <b>L</b> key has been pressed in the current frame or not.
     */
    [[nodiscard]] bool l_pressed();

    /**
     * @brief Indicates if the <b>L</b> key has been released in the current frame or not.
     */
    [[nodiscard]] bool l_released();
}

#endif
