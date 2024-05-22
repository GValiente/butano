/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_HERO_BULLET_LEVEL_H
#define BF_GAME_HERO_BULLET_LEVEL_H

#include "bn_span.h"
#include "bn_color.h"
#include "bn_fixed_size.h"
#include "bn_sound_item.h"
#include "bf_game_hero_bullet_event.h"

namespace bf::game
{

class hero_bullet_level
{

public:
    bn::span<const hero_bullet_event> events;
    bn::fixed_size dimensions;
    bn::sound_item sound_item;
    int experience_to_next_level;
    bn::color color;
    int8_t loop_frames;
    int8_t damage;

    [[nodiscard]] static bn::span<const hero_bullet_level> all_levels();

    [[nodiscard]] static int gem_experience(int level, bn::fixed y);

    constexpr hero_bullet_level(const bn::span<const hero_bullet_event>& _events, const bn::fixed_size& _dimensions,
                                bn::sound_item _sound_item, int _experience_to_next_level, bn::color _color,
                                int _loop_frames, int _damage) :
        events(_events),
        dimensions(_dimensions),
        sound_item(_sound_item),
        experience_to_next_level(_experience_to_next_level),
        color(_color),
        loop_frames(int8_t(_loop_frames)),
        damage(int8_t(_damage))
    {
        BN_ASSERT(! events.empty(), "Events is empty");
        BN_ASSERT(dimensions.width() >= 1 && dimensions.width() <= constants::enemies_grid_size,
                   "Invalid width: ", dimensions.width());
        BN_ASSERT(dimensions.height() >= 1 && dimensions.height() <= constants::enemies_grid_size,
                   "Invalid height: ", dimensions.height());
        BN_ASSERT(_experience_to_next_level >= 1, "Invalid experience to next level: ", _experience_to_next_level);
        BN_ASSERT(_loop_frames >= 1 && _loop_frames < bn::numeric_limits<int8_t>::max(),
                   "Invalid loop frames: ", _loop_frames);
        BN_ASSERT(_validate_loop_frames(), "Loop frames validation failed");
        BN_ASSERT(_damage >= 1 && _damage < bn::numeric_limits<int8_t>::max(), "Invalid damage: ", _damage);
    }

private:
    [[nodiscard]] constexpr bool _validate_loop_frames() const
    {
        for(const hero_bullet_event& event : events)
        {
            if(event.frame >= loop_frames)
            {
                return false;
            }
        }

        return true;
    }
};

}

#endif
