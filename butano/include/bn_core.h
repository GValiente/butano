/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
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
#include "bn_optional.h"
#include "bn_string_view.h"
#include "bn_vblank_callback_type.h"

namespace bn
{
    class color;
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
     * @param transparent_color Initial transparent color of the backgrounds.
     */
    void init(const optional<color>& transparent_color);

    /**
     * @brief This function must be called before using Butano, and it must be called only once.
     * @param keypad_commands Keypad commands recorded with the keypad logger.
     *
     * Instead of reading the keypad of the GBA, these keypad commands are replayed.
     */
    void init(const string_view& keypad_commands);

    /**
     * @brief This function must be called before using Butano, and it must be called only once.
     * @param transparent_color Initial transparent color of the backgrounds.
     * @param keypad_commands Keypad commands recorded with the keypad logger.
     *
     * Instead of reading the keypad of the GBA, these keypad commands are replayed.
     */
    void init(const optional<color>& transparent_color, const string_view& keypad_commands);

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
     * @brief Resets the GBA, going back to the start of main() after showing the BIOS intro.
     *
     * Keep in mind that it doesn't rewind the stack, so alive objects are not destroyed.
     */
    [[noreturn]] void hard_reset();

    /**
     * @brief Returns the CPU usage of the current frame.
     *
     * A CPU usage greater than 1 means that at least one screen refresh should have been missed.
     */
    [[nodiscard]] fixed current_cpu_usage();

    /**
     * @brief Returns the CPU timer ticks of the current frame.
     *
     * A CPU tick count greater than timers::ticks_per_frame() means that at least one screen refresh
     * should have been missed.
     */
    [[nodiscard]] int current_cpu_ticks();

    /**
     * @brief Returns the CPU usage of the last elapsed frame.
     *
     * A CPU usage greater than 1 means that at least one screen refresh should have been missed.
     *
     * If you only want to retrieve the number of missed screen refreshes,
     * core::last_missed_frames is more accurate.
     */
    [[nodiscard]] fixed last_cpu_usage();

    /**
     * @brief Returns the CPU timer ticks of the last elapsed frame.
     *
     * A CPU tick count greater than timers::ticks_per_frame() means that at least one screen refresh
     * should have been missed.
     *
     * If you only want to retrieve the number of missed screen refreshes,
     * core::last_missed_frames is more accurate.
     */
    [[nodiscard]] int last_cpu_ticks();

    /**
     * @brief Returns the maximum V-Blank usage of the last elapsed frame.
     *
     * A V-Blank usage greater than 1 means that the screen is being redrawn
     * before all of GBA display components are updated.
     */
    [[nodiscard]] fixed last_vblank_usage();

    /**
     * @brief Returns the maximum V-Blank timer ticks of the last elapsed frame.
     *
     * A V-Blank tick count greater than timers::ticks_per_vblank() means that the screen is being redrawn
     * before all of GBA display components are updated.
     */
    [[nodiscard]] int last_vblank_ticks();

    /**
     * @brief Returns the number of screen refreshes that were missed in the last core::update call.
     */
    [[nodiscard]] int last_missed_frames();

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

    /**
     * @brief Logs the current stack trace.
     *
     * The stack trace is mangled, you can demangle it with <a href="http://demangler.com/">demangler.com</a>
     * for example.
     */
    void log_stacktrace();
}

#endif
