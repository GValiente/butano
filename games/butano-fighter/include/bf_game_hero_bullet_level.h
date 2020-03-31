#ifndef BF_GAME_HERO_BULLET_LEVEL_H
#define BF_GAME_HERO_BULLET_LEVEL_H

#include "btn_span.h"
#include "btn_color.h"
#include "btn_fixed_size.h"
#include "btn_sound_item.h"
#include "bf_game_hero_bullet_event.h"

namespace bf::game
{

class hero_bullet_level
{

public:
    btn::span<const hero_bullet_event> events;
    btn::fixed_size dimensions;
    btn::sound_item sound_item;
    btn::color color;
    int8_t loop_frames;
    int8_t damage;

    [[nodiscard]] static btn::span<const hero_bullet_level> all_levels();

    constexpr hero_bullet_level(const btn::span<const hero_bullet_event>& _events, const btn::fixed_size& _dimensions,
                                btn::sound_item _sound_item, btn::color _color, int _loop_frames, int _damage) :
        events(_events),
        dimensions(_dimensions),
        sound_item(_sound_item),
        color(_color),
        loop_frames(int8_t(_loop_frames)),
        damage(int8_t(_damage))
    {
        BTN_CONSTEXPR_ASSERT(! events.empty(), "Events is empty");
        BTN_CONSTEXPR_ASSERT(dimensions.width() >= 1 && dimensions.width() <= constants::max_rect_size,
                             "Invalid width");
        BTN_CONSTEXPR_ASSERT(dimensions.height() >= 1 && dimensions.height() <= constants::max_rect_size,
                             "Invalid height");
        BTN_CONSTEXPR_ASSERT(_loop_frames >= 1 && _loop_frames < btn::numeric_limits<int8_t>::max(),
                             "Invalid loop frames");
        BTN_CONSTEXPR_ASSERT(_validate_loop_frames(), "Loop frames validation failed");
        BTN_CONSTEXPR_ASSERT(_damage >= 1 && _damage < btn::numeric_limits<int8_t>::max(), "Invalid damage");
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
