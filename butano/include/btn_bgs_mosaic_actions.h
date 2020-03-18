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
    [[nodiscard]] static fixed get(const int&)
    {
        return bgs_mosaic::horizontal_stretch();
    }

    static void set(fixed horizontal_stretch, int&)
    {
        bgs_mosaic::set_horizontal_stretch(horizontal_stretch);
    }
};


class bgs_mosaic_horizontal_stretch_to_action :
        public to_template_action<int, fixed, bgs_mosaic_horizontal_stretch_manager>
{

public:
    bgs_mosaic_horizontal_stretch_to_action(int duration_frames, fixed final_horizontal_stretch) :
        to_template_action(0, duration_frames, final_horizontal_stretch)
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
        public loop_template_action<int, fixed, bgs_mosaic_horizontal_stretch_manager>
{

public:
    bgs_mosaic_horizontal_stretch_loop_action(int duration_frames, fixed final_horizontal_stretch) :
        loop_template_action(0, duration_frames, final_horizontal_stretch)
    {
        BTN_ASSERT(final_horizontal_stretch >= 0 && final_horizontal_stretch <= 1,
                   "Invalid final horizontal stretch: ", final_horizontal_stretch);
    }

    [[nodiscard]] fixed final_horizontal_stretch() const
    {
        return final_property();
    }
};


// vertical_stretch

class bgs_mosaic_vertical_stretch_manager
{

public:
    [[nodiscard]] static fixed get(const int&)
    {
        return bgs_mosaic::vertical_stretch();
    }

    static void set(fixed vertical_stretch, int&)
    {
        bgs_mosaic::set_vertical_stretch(vertical_stretch);
    }
};


class bgs_mosaic_vertical_stretch_to_action :
        public to_template_action<int, fixed, bgs_mosaic_vertical_stretch_manager>
{

public:
    bgs_mosaic_vertical_stretch_to_action(int duration_frames, fixed final_vertical_stretch) :
        to_template_action(0, duration_frames, final_vertical_stretch)
    {
        BTN_ASSERT(final_vertical_stretch >= 0 && final_vertical_stretch <= 1,
                   "Invalid final vertical stretch: ", final_vertical_stretch);
    }

    [[nodiscard]] fixed final_vertical_stretch() const
    {
        return final_property();
    }
};


class bgs_mosaic_vertical_stretch_loop_action :
        public loop_template_action<int, fixed, bgs_mosaic_vertical_stretch_manager>
{

public:
    bgs_mosaic_vertical_stretch_loop_action(int duration_frames, fixed final_vertical_stretch) :
        loop_template_action(0, duration_frames, final_vertical_stretch)
    {
        BTN_ASSERT(final_vertical_stretch >= 0 && final_vertical_stretch <= 1,
                   "Invalid final vertical stretch: ", final_vertical_stretch);
    }

    [[nodiscard]] fixed final_vertical_stretch() const
    {
        return final_property();
    }
};


// stretch

class bgs_mosaic_stretch_manager
{

public:
    [[nodiscard]] static fixed get(const int&)
    {
        return bgs_mosaic::horizontal_stretch();
    }

    static void set(fixed stretch, int&)
    {
        bgs_mosaic::set_stretch(stretch);
    }
};


class bgs_mosaic_stretch_to_action :
        public to_template_action<int, fixed, bgs_mosaic_stretch_manager>
{

public:
    bgs_mosaic_stretch_to_action(int duration_frames, fixed final_stretch) :
        to_template_action(0, duration_frames, final_stretch)
    {
        BTN_ASSERT(final_stretch >= 0 && final_stretch <= 1,
                   "Invalid final vertical stretch: ", final_stretch);
    }

    [[nodiscard]] fixed final_stretch() const
    {
        return final_property();
    }
};


class bgs_mosaic_stretch_loop_action :
        public loop_template_action<int, fixed, bgs_mosaic_stretch_manager>
{

public:
    bgs_mosaic_stretch_loop_action(int duration_frames, fixed final_stretch) :
        loop_template_action(0, duration_frames, final_stretch)
    {
        BTN_ASSERT(final_stretch >= 0 && final_stretch <= 1,
                   "Invalid final vertical stretch: ", final_stretch);
    }

    [[nodiscard]] fixed final_stretch() const
    {
        return final_property();
    }
};

}

#endif
