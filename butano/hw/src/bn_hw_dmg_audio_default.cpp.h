/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_dmg_audio.h"

#include "bn_assert.h"
#include "bn_dmg_music_type.h"
#include "../3rd_party/vgm-player/include/vgm.h"

extern "C"
{
    #include "../3rd_party/gbt-player/include/gbt_player.h"
}

namespace bn::hw::dmg_audio
{

namespace
{
    class static_data
    {

    public:
        #if BN_CFG_ASSERT_ENABLED
            unsigned vgm_offset_play = 0;
        #endif
        uint16_t control_value = 0;
        bn::dmg_music_type music_type = dmg_music_type::GBT_PLAYER;
        bool music_paused = false;
        #if BN_CFG_ASSERT_ENABLED
            bool vgm_commit_failed = false;
        #endif
    };

    BN_DATA_EWRAM_BSS static_data data;
}

void init()
{
    ::new(static_cast<void*>(&data)) static_data();
}

void enable()
{
    REG_SNDDMGCNT = data.control_value;
}

void disable()
{
    data.control_value = REG_SNDDMGCNT;
    REG_SNDDMGCNT = 0;
}

[[nodiscard]] bool music_playing()
{
    if(data.music_paused)
    {
        return true;
    }

    if(data.music_type == dmg_music_type::GBT_PLAYER)
    {
        return gbt_is_playing();
    }
    else
    {
        return VgmActive();
    }
}

void stop_music()
{
    if(data.music_type == dmg_music_type::GBT_PLAYER)
    {
        gbt_stop();
    }
    else
    {
        VgmStop();
    }

    data.music_paused = false;
}

void play_music(const void* song, dmg_music_type type, int speed, bool loop)
{
    if(type != data.music_type)
    {
        stop_music();
        data.music_type = type;
    }

    if(data.music_type == dmg_music_type::GBT_PLAYER)
    {
        gbt_play(song, speed);
        gbt_loop(loop);
    }
    else
    {
        BN_ASSERT(speed == 1, "Speed change not supported by the VGM player: ", speed);

        VgmPlay(static_cast<const uint8_t*>(song), loop);
    }

    data.music_paused = false;
}

void pause_music()
{
    if(data.music_type == dmg_music_type::GBT_PLAYER)
    {
        gbt_pause(0);
    }
    else
    {
        VgmPause();
    }

    data.music_paused = true;
}

void resume_music()
{
    if(data.music_type == dmg_music_type::GBT_PLAYER)
    {
        gbt_pause(1);
    }
    else
    {
        VgmResume();
    }

    data.music_paused = false;
}

void music_position(int& pattern, int& row)
{
    if(data.music_type == dmg_music_type::GBT_PLAYER)
    {
        gbt_get_position_unsafe(&pattern, &row, nullptr);
    }
    else
    {
        pattern = int(VgmGetOffsetPlay());
        row = 0;
    }
}

void set_music_position(int pattern, int row)
{
    if(data.music_type == dmg_music_type::GBT_PLAYER)
    {
        gbt_set_position(pattern, row);
    }
    else
    {
        BN_BASIC_ASSERT(! row, "Invalid row: ", row);

        VgmSetOffsetPlay(unsigned(pattern));
    }
}

void set_music_volume(int left_volume, int right_volume)
{
    if(data.music_type == dmg_music_type::GBT_PLAYER)
    {
        gbt_volume(unsigned(left_volume), unsigned(right_volume));
    }
    else
    {
        BN_ERROR("Volume change not supported by the VGM player");
    }
}

void commit()
{
    if(data.music_type == dmg_music_type::GBT_PLAYER)
    {
        gbt_update();
    }
    else
    {
        #if BN_CFG_ASSERT_ENABLED
            if(! data.vgm_commit_failed && ! VgmIntrVblank())
            {
                data.vgm_commit_failed = true;
                data.vgm_offset_play = VgmGetOffsetPlay();
            }
        #else
            VgmIntrVblank();
        #endif
    }
}

void check_commit_result()
{
    #if BN_CFG_ASSERT_ENABLED
        BN_BASIC_ASSERT(! data.vgm_commit_failed, "VGM commit failed: ", data.vgm_offset_play);
    #endif
}

}
