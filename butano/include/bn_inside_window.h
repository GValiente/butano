/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_INSIDE_WINDOW_H
#define BTN_INSIDE_WINDOW_H

/**
 * @file
 * btn::inside_window header file.
 *
 * @ingroup inside_window
 */

#include "btn_window.h"

namespace btn
{

/**
 * @brief Window which region can be covered by other window.
 *
 * @ingroup inside_window
 */
class inside_window : public window
{

public:
    /**
     * @brief Returns the closest window to the screen.
     */
    [[nodiscard]] static inside_window internal()
    {
        return inside_window(window::internal().id());
    }

    /**
     * @brief Returns the closest window to the screen not covered by the internal window.
     */
    [[nodiscard]] static inside_window external()
    {
        return inside_window(window::external().id());
    }

    /**
     * @brief Returns the window made out of the visible pixels of the sprites with the window attribute enabled.
     */
    [[nodiscard]] static inside_window sprites()
    {
        return inside_window(window::sprites().id());
    }

    /**
     * @brief Indicates if this window must be committed to the GBA or not.
     */
    [[nodiscard]] bool visible() const;

    /**
     * @brief Sets if this window must be committed to the GBA or not.
     */
    void set_visible(bool visible);

protected:
    /// @cond DO_NOT_DOCUMENT

    explicit inside_window(int id) :
        window(id)
    {
    }

    /// @endcond

private:
    friend class window;
};

}

#endif
