/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_STAGE_H
#define BF_GAME_STAGE_H

#include "bn_music_item.h"
#include "bn_string_view.h"
#include "bn_regular_bg_item.h"
#include "bf_game_boss.h"
#include "bf_game_enemy_event.h"

namespace bf::game
{

class stage
{

public:
    const bn::regular_bg_item& background_bottom_bg_item;
    bn::fixed_point background_bottom_bg_delta_position;
    const bn::regular_bg_item& background_top_bg_item;
    bn::fixed_point background_top_bg_delta_position;
    const bn::sprite_item& intro_sprite_item;
    const bn::sprite_item& intro_alt_sprite_item;
    bn::string_view intro_top_label;
    bn::string_view intro_bottom_label;
    bn::music_item music_item;
    bn::fixed music_volume;
    bn::span<const enemy_event> enemy_events;
    boss::type boss_type;
    bool in_air;

    constexpr stage(
            const bn::regular_bg_item& _background_bottom_bg_item, const bn::fixed_point& _background_bottom_bg_delta_position,
            const bn::regular_bg_item& _background_top_bg_item, const bn::fixed_point& _background_top_bg_delta_position,
            const bn::sprite_item& _intro_sprite_item, const bn::sprite_item& _intro_alt_sprite_item,
            const bn::string_view& _intro_top_label, const bn::string_view& _intro_bottom_label,
            bn::music_item _music_item, bn::fixed _music_volume, const bn::span<const enemy_event>& _enemy_events,
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
        BN_ASSERT(! enemy_events.empty(), "Enemy events is empty");
        BN_ASSERT(music_volume > 0 && music_volume <= 1, "Invalid music volume: ", music_volume);
    }
};

}

#endif
