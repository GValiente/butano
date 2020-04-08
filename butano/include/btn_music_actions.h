#ifndef BTN_MUSIC_ACTIONS_H
#define BTN_MUSIC_ACTIONS_H

#include "btn_fixed.h"
#include "btn_music.h"
#include "btn_template_actions.h"

namespace btn
{

// volume

class music_volume_manager
{

public:
    [[nodiscard]] static fixed get()
    {
        return music::volume();
    }

    static void set(fixed volume)
    {
        music::set_volume(volume);
    }
};


class music_volume_to_action : public to_template_action<fixed, music_volume_manager>
{

public:
    music_volume_to_action(int duration_frames, fixed final_volume) :
        to_template_action(duration_frames, final_volume)
    {
        BTN_ASSERT(final_volume >= 0 && final_volume <= 1, "Invalid final volume: ", final_volume);
    }

    [[nodiscard]] fixed final_volume() const
    {
        return final_property();
    }
};


class music_volume_loop_action :
        public loop_template_action<fixed, music_volume_manager>
{

public:
    music_volume_loop_action(int duration_frames, fixed final_volume) :
        loop_template_action(duration_frames, final_volume)
    {
        BTN_ASSERT(final_volume >= 0 && final_volume <= 1, "Invalid final volume: ", final_volume);
    }

    [[nodiscard]] fixed final_volume() const
    {
        return final_property();
    }
};

}

#endif
