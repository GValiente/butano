#include "bf_game_status.h"

#include "btn_sram.h"
#include "btn_string_view.h"
#include "bf_game_hero_bullet_level.h"

namespace bf::game
{

namespace
{
    struct sram_data
    {
        constexpr static const char* valid_label = "bf10";

        char label[8] = {};
        int high_experience = 0;

        [[nodiscard]] bool read()
        {
            btn::sram::read(*this);
            return btn::string_view(label) == valid_label;
        }

        void write()
        {
            btn::istring_base label_istring(label);
            btn::input_string_stream label_stream(label_istring);
            label_stream.append(valid_label);
            btn::sram::write(*this);
        }
    };
}

status::status()
{
    btn::span<const hero_bullet_level> hero_bullet_levels = hero_bullet_level::all_levels();
    int min_experience = 0;

    for(int index = 0, limit = _level; index < limit; ++index)
    {
        min_experience += hero_bullet_levels[index].experience_to_next_level;
    }

    _experience = btn::max(_experience, min_experience);

    sram_data sram_data_to_read;

    if(sram_data_to_read.read())
    {
        _high_experience = sram_data_to_read.high_experience;
    }
}

bool status::add_level()
{
    btn::span<const hero_bullet_level> hero_bullet_levels = hero_bullet_level::all_levels();

    if(_level == hero_bullet_levels.size() - 1)
    {
        return false;
    }

    ++_level;
    return true;
}

btn::fixed status::next_level_experience_ratio() const
{
    btn::span<const hero_bullet_level> hero_bullet_levels = hero_bullet_level::all_levels();

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
    btn::span<const hero_bullet_level> hero_bullet_levels = hero_bullet_level::all_levels();
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

void status::update_high_experience()
{
    sram_data sram_data_to_write;
    sram_data_to_write.high_experience = btn::max(_experience, _high_experience);
    sram_data_to_write.write();

    sram_data sram_data_to_read;
    BTN_ASSERT(sram_data_to_read.read(), "SRAM read after write failed");
}

}
