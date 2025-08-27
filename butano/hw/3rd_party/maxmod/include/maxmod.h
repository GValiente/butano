// SPDX-License-Identifier: ISC
//
// Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)
// Copyright (c) 2025, Antonio Niño Díaz (antonio_nd@outlook.com)

/****************************************************************************
 *                                                          __              *
 *                ____ ___  ____ __  ______ ___  ____  ____/ /              *
 *               / __ `__ \/ __ `/ |/ / __ `__ \/ __ \/ __  /               *
 *              / / / / / / /_/ />  </ / / / / / /_/ / /_/ /                *
 *             /_/ /_/ /_/\__,_/_/|_/_/ /_/ /_/\____/\__,_/                 *
 *                                                                          *
 *                             GBA Definitions                              *
 *                                                                          *
 ****************************************************************************/

/// @file maxmod.h
///
/// @brief Global include of Maxmod for GBA.

#ifndef MAXMOD_H__
#define MAXMOD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <mm_types.h>

// ***************************************************************************
/// @defgroup gba_init GBA: Initialization/Main Functions
/// @{
// ***************************************************************************

/// Precalculated mix buffer lengths (in bytes)
typedef enum
{
    MM_MIXLEN_8KHZ  = 544,  ///< (8121 hz)
    MM_MIXLEN_10KHZ = 704,  ///< (10512 hz)
    MM_MIXLEN_13KHZ = 896,  ///< (13379 hz)
    MM_MIXLEN_16KHZ = 1056, ///< (15768 hz)
    MM_MIXLEN_18KHZ = 1216, ///< (18157 hz)
    MM_MIXLEN_21KHZ = 1408, ///< (21024 hz)
    MM_MIXLEN_27KHZ = 1792, ///< (26758 hz)
    MM_MIXLEN_31KHZ = 2112, ///< (31536 hz)
} mm_mixlen_enum;

// measurements of channel types (bytes)
#define MM_SIZEOF_MODCH     40
#define MM_SIZEOF_ACTCH     28
#define MM_SIZEOF_MIXCH     (12 + sizeof(uintptr_t))

/// Initialize Maxmod with default settings.
///
/// For GBA, this function uses these default settings (and allocates memory):
/// 16KHz mixing rate, channel buffers in EWRAM, wave buffer in EWRAM, and
/// mixing buffer in IWRAM. It also links the VBlank interrupt to mmVBlank with
/// the libgba interrupt handler.
///
/// @param soundbank
///     Memory address of soundbank (in ROM). A soundbank file can be created
///     with the Maxmod Utility.
/// @param number_of_channels
///     Number of module/mixing channels to allocate. Must be greater or equal
///     to the channel count in your modules. The maximum value allowed is 32.
///
/// @return
///     It returns true on success, false on error.
bool mmInitDefault(mm_addr soundbank, mm_word number_of_channels);

/// Initializes Maxmod with the settings specified.
///
/// Initialize system. Call once at startup.
///
/// For GBA projects, irqInit() should be called before this function.
///
/// Example:
///
/// ```c
/// // Mixing buffer (globals are usually placed in IWRAM by your toolchain).
/// // If it isn't placed in IWRAM the CPU load will _drastially_ increase due
/// // to the slower memory accesses.
/// u8 myMixingBuffer[MM_MIXLEN_16KHZ] __attribute__((aligned(4)));
///
/// void maxmodInit(void)
/// {
///     irqInit();
///     irqSet(IRQ_VBLANK, mmVBlank);
///     irqEnable(IRQ_VBLANK);
///
///     // Allocate data for channel buffers & wave buffer (memory returned by
///     // malloc is in EWRAM). Use the SIZEOF definitions to calculate how many
///     // bytes to reserve
///     u8 *myData = malloc((8 * (MM_SIZEOF_MODCH + MM_SIZEOF_ACTCH + MM_SIZEOF_MIXCH))
///                         + MM_MIXLEN_16KHZ);
///
///     // Setup system information
///     mm_gba_system mySystem =
///     {
///         // 16 KHz software mixing rate, select from the mm_mixmode enum
///         .mixing_mode       = MM_MIX_16KHZ;
///
///         // Number of module/mixing channels. Higher numbers offer better
///         // polyphony at the expense of more memory and/or CPU usage.
///         // The maximum number of channels supported is 32.
///         .mod_channel_count = 8; // Max number of channels for modules only
///         .mix_channel_count = 8; // Max number of channels for modules and effects
///
///         // Assign memory blocks to pointers
///         .module_channels = (mm_addr)(myData + 0);
///         .active_channels = (mm_addr)(myData + (8 * MM_SIZEOF_MODCH));
///         .mixing_channels = (mm_addr)(myData + (8 * (MM_SIZEOF_MODCH + MM_SIZEOF_ACTCH)));
///
///         .mixing_memory   = (mm_addr)myMixingBuffer;
///         .wave_memory     = (mm_addr)(myData + (8 * (MM_SIZEOF_MODCH + MM_SIZEOF_ACTCH
///                                                     + MM_SIZEOF_MIXCH)));
///
///         // Soundbank address in ROM/RAM
///         .soundbank         = (mm_addr)soundbank;
///     };
///
///     // Initialize Maxmod
///     mmInit(&mySystem);
/// }
/// ```
///
/// @param setup
///     Maxmod setup configuration.
///
/// @return
///     It returns true on success, false on error.
bool mmInit(mm_gba_system* setup);

/// Deinitializes Maxmod.
///
/// If Maxmod was initialized with mmInitDefault(), it also frees the memory
/// allocated by it.
///
/// If Maxmod was initialized with mmInit(), the user is responsible for freeing
/// the memory passed to it when Maxmod was initialized.
///
/// @return
///     It returns true on success, false on error.
bool mmEnd(void);

/// This function must be linked directly to the VBlank IRQ.
///
/// During this function, the sound DMA is reset. The timing is extremely
/// critical, so make sure that it is not interrupted, otherwise garbage may be
/// heard in the output.
///
/// If you need another function to execute after this process is finished, use
/// mmVBlankReturn() to install your handler.
///
/// Example setup with libgba system:
/// ```c
/// void setup_interrupts(void)
/// {
///     irqInit();
///     irqSet(IRQ_VBLANK, mmVBlank);
///     irqEnable(IRQ_VBLANK);
///
///     mmVBlankReturn(myVBlankHandler); // This is optional
/// }
/// ```
void mmVBlank(void);

/// Installs a custom handler to be processed after the sound DMA is reset.
///
/// If you need to have a function linked to the VBlank interrupt, use this
/// function (the actual VBlank interrupt must be linked directly to mmVBlank).
///
/// @param function
///     Pointer to your VBlank handler.
void mmSetVBlankHandler(mm_voidfunc function);

/// Install handler to receive song events.
///
/// Use this function to receive song events. Song events occur in two
/// situations. One is by special pattern data in a module (which is triggered
/// by SFx/EFx commands). The other occurs when a module finishes playback (in
/// MM_PLAY_ONCE mode).
///
/// Note for GBA projects: During the song event, Maxmod is in the middle of
/// module processing. Avoid using any Maxmod related functions during your song
/// event handler since they may cause problems in this situation.
///
/// Check the song events tutorial in the documentation for more information.
///
/// @param handler
///     Function pointer to event handler.
void mmSetEventHandler(mm_callback handler);

/// This is the main routine-function that processes music and updates the sound
/// output.
///
/// For GBA, this function must be called every frame. If a call is missed,
/// garbage will be heard in the output and module processing will be delayed.
void mmFrame(void) __attribute((long_call));

/// Returns the number of modules available in the soundbank.
///
/// @return
///     The number of modules.
mm_word mmGetModuleCount(void);

/// Returns the number of samples available in the soundbank.
///
/// @note
///     This number includes the samples used by all the songs in the soundbank,
///     not just the sound effects in WAV format.
///
/// @return
///     The number of samples.
mm_word mmGetSampleCount(void);

// ***************************************************************************
/// @}
/// @defgroup gba_module_playback GBA: Module Playback
/// @{
// ***************************************************************************

/// Begins playback of a module.
///
/// For GBA, the module data is read directly from the cartridge space, so no
/// loading is needed.
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
void mmPause(void);

/// Resume module playback.
///
/// Pause with mmPause().
void mmResume(void);

/// Stops playback of the active module.
///
/// Start again (from the beginning) with mmStart().
///
/// Any channels used by the active module will be freed.
void mmStop(void);

/// Get current number of elapsed ticks in the row being played.
///
/// @return
///     Number of elapsed ticks.
mm_word mmGetPositionTick(void);

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
///     You can initialize mmInit() passing 0 as the number of modules and sound
///     effects.
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
/// @defgroup gba_jingle_playback GBA: Jingle Playback
/// @{
// ***************************************************************************

/// Plays a jingle.
///
/// Jingles are normal modules that can be mixed with the normal module
/// playback.
///
/// For GBA, the module is read directly from the cartridge space. For jingles,
/// the playback mode is fixed to MM_PLAY_ONCE.
///
/// Note that jingles must be limited to 4 channels only.
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

// ***************************************************************************
/// @}
/// @defgroup gba_sound_effects GBA: Sound Effects
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
/// @param sample_ID
///     Index of sample to be played. Values are defined in the soundbank
///     header. (prefixed with "SFX_")
///
/// @return
///     Sound effect handle. This value can be used to modify parameters of the
///     sound effect while it is playing.
mm_sfxhand mmEffect(mm_word sample_ID);

/// Plays a sound effect with custom settings.
///
/// @param sound
///     Structure containing information about the sound to be played.
///
/// @return
///     Sound effect handle that may be used to modify the sound later.
mm_sfxhand mmEffectEx(mm_sound_effect* sound);

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

/// Indicates if a sound effect is active or not.
///
/// @param handle
///     Sound effect handle received from mmEffect() or mmEffectEx().
///
/// @return
///     Non-zero if the sound effect is active, zero if it isn't.
mm_bool mmEffectActive(mm_sfxhand handle);

/// Stops a sound effect. The handle will be invalidated.
///
/// @note
///     It can't stop released effects because their handles are invalid.
///
/// @param handle
///     Sound effect handle received from mmEffect() or mmEffectEx().
///
/// @return
///     Non-zero if the sound was found and stopped, zero on error.
mm_word mmEffectCancel(mm_sfxhand handle);

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

#endif // MAXMOD_H__
