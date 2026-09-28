// SPDX-License-Identifier: ISC
//
// Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)
// Copyright (c) 2025, Antonio Niño Díaz

/****************************************************************************
 *                                                          __              *
 *                ____ ___  ____ __  ______ ___  ____  ____/ /              *
 *               / __ '__ \/ __ '/ |/ / __ '__ \/ __ \/ __  /               *
 *              / / / / / / /_/ />  </ / / / / / /_/ / /_/ /                *
 *             /_/ /_/ /_/\__,_/_/|_/_/ /_/ /_/\____/\__,_/                 *
 *                                                                          *
 *                           Common Definitions                             *
 *                                                                          *
 ****************************************************************************/

/// @file maxmod_common.h
///
/// @brief Global include of Maxmod for functions common to all platforms.

#ifndef MAXMOD_COMMON_H__
#define MAXMOD_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <mm_types.h>

// ***************************************************************************
/// @defgroup maxmod_soundbank_helpers Soundbank-related functions.
/// @{
// ***************************************************************************

#if !(defined(__NDS__) && defined(ARM7))
/// Get the ID of the sample with the provided name.
///
/// The name can be the name of one of the WAV files used to create the
/// soundbank, or the name of a sample of a module that starts with `#` (but the
/// `#` needs to be excluded from the name provided).
///
/// @note
///     The soundbank must include a name dictionary for this function to work.
///     Use option `-D` of `mmutil` to add a dictionary to the end file.
///
/// @warning
///     This isn't implemented in DS in the ARM7.
///
/// @param name
///     Name of the sample to search.
///
/// @return
///     Sample ID, or 0xFFFFFF if it isn't found.
mm_word mmGetSampleIdByName(const char *name);

/// Get the ID of the module with the provided name.
///
/// The name has to be the name of one of the module files used to create the
/// soundbank.
///
/// @note
///     The soundbank must include a name dictionary for this function to work.
///     Use option `-D` of `mmutil` to add a dictionary to the end file.
///
/// @warning
///     This isn't implemented in DS in the ARM7.
///
/// @param name
///     Name of the module to search.
///
/// @return
///     Module ID, or 0xFFFFFF if it isn't found.
mm_word mmGetModuleIdByName(const char *name);

/// Get the name of the sample with the provided ID.
///
/// The name can be the name of one of the WAV files used to create the
/// soundbank, or the name of a sample of a module that starts with `#` (without
/// the `#`).
///
/// The soundbank saves each individual sample of a module as a different
/// sample, but only the ones with a `#` at the beginning of the name are saved.
/// All other samples are saved without a name.
///
/// @note
///     The soundbank must include a name dictionary for this function to work.
///     Use option `-D` of `mmutil` to add a dictionary to the end file.
///
/// @warning
///     This isn't implemented in DS in the ARM7.
///
/// @param id
///     Sample ID.
///
/// @return
///     Sample name, or NULL it isn't found or there is no defined name.
const char *mmGetSampleNameById(mm_word id);

/// Get the name of the module with the provided ID.
///
/// The name is one of the module files used to create the soundbank.
///
/// @note
///     The soundbank must include a name dictionary for this function to work.
///     Use option `-D` of `mmutil` to add a dictionary to the end file.
///
/// @warning
///     This isn't implemented in DS in the ARM7.
///
/// @param id
///     Module ID.
///
/// @return
///     Module name, or NULL it isn't found or there is no defined name.
const char *mmGetModuleNameById(mm_word id);
#endif // !(defined(__NDS__) && defined(ARM7))

// ***************************************************************************
/// @}
/// @defgroup maxmod_module_playback Module Playback
/// @{
// ***************************************************************************

/// Begins playback of a module.
///
/// For DS, the module must be loaded into memory first (mmLoad).
///
/// @warning
///     In DS, make sure the module is loaded with mmLoad() first.
///
/// @param module_ID
///     Index of module to be played. Values are defined in the soundbank header
///     output. (prefixed with "MOD_")
/// @param mode
///     Mode of playback. Can be MM_PLAY_LOOP (play and loop until stopped
///     manually) or MM_PLAY_ONCE (play until end).
void mmStart(mm_word module_ID, mm_pmode mode);

/// Pauses playback of the active module.
///
/// Resume with mmResume().
///
/// @note
///     For DS users: The DS hardware channels do not allow actual pausing. To
///     implement pausing, Maxmod sets the channel frequencies to the minimal
///     value (256Hz).  This may cause some problems with certain samples (such
///     as drum-loops) drifting out of sync with the song temporarily. This is
///     not an issue with the interpolated audio mode since the channels are fed
///     by software.
void mmPause(void);

/// Resume module playback.
///
/// Pause with mmPause().
void mmResume(void);

/// Stops playback of the active module.
///
/// To start again (from the beginning) use mmStart().
///
/// Any channels used by the active module will be freed.
void mmStop(void);

#if !(defined(__NDS__) && defined(ARM9))
/// Get current number of elapsed ticks in the row being played.
///
/// @warning
///     This isn't supported on NDS in the ARM9.
///
/// @return
///     Number of elapsed ticks.
mm_word mmGetPositionTick(void);
#endif

/// Get current row being played.
///
/// @return
///     The current row.
mm_word mmGetPositionRow(void);

/// Get current pattern order being played.
///
/// @return
///     The current pattern.
mm_word mmGetPosition(void);

/// Set the current playback position.
///
/// It sets the sequence [aka order-list] position for the active module and the
/// row inside the pattern.
///
/// @param position
///     New position in module sequence.
/// @param row
///     New row in the destination pattern
void mmSetPositionEx(mm_word position, mm_word row);

/// Set the current sequence [aka order-list] position for the active module.
///
/// @param position
///     New position in module sequence.
static inline void mmSetPosition(mm_word position)
{
    mmSetPositionEx(position, 0);
}

/// Set playback position.
///
/// @deprecated
///     Alias of mmSetPosition().
///
/// @param position
///     New position in module sequence.
__attribute__((deprecated))
static inline void mmPosition(mm_word position)
{
    mmSetPositionEx(position, 0);
}

/// Used to determine if a module is playing.
///
/// @return
///     Nonzero if a module is currently playing.
mm_bool mmActive(void);

/// Use this function to change the master volume scale for module playback.
///
/// @param volume
///     New volume level. Ranges from 0 (silent) to 1024 (normal).
void mmSetModuleVolume(mm_word volume);

/// Change the master tempo for module playback.
///
/// Specifying 1024 will play the module at its normal speed. Minimum and
/// maximum values are 50% (512) and 200% (2048). Note that increasing the tempo
/// will also increase the module processing load.
///
/// It uses a fixed point (Q10) value representing tempo.
///
/// Range = 0x200 -> 0x800 = 0.5 -> 2.0
///
/// @param tempo
///     New tempo value. Tempo = (speed_percentage * 1024) / 100.
void mmSetModuleTempo(mm_word tempo);

/// Change the master pitch scale for module playback.
///
/// Specifying 1024 will play the module at its normal pitch. Minimum/Maximum
/// range of the pitch change is +-1 octave.
///
/// Range = 0x200 -> 0x800 = 0.5 -> 2.0
///
/// @param pitch
///     New pitch scale. Value = 1024 * 2^(semitones/12)
void mmSetModulePitch(mm_word pitch);

#if !(defined(__NDS__) && defined(ARM9))
/// Play individual MAS file from RAM.
///
/// @deprecated
///     This function expects the user to pass a pointer to the MAS file
///     skipping the MAS file prefix, which isn't very intuitive. Use
///     mmPlayMAS() instead.
///
/// @warning
///     You need to initialize Maxmod with mmInit() and provide it a valid
///     soundbank even if you plan to use mmPlayModule() to play everything.
///
/// @param address
///     Address of the MAS file, skipping the first few bytes of the prefix.
///     Add `sizeof(mm_mas_prefix)` to the pointer to your MAS file.
/// @param mode
///     Playback mode: MM_PLAY_ONCE or MM_PLAY_LOOP.
/// @param layer
///     MM_MAIN (main module layer) or MM_JINGLE (sub/jingle layer).
__attribute__((deprecated))
void mmPlayModule(uintptr_t address, mm_word mode, mm_word layer);
#endif

/// Play individual MAS file from RAM.
///
/// A soundbank is a MSL file that contains one or more MAS files. Each MAS file
/// can contain a sample or a module. This function allows you to play samples
/// or modules without the need for a soundbank.
///
/// Normally, Maxmod plays MAS files from the sound bank provided to mmInit().
/// This function lets you play MAS files outside of that soundbank.
///
/// @warning
///     You need to initialize Maxmod with a valid soundbank, or with
///     mmInitNoSoundbank() if you don't plan on using any soundbank at all.
///
/// @param address
///     Address of the MAS file.
/// @param mode
///     Playback mode: MM_PLAY_ONCE or MM_PLAY_LOOP.
/// @param layer
///     MM_MAIN (main module layer) or MM_JINGLE (sub/jingle layer).
void mmPlayMAS(uintptr_t address, mm_word mode, mm_word layer);

// ***************************************************************************
/// @}
/// @defgroup maxmod_jingle_playback Jingle Playback
/// @{
// ***************************************************************************

/// Plays a jingle.
///
/// Jingles are normal modules that can be mixed with the normal module
/// playback.
///
/// For GBA, the module is read directly from the cartridge space.
///
/// Note that jingles must be limited to 4 channels only.
///
/// @warning
///     In DS, make sure the module is loaded with mmLoad() first.
///
/// @param module_ID
///     Index of module to be played. (Defined in soundbank header)
/// @param mode
///     Mode of playback. Can be MM_PLAY_LOOP (play and loop until stopped
///     manually) or MM_PLAY_ONCE (play until end).
void mmJingleStart(mm_word module_ID, mm_pmode mode);

/// Plays a jingle.
///
/// @deprecated
///     Use mmJingleStart() instead.
///
/// @param module_ID
///     Index of module to be played. (Defined in soundbank header)
__attribute__((deprecated))
static inline void mmJingle(mm_word module_ID)
{
    mmJingleStart(module_ID, MM_PLAY_ONCE);
}

/// Pauses playback of the active jingle.
///
/// Resume with mmJingleResume().
void mmJinglePause(void);

/// Resume jingle playback.
///
/// Pause with mmJinglePause().
void mmJingleResume(void);

/// Stops playback of the active jingle.
///
/// Start again (from the beginning) with mmJingleStart().
///
/// Any channels used by the active module will be freed.
void mmJingleStop(void);

/// Check if a jingle is playing or not.
///
/// @return
///     Returns nonzero if a jingle is actively playing.
mm_bool mmJingleActive(void);

/// Check if a jingle is playing or not.
///
/// @deprecated
///     Alias of mmJingleActive().
///
/// @return
///     Returns nonzero if a jingle is actively playing.
__attribute__ ((deprecated))
static inline mm_bool mmActiveSub(void)
{
    return mmJingleActive();
}

/// Use this function to change the master volume scale for jingle playback.
///
/// @param volume
///     New volume level. Ranges from 0 (silent) to 1024 (normal).
void mmSetJingleVolume(mm_word volume);

/// Change the master tempo for jingle playback.
///
/// Specifying 1024 will play the jingle at its normal speed. Minimum and
/// maximum values are 50% (512) and 200% (2048). Note that increasing the tempo
/// will also increase the jingle processing load.
///
/// It uses a fixed point (Q10) value representing tempo.
///
/// Range = 0x200 -> 0x800 = 0.5 -> 2.0
///
/// @param tempo
///     New tempo value. Tempo = (speed_percentage * 1024) / 100.
void mmSetJingleTempo(mm_word tempo);

/// Change the master pitch scale for jingle playback.
///
/// Specifying 1024 will play the jingle at its normal pitch. Minimum/Maximum
/// range of the pitch change is +-1 octave.
///
/// Range = 0x200 -> 0x800 = 0.5 -> 2.0
///
/// @param pitch
///     New pitch scale. Value = 1024 * 2^(semitones/12)
void mmSetJinglePitch(mm_word pitch);

// ***************************************************************************
/// @}
/// @defgroup maxmod_sound_effects Sound Effects
/// @{
// ***************************************************************************

/// Plays a sound effect with default settings.
///
/// Default settings are: Volume=Max, Panning=Center, Rate=Center (specified in
/// sample).
///
/// The value returned from this function is a handle and can be used to modify
/// the sound effect while it's actively playing.
///
/// @warning
///     In DS, make sure the sample is loaded with mmLoadEffect() first.
///
/// @param sample_ID
///     Index of sample to be played. Values are defined in the soundbank
///     header. (prefixed with "SFX_")
///
/// @return
///     On success, sound effect handle that can be used to modify parameters of
///     the sound effect while it is playing. On error, MM_SFXHAND_INVALID.
mm_sfxhand mmEffect(mm_word sample_ID);

/// Plays a sound effect with custom settings.
///
/// @warning
///     In DS, make sure the sample is loaded with mmLoadEffect() first.
///
/// @param sound
///     Structure containing information about the sound to be played.
///
/// @return
///     On success, sound effect handle that can be used to modify parameters of
///     the sound effect while it is playing. On error, MM_SFXHAND_INVALID.
mm_sfxhand mmEffectEx(mm_sound_effect* sound);

#ifndef __NDS__
/// Indicates if a sound effect is active or not.
///
/// @warning
///     Function not available on NDS.
///
/// @param handle
///     Sound effect handle received from mmEffect() or mmEffectEx().
///
/// @return
///     Non-zero if the sound effect is active, zero if it isn't.
mm_bool mmEffectActive(mm_sfxhand handle);
#endif // __NDS__

/// Changes the volume of a sound effect.
///
/// @param handle
///     Sound effect handle received from mmEffect() or mmEffectEx().
/// @param volume
///     New volume level. Ranges from 0 (silent) to 255 (normal).
void mmEffectVolume(mm_sfxhand handle, mm_word volume);

/// Changes the panning of a sound effect.
///
/// @param handle
///     Sound effect handle received from mmEffect() or mmEffectEx().
/// @param panning
///     New panning level. Ranges from 0 (left) to 255 (right).
void mmEffectPanning(mm_sfxhand handle, mm_byte panning);

/// Changes the playback rate for a sound effect.
///
/// The actual playback rate depends on this value and the base frequency of the
/// sample. This parameter is a 6.10 fixed point value, passing 1024 will return
/// the sound to its original pitch, 2048 will raise the pitch by one octave,
/// and 512 will lower the pitch by an octave. To calculate a value from
/// semitones: Rate = 1024 * 2^(Semitones/12). (please don't try to do that with
/// integer maths)
///
/// @param handle
///     Sound effect handle received from mmEffect() or mmEffectEx().
/// @param rate
///     New playback rate.
void mmEffectRate(mm_sfxhand handle, mm_word rate);

/// Scales the rate of the sound effect by a certain factor.
///
/// @param handle
///     Sound effect handle received from mmEffect() or mmEffectEx().
/// @param factor
///     6.10 fixed point factor.
void mmEffectScaleRate(mm_sfxhand handle, mm_word factor);

/// Stops a sound effect. The handle will be invalidated.
///
/// @note
///     It can't stop released effects because their handles are invalid.
///
/// @warning
///     In NDS, this function returns `void`.
///
/// @param handle
///     Sound effect handle received from mmEffect() or mmEffectEx().
///
/// @return
///     Non-zero if the sound was found and stopped, zero on error.
#if defined(__NDS__) && defined(ARM9)
void mmEffectCancel(mm_sfxhand handle);
#else
mm_word mmEffectCancel(mm_sfxhand handle);
#endif

/// Marks a sound effect as unimportant.
///
/// This enables the sound effect to be interrupted by music/other sound effects
/// if the need arises. The handle will be invalidated.
///
/// @warning
///     A released effect can't be stopped with mmEffectCancel() because the
///     handle is invalid. It can be cancelled with mmEffectCancelAll().
///
/// @param handle
///     Sound effect handle received from mmEffect() or mmEffectEx().
void mmEffectRelease(mm_sfxhand handle);

/// Set master volume scale for effect playback.
///
/// @param volume
///     Master volume. 0->1024 representing 0%->100% volume
void mmSetEffectsVolume(mm_word volume);

/// Stop all sound effects and reset the effect system.
///
/// It stops even released effects.
void mmEffectCancelAll(void);

// ***************************************************************************
/// @}
// ***************************************************************************

#ifdef __cplusplus
}
#endif

#endif // MAXMOD_COMMON_H__
