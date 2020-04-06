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
    const btn::sprite_item& sprite_item;
    btn::sound_item death_sound_item;
    btn::fixed_size dimensions;
    int16_t graphics_index_1;
    int16_t graphics_index_2;
    int16_t life;
    int8_t experience;

    constexpr enemy_data(const btn::sprite_item& _sprite_item, const btn::sound_item& _death_sound_item,
                         const btn::fixed_size& _dimensions, int _graphics_index_1, int _graphics_index_2,
                         int _life, int _experience) :
        sprite_item(_sprite_item),
        death_sound_item(_death_sound_item),
        dimensions(_dimensions),
        graphics_index_1(int16_t(_graphics_index_1)),
        graphics_index_2(int16_t(_graphics_index_2)),
        life(int16_t(_life)),
        experience(int8_t(_experience))
    {
        BTN_CONSTEXPR_ASSERT(dimensions.width() >= 1 && dimensions.width() <= constants::max_enemy_size,
                             "Invalid width");
        BTN_CONSTEXPR_ASSERT(dimensions.height() >= 1 && dimensions.height() <= constants::max_enemy_size,
                             "Invalid height");
        BTN_CONSTEXPR_ASSERT(graphics_index_1 >= 0 && graphics_index_1 < sprite_item.tiles_item().graphics_count(),
                             "Invalid graphics index 1");
        BTN_CONSTEXPR_ASSERT(graphics_index_2 >= 0 && graphics_index_2 < sprite_item.tiles_item().graphics_count(),
                             "Invalid graphics index 2");
        BTN_CONSTEXPR_ASSERT(_life >= 1 && _life < btn::numeric_limits<int16_t>::max(), "Invalid life");
        BTN_CONSTEXPR_ASSERT(_experience >= 1 && _experience < btn::numeric_limits<int8_t>::max(),
                             "Invalid experience");
    }
};

}

#endif
