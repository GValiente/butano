// SPDX-FileCopyrightText: Copyright 2023-2025 akkera102
// SPDX-FileCopyrightText: Copyright 2026 copyrat90
// SPDX-License-Identifier: 0BSD

#ifndef ADVGM_H
#define ADVGM_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum advgm_master_volume_t
{
    ADVGM_MASTER_VOLUME_QUARTER, // 25%
    ADVGM_MASTER_VOLUME_HALF,    // 50%
    ADVGM_MASTER_VOLUME_FULL,    // 100%
} advgm_master_volume;

/// @brief Sets the DMG master volume of the GBA APU.
///
/// This is set to 25% by default, so typically,
/// you'll want to call this with `ADVGM_MASTER_VOLUME_FULL` on startup.
///
/// @param master_volume Desired master volume.
void advgm_set_master_volume(advgm_master_volume master_volume);

/// @brief Gets the DMG master volume of the GBA APU.
/// @return DMG master volume of the GBA APU.
advgm_master_volume advgm_get_master_volume(void);

/// @brief Updates the advgm playback.
/// This should be called once for each vblank interrupt.
///
/// @return Whether the music command was properly processed or not.
/// If this ever returns `false`, it's either you set the invalid music offset, or a bug in the advgm itself.
bool advgm_vblank_callback(void);

/// @brief Starts playing the provided music.
/// @param music Pointer to a music.
/// @param loop Whether the loop is enabled or not.
void advgm_play(const uint8_t* music, bool loop);

/// @brief Stops the playing music.
void advgm_stop(void);

/// @brief Pauses the playing music.
/// @note If a music was not playing, this is no-op.
void advgm_pause(void);

/// @brief Resumes the paused music.
/// @note If the music was not paused, this is no-op.
void advgm_resume(void);

/// @brief Indicates if the music is playing or not.
/// @note Paused state is considered as playing, so it returns `true`.
bool advgm_playing(void);

/// @brief Indicates if the music is paused or not.
bool advgm_paused(void);

/// @brief Gets the pointer to the currently active (playing/paused) music.
/// @return Pointer to the currently active music, or `NULL` if no music was active.
const uint8_t* advgm_get_active_music(void);

/// @brief Gets the data offset of the playing/paused music.
/// @return Data offset of the music, or `0` if the music was not playing or paused.
uint32_t advgm_get_music_offset(void);

/// @brief Sets the data offset of the playing/paused music.
/// @param offset Data offset of the music.
/// @note You should use a valid offset previously obtained from `advgm_get_music_offset()` or `0`,
/// otherwise the behavior is undefined.
///
/// Also note that it can still sound weird when jumping around valid offsets,
/// as you might be skipping vital audio commands like stop vibrato, cut channel, etc.
void advgm_set_music_offset(uint32_t offset);

#ifdef __cplusplus
}
#endif

#endif // ADVGM_H
