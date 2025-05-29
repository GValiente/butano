// SPDX-License-Identifier: ISC
//
// Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)

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

#ifndef MAXMOD_H
#define MAXMOD_H

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
#define MM_SIZEOF_MIXCH     16

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
///     to the channel count in your modules.
void mmInitDefault(mm_addr soundbank, mm_word number_of_channels);

/// Initializes Maxmod with the settings specified.
///
/// Initialize system. Call once at startup.
///
/// For GBA projects, irqInit() should be called before this function.
///
/// @param setup
///     Maxmod setup configuration.
///
/// Example:
///
/// ```c
/// // Mixing buffer (globals should go in IWRAM)
/// // Mixing buffer SHOULD be in IWRAM, otherwise the CPU load will
/// // _drastially_ increase.
/// u8 myMixingBuffer[MM_MIXLEN_16KHZ] __attribute((aligned(4)));
///
/// void maxmodInit(void)
/// {
///     irqSet(IRQ_VBLANK, mmVBlank);
///
///     u8 *myData;
///     mm_gba_system mySystem;
///
///     // Allocate data for channel buffers & wave buffer (malloc'd data goes
///     // to EWRAM).
///     // Use the SIZEOF definitions to calculate how many bytes to reserve
///     myData = (u8*)malloc(8 * (MM_SIZEOF_MODCH + MM_SIZEOF_ACTCH + MM_SIZEOF_MIXCH)
///                          + MM_MIXLEN_16KHZ);
///
///     // setup system info
///     // 16KHz software mixing rate, select from mm_mixmode
///     mySystem.mixing_mode       = MM_MIX_16KHZ;
///
///     // number of module/mixing channels
///     // higher numbers offer better polyphony at the expense
///     // of more memory and/or CPU usage.
///     mySystem.mod_channel_count = 8;
///     mySystem.mix_channel_count = 8;
///
///     // Assign memory blocks to pointers
///     mySystem.module_channels   = (mm_addr)(myData + 0);
///     mySystem.active_channels   = (mm_addr)(myData + (8 * MM_SIZEOF_MODCH));
///     mySystem.mixing_channels   = (mm_addr)(myData + (8 * (MM_SIZEOF_MODCH
///                                                         + MM_SIZEOF_ACTCH)));
///     mySystem.mixing_memory     = (mm_addr)myMixingBuffer;
///     mySystem.wave_memory       = (mm_addr)(myData + (8 * (MM_SIZEOF_MODCH
///                                                         + MM_SIZEOF_ACTCH
///                                                         + MM_SIZEOF_MIXCH)));
///     // Pass soundbank address
///     mySystem.soundbank         = (mm_addr)soundbank;
///
///     // Initialize Maxmod
///     mmInit(&mySystem);
/// }
/// ```
void mmInit(mm_gba_system* setup);

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
/// @param handler
///     Function pointer to event handler.
void mmSetEventHandler(mm_callback handler);

/// This is the main routine-function that processes music and updates the sound
/// output.
///
/// For GBA, this function must be called every frame. If a call is missed,
/// garbage will be heard in the output and module processing will be delayed.
void mmFrame(void) __attribute((long_call));

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

/// Set the current sequence [aka order-list] position for the active module.
///
/// @param position
///     New position in module sequence.
void mmPosition(mm_word position);

/// Set playback position (alias for mmPosition()).
///
/// @param position
///     New position in module sequence.
static inline void mmSetPosition(mm_word position)
{
    mmPosition(position);
}

/// Used to determine if a module is playing.
///
/// @return
///     Nonzero if a module is currently playing.
mm_bool mmActive(void);

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
void mmJingle(mm_word module_ID);

/// Check if a jingle is playing or not.
///
/// @return
///     Returns nonzero if a jingle is actively playing.
mm_bool mmActiveSub(void);

/// Use this function to change the master volume scale for module playback.
///
/// @param volume
///     New volume level. Ranges from 0 (silent) to 1024 (normal).
void mmSetModuleVolume(mm_word volume);

/// Use this function to change the master volume scale for jingle playback.
///
/// @param volume
///     New volume level. Ranges from 0 (silent) to 1024 (normal).
void mmSetJingleVolume(mm_word volume);

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

/// Play direct MAS file.
///
/// @param address
///     TODO
/// @param mode
///     TODO
/// @param layer
///     TODO
void mmPlayModule(mm_word address, mm_word mode, mm_word layer);

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
/// TODO: Does volume go from 0 to 65535?
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
/// @param handle
///     Sound effect handle received from mmEffect() or mmEffectEx().
void mmEffectRelease(mm_sfxhand handle);

/// Set master volume scale for effect playback.
///
/// @param volume
///     Master volume. 0->1024 representing 0%->100% volume
void mmSetEffectsVolume(mm_word volume);

/// Stop all sound effects and reset the effect system.
void mmEffectCancelAll();

// ***************************************************************************
/// @}
/// @defgroup gba_playback_events GBA: Playback events
/// @{
// ***************************************************************************

/// This event occurs when an SFx (S3M/IT) or EFx (MOD/XM) effect is parsed from
/// the pattern data. If you have an event handler installed it will be
/// forwarded to there. param will contain 'x' from SFx/EFx.
#define MMCB_SONGMESSAGE    0x2A

/// This event occurs when a module has finished playing the last pattern and
/// has stopped active status. param == 0 if main module, == 1 otherwise
/// (jingle)
#define MMCB_SONGFINISHED   0x2B

// ***************************************************************************
/// @}
/// @defgroup gba_misc GBA: Misc definitions / reference
/// @{
// ***************************************************************************

extern mm_byte mp_mix_seg;      // current mixing segment
extern mm_word mp_writepos;     // mixer's write position

// ***************************************************************************
/// @}
// ***************************************************************************

#ifdef __cplusplus
}
#endif

#endif // MAXMOD_H
