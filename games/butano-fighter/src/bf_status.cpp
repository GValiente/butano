#include "bf_status.h"

#include "btn_sram.h"
#include "btn_sstream.h"
#include "btn_string_view.h"
#include "btn_bg_palettes.h"
#include "btn_sprite_palettes.h"
#include "bf_game_stage_1.h"
#include "bf_game_stage_2.h"
#include "bf_game_hero_bullet_level.h"

namespace bf
{

namespace
{
    struct sram_data
    {
        constexpr static const char* valid_label = "bf001";

        char label[8] = {};
        int high_experience = 0;
        int brightness = 0;
        bool how_to_play_viewed = false;

        [[nodiscard]] bool read()
        {
            btn::sram::read(*this);
            return btn::string_view(label) == valid_label;
        }

        void write()
        {
            btn::istring_base label_istring(label);
            btn::ostringstream label_stream(label_istring);
            label_stream.append(valid_label);
            btn::sram::write(*this);
        }
    };

    void _write_sram(status& status)
    {
        sram_data sram_data_to_write;
        sram_data_to_write.high_experience = status.high_experience();
        sram_data_to_write.brightness = btn::bg_palettes::brightness().data();
        sram_data_to_write.how_to_play_viewed = status.how_to_play_viewed();
        sram_data_to_write.write();
    }
}

status::status() :
    _current_stage(&game::stage_2::get())
{
    btn::span<const game::hero_bullet_level> hero_bullet_levels = game::hero_bullet_level::all_levels();

    if(_level > 0)
    {
        int experience_to_current_level = hero_bullet_levels[_level - 1].experience_to_next_level;
        _experience = btn::max(_experience, experience_to_current_level);
    }

    sram_data sram_data_to_read;

    if(sram_data_to_read.read())
    {
        _high_experience = sram_data_to_read.high_experience;
        _how_to_play_viewed = sram_data_to_read.how_to_play_viewed;

        btn::fixed brightness = btn::fixed::from_data(sram_data_to_read.brightness);
        btn::bg_palettes::set_brightness(brightness);
        btn::sprite_palettes::set_brightness(brightness);
    }
}

bool status::go_to_next_stage()
{
    if(_current_stage == &game::stage_1::get())
    {
        _current_stage = &game::stage_2::get();
        return true;
    }

    return false;
}

bool status::add_level()
{
    btn::span<const game::hero_bullet_level> hero_bullet_levels = game::hero_bullet_level::all_levels();

    if(_level == hero_bullet_levels.size() - 1)
    {
        return false;
    }

    ++_level;
    return true;
}

btn::fixed status::next_level_experience_ratio() const
{
    btn::span<const game::hero_bullet_level> hero_bullet_levels = game::hero_bullet_level::all_levels();

    if(_level == hero_bullet_levels.size() - 1)
    {
        return 1;
    }

    int experience_to_current_level = _level == 0 ? 0 : hero_bullet_levels[_level - 1].experience_to_next_level;
    int next_level_experience = hero_bullet_levels[_level].experience_to_next_level - experience_to_current_level;
    btn::fixed experience = _experience - experience_to_current_level;
    return btn::min(experience / next_level_experience, btn::fixed(1));
}

bool status::add_experience(int experience)
{
    btn::span<const game::hero_bullet_level> hero_bullet_levels = game::hero_bullet_level::all_levels();
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
    _high_experience = btn::max(_experience, _high_experience);
    _write_sram(*this);
}

void status::mark_how_to_play_as_viewed()
{
    _how_to_play_viewed = true;
    _write_sram(*this);
}

}
