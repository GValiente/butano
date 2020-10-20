/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_HBLANK_EFFECT_ACTIONS_H
#define BTN_HBLANK_EFFECT_ACTIONS_H

#include "btn_hblank_effect_ptr.h"
#include "btn_value_template_actions.h"

namespace btn
{

// visible

class hblank_effect_visible_manager
{

public:
    [[nodiscard]] static bool get(const hblank_effect_ptr& hblank_effect)
    {
        return hblank_effect.visible();
    }

    static void set(bool visible, hblank_effect_ptr& hblank_effect)
    {
        hblank_effect.set_visible(visible);
    }
};


class hblank_effect_visible_toggle_action :
        public bool_toggle_value_template_action<hblank_effect_ptr, hblank_effect_visible_manager>
{

public:
    hblank_effect_visible_toggle_action(const hblank_effect_ptr& hblank_effect, int duration_frames) :
        bool_toggle_value_template_action(hblank_effect, duration_frames)
    {
    }

    hblank_effect_visible_toggle_action(hblank_effect_ptr&& hblank_effect, int duration_frames) :
        bool_toggle_value_template_action(move(hblank_effect), duration_frames)
    {
    }

    [[nodiscard]] const hblank_effect_ptr& hblank_effect() const
    {
        return value();
    }
};

}

#endif
