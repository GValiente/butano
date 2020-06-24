#ifndef BF_GAME_BOSS_INTRO_H
#define BF_GAME_BOSS_INTRO_H

#include "btn_vector.h"
#include "btn_optional.h"
#include "btn_sprite_ptr.h"
#include "btn_music_actions.h"
#include "btn_sprite_affine_mat_hblank_effects.h"

namespace bf::game
{

class boss_intro
{

public:
    [[nodiscard]] bool active() const
    {
        return _state == state::ACTIVE;
    }

    [[nodiscard]] bool done() const
    {
        return _state == state::DONE;
    }

    void enable();

    void update();

private:
    enum class state
    {
        INIT,
        ACTIVE,
        DONE
    };

    btn::vector<btn::sprite_ptr, 7> _sprites;
    btn::optional<btn::music_volume_to_action> _music_volume_action;
    btn::optional<btn::sprite_affine_mat_attributes_hblank_effect_ptr> _hblank_effect;
    state _state = state::INIT;
    int _loops = 0;
    int _counter = 0;
};

}

#endif
