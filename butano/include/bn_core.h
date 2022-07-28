/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
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

#include "bn_span.h"
#include "bn_fixed.h"
#include "bn_string_view.h"
#include "bn_vblank_callback_type.h"

namespace bn
{
    class system_font;
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
     * @brief Updates the screen and all Butano subsystems.
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
     * @brief Returns the user function called in V-Blank.
     */
    [[nodiscard]] vblank_callback_type vblank_callback();

    /**
     * @brief Sets the user function called in V-Blank.
     */
    void set_vblank_callback(vblank_callback_type vblank_callback);

    /**
     * @brief Indicates if a slow game pak like the SuperCard SD has been detected or not.
     */
    [[nodiscard]] bool slow_game_pak();

    /**
     * @brief Returns the font used to show assert and profiling messages.
     */
    [[nodiscard]] const system_font& system_font();

    /**
     * @brief Sets the font used to show assert and profiling messages.
     */
    void set_system_font(const bn::system_font& font);

    /**
     * @brief Returns the tag displayed in assert messages.
     */
    [[nodiscard]] const string_view& assert_tag();

    /**
     * @brief Sets the tag displayed in assert messages.
     */
    void set_assert_tag(const string_view& assert_tag);
}

#endif
