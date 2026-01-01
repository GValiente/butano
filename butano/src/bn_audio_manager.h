/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AUDIO_MANAGER_H
#define BN_AUDIO_MANAGER_H

#include "bn_span.h"
#include "bn_fixed.h"
#include "bn_optional.h"
#include "bn_dmg_music_master_volume.h"

namespace bn
{
    class music_item;
    class sound_item;
    class dmg_music_item;
    class dmg_music_position;
    enum class audio_mixing_rate : uint8_t;
}

namespace bn::audio_manager
{
    void init();

    void enable();

    void disable();


    // music

    [[nodiscard]] bool music_playing();

    [[nodiscard]] optional<music_item> playing_music_item();

    void play_music(music_item item, fixed volume, bool loop);

    void stop_music();

    [[nodiscard]] bool music_paused();

    void pause_music();

    void resume_music();

    [[nodiscard]] int music_position();

    void set_music_position(int position);

    [[nodiscard]] fixed music_volume();

    void set_music_volume(fixed volume);

    [[nodiscard]] fixed music_tempo();

    void set_music_tempo(fixed tempo);

    [[nodiscard]] fixed music_pitch();

    void set_music_pitch(fixed pitch);


    // jingle

    [[nodiscard]] bool jingle_playing();

    [[nodiscard]] optional<music_item> playing_jingle_item();

    void play_jingle(music_item item, fixed volume);

    void stop_jingle();

    [[nodiscard]] bool jingle_paused();

    void pause_jingle();

    void resume_jingle();

    [[nodiscard]] fixed jingle_volume();

    void set_jingle_volume(fixed volume);


    // dmg_music

    [[nodiscard]] bool dmg_music_playing();

    [[nodiscard]] optional<dmg_music_item> playing_dmg_music_item();

    void play_dmg_music(const dmg_music_item& item, int speed, bool loop);

    void stop_dmg_music();

    [[nodiscard]] bool dmg_music_paused();

    void pause_dmg_music();

    void resume_dmg_music();

    [[nodiscard]] const dmg_music_position& dmg_music_position();

    void set_dmg_music_position(const bn::dmg_music_position& position);

    [[nodiscard]] fixed dmg_music_left_volume();

    [[nodiscard]] fixed dmg_music_right_volume();

    void set_dmg_music_left_volume(fixed left_volume);

    void set_dmg_music_right_volume(fixed right_volume);

    void set_dmg_music_volume(fixed left_volume, fixed right_volume);

    [[nodiscard]] bn::dmg_music_master_volume dmg_music_master_volume();

    void set_dmg_music_master_volume(bn::dmg_music_master_volume volume);


    // sound

    struct sound_data_type;

    [[nodiscard]] sound_data_type* sound_data(uint16_t handle);

    [[nodiscard]] uint16_t play_sound(int priority, sound_item item);

    [[nodiscard]] uint16_t play_sound(int priority, sound_item item, fixed volume, fixed speed, fixed panning);

    void stop_sound(uint16_t handle);

    void release_sound(uint16_t handle);

    [[nodiscard]] sound_item sound_item(uint16_t handle);

    [[nodiscard]] fixed sound_speed(uint16_t handle);

    void set_sound_speed(uint16_t handle, fixed speed);

    [[nodiscard]] fixed sound_panning(uint16_t handle);

    void set_sound_panning(uint16_t handle, fixed panning);

    void stop_all_sounds();

    [[nodiscard]] fixed sound_master_volume();

    void set_sound_master_volume(fixed volume);


    // other

    [[nodiscard]] span<const audio_mixing_rate> available_mixing_rates();

    [[nodiscard]] audio_mixing_rate mixing_rate();

    void set_mixing_rate(audio_mixing_rate mixing_rate);

    [[nodiscard]] bool update_on_vblank();

    void set_update_on_vblank(bool update_on_vblank);

    [[nodiscard]] bool event_handler_enabled();

    void set_event_handler_enabled(bool enabled);

    [[nodiscard]] span<uint8_t> event_ids();

    void update();

    void execute_commands();

    void vblank_commit();

    void delayed_commit();

    void stop();
}

#endif
