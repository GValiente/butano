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
 *               Nintendo DS & Gameboy Advance Sound System                 *
 *                                                                          *
 ****************************************************************************/

/// @file mm_types.h
///
/// @brief Type definitions for Maxmod.

#ifndef MM_TYPES_H__
#define MM_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

// ***************************************************************************
/// @defgroup maxmod_types Maxmod Definitions and Types
/// @{
// ***************************************************************************

typedef unsigned int    mm_word;    ///< Generic unsigned 32-bit value
typedef signed int      mm_sword;   ///< Generic signed 32-bit value
typedef unsigned short  mm_hword;   ///< Generic unsigned 16-bit value
typedef signed short    mm_shword;  ///< Generic signed 16-bit value
typedef unsigned char   mm_byte;    ///< Generic unsigned 8-bit value
typedef signed char     mm_sbyte;   ///< Generic signed 8-bit value

/// Sound effect handle.
///
/// Returned by mmEffect() and mmEffectEx(). May be used to modify a sound
/// effect while it is playing.
typedef unsigned short  mm_sfxhand;

typedef unsigned char   mm_bool;    ///< Boolean. Non-zero = true, zero = false.

typedef void            *mm_addr;   ///< Memory address (pointer)
typedef void            *mm_reg;    ///< Hardware register (pointer)

/// Audio modes for the DS system. Pass to mmSelectMode().
typedef enum
{
    MM_MODE_A, ///< Selects the full-hardware audio mode.
    MM_MODE_B, ///< Selects the interpolated audio mode.
    MM_MODE_C  ///< Selects the extended mixing audio mode.
} mm_mode_enum;

/// Layer types
typedef enum
{
    /// Main module song being played.
    ///
    /// On GBA the max number of channels is defined when calling mmInit(), in
    /// the `module_channels` field. On DS the max number of channels is 32.
    MM_MAIN = 0,

    /// Sub module song being played (jingle).
    ///
    /// This layer is limited to 4 channels.
    MM_JINGLE = 1
}
mm_layer_type;

/// Formats for software streaming.
///
/// ADPCM streaming is not supported by the DS hardware. The loop point data
/// gets recorded so ring buffers are not possible.
typedef enum
{
    /// 8-bit single channel output.
    MM_STREAM_8BIT_MONO     = 0x0,    // 000b
    /// 8-bit dual channel output (interleaved data).
    MM_STREAM_8BIT_STEREO   = 0x1,    // 001b

    /// 16-bit single channel output.
    MM_STREAM_16BIT_MONO    = 0x2,    // 010b
    /// 16-bit dual channel output (interleaved data).
    MM_STREAM_16BIT_STEREO  = 0x3,    // 011b

#ifdef MM_SUPPORT_4BIT_STREAM
    MM_STREAM_4BIT_MONO     = 0x4,    // 100b
    MM_STREAM_4BIT_STEREO   = 0x5,    // 101b
#endif

} mm_stream_formats;

/// Function pointer definition for handling song events.
///
/// @param msg   Message type.
/// @param param Additional data about the message.
typedef mm_word (*mm_callback)(mm_word msg, mm_word param);

/// Void function pointer.
typedef void (*mm_voidfunc)(void);

/// Function pointer definition for handling stream fill requests (DS mode).
///
/// @param length Number of samples to write to the output.
/// @param dest   Output address.
/// @param format Stream format.
typedef mm_word (*mm_stream_func)(mm_word length, mm_addr dest, mm_stream_formats format);

/// Reverb configuration flags.
///
/// The first few flags enable the values in the mm_reverb_cfg struct (to be
/// applied to the channels select with MMRF_LEFT/RIGHT). The rest of the flags
/// are actually commands.
typedef enum
{
    MMRF_MEMORY         = 0x01,     ///< Set memory setting.
    MMRF_DELAY          = 0x02,     ///< Set delay setting.
    MMRF_RATE           = 0x04,     ///< Set rate setting.
    MMRF_FEEDBACK       = 0x08,     ///< Set feedback setting.
    MMRF_PANNING        = 0x10,     ///< Set panning setting.
    MMRF_LEFT           = 0x20,     ///< Affect left output.
    MMRF_RIGHT          = 0x40,     ///< Affect right output.
    MMRF_BOTH           = 0x60,     ///< Affect both outputs.

    MMRF_INVERSEPAN     = 0x80,     ///< Inverse panning level for right output.
    MMRF_NODRYLEFT      = 0x100,    ///< Disable dry output for the left channel.
    MMRF_NODRYRIGHT     = 0x200,    ///< Disable dry output for the right channel.
    MMRF_8BITLEFT       = 0x400,    ///< Set 8-bit format for the left channel.
    MMRF_16BITLEFT      = 0x800,    ///< Set 16-bit format for the left channel.
    MMRF_8BITRIGHT      = 0x1000,   ///< Set 8-bit format for the right channel.
    MMRF_16BITRIGHT     = 0x2000,   ///< Set 16-bit format for the right channel.
    MMRF_DRYLEFT        = 0x4000,   ///< Enable dry output for the left channel.
    MMRF_DRYRIGHT       = 0x8000    ///< Enable dry output for the right channel.
} mm_reverbflags;

/// Selection of reverb channels to start/stop.
typedef enum
{
    MMRC_LEFT   = 1,    ///< Select left reverb channel.
    MMRC_RIGHT  = 2,    ///< Select right reverb channel.
    MMRC_BOTH   = MMRC_LEFT | MMRC_RIGHT ///< Select both reverb channels.
} mm_reverbch;

/// Reverb configuration settings.
///
/// The flags entry selects which data in the struct is valid. It also selects
/// which reverb channels to apply the settings to, and also some other
/// commands.
typedef struct mmreverbcfg
{
    mm_word     flags;    ///< Configuration flags (mm_reverbflags).
    mm_addr     memory;   ///< Memory address of reverb buffer.
    mm_hword    delay;    ///< Reverb buffer size, measured in words.
    mm_hword    rate;     ///< Reverb sampling rate.
    mm_hword    feedback; ///< Reverb feedback level. Range = 0..2047
    mm_byte     panning;  ///< Reverb panning level. Range = 0..127
} mm_reverb_cfg;

/// Module looping modes for mmStart().
typedef enum
{
    MM_PLAY_LOOP, ///< Loop module forever (until stopped).
    MM_PLAY_ONCE  ///< Stop module after playing the last pattern.
} mm_pmode;

/// Software mixing rates for GBA system.
typedef enum
{
    MM_MIX_8KHZ,  ///< 8 KHz, provides poor quality.
    MM_MIX_10KHZ, ///< 10 Khz, provides low quality.
    MM_MIX_13KHZ, ///< 13 Khz, provides almost OK quality.
    MM_MIX_16KHZ, ///< 16 Khz, provides OK quality, **standard setting**.
    MM_MIX_18KHZ, ///< 18 Khz, provies good quality, higher CPU load.
    MM_MIX_21KHZ, ///< 21 Khz, provies better quality, high CPU load.
    MM_MIX_27KHZ, ///< 27 KHz, provides even better quality, very high CPU load.
    MM_MIX_31KHZ  ///< 31 KHz, provides maximum quality, highest CPU load.
} mm_mixmode;

/// Timer selection for counting samples during streaming.
typedef enum
{
    MM_TIMER0,  // Hardware timer 0
    MM_TIMER1,  // Hardware timer 1
    MM_TIMER2,  // Hardware timer 2
    MM_TIMER3   // Hardware timer 3
} mm_stream_timer;

/// Sample structure.
///
/// If the sample loops, then loop_start should be set to the position of the
/// loop starting point, and loop_length should be set to the length of the
/// loop. Otherwise, loop_start should be set to zero, and length should be set
/// to the length of the sample.
///
/// The length and position values are measured in words. That is, samples/4 for
/// 8-bit, and samples/2 for 16-bit (and samples/8 for 4-bit compressed). The
/// sample data must be aligned and maybe resampled or padded to fit along the
/// boundaries.
typedef struct t_mmdssample
{
    /// Start of sample loop. Measured in words.
    mm_word     loop_start;

    union
    {
        /// Length of sample loop. Measured in words.
        mm_word     loop_length;
        /// Length of sample. Measured in words.
        mm_word     length;
    };

    /// Sample format. 0 = 8-bit PCM, 1 = 16-bit PCM, 2 = IMA-ADPCM
    mm_byte     format;
    /// Looping mode. 1 = Forward Loop, 2 = One-Shot
    mm_byte     repeat_mode;
    /// Center playback rate. Value = Hz * 1024 / 32768.
    mm_hword    base_rate;
    /// Pointer to sample data.
    mm_addr     data;

} mm_ds_sample;

/// Information for playing a sound effect.
///
/// The sample entry is only supported on DS and may be used to play an external
/// sample source.
typedef struct t_mmsoundeffect
{
    union
    {
        /// ID of sample to be played. Values are defined in the soundbank
        /// header.
        mm_word         id;

        /// Address of external sample, must be in main RAM. The ID is ignored
        /// if this entry is used. This field isn't valid in GBA systems.
        mm_ds_sample    *sample;
    };

    /// Playback rate. 6.10 fixed point number. (1024 = original sound)
    mm_hword    rate;

    /// Handle of previous sound effect. If a valid handle is given, it will be
    /// recycled.
    mm_sfxhand  handle;

    /// Volume level. Ranges from 0 (silent) to 255 (normal).
    mm_byte     volume;

    /// Panning level. Ranges from 0 (far-left) to 255 (far-right).
    mm_byte     panning;

} mm_sound_effect;

/// GBA setup information, passed to mmInit().
///
/// About mixing_memory: The mixing memory is a heavily used area of memory and
/// should most definitely be located in the fast IWRAM. By default, most GBA
/// toolchains place any global variables in IWRAM. The size of this buffer
/// depeds on the mixing rate selected.
///
/// Check the mm_mixlen_enum values. These values contain the size of the mixing
/// buffer in bytes. If you're using 16KHz mixing rate, your mixing buffer
/// should be defined like this (as a global array):
///
/// ```c
/// u8 my_mixing_buffer[MM_MIXLEN_16KHZ] __attribute__((aligned(4)));
/// ```
///
/// Notice also that the mixing buffer should be aligned by 4 bytes.
///
/// About the wave buffer: The wave buffer is a not-so-heavily used area of
/// memory and can be placed in EWRAM. The wave buffer contains the final
/// waveform data that is DMA copied to the sound FIFO. The size of the waveform
/// buffer must be equal to the size of the mixing buffer. By default, in most
/// GBA toolchains, the malloc function can be used to allocate the wave buffer
/// in EWRAM. The wave buffer must be aligned by 4 bytes too (malloc returns
/// pointers aligned to 4 bytes).
typedef struct t_mmgbasystem
{
    /// Software mixing rate. May be 8, 10, 13, 16, 18, or 21 KHz (select value
    /// from enum). Higher values offer better quality at expense of a larger
    /// CPU and memory load.
    mm_mixmode  mixing_mode;

    /// This is the amount of module channels there will be. It must be greater
    /// or equal to the largest channel number used by your modules (notice: NOT
    /// virtual channel number).
    mm_word     mod_channel_count;

    /// Number of mixing channels. Higher numbers offer better polyphony at
    /// expense of larger memory footprint and CPU load.
    mm_word     mix_channel_count;

    /// Pointer to module channel buffer, this can be placed in EWRAM. Size of
    /// buffer must be ``MM_SIZEOF_MODCH * mod_channel_count`` bytes.
    mm_addr     module_channels;

    /// Pointer to active channel buffer, this can be placed in EWRAM. Size of
    /// buffer must be ``MM_SIZEOF_ACTCH * mix_channel_count`` bytes.
    mm_addr     active_channels;

    /// Pointer to mixing channel buffer, this can be placed in EWRAM. Size of
    /// buffer must be ``MM_SIZEOF_MIXCH * mix_channel_count`` bytes.
    mm_addr     mixing_channels;

    /// Pointer to mixing buffer, should be placed in IWRAM. Placing this buffer
    /// in EWRAM will cause a *much* higher CPU load. See description for size
    /// specification.
    mm_addr     mixing_memory;

    /// Pointer to wave output buffer, this can be placed in EWRAM. see
    /// description for size specification.
    mm_addr     wave_memory;

    /// Pointer to your soundbank file. (Most likely somewhere in the cartridge
    /// space).
    mm_addr     soundbank;

} mm_gba_system;

/// DS setup information.
///
/// More about mem_bank: Maxmod needs some memory to manage samples and modules
/// that are loaded/unloaded into/from memory. This pointer should reference an
/// area of memory that is mod_count+samp_count WORDS in size:
///
/// ```c
/// #include "my_soundbank.h"
/// u32 my_mem_bank[ MSL_BANKSIZE ];
/// ```
///
/// MSL_BANKSIZE is defined as the sum of the module and sample count in the
/// soundbank.
///
/// The mmInitDefault()/mmInitDefaultMem() functions may be used to quickly
/// initialize maxmod default settings. Using these functions, mod_count and
/// samp_count will be automatically filled using data from the soundbank, and
/// mem_bank will be allocated with malloc().
typedef struct t_mmdssystem
{
    /// Number of modules in the soundbank file. Write MSL_NSONGS here.
    mm_word     mod_count;

    /// Number of samples in the soundbank file. Write MSL_NSAMPS here.
    mm_word     samp_count;

    /// Pointer to a memory region to be used for managing modules/samples:
    /// ```c
    /// mm_word mem_bank[MSL_BANKSIZE];
    /// ```
    mm_word     *mem_bank;

    /// FIFO channel to use for communicating with ARM7. Usually FIFO_MAXMOD.
    mm_word     fifo_channel;

} mm_ds_system;

/// Information about a stream to be opened.
///
/// More about manual mode: If this is set, then you must call mmStreamUpdate()
/// periodically to push data into the stream buffer (mmStreamUpdate makes
/// requests to your callback function to fill the stream). Manual mode can be
/// used to have control over when the stream is filled.
typedef struct t_mmstream
{
    /// Sampling rate of the stream, measured in Hertz. (1024->32768)
    mm_word         sampling_rate;
    /// Amount of samples to buffer. Should be a multiple of 16.
    mm_word         buffer_length;
    /// Function pointer to your routine to fill the stream.
    mm_stream_func  callback;
    /// Sample data format (mm_stream_formats).
    mm_word         format;
    /// Hardware timer selection. (will be used for timing the data requests)
    /// (mm_stream_timers).
    mm_word         timer;
    /// Disable automatic fill requests. If set, user must call mmStreamUpdate()
    /// manually.
    mm_bool         manual;

} mm_stream;

typedef struct t_mmstreamdata
{
    mm_bool             is_active;
    mm_stream_formats   format;
    mm_bool             is_auto;
    mm_byte             hw_timer_num;
    mm_hword            clocks;
    mm_hword            timer;
    mm_hword            length_cut;
    mm_hword            length_words;
    mm_hword            position;
    mm_hword            reserved2;
    volatile mm_hword   *hw_timer;
    mm_addr             wave_memory;
    mm_addr             work_memory;
    mm_stream_func      callback;
    mm_word             remainder;
} mm_stream_data;

typedef struct tmm_voice
{
    // data source information
    mm_addr     source;     // address to sample data
    mm_word     length;     // length of sample data OR loop length (expressed in WORDS)
    mm_hword    loop_start; // loop start position (expressed in WORDS)

    // frequency divider
    mm_hword    timer;

    // update flags
    mm_byte     flags;

    // source format
    mm_byte     format;     // 0: 8-bit
                            // 1: 16-bit
                            // 2: adpcm
    // repeat mode
    mm_byte     repeat;     // 0: manual
                            // 1: forward loop
                            // 2: one shot

    // volume + divider setting
    mm_byte     volume;     // 0->127
    mm_byte     divider;    // 0->3 = /1, /2, /4, /16

    // panning setting
    mm_byte     panning;    // 0->127

    // index of voice
    mm_byte     index;      // 0->15

    // pad to 20 bytes
    mm_byte     reserved[1];

} mm_voice;

enum
{
    MMVF_FREQ       = 2,    // update frequency when this flag is set
    MMVF_VOLUME     = 4,    // update volume
    MMVF_PANNING    = 8,    // update panning
    MMVF_SOURCE     = 16,   // update source and start note
    MMVF_STOP       = 32    // stop voice (cut sound)
};

// ***************************************************************************
/// @}
/// @defgroup maxmod_playback_events Playback events
/// @{
// ***************************************************************************

/// A special effect has been found in a song.
///
/// This event occurs when an SFx (S3M/IT) or EFx (MOD/XM) effect is parsed from
/// the pattern data. If you have an event handler installed it will be
/// forwarded to there.
///
/// Param contains 'x' from SFx/EFx in the 4 low bits, and the layer in the
/// top 4 bits (MM_MAIN or MM_JINGLE).
///
/// For example, if the jingle is a S3M file with a SF2 effect, param will
/// contain `((MM_JINGLE << 4) | 2)`.
///
/// The chosen effects aren't really used by most songs, it's your
/// responsibility to add them to it to get events when you need them for your
/// application.
#define MMCB_SONGMESSAGE    0x2A

/// A song has finished.
///
/// This event occurs when a module has finished playing the last pattern and
/// has stopped active status. param is MM_MAIN if the main module has finished
/// or MM_JINGLE if the jingle has finished.
#define MMCB_SONGFINISHED   0x2B

/// There has been an error while playing the song.
///
/// The value of param is MM_MAIN or MM_JINGLE, depending on the song that has
/// had the error.
///
/// At the moment, the only way this event can be triggered is if Maxmod is
/// asked to play a song with more channels than available for the layer.
/// Jingles always have 4 available channels. On DS, main modules have 32
/// available channels. On GBA, the user decides how many channels are available
/// when calling `mmInit()` or `mmInitDefault()`.
#define MMCB_SONGERROR      0x2C

// ***************************************************************************
/// @}
// ***************************************************************************

#ifdef __cplusplus
}
#endif

#endif // MM_TYPES_H__
