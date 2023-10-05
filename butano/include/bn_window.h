/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_WINDOW_H
#define BN_WINDOW_H

/**
 * @file
 * bn::window header file.
 *
 * @ingroup window
 */

#include "bn_functional.h"

namespace bn
{

class affine_bg_ptr;
class regular_bg_ptr;

/**
 * @brief Base window class.
 *
 * @ingroup window
 */
class window
{

public:
    /**
     * @brief Returns the closest window to the screen.
     */
    [[nodiscard]] static window internal()
    {
        return window(0);
    }

    /**
     * @brief Returns the closest window to the screen not covered by the internal window.
     */
    [[nodiscard]] static window external()
    {
        return window(1);
    }

    /**
     * @brief Returns the window made out of the visible pixels of the sprites with the window attribute enabled.
     */
    [[nodiscard]] static window sprites()
    {
        return window(2);
    }

    /**
     * @brief Returns the window of the region not covered by any other window.
     */
    [[nodiscard]] static window outside()
    {
        return window(3);
    }

    /**
     * @brief Returns the internal ID.
     */
    [[nodiscard]] int id() const
    {
        return _id;
    }

    /**
     * @brief Indicates if the specified background is shown in this window.
     * @param regular_bg Regular BG to ask for.
     * @return `true` if the specified background is shown in this window, otherwise `false`.
     */
    [[nodiscard]] bool show_bg(const regular_bg_ptr& regular_bg) const;

    /**
     * @brief Indicates if the specified background is shown in this window.
     * @param affine_bg Affine BG to ask for.
     * @return `true` if the specified background is shown in this window, otherwise `false`.
     */
    [[nodiscard]] bool show_bg(const affine_bg_ptr& affine_bg) const;

    /**
     * @brief Sets if the specified background must be shown in this window.
     * @param regular_bg Regular BG to show or hide.
     * @param show `true` if the specified background must be shown in this window, otherwise `false`.
     */
    void set_show_bg(const regular_bg_ptr& regular_bg, bool show);

    /**
     * @brief Sets if the specified background must be shown in this window.
     * @param affine_bg Affine BG to show or hide.
     * @param show `true` if the specified background must be shown in this window, otherwise `false`.
     */
    void set_show_bg(const affine_bg_ptr& affine_bg, bool show);

    /**
     * @brief Indicates if sprites are shown in this window.
     */
    [[nodiscard]] bool show_sprites() const;

    /**
     * @brief Sets if sprites must be shown in this window.
     * @param show `true` if sprites must be shown in this window, otherwise `false`.
     */
    void set_show_sprites(bool show);

    /**
     * @brief Indicates if blending is shown in this window.
     */
    [[nodiscard]] bool show_blending() const;

    /**
     * @brief Sets if blending must be shown in this window.
     * @param show `true` if blending must be shown in this window, otherwise `false`.
     */
    void set_show_blending(bool show);

    /**
     * @brief Indicates if sprites, backgrounds and blending are shown in this window.
     */
    [[nodiscard]] bool show_all() const;

    /**
     * @brief Show sprites, backgrounds and blending in this window.
     */
    void set_show_all();

    /**
     * @brief Indicates if sprites, backgrounds and blending are hidden in this window.
     */
    [[nodiscard]] bool show_nothing() const;

    /**
     * @brief Hide sprites, backgrounds and blending in this window.
     */
    void set_show_nothing();

    /**
     * @brief Restores this window to its initial state.
     */
    void restore();

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] friend bool operator==(const window& a, const window& b) = default;

protected:
    /// @cond DO_NOT_DOCUMENT

    explicit window(int id) :
        _id(int8_t(id))
    {
    }

    /// @endcond

private:
    int8_t _id;
};


/**
 * @brief Hash support for window.
 *
 * @ingroup window
 * @ingroup functional
 */
template<>
struct hash<window>
{
    /**
     * @brief Returns the hash of the given window.
     */
    [[nodiscard]] unsigned operator()(const window& value) const
    {
        return make_hash(value.id());
    }
};

}

#endif
