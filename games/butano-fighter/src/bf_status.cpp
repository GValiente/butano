/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_status.h"

#include "bn_sram.h"
#include "bn_sstream.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_palettes.h"
#include "bf_game_stage_1.h"
#include "bf_game_stage_2.h"
#include "bf_game_stage_3.h"
#include "bf_game_stage_4.h"
#include "bf_game_hero_bullet_level.h"

namespace bf
{

namespace
{
    struct sram_data
    {
        constexpr static const char* valid_label = "bf002";

        char label[8] = {};
        int high_experience = 0;
        int brightness = 0;
        bool rumble_enabled = false;
        bool how_to_play_viewed = false;

        [[nodiscard]] bool read()
        {
            bn::sram::read(*this);
            return bn::string_view(label) == valid_label;
        }

        void write()
        {
            bn::istring_base label_istring(label);
            bn::ostringstream label_stream(label_istring);
            label_stream.append(valid_label);
            bn::sram::write(*this);
        }
    };

    void _write_sram(status& status)
    {
        sram_data sram_data_to_write;
        sram_data_to_write.high_experience = status.high_experience();
        sram_data_to_write.brightness = bn::bg_palettes::brightness().data();
        sram_data_to_write.rumble_enabled = status.rumble_enabled();
        sram_data_to_write.how_to_play_viewed = status.how_to_play_viewed();
        sram_data_to_write.write();
    }
}

status::status() :
    _current_stage(&game::stage_1::get())
{
    bn::span<const game::hero_bullet_level> hero_bullet_levels = game::hero_bullet_level::all_levels();

    if(_level > 0)
    {
        int experience_to_current_level = hero_bullet_levels[_level - 1].experience_to_next_level;
        _experience = bn::max(_experience, experience_to_current_level);
    }

    sram_data sram_data_to_read;

    if(sram_data_to_read.read())
    {
        _high_experience = sram_data_to_read.high_experience;
        _rumble_enabled = sram_data_to_read.rumble_enabled;
        _how_to_play_viewed = sram_data_to_read.how_to_play_viewed;

        bn::fixed brightness = bn::fixed::from_data(sram_data_to_read.brightness);
        bn::bg_palettes::set_brightness(brightness);
        bn::sprite_palettes::set_brightness(brightness);
    }
}

bool status::go_to_next_stage()
{
    if(_current_stage == &game::stage_1::get())
    {
        _current_stage = &game::stage_2::get();
        return true;
    }

    if(_current_stage == &game::stage_2::get())
    {
        _current_stage = &game::stage_3::get();
        return true;
    }

    if(_current_stage == &game::stage_3::get())
    {
        _current_stage = &game::stage_4::get();
        return true;
    }

    return false;
}

bool status::add_level()
{
    bn::span<const game::hero_bullet_level> hero_bullet_levels = game::hero_bullet_level::all_levels();

    if(_level == hero_bullet_levels.size() - 1)
    {
        return false;
    }

    ++_level;
    return true;
}

bn::fixed status::next_level_experience_ratio() const
{
    bn::span<const game::hero_bullet_level> hero_bullet_levels = game::hero_bullet_level::all_levels();

    if(_level == hero_bullet_levels.size() - 1)
    {
        return 1;
    }

    int experience_to_current_level = _level == 0 ? 0 : hero_bullet_levels[_level - 1].experience_to_next_level;
    int next_level_experience = hero_bullet_levels[_level].experience_to_next_level - experience_to_current_level;

    if(next_level_experience <= 0)
    {
        return 1;
    }

    bn::fixed experience = _experience - experience_to_current_level;
    return bn::min(experience / next_level_experience, bn::fixed(1));
}

bool status::add_experience(int experience)
{
    bn::span<const game::hero_bullet_level> hero_bullet_levels = game::hero_bullet_level::all_levels();
    int level = _level;
    _experience += experience;

    if(level == hero_bullet_levels.size() - 1)
    {
        return false;
    }

    return _experience >= hero_bullet_levels[level].experience_to_next_level;
}

bool status::add_bomb()
{
    if(_bombs_count == constants::max_hero_bombs)
    {
        return false;
    }

    ++_bombs_count;
    return true;
}

bool status::throw_bomb()
{
    if(! _bombs_count)
    {
        return false;
    }

    --_bombs_count;
    return true;
}

bool status::throw_shield()
{
    if(_bombs_count <= 1)
    {
        return false;
    }

    _bombs_count = 0;
    return true;
}

void status::update_high_experience()
{
    _high_experience = bn::max(_experience, _high_experience);
    _write_sram(*this);
}

void status::set_rumble_enabled(bool rumble_enabled)
{
    _rumble_enabled = rumble_enabled;
    _write_sram(*this);
}

void status::mark_how_to_play_as_viewed()
{
    _how_to_play_viewed = true;
    _write_sram(*this);
}

}
