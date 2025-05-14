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
 *                            ARM9 Definitions                              *
 *                                                                          *
 ****************************************************************************/

/// @file maxmod9.h
///
/// @brief Global include of Maxmod for DS (ARM9 side).

#ifdef ARM7
#error "maxmod9.h is for ARM9!"
#endif

#ifndef MAXMOD_H
#define MAXMOD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mm_types.h>

// ***************************************************************************
/// @defgroup nds_arm9_init NDS: ARM9 Initialization/Main Functions
/// @{
// ***************************************************************************

/// Initialize Maxmod with default settings.
///
/// For DS, this function also sets up the internal soundbank interface to use
/// the file specified.
///
/// @param soundbank_file
///     Filename of soundbank. A soundbank file can be created with the Maxmod
///     Utility.
void mmInitDefault( const char* soundbank_file );

/// Initialize Maxmod with default settings.
///
/// This function also sets up the internal soundbank interface to use the file
/// that is located somewhere in memory.
///
/// Use this when you have the entire soundbank loaded into memory.
///
/// @param soundbank
///     Address of soundbank data.
void mmInitDefaultMem( mm_addr soundbank );

/// Initializes Maxmod with the settings specified.
///
/// Initialize system. Call once at startup.
///
/// For DS projects, you must also setup a soundbank interface with one of the
/// mmSoundBank* functions.
///
/// @param system
///     Maxmod setup configuration.
///
/// Example:
/// ```c
/// void maxmodInit( void )
/// {
///     mm_ds_system sys;
///
///     // Number of modules in your soundbank (defined in output header)
///     sys.mod_count = MSL_NSONGS;
///
///     // Number of samples in your soundbank (defined in output header)
///     sys.samp_count= MSL_NSAMPS;
///
///     // Memory bank, allocate BANKSIZE (or NSONGS+NSAMPS) words
///     sys.mem_bank = malloc( MSL_BANKSIZE * 4 );
///
///     // Select FIFO channel
///     sys.fifo_channel = FIFO_MAXMOD;
///
///     // Initialize maxmod
///     mmInit( &sys );
///
///     mmSoundBankInMemory( (mm_addr)my_soundbank );
///
///     // or
///     //
///     //mmSoundBankInFiles( "my_soundbank.msl" );
/// }
/// ```
void mmInit( mm_ds_system* system );

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

/// Loads a module into memory for playback.
///
/// Must be used before starting to play a module with mmStart().
///
/// @param module_ID
///     Index of module to be loaded. Values are defined in the soundbank header
///     output. (prefixed with "MOD_")
void mmLoad( mm_word module_ID );

/// Unloads and frees memory space occupied by a module.
///
/// Caution: Ensure that the module to be unloaded isn't playing!
///
/// @param module_ID
///     Index of module to be unloaded from memory.
void mmUnload( mm_word module_ID );

/// Loads a sound effect into memory for playback.
///
/// May be played with mmEffect()/mmEffectEx() afterwards.
///
/// @param sample_ID
///     Index of sample to be loaded. Values are defined in the soundbank header
///     output (prefixed with "SFX_)
void mmLoadEffect( mm_word sample_ID );

/// Unloads a sound effect and frees the memory if the reference count becomes zero.
///
/// @param sample_ID
///     Index of sample to be unloaded.
void mmUnloadEffect( mm_word sample_ID );

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

/// Install handler to receive song events.
///
/// Use this function to receive song events. Song events occur in two
/// situations. One is by special pattern data in a module (which is triggered
/// by SFx/EFx commands). The other occurs when a module finishes playback (in
/// MM_PLAY_ONCE mode).
///
/// @param handler
///     Function pointer to event handler.
void mmSetEventHandler( mm_callback handler );

/// Setup the standard interface for a soundbank that is loaded in the file
/// system.
///
/// This function should be called after mmInit and before
/// any Load or Unload operations. This function is called by mmInitDefault.
///
/// @param filename
///     Filename of your soundbank binary.
void mmSoundBankInFiles( const char* filename );

/// Enable the standard interface for a soundbank that is loaded into memory.
///
/// This function should be called after mmInit and before any Load or Unload
/// functions. This function is called by mmInitDefaultMem.
///
/// @param address
///     Memory address of soundbank file.
void mmSoundBankInMemory( mm_addr address );

/// Install a custom routine to interface with the soundbank data.
///
/// The routine will be responsible for handling requests from Maxmod to access
/// data in the soundbank.
///
/// @param p_loader
///     Function pointer to soundbank request handler.
void mmSetCustomSoundBankHandler( mm_callback p_loader );

// ***************************************************************************
/// @}
/// @defgroup nds_arm9_module_playback NDS: ARM9 Module Playback
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
void mmSetModuleTempo( mm_word tempo );

/// Change the master pitch scale for module playback.
///
/// Specifying 1024 will play the module at its normal pitch. Minimum/Maximum
/// range of the pitch change is +-1 octave.
///
/// Range = 0x200 -> 0x800 = 0.5 -> 2.0
///
/// @param pitch
///     New pitch scale. Value = 1024 * 2 ^ (semitones / 12)
void mmSetModulePitch( mm_word pitch );

/// Used to determine if a module is playing.
///
/// @return
///     Nonzero if a module is currently playing.
mm_bool mmActive( void );

// ***************************************************************************
/// @}
/// @defgroup nds_arm9_sound_effects NDS: ARM9 Sound Effects
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
/// @param handle
///     Sound effect handle received from mmEffect() or mmEffectEx().
/// @param rate
///     New playback rate.
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
void mmEffectCancel( mm_sfxhand handle );

/// Marks a sound effect as unimportant.
///
/// This enables the sound effect to be interrupted by music/other sound effects
/// if the need arises. The handle will be invalidated.
///
/// @param handle
///     Sound effect handle received from mmEffect() or mmEffectEx().
void mmEffectRelease( mm_sfxhand handle );

/// Play external sound.
///
/// @param sound
///     Pointer to sound effect info.
/// @param sample
///     Pointer to external sample.
///
/// @return
///     Sound effect handle that may be used to modify the sound later.
mm_sfxhand mmEffectExt( mm_sound_effect* sound, mm_ds_sample* sample );

/// Set master volume scale for effect playback.
///
/// @param volume
///     Master volume. 0->1024 representing 0%->100% volume
void mmSetEffectsVolume( mm_word volume );

/// Stop all sound effects and reset the effect system.
void mmEffectCancelAll( void );

// ***************************************************************************
/// @}
/// @defgroup nds_arm9_streaming NDS: ARM9 Streaming
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
void mmStreamOpen( mm_stream* stream );

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
mm_word mmStreamGetPosition( void );

// ***************************************************************************
/// @}
/// @defgroup nds_arm9_reverb NDS: ARM9 Reverb
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
// ***************************************************************************

// NDS: ARM9 Soundbank interface messages
#define MMCB_SONGREQUEST	0x1A
#define MMCB_SAMPREQUEST	0x1B
#define MMCB_DELETESONG		0x1C
#define MMCB_DELETESAMPLE	0x1D

// ***************************************************************************
/// @defgroup nds_arm9_playback_events NDS: ARM9 Playback events
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
// ***************************************************************************

#ifdef __cplusplus
}
#endif

#endif // MAXMOD_H
