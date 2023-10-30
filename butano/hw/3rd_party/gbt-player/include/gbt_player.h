// GBT Player v4.4.1
//
// SPDX-License-Identifier: MIT
//
// Copyright (c) 2009-2022 Antonio Niño Díaz <antonio_nd@outlook.com>

#ifndef GBT_PLAYER_H__
#define GBT_PLAYER_H__

// Song playback
// -------------

// Starts playing a song. The speed is only used for MOD songs. S3M songs
// include their own startup speed, so this value is ignored. If the speed is 0,
// it acts as if it was 256.
//
// Note: This function sets the PSG global volume to the max.
//
// Note: This function won't silence any channel that is disabled in GBT Player
// (any sound effect played by the user will keep playing even when the song
// starts).
void gbt_play(const void *song, int speed);

// Pauses or unpauses the song. 0 pauses the song, anything else unpauses it.
void gbt_pause(int play);

// Enables/disables looping at the end of the song. 0 stops playback when the
// end of the song is reached. Anything else will loop to the beginning.
void gbt_loop(int loop);

// Stops the song.
void gbt_stop(void);

// Updates the player, must be called each VBL.
void gbt_update(void);

// Returns 1 if a song is playing, 0 otherwise.
int gbt_is_playing(void);

// Song control
// ------------

#define GBT_VOLUME_MAX      8

// Sets volume for left and right speakers. Valid values are 0 to 8.
void gbt_volume(unsigned int left, unsigned int right);

// Defines to specify which channels to enable
#define GBT_ENABLE_CH1      (1 << 0)
#define GBT_ENABLE_CH2      (1 << 1)
#define GBT_ENABLE_CH3      (1 << 2)
#define GBT_ENABLE_CH4      (1 << 3)

#define GBT_ENABLE_CH_ALL   (GBT_ENABLE_CH1 | GBT_ENABLE_CH2 | \
                             GBT_ENABLE_CH3 | GBT_ENABLE_CH4)

// Enables given channels. For example: GBT_ENABLE_CH1 | GBT_ENABLE_CH2.
// When a channel is disabled it keeps playing whatever it was playing before,
// so it needs to be silenced manually after calling this function. The setting
// persists across different gbt_play() calls.
void gbt_enable_channels(int flags);

// Returns the current pattern order and current row inside the pattern, as well
// as the current tick. It is possible to pass NULL to any argument if you don't
// need that value.
void gbt_get_position(int *order, int *row, int *tick);

// Returns the current pattern order and current row inside the pattern, as well
// as the current tick, ignoring if the music is paused or not playing.
// It is possible to pass NULL to any argument if you don't need that value.
void gbt_get_position_unsafe(int *order, int *row, int *tick);

// Sets the current position in the song. This isn't normally meant to happen,
// so some effects (like arpeggio) may be broken right after the jump until they
// are restarted.
void gbt_set_position(int order, int row);

// Event callback system
// ---------------------

typedef void (*gbt_event_callback)(int event, int order, int row);

#define GBT_EVENT_END       -1

// This function sets the callback for song events (such as when the song ends)
// as well as custom user events, that are added to the S3M file by using effect
// SFx.
//
// The function "callback" gets an event identifier, as well as the pattern
// order and row in which it happens.
//
// - Song end:      event = GBT_EVENT_END | order = -1 | row = -1
// - SFx command:   event = x             | order and row = position in song
//
// Note: The callback can be disabled by calling this function with NULL as
// argument. The callback is also disabled whenever gbt_play() is called, so it
// has to be set whenever a new song starts.
void gbt_set_event_callback_handler(gbt_event_callback callback);

#endif // GBT_PLAYER_H__
