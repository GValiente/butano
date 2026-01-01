/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_dmg_audio_default.h"

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
    [[nodiscard]] int _hw_music_volume(fixed volume)
    {
        return fixed_t<3>(volume).data();
    }

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

    alignas(static_data) BN_DATA_EWRAM_BSS char data_buffer[sizeof(static_data)];

    [[nodiscard]] static_data& data_ref()
    {
        return *reinterpret_cast<static_data*>(data_buffer);
    }
}

void init()
{
    ::new(static_cast<void*>(data_buffer)) static_data();
}

void enable()
{
    REG_SNDDMGCNT = data_ref().control_value;
}

void disable()
{
    data_ref().control_value = REG_SNDDMGCNT;
    REG_SNDDMGCNT = 0;
}

[[nodiscard]] bool music_playing()
{
    static_data& data = data_ref();

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
    static_data& data = data_ref();

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
    static_data& data = data_ref();

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
    static_data& data = data_ref();

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
    static_data& data = data_ref();

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
    if(data_ref().music_type == dmg_music_type::GBT_PLAYER)
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
    if(data_ref().music_type == dmg_music_type::GBT_PLAYER)
    {
        gbt_set_position(pattern, row);
    }
    else
    {
        BN_BASIC_ASSERT(! row, "Invalid row: ", row);

        VgmSetOffsetPlay(unsigned(pattern));
    }
}

void set_music_volume(fixed left_volume, fixed right_volume)
{
    if(data_ref().music_type == dmg_music_type::GBT_PLAYER)
    {
        gbt_volume(unsigned(_hw_music_volume(left_volume)), unsigned(_hw_music_volume(right_volume)));
    }
    else
    {
        BN_ERROR("Volume change not supported by the VGM player");
    }
}

void commit()
{
    static_data& data = data_ref();

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
        BN_BASIC_ASSERT(! data_ref().vgm_commit_failed, "VGM commit failed: ", data_ref().vgm_offset_play);
    #endif
}

}
