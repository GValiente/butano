#ifndef BF_SCOREBOARD_H
#define BF_SCOREBOARD_H

#include "btn_vector.h"
#include "btn_optional.h"
#include "btn_sprite_ptr.h"
#include "btn_sprite_palette_actions.h"
#include "bf_constants.h"

namespace btn
{
    class sprite_text_generator;
}


namespace bf
{

class hero;

class scoreboard
{

public:
    explicit scoreboard(btn::sprite_text_generator& text_generator);

    void update(const hero& hero);

private:
    btn::sprite_text_generator& _text_generator;
    btn::vector<btn::sprite_ptr, 2> _level_sprites;
    btn::vector<btn::sprite_ptr, 4> _experience_sprites;
    btn::vector<btn::sprite_ptr, 4> _experience_bar_sprites;
    btn::vector<btn::sprite_ptr, constants::max_hero_bombs> _bomb_sprites;
    btn::optional<btn::sprite_palette_rotate_by_action> _experience_bar_palette_action;
    btn::optional<btn::sprite_palette_inverse_to_action> _bomb_palette_action;
    int _last_level = -1;
    int _last_experience = -1;
    int _last_bombs_count = -1;
};

}

#endif
