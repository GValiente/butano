/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_ENEMY_DATA_H
#define BF_GAME_ENEMY_DATA_H

#include "btn_span.h"
#include "btn_fixed_size.h"
#include "btn_sound_item.h"
#include "btn_sprite_item.h"
#include "bf_constants.h"

namespace bf::game
{

class enemy_data
{

public:
    enum class death_anim_type
    {
        ROTATE,
        HORIZONTAL_SCALE,
        VERTICAL_SCALE,
        MINI_EXPLOSION,
        EXPLOSION
    };

    using graphics_indexes_group = btn::array<uint16_t, 4>;

    const btn::sprite_item& sprite_item;
    death_anim_type death_anim;
    btn::sound_item death_sound_item;
    btn::fixed_size dimensions;
    btn::fixed_size half_dimensions;
    btn::array<graphics_indexes_group, 4> graphics_indexes_groups;
    int16_t life;
    int16_t experience;
    int8_t animation_wait_frames;

    constexpr enemy_data(const btn::sprite_item& _sprite_item, death_anim_type _death_anim,
                         btn::sound_item _death_sound_item, const btn::fixed_size& _dimensions,
                         int graphics_index_1, int graphics_index_2, int _life, int _experience,
                         int _animation_wait_frames = 16) :
        enemy_data(_sprite_item, _death_anim, _death_sound_item, _dimensions,
                   graphics_indexes_group{ uint16_t(graphics_index_1), uint16_t(graphics_index_2),
                                           uint16_t(graphics_index_1), uint16_t(graphics_index_2) },
                   _life, _experience, _animation_wait_frames)
    {
        BTN_ASSERT(graphics_index_1 >= 0 && graphics_index_1 < sprite_item.tiles_item().graphics_count(),
                   "Invalid graphics index 1: ", graphics_index_1, " - ", sprite_item.tiles_item().graphics_count());
        BTN_ASSERT(graphics_index_2 >= 0 && graphics_index_2 < sprite_item.tiles_item().graphics_count(),
                   "Invalid graphics index 2: ", graphics_index_2, " - ", sprite_item.tiles_item().graphics_count());
    }

    constexpr enemy_data(const btn::sprite_item& _sprite_item, death_anim_type _death_anim,
                         btn::sound_item _death_sound_item, const btn::fixed_size& _dimensions,
                         const btn::array<uint16_t, 4>& graphics_indexes, int _life, int _experience,
                         int _animation_wait_frames = 16) :
        enemy_data(_sprite_item, _death_anim, _death_sound_item, _dimensions,
                   { graphics_indexes, graphics_indexes, graphics_indexes, graphics_indexes }, _life, _experience,
                   _animation_wait_frames)
    {
    }

    constexpr enemy_data(const btn::sprite_item& _sprite_item, death_anim_type _death_anim,
                         btn::sound_item _death_sound_item, const btn::fixed_size& _dimensions,
                         const btn::array<graphics_indexes_group, 4> _graphics_indexes_groups, int _life,
                         int _experience, int _animation_wait_frames = 16) :
        sprite_item(_sprite_item),
        death_anim(_death_anim),
        death_sound_item(_death_sound_item),
        dimensions(_dimensions),
        half_dimensions(_dimensions / 2),
        graphics_indexes_groups(_graphics_indexes_groups),
        life(int16_t(_life)),
        experience(int16_t(_experience)),
        animation_wait_frames(int8_t(_animation_wait_frames))
    {
        BTN_ASSERT(dimensions.width() >= 1 && dimensions.width() <= constants::max_enemy_size,
                   "Invalid width: ", dimensions.width());
        BTN_ASSERT(dimensions.height() >= 1 && dimensions.height() <= constants::max_enemy_size,
                   "Invalid height: ", dimensions.height());
        BTN_ASSERT(_validate_graphic_indexes_groups(), "Invalid graphic indexes groups");
        BTN_ASSERT(_life >= 1 && _life < btn::numeric_limits<int16_t>::max(), "Invalid life: ", _life);
        BTN_ASSERT(_experience >= 1 && _experience < btn::numeric_limits<int16_t>::max(),
                   "Invalid experience: ", _experience);
        BTN_ASSERT(_animation_wait_frames >= 1 && _animation_wait_frames < btn::numeric_limits<int8_t>::max(),
                   "Invalid animation wait frames: ", _animation_wait_frames);
    }

private:
    [[nodiscard]] constexpr bool _validate_graphic_indexes_groups() const
    {
        int graphics_count = sprite_item.tiles_item().graphics_count();

        for(const graphics_indexes_group& group : graphics_indexes_groups)
        {
            for(int graphics_index : group)
            {
                if(graphics_index >= graphics_count)
                {
                    return false;
                }
            }
        }

        return true;
    }
};

}

#endif
