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
 *                            ARM7 Definitions                              *
 *                                                                          *
 ****************************************************************************/

/// @file maxmod7.h
///
/// @brief Global include of Maxmod for DS (ARM7 side).

#ifdef ARM9
#error "maxmod7.h is for ARM7!"
#endif

#ifndef MAXMOD_H
#define MAXMOD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mm_types.h>

// ***************************************************************************
/// @defgroup nds_arm7_init NDS: ARM7 Initialization/Main Functions
/// @{
// ***************************************************************************

/// Installs the Maxmod system on the ARM7 side.
///
/// Normally, this is the only function you need to call on ARM7.
///
/// @param fifo_channel
///     FIFO communication channel to use. Must equal the channel the ARM9 side
///     will be sending data to.  (usually FIFO_MAXMOD)
void mmInstall( int fifo_channel );

/// Lock audio channels to prevent Maxmod from using them.
///
/// This is for when you need to operate on the DS hardware channels directly.
/// Note that if you use this function while music or sound effects are playing,
/// any active notes that are using the channels to be locked will be cut.
///
/// @param bitmask
///     Selection of channels to lock. Bit0 = Channel0, Bit1 = Channel1 ...
///     Bit15 = Channel15
void mmLockChannels( mm_word bitmask );

/// Unlocks audio channels to allow Maxmod to use them.
///
/// This function can be used to restore channel usage to Maxmod when you are
/// finished using certain channels.
///
/// Note that in the "Interpolated Audio" mode, channels **can not** be
/// unlocked. To unlock channels in the interpolated mode you must reset the
/// audio system. To reset the audio system, use mmSelectMode.
///
/// @param bitmask
///     Selection of channels to unlock. Bit0 = Channel0, Bit1 = Channel1, Bit2
///     = Channel2 ... Bit15 = Channel15
void mmUnlockChannels( mm_word bitmask );

/// Returns whether or not the system is initialized and ready for playback.
///
/// @return
///     Non-zero if the system is initialized, zero otheriwse.
mm_bool mmIsInitialized( void );

/// Switches the audio mode for Maxmod DS.
///
/// Hardware mixing offers 16-channel audio with minimal CPU load.
///
/// Interpolated mixing extends the capability of the hardware channels by
/// adding linear interpolation in software.
///
/// Extended mixing increases the channel count to 30 with software mixing.
///
/// @param mode
///     New audio mode. Pass MM_MODE_A for complete hardware mixing, MM_MODE_B
///     for interpolated mixing, or MM_MODE_C for extended mixing.
void mmSelectMode( mm_mode_enum mode );

/// This is the main routine-function that processes music and updates the sound
/// output.
///
/// For DS (ARM7), this function is called automatically.
void mmFrame( void );

// Note about using sleep functions:
// - Do not use during an interrupt!! It's not safe.
// - And do not exit sleep unless it is already sleeping.

// ***************************************************************************
/// @}
/// @defgroup nds_arm7_module_playback NDS: ARM7 Module Playback
/// @{
// ***************************************************************************

/// Begins playback of a module.
///
/// For DS, the module must be loaded into memory first (mmLoad).
///
/// @param module_ID
///     Index of module to be played. Values are defined in the soundbank header
///     output. (prefixed with "MOD_")
/// @param mode
///     Mode of playback. Can be MM_PLAY_LOOP (play and loop until stopped
///     manually) or MM_PLAY_ONCE (play until end).
void mmStart( mm_word module_ID, mm_pmode mode );

/// Pauses playback of the active module.
///
/// Resume with mmResume().
///
/// DS Note: The DS hardware channels do not allow actual pausing. To implement
/// pausing, Maxmod sets the channel frequencies to the minimal value (256Hz).
/// This may cause some problems with certain samples (such as drum-loops)
/// drifting out of sync with the song temporarily. This is not an issue with
/// the interpolated audio mode since the channels are fed by software.
void mmPause( void );

/// Resume module playback.
///
/// Pause with mmPause().
void mmResume( void );

/// Stops playback of the active module.
///
/// Start again (from the beginning) with mmStart().
///
/// Any channels used by the active module will be freed.
void mmStop( void );

/// Set the current sequence [aka order-list] position for the active module.
///
/// @param position
///     New position in module sequence.
void mmPosition( mm_word position );

/// Used to determine if a module is playing.
///
/// @return
///     Nonzero if a module is currently playing.
mm_bool mmActive( void );

/// Plays a jingle.
///
/// Jingles are normal modules that can be mixed with the normal module
/// playback.
///
/// For DS, the module must be loaded into memory first (mmLoad).
///
/// Note that jingles must be limited to 4 channels only.
///
/// @param module_ID
///     Index of module to be played. (Defined in soundbank header)
void mmJingle( mm_word module_ID );

/// Check if a jingle is playing or not.
///
/// @return
///     Returns nonzero if a jingle is actively playing.
mm_bool mmActiveSub( void );

/// Use this function to change the master volume scale for module playback.
///
/// @param volume
///     New volume level. Ranges from 0 (silent) to 1024 (normal).
void mmSetModuleVolume( mm_word volume );

/// Use this function to change the master volume scale for jingle playback.
///
/// @param volume
///     New volume level. Ranges from 0 (silent) to 1024 (normal).
void mmSetJingleVolume( mm_word volume );

/// Set tempo of playback.
///
/// @param tempo
///     Fixed point (Q10) value representing tempo.
///     Range = 0x200 -> 0x800 = 0.5 -> 2.0
void mmSetModuleTempo( mm_word tempo );

/// Set pitch of playback.
///
/// @param pitch
///     Range = 0x200 -> 0x800 = 0.5 -> 2.0
void mmSetModulePitch( mm_word pitch );

/// Play direct MAS file.
///
/// @param address
///     TODO
/// @param mode
///     TODO
/// @param layer
///     TODO
void mmPlayModule( mm_word address, mm_word mode, mm_word layer );

// ***************************************************************************
/// @}
/// @defgroup nds_arm7_sound_effects NDS: ARM7 Sound Effects
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
/// For DS, make sure the sample is loaded (mmLoadEffect).
///
/// @param sample_ID
///     Index of sample to be played. Values are defined in the soundbank
///     header. (prefixed with "SFX_")
///
/// @return
///     Sound effect handle. This value can be used to modify parameters of the
///     sound effect while it is playing.
mm_sfxhand mmEffect( mm_word sample_ID );

/// Plays a sound effect with custom settings.
///
/// Remember to load the effect via mmLoadEffect first.
///
/// @param sound
///     Structure containing information about the sound to be played.
///
/// @return
///     Sound effect handle that may be used to modify the sound later.
mm_sfxhand mmEffectEx( mm_sound_effect* sound );

/// Changes the volume of a sound effect.
///
/// TODO: Is it from 0 to 65535?
///
/// @param handle
///     Sound effect handle received from mmEffect() or mmEffectEx().
/// @param volume
///     New volume level. Ranges from 0 (silent) to 255 (normal).
void mmEffectVolume( mm_sfxhand handle, mm_word volume );

/// Changes the panning of a sound effect.
///
/// @param handle
///     Sound effect handle received from mmEffect() or mmEffectEx().
/// @param panning
///     New panning level. Ranges from 0 (left) to 255 (right).
void mmEffectPanning( mm_sfxhand handle, mm_byte panning );

/// Changes the playback rate for a sound effect.
///
/// The actual playback rate depends on this value and the base frequency of the
/// sample. This parameter is a 6.10 fixed point value, passing 1024 will return
/// the sound to its original pitch, 2048 will raise the pitch by one octave,
/// and 512 will lower the pitch by an octave. To calculate a value from
/// semitones: Rate = 1024 * 2^(Semitones/12). (please don't try to do that with
/// integer maths)
///
/// @param handle Sound effect handle received from mmEffect() or mmEffectEx().
/// @param rate   New playback rate.
void mmEffectRate( mm_sfxhand handle, mm_word rate );

/// Scales the rate of the sound effect by a certain factor.
///
/// @param handle
///     Sound effect handle received from mmEffect() or mmEffectEx().
/// @param factor
///     6.10 fixed point factor.
void mmEffectScaleRate( mm_sfxhand handle, mm_word factor );

/// Stops a sound effect. The handle will be invalidated.
///
/// @param handle
///     Sound effect handle received from mmEffect() or mmEffectEx().
///
/// @return
///     Non-zero if the sound was found and stopped, zero on error.
mm_word mmEffectCancel( mm_sfxhand handle );

/// Marks a sound effect as unimportant.
///
/// This enables the sound effect to be interrupted by music/other sound effects
/// if the need arises. The handle will be invalidated.
///
/// @param handle
///     Sound effect handle received from mmEffect() or mmEffectEx().
void mmEffectRelease( mm_sfxhand handle );

/// Set master volume scale for effect playback.
///
/// @param volume
///     Master volume. 0->1024 representing 0%->100% volume
void mmSetEffectsVolume( mm_word volume );

/// Stop all sound effects and reset the effect system.
void mmEffectCancelAll( void );

// ***************************************************************************
/// @}
/// @defgroup nds_arm7_streaming NDS: ARM7 Streaming
/// @{
// ***************************************************************************

/// Opens an audio stream.
///
/// See the tutorials for more information.
///
/// Caution: If you have some heavy interrupt-routines that are enabled while
/// calling this function, you may corrupt the stream. The function cannot
/// disable interrupts internally because it requires the communications to be
/// active.
///
/// @param stream
///     Pointer to a structure containing information about how the stream will
///     operate.
/// @param wavebuffer
///     Wave memory, must be aligned.
/// @param workbuffer
///     Work memory, must be aligned.
void mmStreamOpen( mm_stream* stream, mm_addr wavebuffer, mm_addr workbuffer );

/// Check buffering state and fill stream with data.
///
/// Requests will be made to your callback to fill parts of the wave buffer(s).
/// This function only needs to be called manually if the stream isn't in
/// auto-fill mode. This function shouldn't be used when the stream is
/// automatically filled.
void mmStreamUpdate( void );

/// Close audio stream.
void mmStreamClose( void );

/// Get number of samples elapsed since the stream was opened.
///
/// The 32-bit value will wrap every 36 hours or so (at 32khz).
///
/// @return
///     The nummber of samples.
mm_word mmStreamGetPosition(void);

// ***************************************************************************
/// @}
/// @defgroup nds_arm7_reverb NDS: ARM7 Reverb
/// @{
// ***************************************************************************

/// Enables the reverb system.
///
/// This function locks 2 channels (1 & 3) to reserve them for reverb output.
/// This must be called before the configuration functions can be used.
///
/// If music is playing when this function is used, a note or two may be cut due
/// to the locking.
void mmReverbEnable( void );

/// Configures the reverb system.
///
/// See the reverb tutorial for a full explanation of the configuration
/// structure.
///
/// @param config
///     Reverb configuration structure.
void mmReverbConfigure( mm_reverb_cfg* config );

/// Starts reverb output in the selected channels.
///
/// @param channels
///     Reverb channel selection. Can use MMRC_LEFT or MMRC_RIGHT or (MMRC_LEFT
///     | MMRC_RIGHT).
void mmReverbStart( mm_reverbch channels );

/// Stops reverb output for the selected channels.
///
/// Can select left output, right output, or both.
///
/// @param channels
///     Reverb channel selection.
void mmReverbStop( mm_reverbch channels );

/// This calculates the amount of memory needed for the reverb buffer.
///
/// It calculates the reverb buffer size based on bit depth, delay and sampling
/// rate.
///
/// @param bit_depth
///     May be 8 or 16 to represent 8-bit or 16-bit bit-depth.
/// @param sampling_rate
///     Sampling rate in Hertz.
/// @param delay
///     Reverb delay value in milliseconds.
///
/// @return
///     Amount of memory required, measured in words (32-bit units).
static inline mm_word mmReverbBufferSize( mm_word bit_depth, mm_word sampling_rate, mm_word delay )
{
	if( bit_depth == 16 )
	{
		return ((((sampling_rate * delay * 2) / 1000) + 3) & (~3)) / 4;
	}
	else
	{
		return ((((sampling_rate * delay) / 1000) + 3) & (~3)) / 4;
	}
}

/// Disables the reverb system and restores the channels.
///
/// Note: In the interpolated audio mode, the channels cannot be restored by
/// this function and the mixer must be reset instead.
void mmReverbDisable( void );

// ***************************************************************************
/// @}
/// @defgroup nds_arm7_playback_events NDS: ARM7 Playback events
/// @{
// ***************************************************************************

/// This event occurs when an SFx (S3M/IT) or EFx (MOD/XM) effect is parsed from
/// the pattern data. If you have an event handler installed it will be
/// forwarded to there. param will contain 'x' from SFx/EFx.
#define MMCB_SONGMESSAGE	0x2A

/// This event occurs when a module has finished playing the last pattern and
/// has stopped active status. param == 0 if main module, == 1 otherwise
/// (jingle)
#define MMCB_SONGFINISHED	0x2B

// ***************************************************************************
/// @}
/// @defgroup nds_arm7_misc NDS: ARM7 Misc definitions / reference
/// @{
// ***************************************************************************

// main/sub layer attributes
//extern mm_modlayer mmLayerMain;
//extern mm_modlayer mmLayerSub;
//extern mpl_layer_information mmLayerMain;
//extern mpl_layer_information mmLayerSub;

// ***************************************************************************
/// @}
// ***************************************************************************

#ifdef __cplusplus
}
#endif

#endif // MAXMOD_H
