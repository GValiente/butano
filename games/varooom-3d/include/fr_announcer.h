/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_ANNOUNCER_H
#define FR_ANNOUNCER_H

#include "bn_deque.h"
#include "bn_sound_item.h"

#include "fr_random_choice.h"

namespace fr
{

class announcer
{

public:
    [[nodiscard]] bool talking() const
    {
        return _wait_updates || ! _entries_queue.empty();
    }

    void clear_queue()
    {
        return _entries_queue.clear();
    }

    void play_title_voice();

    void play_race_start_voice(int delay_frames);

    void play_player_overtake_voice(int current_position);

    void play_rival_overtake_voice();

    void play_boost_charged_voice(int delay_frames);

    void play_boost_used_voice(int delay_frames);

    void play_rival_crash_voice(int delay_frames);

    void play_wall_crash_voice(int delay_frames);

    void play_new_lap(int pending_laps, int delay_frames);

    void play_new_middle_checkpoint(int delay_frames);

    void play_pending_time_warning(int delay_frames);

    void play_finish_voice();

    void play_win_voice();

    void play_finish_position_voice(int finish_position);

    void play_unlocked_stages_voice(int unlocked_stages);

    void play_unlocked_models_voice(int unlocked_models);

    void play_restart_voice(int delay_frames);

    void play_exit_voice(int delay_frames);

    template<unsigned NumChoices>
    void refesh_choice()
    {
        [[maybe_unused]] unsigned unused_choice = _random_choice.get<NumChoices>();
    }

    void update();

private:
    struct entry
    {
        bn::sound_item sound_item;
        int16_t sound_length_frames;
        int16_t tag;
    };

    bn::deque<entry, 4> _entries_queue;
    random_choice _random_choice;
    int _wait_updates = 0;
    int _current_tag = 0;
    bool _pending_time_warning_played = false;

    void _add_entry(bn::sound_item sound_item, int sound_length_frames, int delay_frames, int tag = 0);

    void _erase_all_entries()
    {
        _entries_queue.clear();
    }

    void _erase_entries_with_tag(int tag);
};

}

#endif
