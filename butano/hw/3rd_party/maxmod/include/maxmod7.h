/****************************************************************************
 *                                                          __              *
 *                ____ ___  ____ __  ______ ___  ____  ____/ /              *
 *               / __ `__ \/ __ `/ |/ / __ `__ \/ __ \/ __  /               *
 *              / / / / / / /_/ />  </ / / / / / /_/ / /_/ /                *
 *             /_/ /_/ /_/\__,_/_/|_/_/ /_/ /_/\____/\__,_/                 *
 *                                                                          *
 *                            ARM7 Definitions                              *
 *                                                                          *
 *         Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)         *
 *                                                                          *
 * Permission to use, copy, modify, and/or distribute this software for any *
 * purpose with or without fee is hereby granted, provided that the above   *
 * copyright notice and this permission notice appear in all copies.        *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES *
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF         *
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR  *
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES   *
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN    *
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF  *
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.           *
 ****************************************************************************/

#ifdef ARM9
#error "maxmod7.h is for ARM7!"
#endif

//----------------------------------------------------------------
#ifndef MAXMOD_H
//----------------------------------------------------------------

#define MAXMOD_H

#include <mm_types.h>

//----------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//----------------------------------------------------------------

/*****************************************************************
 * mmInstall( channel )
 *
 * Install Maxmod.
 *
 * channel : IPC/FIFO channel to use, usually 7.
 *****************************************************************/
 
void mmInstall( int fifo_channel );

/*****************************************************************
 * mmLockChannels( mm_word bitmask )
 *
 * Lock audio channels from being used by Maxmod.
 *
 * bitmask : Bitmask of which channels should be locked.
 * (&1 = ch0, &2 = ch1, &4 = ch2, &8 = ch4, etc...)
 *****************************************************************/
 
void mmLockChannels( mm_word bitmask );

/*****************************************************************
 * mmUnlockChannels( mm_word bitmask )
 *
 * Unlock audio channels so Maxmod can use them.
 *
 * bitmask : Bitmask of which channels should be unlocked.
 * (&1 = ch0, &2 = ch1, &4 = ch2, &8 = ch4, etc...)
 *****************************************************************/
 
void mmUnlockChannels( mm_word bitmask );

/*****************************************************************
 * mmIsInitialize()
 *
 * Returns whether or not the system is ready for playback.
 *****************************************************************/

mm_bool mmIsInitialized( void );

/*****************************************************************
 * mmSelectMode( mm_mode_enum mode )
 *
 * Select audio mode
 *
 * mode:
 *   MM_MODE_A : Hardware Mixing
 *   MM_MODE_B : Interpolated Mixing
 *   MM_MODE_C : Extended Mixing
 *****************************************************************/
 
void mmSelectMode( mm_mode_enum mode );

/*****************************************************************
 * mmFrame()
 *
 * Work routine (called automatically)
 *****************************************************************/

void mmFrame( void );

// note about using sleep functions:
// do not use during an interrupt!! it's not safe
// and do not exit sleep unless it is already sleeping.



/*****************************************************************
 *
 * Module Playback
 *
 *****************************************************************/



/*****************************************************************
 * mmStart( mm_word module_ID, mm_pmode mode )
 *
 * Start module playback
 *
 * module_ID : Index of module to play. (defined in soundbank header)
 * mode : Mode of playback: MM_PLAY_ONCE or MM_PLAY_LOOP
 *****************************************************************/
 
void mmStart( mm_word module_ID, mm_pmode mode );

/*****************************************************************
 * mmPause()
 *
 * Pause module playback.
 *****************************************************************/
 
void mmPause( void );

/*****************************************************************
 * mmResume()
 *
 * Resume module playback
 *****************************************************************/
 
void mmResume( void );

/*****************************************************************
 * mmStop()
 *
 * Stop module playback. Start again (from the beginning)
 * with mmStart(...).
 *****************************************************************/
 
void mmStop( void );

/*****************************************************************
 * mmPosition( mm_word position )
 *
 * Set playback position.
 *
 * position : New position in module sequence.
 *****************************************************************/
 
void mmPosition( mm_word position );

/*****************************************************************
 * mmActive()
 *
 * Returns nonzero if module is playing
 *****************************************************************/
 
mm_bool	mmActive( void );

/*****************************************************************
 * mmJingle( mm_word module_ID )
 *
 * Play module as jingle. Jingles are limited to 4 channels only.
 *
 * module_ID : ID of module (defined in soundbank header)
 *****************************************************************/

void mmJingle( mm_word module_ID );

/*****************************************************************
 * mmActiveSub()
 *
 * Returns nonzero if a jingle is actively playing.
 *****************************************************************/
 
mm_bool	mmActiveSub( void );

/*****************************************************************
 * mmSetModuleVolume( mm_word volume )
 * mmSetJingleVolume( mm_word volume )
 *
 * Set volume scaler for music/jingles.
 *
 * volume : 0->1024 = silent..normal
 *****************************************************************/

void mmSetModuleVolume( mm_word volume );
void mmSetJingleVolume( mm_word volume );

/*****************************************************************
 * mmPlayModule( mm_word address, mm_word mode, mm_word layer )
 *
 * Play direct MAS file
 *****************************************************************/

void mmPlayModule( mm_word address, mm_word mode, mm_word layer );



/*****************************************************************
 *
 * Sound Effects
 *
 *****************************************************************/



/*****************************************************************
 * mmEffect( mm_word sample_ID )
 *
 * Play a sound effect at its default frequency with full volume
 * and centered panning. Effect must be loaded first via
 * mmLoadEffect.
 *
 * sample_ID : Sound effect ID. (defined in soundbank header)
 *****************************************************************/
 
mm_sfxhand mmEffect( mm_word sample_ID );

/*****************************************************************
 * mmEffectEx( mm_sound_effect* sound )
 *
 * Play a sound effect with the parameters supplied.
 *
 * sound : Sound effect attributes.
 *****************************************************************/

mm_sfxhand mmEffectEx( mm_sound_effect* sound );

/*****************************************************************
 * mmEffectVolume( mm_sfxhand handle, mm_word volume )
 *
 * Set the volume of a sound effect.
 *
 * handle : Sound effect handle.
 * volume : 0->65535
 *****************************************************************/
 
void mmEffectVolume( mm_sfxhand handle, mm_word volume );

/*****************************************************************
 * mmEffectPanning( mm_sfxhand handle, mm_word panning )
 *
 * Set the panning of a sound effect.
 *
 * handle : Sound effect handle.
 * panning : 0->255 = left..right
 *****************************************************************/
 
void mmEffectPanning( mm_sfxhand handle, mm_byte panning );

/*****************************************************************
 * mmEffectRate( mm_sfxhand handle, mm_word rate )
 *
 * Set the playback rate of an effect.
 *
 * handle : Sound effect handle.
 * rate: 6.10 factor
 *****************************************************************/
 
void mmEffectRate( mm_sfxhand handle, mm_word rate );

/*****************************************************************
 * mmEffectScaleRate( mm_sfxhand handle, mm_word factor )
 *
 * Scale the playback rate of an effect.
 *
 * handle : Sound effect handle.
 * factor : 6.10 fixed point factor.
 *****************************************************************/
 
void mmEffectScaleRate( mm_sfxhand handle, mm_word factor );

/*****************************************************************
 * mmEffectCancel( mm_sfxhand handle )
 *
 * Stop sound effect.
 *
 * handle : Sound effect handle.
 *****************************************************************/

void mmEffectCancel( mm_sfxhand handle );

/*****************************************************************
 * mmEffectRelease( mm_sfxhand handle )
 *
 * Release sound effect (invalidate handle and allow interruption)
 *
 * handle : Sound effect handle.
 *****************************************************************/
 
void mmEffectRelease( mm_sfxhand handle );

/*****************************************************************
 * mmSetEffectsVolume( mm_word volume )
 *
 * Set master volume scale for effect playback.
 *
 * volume : 0->1024 representing 0%->100% volume
 *****************************************************************/

void mmSetEffectsVolume( mm_word volume );

/*****************************************************************
 * mmEffectCancelAll()
 *
 * Stop all sound effects
 *****************************************************************/
 
void mmEffectCancelAll();



/*****************************************************************
 *
 * Streaming
 *
 *****************************************************************/


/*****************************************************************
 * mmStreamOpen( mm_stream* stream, mm_addr wavebuffer, mm_addr workbuffer )
 *
 * Open audio stream.
 *
 * stream : Configuration struct
 * wavebuffer : wave memory, must be aligned
 * workbuffer : work memory, must be aligned
 *****************************************************************/

void mmStreamOpen( mm_stream* stream, mm_addr wavebuffer, mm_addr workbuffer );

/*****************************************************************
 * mmStreamUpdate()
 *
 * Fills the stream with wave data.
 * This only needs to be called in 'manual' mode. It
 * is called automatically in 'auto' mode.
 *****************************************************************/

void mmStreamUpdate( void );

/*****************************************************************
 * mmStreamClose()
 *
 * Close audio stream.
 *****************************************************************/

void mmStreamClose( void );

/*****************************************************************
 * mmStreamGetPosition()
 *
 * Get number of samples elapsed since the stream was opened.
 * The 32-bit value will wrap every 36 hours or so (at 32khz)
 *****************************************************************/
 
mm_word mmStreamGetPosition();



/*****************************************************************
 *
 * Reverb
 *
 *****************************************************************/
 
 

/*****************************************************************
 * mmReverbEnable()
 *
 * Enable reverb system. (use before configuring!)
 *****************************************************************/
 
void mmReverbEnable( void );
 
/*****************************************************************
 * mmReverbConfigure( mm_reverb_cfg* config )
 *
 * Configure reverb parameters.
 * config : Configuration data.
 *****************************************************************/
 
void mmReverbConfigure( mm_reverb_cfg* config );

/*****************************************************************
 * mmReverbStart( mm_reverbch channels )
 *
 * Start reverb output.
 *****************************************************************/

void mmReverbStart( mm_reverbch channels );

/*****************************************************************
 * mmReverbStop( mm_reverch channels )
 *
 * Stop reverb output.
 *****************************************************************/

void mmReverbStop( mm_reverbch channels );

/*****************************************************************
 * mmReverbBufferSize( mm_word bit_depth, mm_word sampling_rate,
 *                     mm_word delay )
 *
 * Calculate reverb buffer size based on bit depth, delay
 * and sampling rate.
 *
 * bit_depth : Pass 8 or 16 for 8-bit/16-bit
 * sampling_rate :
 * delay : In milliseconds
 *
 * Returns size in WORDS.
 *****************************************************************/

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

/*****************************************************************
 * mmReverbDisable()
 *
 * Disable reverb system.
 *****************************************************************/

void mmReverbDisable( void );



//----------------------------------------------------------------
#ifdef __cplusplus
}
#endif
//----------------------------------------------------------------

/*****************************************************************
 * Playback events
 *****************************************************************/

//----------------------------------------------------------------
// This happens when Maxmod reads an SFx (or mod/xm EFx) effect
// from a module. It will pass 'x' in param.
//----------------------------------------------------------------
#define MMCB_SONGMESSAGE	0x2A
//----------------------------------------------------------------

//----------------------------------------------------------------
// A module has finished playing.
// param == 0 if main module, ==1 otherwise
//----------------------------------------------------------------
#define MMCB_SONGFINISHED	0x2B
//----------------------------------------------------------------

//=======================================================================================
// misc definitions / references
//=======================================================================================

// main/sub layer attributes
extern mm_modlayer mmLayerMain;
extern mm_modlayer mmLayerSub;

#endif
