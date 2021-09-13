/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CORE_H
#define BN_CORE_H

/**
 * @file
 * bn::core header file.
 *
 * @ingroup core
 */

#include "bn_span_fwd.h"
#include "bn_fixed_fwd.h"

namespace bn
{
    class string_view;
}

namespace bn::keypad
{
    enum class key_type : uint16_t;
}

/**
 * @brief Core related functions.
 *
 * @ingroup core
 */
namespace bn::core
{
    /**
     * @brief This function must be called before using Butano, and it must be called only once.
     */
    void init();

    /**
     * @brief This function must be called before using Butano, and it must be called only once.
     * @param keypad_commands Keypad commands recorded with the keypad logger.
     *
     * Instead of reading the keypad of the GBA, these keypad commands are replayed.
     */
    void init(const string_view& keypad_commands);

    /**
     * @brief Returns the number of frames to skip.
     *
     * 0 skip frames means ~60 frames per second, 1 skip frame means ~30 frames per second, and so on.
     */
    [[nodiscard]] int skip_frames();

    /**
     * @brief Sets the number of frames to skip.
     *
     * 0 skip frames means ~60 frames per second, 1 skip frame means ~30 frames per second, and so on.
     */
    void set_skip_frames(int skip_frames);

    /**
     * @brief Updates the screen and all of Butano's subsystems.
     */
    void update();

    /**
     * @brief Sleeps the GBA until the given keypad key is pressed.
     */
    void sleep(keypad::key_type wake_up_key);

    /**
     * @brief Sleeps the GBA until the given keypad keys are pressed.
     */
    void sleep(const span<const keypad::key_type>& wake_up_keys);

    /**
     * @brief Resets the GBA, going back to the start of main().
     *
     * Keep in mind that it doesn't rewind the stack, so alive objects are not destroyed.
     */
    [[noreturn]] void reset();

    /**
     * @brief Returns the current CPU usage.
     *
     * A CPU usage greater than 1 means that at least one screen refresh has been missed.
     */
    [[nodiscard]] fixed current_cpu_usage();

    /**
     * @brief Returns the CPU usage of the last elapsed frame.
     *
     * A CPU usage greater than 1 means that at least one screen refresh has been missed.
     */
    [[nodiscard]] fixed last_cpu_usage();

    /**
     * @brief Returns the V-Blank usage of the last elapsed frame.
     *
     * A V-Blank usage greater than 1 means that the screen is being redrawn
     * before all of GBA display components being updated.
     */
    [[nodiscard]] fixed last_vblank_usage();

    /**
     * @brief Indicates if a slow game pak like the SuperCard SD has been detected or not.
     */
    [[nodiscard]] bool slow_game_pak();
}

#endif
