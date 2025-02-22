// Copyright (c) 2003-2021 James Daniels
// Distributed under the MIT License
// license terms: see LICENSE file in root or http://opensource.org/licenses/MIT

// Mixer-specific AAS include
//
// The functions and variables declared here should only be used if you need
// to directly access AAS_MixAudio(). This is not recommended and doing so
// will make it unsafe to use any other AAS functions except AAS_ShowLogo()
// and AAS_DoDMA3(). See the "Mixer" section of the documentation for more
// information.

#ifndef __AAS_MIXER__
#define __AAS_MIXER__

#include "AAS.h"

AAS_BEGIN_DECLS

#define AAS_IN_IWRAM __attribute__((section(".iwram")))
#define AAS_IN_EWRAM __attribute__((section(".ewram")))

struct AAS_Channel {
  AAS_u8 effective_volume; // 0 : effective_volume = (active&volume)?volume:0
  AAS_BOOL active;         // 1 : 0 = Channel inactive, 1 = channel active
  AAS_u8 volume;           // 2 : Total vol of chan set must be <= 256, each
                           //     vol usually in range 0-64
  AAS_u8 pos_fraction;     // 3 : Fraction component of pos
  AAS_u16 frequency;       // 4 : Frequency (Hz)
  AAS_u16 delta;           // 6 : Delta
  const AAS_s8 *pos;       // 8 : Current sample address, fraction component
                           //     in pos_fraction
  const AAS_s8 *end;       // 12 : Address of end of sample
  AAS_u32 loop_length;     // 16 : 0 = No repeat, Other value = Bytes from
                           //      end back to restart point
}; // Length = 20 bytes

extern struct AAS_Channel AAS_channels[16] AAS_IN_EWRAM;
extern AAS_u32 AAS_mix_buffer[640] AAS_IN_EWRAM;

#define AAS_INIT_BRANCH void (*ptr2Function)();
#define AAS_BRANCH(a, b...)                                                    \
  ({                                                                           \
    ptr2Function = a;                                                          \
    ptr2Function(b);                                                           \
  })

// AAS_MixAudio() is in IWRAM, call from ROM as follows: (same for
// AAS_MixAudio_NoChange())
//   AAS_INIT_BRANCH
//   AAS_BRANCH( AAS_MixAudio, mix_buffer, chans, iterations );
void AAS_MixAudio(AAS_s8 *mix_buffer, struct AAS_Channel chans[],
                  int iterations);
void AAS_MixAudio_NoChange(
    AAS_s8 *mix_buffer, struct AAS_Channel chans[],
    int iterations); // Only call if no changes to chans[] since previous call
                     // to AAS_MixAudio(). Do not call twice in a row.

#define AAS_MIXAUDIO_MODE_NORMAL 0 // Total vol must be <= 256,
                                   // normal vol - default
#define AAS_MIXAUDIO_MODE_BOOST  1 // Total vol must be <= 128, double vol
#define AAS_MIXAUDIO_MODE_BOOSTANDCLIP 2 // Total vol must be <= 256, double vol

void AAS_MixAudio_SetMode(int mode); // Set mixer mode, only call if 100% sure
                                     // AAS_MixAudio won't interrupt

// Set maximum number of channels in set (lower=faster), only call if 100% sure
// AAS_MixAudio won't interrupt
void AAS_MixAudio_SetMaxChans_2();
void AAS_MixAudio_SetMaxChans_4(); // Default
void AAS_MixAudio_SetMaxChans_8();

AAS_END_DECLS

#endif
