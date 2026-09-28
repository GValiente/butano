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
#include <maxmod_common.h>

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

/// Returns the VBlank handler previously installed by the user.
///
/// @return
///     Function pointer to the VBlank handler currently installed.
mm_voidfunc mmGetVBlankHandler(void);

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

/// Returns the event handler previously installed by the user.
///
/// @return
///     Function pointer to the event handler currently installed.
mm_callback mmGetEventHandler(void);

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
// ***************************************************************************

#ifdef __cplusplus
}
#endif

#endif // MAXMOD_H__
