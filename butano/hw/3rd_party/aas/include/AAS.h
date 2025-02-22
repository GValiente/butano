// Copyright (c) 2003-2021 James Daniels
// Distributed under the MIT License
// license terms: see LICENSE file in root or http://opensource.org/licenses/MIT

// Main AAS include
//
// See API documentation for more information.

#ifndef __AAS__
#define __AAS__

#ifdef __cplusplus
#define AAS_BEGIN_DECLS extern "C" {
#define AAS_END_DECLS }
#else
#define AAS_BEGIN_DECLS
#define AAS_END_DECLS
#endif

AAS_BEGIN_DECLS

#define AAS_VERSION 0x113  // v1.13

// Types
#define AAS_u32 unsigned int
#define AAS_s32 signed int
#define AAS_u16 unsigned short
#define AAS_s16 signed short
#define AAS_u8 unsigned char
#define AAS_s8 signed char
#define AAS_BOOL unsigned char
#define AAS_TRUE 1
#define AAS_FALSE 0
#define AAS_NULL 0

// Return values
#define AAS_OK                             0
#define AAS_ERROR_VOLUME_OUT_OF_RANGE     -1
#define AAS_ERROR_CHANNEL_NOT_AVAILABLE   -2
#define AAS_ERROR_FREQUENCY_OUT_OF_RANGE  -3
#define AAS_ERROR_MOD_DOES_NOT_EXIST      -4
#define AAS_ERROR_CALL_SET_CONFIG_FIRST   -5
#define AAS_ERROR_INVALID_CONFIG          -6
#define AAS_ERROR_INVALID_SAMPLE_ADDRESS  -7
#define AAS_ERROR_NO_MOD_PLAYING          -8
#define AAS_ERROR_NOT_ENOUGH_CHANNELS     -9
#define AAS_ERROR_CHANNEL_ACTIVE          -10
#define AAS_ERROR_CHANNEL_UNRESUMEABLE    -11
#define AAS_ERROR_INVALID_SONG_POS        -12

// AAS_SetConfig() mix settings
#define AAS_CONFIG_MIX_32KHZ       1
#define AAS_CONFIG_MIX_28KHZ       2
#define AAS_CONFIG_MIX_24KHZ       3
#define AAS_CONFIG_MIX_20KHZ       4
#define AAS_CONFIG_MIX_16KHZ       5
#define AAS_CONFIG_MIX_12KHZ       6
#define AAS_CONFIG_MIX_8KHZ        7

// AAS_SetConfig() channel settings
#define AAS_CONFIG_CHANS_16_LOUD   6
#define AAS_CONFIG_CHANS_8_LOUD    5
#define AAS_CONFIG_CHANS_4_LOUD    4
#define AAS_CONFIG_CHANS_16        3
#define AAS_CONFIG_CHANS_8         2
#define AAS_CONFIG_CHANS_4         1

// AAS_SetConfig() spatial settings
#define AAS_CONFIG_SPATIAL_STEREO  2
#define AAS_CONFIG_SPATIAL_MONO    1

// AAS_SetConfig() dynamic mixing settings
#define AAS_CONFIG_DYNAMIC_OFF     0
#define AAS_CONFIG_DYNAMIC_ON      1

// General commands
int AAS_SetConfig( int config_mix, int config_chans, int config_spatial, int config_dynamic );  // Must call at least once before doing anything else
void AAS_DoDMA3( void* source, void* dest, AAS_u32 flags_and_length );
void AAS_ShowLogo();

// Interrupt handling commands
void AAS_Timer1InterruptHandler(); // Use when there are no other CPU-intensive interrupts
void AAS_FastTimer1InterruptHandler(); // Use when there are other CPU-intensive interrupts
void AAS_DoWork(); // Must be called at least 50 times/sec if using AAS_FastTimer1InterruptHandler()

// Sample playing commands
int AAS_SFX_Play( int channel, int sample_volume, int sample_frequency, const AAS_s8* sample_start, const AAS_s8* sample_end, const AAS_s8* sample_restart );
AAS_BOOL AAS_SFX_ChannelExists( int channel ); // returns AAS_TRUE only if AAS_SFX_Play will succeed for this channel
AAS_BOOL AAS_SFX_IsActive( int channel ); // returns AAS_TRUE if channel is valid and active, AAS_FALSE otherwise
int AAS_SFX_EndLoop( int channel ); // If sample was looping, will stop at end of current iteration
int AAS_SFX_SetFrequency( int channel, int sample_frequency );
int AAS_SFX_SetVolume( int channel, int sample_volume );
int AAS_SFX_Stop( int channel );
int AAS_SFX_Resume( int channel );
int AAS_SFX_GetNumChannels(); // returns number of SFX channels

// MOD commands
int AAS_MOD_Play( int song_num ); // loops by default
int AAS_MOD_SetLoop( AAS_BOOL loop ); // specify whether current song will loop
void AAS_MOD_Stop();
AAS_BOOL AAS_MOD_IsPlaying(); // is a song playing?
AAS_BOOL AAS_MOD_HasLooped(); // has the current song looped?
int AAS_MOD_GetVolume(); // 0: silent, 256 = max
int AAS_MOD_SetVolume( int vol ); // 0: silent, 256 = max (do not set above 256 or below 0!)
int AAS_MOD_GetSongPos();
int AAS_MOD_SetSongPos( int song_pos ); // Immediately jumps to the specified song position.
int AAS_MOD_QueueSongPos( int song_pos ); // Jumps to the specified song position when the current pattern finishes.
int AAS_MOD_GetLineNum();
int AAS_MOD_GetLastFilterValue(); // Returns the value specified by the most recent "E0: Set Filter" effect
void AAS_MOD_Pause(); // Stops the MOD in a way that allows it to be safely resumed
void AAS_MOD_Resume(); // Should only be used after AAS_MOD_Pause()
int AAS_MOD_GetNumChannels(); // Returns number of channels currently being reserved by the MOD

// Misc commands
const AAS_s8* AAS_GetOutputBufferAddress( int buffer ); // buffer should be 0 or 1, otherwise will return AAS_NULL
int AAS_GetOutputBufferLength();
int AAS_GetActualMixRate();

AAS_END_DECLS

#endif
