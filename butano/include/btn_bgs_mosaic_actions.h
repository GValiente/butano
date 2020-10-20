/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BGS_MOSAIC_ACTIONS_H
#define BTN_BGS_MOSAIC_ACTIONS_H

#include "btn_fixed.h"
#include "btn_bgs_mosaic.h"
#include "btn_template_actions.h"

namespace btn
{

// horizontal_stretch

class bgs_mosaic_horizontal_stretch_manager
{

public:
    [[nodiscard]] static fixed get()
    {
        return bgs_mosaic::horizontal_stretch();
    }

    static void set(fixed horizontal_stretch)
    {
        bgs_mosaic::set_horizontal_stretch(horizontal_stretch);
    }
};


class bgs_mosaic_horizontal_stretch_to_action : public to_template_action<fixed, bgs_mosaic_horizontal_stretch_manager>
{

public:
    bgs_mosaic_horizontal_stretch_to_action(int duration_frames, fixed final_horizontal_stretch) :
        to_template_action(duration_frames, final_horizontal_stretch)
    {
        BTN_ASSERT(final_horizontal_stretch >= 0 && final_horizontal_stretch <= 1,
                   "Invalid final horizontal stretch: ", final_horizontal_stretch);
    }

    [[nodiscard]] fixed final_horizontal_stretch() const
    {
        return final_property();
    }
};


class bgs_mosaic_horizontal_stretch_loop_action :
        public loop_template_action<fixed, bgs_mosaic_horizontal_stretch_manager>
{

public:
    bgs_mosaic_horizontal_stretch_loop_action(int duration_frames, fixed final_horizontal_stretch) :
        loop_template_action(duration_frames, final_horizontal_stretch)
    {
        BTN_ASSERT(final_horizontal_stretch >= 0 && final_horizontal_stretch <= 1,
                   "Invalid final horizontal stretch: ", final_horizontal_stretch);
    }

    [[nodiscard]] fixed final_horizontal_stretch() const
    {
        return final_property();
    }
};


class bgs_mosaic_horizontal_stretch_toggle_action :
        public toggle_template_action<fixed, bgs_mosaic_horizontal_stretch_manager>
{

public:
    bgs_mosaic_horizontal_stretch_toggle_action(int duration_frames, fixed new_horizontal_stretch) :
        toggle_template_action(duration_frames, new_horizontal_stretch)
    {
        BTN_ASSERT(new_horizontal_stretch >= 0 && new_horizontal_stretch <= 1,
                   "Invalid new horizontal stretch: ", new_horizontal_stretch);
    }

    [[nodiscard]] fixed new_horizontal_stretch() const
    {
        return new_property();
    }
};


// vertical_stretch

class bgs_mosaic_vertical_stretch_manager
{

public:
    [[nodiscard]] static fixed get()
    {
        return bgs_mosaic::vertical_stretch();
    }

    static void set(fixed vertical_stretch)
    {
        bgs_mosaic::set_vertical_stretch(vertical_stretch);
    }
};


class bgs_mosaic_vertical_stretch_to_action : public to_template_action<fixed, bgs_mosaic_vertical_stretch_manager>
{

public:
    bgs_mosaic_vertical_stretch_to_action(int duration_frames, fixed final_vertical_stretch) :
        to_template_action(duration_frames, final_vertical_stretch)
    {
        BTN_ASSERT(final_vertical_stretch >= 0 && final_vertical_stretch <= 1,
                   "Invalid final vertical stretch: ", final_vertical_stretch);
    }

    [[nodiscard]] fixed final_vertical_stretch() const
    {
        return final_property();
    }
};


class bgs_mosaic_vertical_stretch_loop_action : public loop_template_action<fixed, bgs_mosaic_vertical_stretch_manager>
{

public:
    bgs_mosaic_vertical_stretch_loop_action(int duration_frames, fixed final_vertical_stretch) :
        loop_template_action(duration_frames, final_vertical_stretch)
    {
        BTN_ASSERT(final_vertical_stretch >= 0 && final_vertical_stretch <= 1,
                   "Invalid final vertical stretch: ", final_vertical_stretch);
    }

    [[nodiscard]] fixed final_vertical_stretch() const
    {
        return final_property();
    }
};


class bgs_mosaic_vertical_stretch_toggle_action :
        public toggle_template_action<fixed, bgs_mosaic_vertical_stretch_manager>
{

public:
    bgs_mosaic_vertical_stretch_toggle_action(int duration_frames, fixed new_vertical_stretch) :
        toggle_template_action(duration_frames, new_vertical_stretch)
    {
        BTN_ASSERT(new_vertical_stretch >= 0 && new_vertical_stretch <= 1,
                   "Invalid new vertical stretch: ", new_vertical_stretch);
    }

    [[nodiscard]] fixed new_vertical_stretch() const
    {
        return new_property();
    }
};


// stretch

class bgs_mosaic_stretch_manager
{

public:
    [[nodiscard]] static fixed get()
    {
        return bgs_mosaic::horizontal_stretch();
    }

    static void set(fixed stretch)
    {
        bgs_mosaic::set_stretch(stretch);
    }
};


class bgs_mosaic_stretch_to_action : public to_template_action<fixed, bgs_mosaic_stretch_manager>
{

public:
    bgs_mosaic_stretch_to_action(int duration_frames, fixed final_stretch) :
        to_template_action(duration_frames, final_stretch)
    {
        BTN_ASSERT(final_stretch >= 0 && final_stretch <= 1, "Invalid final stretch: ", final_stretch);
    }

    [[nodiscard]] fixed final_stretch() const
    {
        return final_property();
    }
};


class bgs_mosaic_stretch_loop_action : public loop_template_action<fixed, bgs_mosaic_stretch_manager>
{

public:
    bgs_mosaic_stretch_loop_action(int duration_frames, fixed final_stretch) :
        loop_template_action(duration_frames, final_stretch)
    {
        BTN_ASSERT(final_stretch >= 0 && final_stretch <= 1, "Invalid final stretch: ", final_stretch);
    }

    [[nodiscard]] fixed final_stretch() const
    {
        return final_property();
    }
};


class bgs_mosaic_stretch_toggle_action : public toggle_template_action<fixed, bgs_mosaic_stretch_manager>
{

public:
    bgs_mosaic_stretch_toggle_action(int duration_frames, fixed new_stretch) :
        toggle_template_action(duration_frames, new_stretch)
    {
        BTN_ASSERT(new_stretch >= 0 && new_stretch <= 1, "Invalid new stretch: ", new_stretch);
    }

    [[nodiscard]] fixed new_stretch() const
    {
        return new_property();
    }
};

}

#endif
