#ifndef BF_GAME_STAGE_H
#define BF_GAME_STAGE_H

#include "btn_music_item.h"
#include "btn_string_view.h"
#include "btn_regular_bg_item.h"
#include "bf_game_enemy_event.h"

namespace bf::game
{

class stage
{

public:
    const btn::regular_bg_item& background_bottom_bg_item;
    const btn::regular_bg_item& background_top_bg_item;
    const btn::sprite_item& intro_sprite_item;
    const btn::sprite_item& intro_alt_sprite_item;
    btn::string_view intro_top_label;
    btn::string_view intro_bottom_label;
    const btn::music_item& music_item;
    btn::span<const enemy_event> enemy_events;

    constexpr stage(
            const btn::regular_bg_item& _background_bottom_bg_item, const btn::regular_bg_item& _background_top_bg_item,
            const btn::sprite_item& _intro_sprite_item, const btn::sprite_item& _intro_alt_sprite_item,
            const btn::string_view& _intro_top_label, const btn::string_view& _intro_bottom_label,
            const btn::music_item& _music_item, const btn::span<const enemy_event>& _enemy_events) :
        background_bottom_bg_item(_background_bottom_bg_item),
        background_top_bg_item(_background_top_bg_item),
        intro_sprite_item(_intro_sprite_item),
        intro_alt_sprite_item(_intro_alt_sprite_item),
        intro_top_label(_intro_top_label),
        intro_bottom_label(_intro_bottom_label),
        music_item(_music_item),
        enemy_events(_enemy_events)
    {
        BTN_CONSTEXPR_ASSERT(! enemy_events.empty(), "Enemy events is empty");
    }
};

}

#endif
