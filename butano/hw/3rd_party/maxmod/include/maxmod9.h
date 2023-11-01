/****************************************************************************
 *                                                          __              *
 *                ____ ___  ____ __  ______ ___  ____  ____/ /              *
 *               / __ `__ \/ __ `/ |/ / __ `__ \/ __ \/ __  /               *
 *              / / / / / / /_/ />  </ / / / / / /_/ / /_/ /                *
 *             /_/ /_/ /_/\__,_/_/|_/_/ /_/ /_/\____/\__,_/                 *
 *                                                                          *
 *                            ARM9 Definitions                              *
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

#ifdef ARM7
#error "maxmod9.h is for ARM9!"
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
 * mmInitDefault( char* soundbank_file )
 *
 * Initialize Maxmod using default settings.
 *
 * soundbank_file : Filename of the soundbank to use.
 *****************************************************************/
 
void mmInitDefault( char* soundbank_file );

/*****************************************************************
 * mmInitDefaultMem( mm_addr soundbank )
 *
 * Initialize Maxmod using default settings.
 * Use this when you have the entire soundbank loaded into memory.
 * 
 * soundbank : Address of soundbank data.
 *****************************************************************/
 
void mmInitDefaultMem( mm_addr soundbank );

/*****************************************************************
 * mmInit( mm_ds_system* system )
 *
 * Initialize Maxmod
 *
 * system : Configuration info.
 *****************************************************************/
 
void mmInit( mm_ds_system* system );

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
 * mmLoad( mm_word module_ID )
 *
 * Load a module into memory. Must be used before starting
 * a module.
 *
 * module_ID : ID of module. (defined in soundbank header)
 *****************************************************************/
 
void mmLoad( mm_word module_ID );

/*****************************************************************
 * mmUnload( mm_word module_ID )
 *
 * Unload a module from memory.
 *
 * module_ID : ID of module. (defined in soundbank header)
 *****************************************************************/

void mmUnload( mm_word module_ID );

/*****************************************************************
 * mmLoadEffect( mm_word sample_ID )
 *
 * Load a sound effect into memory. Use before mmEffect.
 *
 * sample_ID : ID of sample. (defined in soundbank header)
 *****************************************************************/
 
void mmLoadEffect( mm_word sample_ID );

/*****************************************************************
 * mmUnloadEffect( mm_word sample_ID )
 *
 * Unload sound effect from memory.
 *
 * sample_ID : ID of sample. (defined in soundbank header)
 *****************************************************************/
 
void mmUnloadEffect( mm_word sample_ID );

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
 * mmSetEventHandler( mm_callback handler )
 *
 * Setup handler to receive playback events.
 *
 * handler : Pointer to your function.
 *****************************************************************/

void mmSetEventHandler( mm_callback handler );

/*****************************************************************
 * mmSoundBankInMemory( mm_addr address )
 * mmSoundBankInFiles( char* filename )
 * mmSetCustomSoundBankHandler( mm_callback p_loader )
 *
 * Setup soundbank interface.
 * This is automatically done by mmInitDefault/mmInitDefaultMem.
 *****************************************************************/

void mmSoundBankInMemory( mm_addr address );
void mmSoundBankInFiles( char* filename );
void mmSetCustomSoundBankHandler( mm_callback p_loader );



/*****************************************************************
 * 
 * Module Playback
 *
 *****************************************************************/



/*****************************************************************
 * mmStart( mm_word module_ID, mm_pmode mode )
 *
 * Start module playback.
 *
 * module_ID : ID of module to play (defined in soundbank header)
 *             Make sure the module is loaded via mmLoad
 *
 * mode : Mode of playback: MM_PLAY_ONCE or MM_PLAY_LOOP
 *****************************************************************/

void mmStart( mm_word module_ID, mm_pmode mode );

/*****************************************************************
 * mmPause()
 *
 * Pause module playback. Resume with mmResume().
 *****************************************************************/
 
void mmPause( void );

/*****************************************************************
 * mmResume()
 *
 * Resume module playback. Pause with mmPause().
 *****************************************************************/
 
void mmResume( void );

/*****************************************************************
 * mmStop()
 *
 * Stop module playback. Start again (from the beginning)
 * with mmStart().
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
 * mmJingle( mm_word module_ID )
 *
 * Play module as jingle. Jingles are limited to 4 channels only.
 *
 * module_ID : ID of module
 *****************************************************************/

void mmJingle( mm_word module_ID );

/*****************************************************************
 * mmSetModuleVolume( mm_word volume )
 * mmSetJingleVolume( mm_word volume )
 *
 * Set master volume scale for music/jingle playback.
 *
 * volume : 0->1024 representing 0%->100% volume
 *****************************************************************/

void mmSetModuleVolume( mm_word volume );
void mmSetJingleVolume( mm_word volume );

/*****************************************************************
 * mmSetModuleTempo( mm_word tempo )
 *
 * Set tempo of playback.
 *
 * tempo : Fixed point (Q10) value representing tempo.
 *         Range = 0x200 -> 0x800 = 0.5 -> 2.0
 *****************************************************************/

void mmSetModuleTempo( mm_word tempo );

/*****************************************************************
 * mmSetModulePitch( mm_word pitch )
 *
 * Set pitch of playback.
 *
 * pitch : Range = 0x200 -> 0x800 = 0.5 -> 2.0
 *****************************************************************/
 
void mmSetModulePitch( mm_word pitch );

/*****************************************************************
 * mmActive()
 *
 * Returns nonzero if module is playing.
 *****************************************************************/

mm_bool mmActive( void );

 
 
/*****************************************************************
 *
 * Sound Effects
 *
 *****************************************************************/
 
 
 
/*****************************************************************
 * mmEffect( mm_word sample_ID )
 *
 * Play a sound effect at its default rate with full volume
 * and centered panning.
 * Remember to load the effect via mmLoadEffect first.
 *
 * sample_ID : ID of sample to play (defined in soundbank header)
 *
 * Returns mm_sfxhand which may be used to modify the sound later.
 *****************************************************************/

mm_sfxhand mmEffect( mm_word sample_ID );

/*****************************************************************
 * mmEffectEx( mm_sound_effect* sound )
 *
 * Play a sound effect with rate/volume/panning specified.
 * Remember to load the effect via mmLoadEffect first.
 *
 * sound : sound effect info
 *****************************************************************/
 
mm_sfxhand mmEffectEx( mm_sound_effect* sound );

/*****************************************************************
 * mmEffectVolume( mm_sfxhand handle, mm_byte volume )
 *
 * Set effect volume.
 *
 * handle : Sound effect handle (returned from mmEffect/Ex)
 * volume : 0..255 = silent..normal
 *****************************************************************/

void mmEffectVolume( mm_sfxhand handle, mm_word volume );

/*****************************************************************
 * mmEffectPanning( mm_sfxhand handle, mm_byte panning )
 *
 * Set effect panning.
 *
 * handle : Sound effect handle (returned from mmEffect/Ex)
 * panning : 0..255 = hard left..hard right
 *****************************************************************/
 
void mmEffectPanning( mm_sfxhand handle, mm_byte panning );

/*****************************************************************
 * mmEffectRate( mm_sfxhandle, mm_word rate )
 *
 * Set effect playback rate.
 *
 * handle : Sound effect handle (returned from mmEffect/Ex)
 * rate : 6.10 fixed point value representing rate. A value of
 *        2048 will make the pitch one octave higher than the
 *        default setting
 *****************************************************************/
 
void mmEffectRate( mm_sfxhand handle, mm_word rate );

/*****************************************************************
 * mmEffectScaleRate( mm_sfxhandle, mm_word factor )
 *
 * Scale effect playback rate.
 *
 * handle : Sound effect handle (returned from mmEffect/Ex)
 * factor : 6.10 fixed point value to multiply the rate by.
 *****************************************************************/

void mmEffectScaleRate( mm_sfxhand handle, mm_word factor );

/*****************************************************************
 * mmEffectCancel( mm_sfx_handle handle )
 *
 * Stop sound effect.
 *
 * handle : Sound effect handle (returned from mmEffect/Ex)
 *****************************************************************/
 
void mmEffectCancel( mm_sfxhand handle );

/*****************************************************************
 * mmEffectRelease( mm_sfxhand handle )
 *
 * Release sound effect. This discards the handle and allows the
 * effect to be overidden by music/other effects.
 *****************************************************************/
 
void mmEffectRelease( mm_sfxhand handle );

/*****************************************************************
 * mmEffectExt( mm_sound_effect* sound, mm_sample* sample )
 *
 * Play external sound.
 *
 * sound : Pointer to sound effect info
 * sample : Pointer to external sample
 *****************************************************************/

mm_sfxhand mmEffectExt( mm_sound_effect* sound, mm_ds_sample* sample );

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
 * mmStreamOpen( mm_stream_info* info )
 *
 * Open audio stream.
 *
 * stream : Configuration struct
 *****************************************************************/

void mmStreamOpen( mm_stream* stream );

/*****************************************************************
 * mmStreamUpdate()
 *
 * Fill audio stream with data. This needs to be called
 * in 'manual' mode. This is called automatically in 'auto' mode.
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
 * Soundbank interface messages
 *****************************************************************/
 
#define MMCB_SONGREQUEST	0x1A
#define MMCB_SAMPREQUEST	0x1B
#define MMCB_DELETESONG		0x1C
#define MMCB_DELETESAMPLE	0x1D

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

//----------------------------------------------------------------
#endif
//----------------------------------------------------------------
