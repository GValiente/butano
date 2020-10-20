/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_STAGE_H
#define BF_GAME_STAGE_H

#include "btn_music_item.h"
#include "btn_string_view.h"
#include "btn_regular_bg_item.h"
#include "bf_game_boss.h"
#include "bf_game_enemy_event.h"

namespace bf::game
{

class stage
{

public:
    const btn::regular_bg_item& background_bottom_bg_item;
    btn::fixed_point background_bottom_bg_delta_position;
    const btn::regular_bg_item& background_top_bg_item;
    btn::fixed_point background_top_bg_delta_position;
    const btn::sprite_item& intro_sprite_item;
    const btn::sprite_item& intro_alt_sprite_item;
    btn::string_view intro_top_label;
    btn::string_view intro_bottom_label;
    btn::music_item music_item;
    btn::fixed music_volume;
    btn::span<const enemy_event> enemy_events;
    boss::type boss_type;
    bool in_air;

    constexpr stage(
            const btn::regular_bg_item& _background_bottom_bg_item, const btn::fixed_point& _background_bottom_bg_delta_position,
            const btn::regular_bg_item& _background_top_bg_item, const btn::fixed_point& _background_top_bg_delta_position,
            const btn::sprite_item& _intro_sprite_item, const btn::sprite_item& _intro_alt_sprite_item,
            const btn::string_view& _intro_top_label, const btn::string_view& _intro_bottom_label,
            btn::music_item _music_item, btn::fixed _music_volume, const btn::span<const enemy_event>& _enemy_events,
            boss::type _boss_type, bool _in_air) :
        background_bottom_bg_item(_background_bottom_bg_item),
        background_bottom_bg_delta_position(_background_bottom_bg_delta_position),
        background_top_bg_item(_background_top_bg_item),
        background_top_bg_delta_position(_background_top_bg_delta_position),
        intro_sprite_item(_intro_sprite_item),
        intro_alt_sprite_item(_intro_alt_sprite_item),
        intro_top_label(_intro_top_label),
        intro_bottom_label(_intro_bottom_label),
        music_item(_music_item),
        music_volume(_music_volume),
        enemy_events(_enemy_events),
        boss_type(_boss_type),
        in_air(_in_air)
    {
        BTN_ASSERT(! enemy_events.empty(), "Enemy events is empty");
        BTN_ASSERT(music_volume > 0 && music_volume <= 1, "Invalid music volume: ", music_volume);
    }
};

}

#endif
