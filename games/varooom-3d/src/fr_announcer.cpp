/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_announcer.h"

#include "bn_sound_items.h"

namespace fr
{

namespace
{
    constexpr int secondary_tag = 1;
    constexpr int boost_charged_tag = 2;
}

void announcer::play_title_voice()
{
    _add_entry(bn::sound_items::v2_54_varoom3d_echo, 281, 0);
}

void announcer::play_race_start_voice(int delay_frames)
{
    unsigned choice = _random_choice.get<5>();

    switch(choice)
    {

    case 0:
        _add_entry(bn::sound_items::v2_43_thankyouforplaying, 93, delay_frames);
        break;

    case 1:
        _add_entry(bn::sound_items::v2_46_herewego, 83, delay_frames);
        break;

    case 2:
        _add_entry(bn::sound_items::v2_48_anotherdayofmotorsporting, 101, delay_frames);
        break;

    case 3:
        _add_entry(bn::sound_items::v2_49_thisisthelife, 58, delay_frames);
        break;

    case 4:
        _add_entry(bn::sound_items::v3_05_areyouready, 62, delay_frames);
        break;

    default:
        BN_ERROR("Unexpected choice: ", choice);
        break;
    }
}

void announcer::play_player_overtake_voice(int current_position)
{
    if(talking())
    {
        return;
    }

    if(current_position)
    {
        unsigned choice = _random_choice.get<9>();

        switch(choice)
        {

        case 0:
            _add_entry(bn::sound_items::v_22youredoinggreat, 65, 0, secondary_tag);
            break;

        case 1:
            _add_entry(bn::sound_items::v_23thatsawesome, 90, 0, secondary_tag);
            break;

        case 2:
            _add_entry(bn::sound_items::v_24greatjob, 63, 0, secondary_tag);
            break;

        case 3:
            _add_entry(bn::sound_items::v2_35_great, 40, 0, secondary_tag);
            break;

        case 4:
            _add_entry(bn::sound_items::v2_36_great, 36, 0, secondary_tag);
            break;

        case 5:
            _add_entry(bn::sound_items::v2_37_awesome, 58, 0, secondary_tag);
            break;

        case 6:
            _add_entry(bn::sound_items::v_07wow, 39, 0, secondary_tag);
            break;

        case 7:
            _add_entry(bn::sound_items::v3_03_goodjob, 54, 0, secondary_tag);
            break;

        case 8:
            _add_entry(bn::sound_items::v3_04_nicemove, 72, 0, secondary_tag);
            break;

        default:
            BN_ERROR("Unexpected choice: ", choice);
            break;
        }
    }
    else
    {
        unsigned choice = _random_choice.get<6>();

        switch(choice)
        {

        case 0:
            _add_entry(bn::sound_items::v_02firstplace, 93, 0, secondary_tag);
            break;

        case 1:
            _add_entry(bn::sound_items::v_06firstplace, 70, 0, secondary_tag);
            break;

        case 2:
            _add_entry(bn::sound_items::v2_38_unbeliavable, 81, 0, secondary_tag);
            break;

        case 3:
            _add_entry(bn::sound_items::v2_39_icannotbelievewhatimseeing, 126, 0, secondary_tag);
            break;

        case 4:
            _add_entry(bn::sound_items::v2_41_wowthatwasunbeliavable, 112, 0, secondary_tag);
            break;

        case 5:
            _add_entry(bn::sound_items::v3_09_spectacular, 83, 0, secondary_tag);
            break;

        default:
            BN_ERROR("Unexpected choice: ", choice);
            break;
        }
    }
}

void announcer::play_rival_overtake_voice()
{
    if(talking())
    {
        return;
    }

    unsigned choice = _random_choice.get<5>();

    switch(choice)
    {

    case 0:
        _add_entry(bn::sound_items::v_03whatashame, 70, 0, secondary_tag);
        break;

    case 1:
        _add_entry(bn::sound_items::v2_31_thisisntasundaydrive, 87, 0, secondary_tag);
        break;

    case 2:
        _add_entry(bn::sound_items::v_11tooslow, 64, 0, secondary_tag);
        break;

    case 3:
        _add_entry(bn::sound_items::v2_34_takeyourtimeitsnotliketheresracegoingon, 173, 0, secondary_tag);
        break;

    case 4:
        _add_entry(bn::sound_items::v_10youdrivelikemygrandmother, 126, 0, secondary_tag);
        break;

    default:
        BN_ERROR("Unexpected choice: ", choice);
        break;
    }
}

void announcer::play_boost_charged_voice(int delay_frames)
{
    if(talking())
    {
        return;
    }

    unsigned choice = _random_choice.get<4>();

    switch(choice)
    {

    case 0:
        _add_entry(bn::sound_items::v3_10_turboavailable, 72, delay_frames, boost_charged_tag);
        break;

    case 1:
        _add_entry(bn::sound_items::v2_19_boostcharged, 71, delay_frames, boost_charged_tag);
        break;

    case 2:
        _add_entry(bn::sound_items::v3_01_turboready, 60, delay_frames, boost_charged_tag);
        break;

    case 3:
        _add_entry(bn::sound_items::v3_02_turboready_variation, 59, delay_frames, boost_charged_tag);
        break;

    default:
        BN_ERROR("Unexpected choice: ", choice);
        break;
    }
}

void announcer::play_boost_used_voice(int delay_frames)
{
    _erase_entries_with_tag(boost_charged_tag);

    unsigned choice = _random_choice.get<4>();

    switch(choice)
    {

    case 0:
        _add_entry(bn::sound_items::v_19pedaltothemetal, 84, delay_frames, secondary_tag);
        break;

    case 1:
        _add_entry(bn::sound_items::v_21gottagofast, 70, delay_frames, secondary_tag);
        break;

    case 2:
        _add_entry(bn::sound_items::v_26burningrubber, 77, delay_frames, secondary_tag);
        break;

    case 3:
        _add_entry(bn::sound_items::v2_18_turboboost, 72, delay_frames, secondary_tag);
        break;

    default:
        BN_ERROR("Unexpected choice: ", choice);
        break;
    }
}

void announcer::play_rival_crash_voice(int delay_frames)
{
    if(talking())
    {
        return;
    }

    unsigned choice = _random_choice.get<7>();

    switch(choice)
    {

    case 0:
        _add_entry(bn::sound_items::v_14heywatchout, 92, delay_frames, secondary_tag);
        break;

    case 1:
        _add_entry(bn::sound_items::v_15hey, 29, delay_frames, secondary_tag);
        break;

    case 2:
        _add_entry(bn::sound_items::v2_50_canyouthinkofanythingbetter, 98, delay_frames, secondary_tag);
        break;

    case 3:
        _add_entry(bn::sound_items::v2_47_aah, 40, delay_frames, secondary_tag);
        break;

    case 4:
        _add_entry(bn::sound_items::v2_40_boo, 71, delay_frames, secondary_tag);
        break;

    case 5:
        _add_entry(bn::sound_items::v3_11_woahlaugh, 85, delay_frames, secondary_tag);
        break;

    case 6:
        _add_entry(bn::sound_items::v3_13_sigh, 66, delay_frames, secondary_tag);
        break;

    default:
        BN_ERROR("Unexpected choice: ", choice);
        break;
    }
}

void announcer::play_wall_crash_voice(int delay_frames)
{
    _erase_all_entries();

    unsigned choice = _random_choice.get<6>();

    switch(choice)
    {

    case 0:
        _add_entry(bn::sound_items::v_09trytostayonthecourseidiot, 125, delay_frames);
        break;

    case 1:
        _add_entry(bn::sound_items::v_16whatareyoudoing, 97, delay_frames);
        break;

    case 2:
        _add_entry(bn::sound_items::v_18heythosecarsarentcheapyouknow, 114, delay_frames);
        break;

    case 3:
        _add_entry(bn::sound_items::v2_33_didyougetyourdrivinglicencefromacerealbox, 181, delay_frames);
        break;

    case 4:
        _add_entry(bn::sound_items::v3_12_keepittogetherman, 82, delay_frames);
        break;

    case 5:
        _add_entry(bn::sound_items::v3_14_icantbelieveibetmymoneyonyou, 142, delay_frames);
        break;

    default:
        BN_ERROR("Unexpected choice: ", choice);
        break;
    }
}

void announcer::play_new_lap(int pending_laps, int delay_frames)
{
    _erase_entries_with_tag(secondary_tag);
    _erase_entries_with_tag(boost_charged_tag);

    unsigned choice;

    switch(pending_laps)
    {

    case 1:
        choice = _random_choice.get<3>();

        switch(choice)
        {

        case 0:
            _add_entry(bn::sound_items::v_01finallap, 78, delay_frames);
            break;

        case 1:
            _add_entry(bn::sound_items::v2_00_finallap, 70, delay_frames);
            break;

        case 2:
            _add_entry(bn::sound_items::v2_03_1laptogo, 69, delay_frames);
            break;

        default:
            BN_ERROR("Unexpected choice: ", choice);
            break;
        }
        break;

    case 2:
        choice = _random_choice.get<2>();

        switch(choice)
        {

        case 0:
            _add_entry(bn::sound_items::v2_02_2lapstogo, 62, delay_frames);
            break;

        case 1:
            _add_entry(bn::sound_items::v2_27_2lapstogo, 65, delay_frames);
            break;

        default:
            BN_ERROR("Unexpected choice: ", choice);
            break;
        }
        break;

    case 3:
        choice = _random_choice.get<2>();

        switch(choice)
        {

        case 0:
            _add_entry(bn::sound_items::v2_01_3lapstogo, 84, delay_frames);
            break;

        case 1:
            _add_entry(bn::sound_items::v2_26_3lapstogo, 63, delay_frames);
            break;

        default:
            BN_ERROR("Unexpected choice: ", choice);
            break;
        }
        break;

    case 4:
        choice = _random_choice.get<2>();

        switch(choice)
        {

        case 0:
            _add_entry(bn::sound_items::v2_25_4lapstogo, 71, delay_frames);
            break;

        case 1:
            _add_entry(bn::sound_items::v2_05_newlap, 52, delay_frames);
            break;

        default:
            BN_ERROR("Unexpected choice: ", choice);
            break;
        }
        break;

    case 5:
        choice = _random_choice.get<2>();

        switch(choice)
        {

        case 0:
            _add_entry(bn::sound_items::v2_24_5lapstogo, 78, delay_frames);
            break;

        case 1:
            _add_entry(bn::sound_items::v2_04_newlap, 52, delay_frames);
            break;

        default:
            BN_ERROR("Unexpected choice: ", choice);
            break;
        }
        break;

    case 6:
        choice = _random_choice.get<2>();

        switch(choice)
        {

        case 0:
            _add_entry(bn::sound_items::v2_23_6lapstogo, 74, delay_frames);
            break;

        case 1:
            _add_entry(bn::sound_items::v2_05_newlap, 52, delay_frames);
            break;

        default:
            BN_ERROR("Unexpected choice: ", choice);
            break;
        }
        break;

    case 7:
        choice = _random_choice.get<2>();

        switch(choice)
        {

        case 0:
            _add_entry(bn::sound_items::v2_22_7lapstogo, 75, delay_frames);
            break;

        case 1:
            _add_entry(bn::sound_items::v2_04_newlap, 52, delay_frames);
            break;

        default:
            BN_ERROR("Unexpected choice: ", choice);
            break;
        }
        break;

    case 8:
        choice = _random_choice.get<2>();

        switch(choice)
        {

        case 0:
            _add_entry(bn::sound_items::v2_21_8lapstogo, 80, delay_frames);
            break;

        case 1:
            _add_entry(bn::sound_items::v2_05_newlap, 52, delay_frames);
            break;

        default:
            BN_ERROR("Unexpected choice: ", choice);
            break;
        }
        break;

    default:
        choice = _random_choice.get<2>();

        switch(choice)
        {

        case 0:
            _add_entry(bn::sound_items::v2_20_9lapstogo, 70, delay_frames);
            break;

        case 1:
            _add_entry(bn::sound_items::v2_04_newlap, 52, delay_frames);
            break;

        default:
            BN_ERROR("Unexpected choice: ", choice);
            break;
        }
        break;
    }
}

void announcer::play_new_middle_checkpoint(int delay_frames)
{
    _erase_entries_with_tag(secondary_tag);
    _erase_entries_with_tag(boost_charged_tag);

    unsigned choice = _random_choice.get<4>();

    switch(choice)
    {

    case 0:
        _add_entry(bn::sound_items::v3_00_checkpoint, 56, delay_frames);
        break;

    case 1:
        _add_entry(bn::sound_items::v3_18_timeadded, 61, delay_frames);
        break;

    case 2:
        _add_entry(bn::sound_items::v3_19_checkpoint, 62, delay_frames);
        break;

    case 3:
        _add_entry(bn::sound_items::v3_20_keepitgoing, 55, delay_frames);
        break;

    default:
        BN_ERROR("Unexpected choice: ", choice);
        break;
    }
}

void announcer::play_pending_time_warning(int delay_frames)
{
    if(_pending_time_warning_played || talking())
    {
        return;
    }

    _pending_time_warning_played = true;

    unsigned choice = _random_choice.get<5>();

    switch(choice)
    {

    case 0:
        _add_entry(bn::sound_items::v_13becarefulnow, 64, delay_frames, secondary_tag);
        break;

    case 1:
        _add_entry(bn::sound_items::v2_45_youcandoitibelieveinyou, 115, delay_frames, secondary_tag);
        break;

    case 2:
        _add_entry(bn::sound_items::v3_06_dontletmedown, 76, delay_frames, secondary_tag);
        break;

    case 3:
        _add_entry(bn::sound_items::v3_07_hurryup, 51, delay_frames, secondary_tag);
        break;

    case 4:
        _add_entry(bn::sound_items::v3_08_nomuchtimeleft, 76, delay_frames, secondary_tag);
        break;

    default:
        BN_ERROR("Unexpected choice: ", choice);
        break;
    }
}

void announcer::play_finish_voice()
{
    if(talking())
    {
        return;
    }

    _add_entry(bn::sound_items::v3_17_finish, 76, 0);
}

void announcer::play_win_voice()
{
    _add_entry(bn::sound_items::v_07wow, 39, 0);
    _add_entry(bn::sound_items::v2_06_youwin, 54, 0);
}

void announcer::play_finish_position_voice(int finish_position)
{
    switch(finish_position)
    {

    case 0:
        _add_entry(bn::sound_items::v2_07_youfinished1st, 88, 0);
        break;

    case 1:
        _add_entry(bn::sound_items::v2_08_youfinished2nd, 88, 0);
        break;

    default:
        _add_entry(bn::sound_items::v2_09_youfinished3rd, 88, 0);
        break;
    }
}

void announcer::play_unlocked_stages_voice(int unlocked_stages)
{
    if(unlocked_stages == 1)
    {
        _add_entry(bn::sound_items::v2_10_youunlocked1newcourse, 131, 0);
    }
    else
    {
        _add_entry(bn::sound_items::v2_11_youunlocked2newcourses, 141, 0);
    }
}

void announcer::play_unlocked_models_voice(int unlocked_models)
{
    switch(unlocked_models)
    {

    case 1:
        _add_entry(bn::sound_items::v2_12_youunlocked1new3dmodel, 165, 0);
        break;

    case 2:
        _add_entry(bn::sound_items::v2_13_youunlocked2new3dmodels, 153, 0);
        break;

    case 3:
        _add_entry(bn::sound_items::v2_14_youunlocked3new3dmodels, 175, 0);
        break;

    case 4:
        _add_entry(bn::sound_items::v2_15_youunlocked4new3dmodels, 149, 0);
        break;

    default:
        _add_entry(bn::sound_items::v2_16_youunlocked5new3dmodels, 163, 0);
        break;
    }

    _add_entry(bn::sound_items::v2_17_checkthemin3dviewer, 100, 0);
}

void announcer::play_restart_voice(int delay_frames)
{
    _add_entry(bn::sound_items::v_12tryagain, 62, delay_frames);
}

void announcer::play_exit_voice(int delay_frames)
{
    unsigned choice = _random_choice.get<3>();

    switch(choice)
    {

    case 0:
        _add_entry(bn::sound_items::v_05betterlucknexttime, 83, delay_frames);
        break;

    case 1:
        _add_entry(bn::sound_items::v2_43_thankyouforplaying, 93, delay_frames);
        break;

    case 2:
        _add_entry(bn::sound_items::v2_44_seeyounexttime, 78, delay_frames);
        break;

    default:
        BN_ERROR("Unexpected choice: ", choice);
        break;
    }
}

void announcer::update()
{
    if(int wait_updates = _wait_updates)
    {
        _wait_updates = wait_updates - 1;
        return;
    }

    if(_entries_queue.empty())
    {
        _current_tag = 0;
    }
    else
    {
        const entry& first_entry = _entries_queue.front();
        int wait_frames = first_entry.sound_length_frames;
        _wait_updates = wait_frames + 1;
        _current_tag = first_entry.tag;
        first_entry.sound_item.play();
        _entries_queue.pop_front();
    }
}

void announcer::_add_entry(bn::sound_item sound_item, int sound_length_frames, int delay_frames, int tag)
{
    if(_entries_queue.full())
    {
        _entries_queue.pop_front();
    }

    _entries_queue.push_back(entry{ sound_item, int16_t(sound_length_frames), int16_t(tag) });
    _wait_updates = bn::max(_wait_updates, delay_frames + 1);
}

void announcer::_erase_entries_with_tag(int tag)
{
    auto it = _entries_queue.begin();
    auto end = _entries_queue.end();

    while(it != end)
    {
        if(it->tag == tag)
        {
            it = _entries_queue.erase(it);
            end = _entries_queue.end();
        }
        else
        {
            ++it;
        }
    }
}

}
