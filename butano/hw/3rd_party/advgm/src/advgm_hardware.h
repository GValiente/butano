// SPDX-FileCopyrightText: Copyright 2020-2022 Antonio Niño Díaz
// SPDX-FileCopyrightText: Copyright 2026 copyrat90
// SPDX-License-Identifier: 0BSD

// GBA Audio Registers: https://gist.github.com/copyrat90/36cbc9b1be2477aae64361cc93db9646

// I/O registers naming scheme : Tonc's
// their values  naming scheme : UGBA's

#ifndef ADVGM_HARDWARE_H
#define ADVGM_HARDWARE_H

#include <stdint.h>

#define ADVGM_ARM_CODE __attribute__((target("arm")))
#define ADVGM_THUMB_CODE __attribute__((target("thumb")))
#define ADVGM_IWRAM_BSS  // IWRAM is the default location for .bss symbols
#define ADVGM_IWRAM_DATA // IWRAM is the default location for .data symbols
#define ADVGM_IWRAM_CODE __attribute__((section(".iwram_code"), long_call))
#define ADVGM_EWRAM_BSS __attribute__((section(".sbss")))
#define ADVGM_EWRAM_DATA __attribute__((section(".ewram_data")))
#define ADVGM_EWRAM_CODE __attribute__((section(".ewram_code"), long_call))

// Registers
// ---------------

#define ADVGM_MEM_IO_ADDR (0x04000000)
#define ADVGM_MEM_IO ((void*)ADVGM_MEM_IO_ADDR)

#define ADVGM_REG_8(r) *((volatile uint8_t*)(ADVGM_MEM_IO_ADDR + (r)))
#define ADVGM_REG_16(r) *((volatile uint16_t*)(ADVGM_MEM_IO_ADDR + (r)))
#define ADVGM_REG_32(r) *((volatile uint32_t*)(ADVGM_MEM_IO_ADDR + (r)))

#define ADVGM_PTR_REG_8(r) ((volatile uint8_t*)(ADVGM_MEM_IO_ADDR + (r)))
#define ADVGM_PTR_REG_16(r) ((volatile uint16_t*)(ADVGM_MEM_IO_ADDR + (r)))
#define ADVGM_PTR_REG_32(r) ((volatile uint32_t*)(ADVGM_MEM_IO_ADDR + (r)))

#define ADVGM_OFFSET_SND1SWEEP (0x060) // R/W
#define ADVGM_OFFSET_SND1CNT (0x062)   // R/W
#define ADVGM_OFFSET_SND1FREQ (0x064)  // R/W
#define ADVGM_OFFSET_SND2CNT (0x068)   // R/W
#define ADVGM_OFFSET_SND2FREQ (0x06C)  // R/W
#define ADVGM_OFFSET_SND3SEL (0x070)   // R/W
#define ADVGM_OFFSET_SND3CNT (0x072)   // R/W
#define ADVGM_OFFSET_SND3FREQ (0x074)  // R/W
#define ADVGM_OFFSET_SND4CNT (0x078)   // R/W
#define ADVGM_OFFSET_SND4FREQ (0x07C)  // R/W
#define ADVGM_OFFSET_SNDDMGCNT (0x080) // R/W
#define ADVGM_OFFSET_SNDDSCNT (0x082)  // R/W
#define ADVGM_OFFSET_SNDSTAT (0x084)   // R/W
#define ADVGM_OFFSET_SNDBIAS (0x088)   // BIOS

#define ADVGM_OFFSET_WAVE_RAM (0x090)    // R/W -- 2x10h bytes
#define ADVGM_OFFSET_WAVE_RAM0 (0x090)   // R/W
#define ADVGM_OFFSET_WAVE_RAM0_L (0x090) // R/W
#define ADVGM_OFFSET_WAVE_RAM0_H (0x092) // R/W
#define ADVGM_OFFSET_WAVE_RAM1 (0x094)   // R/W
#define ADVGM_OFFSET_WAVE_RAM1_L (0x094) // R/W
#define ADVGM_OFFSET_WAVE_RAM1_H (0x096) // R/W
#define ADVGM_OFFSET_WAVE_RAM2 (0x098)   // R/W
#define ADVGM_OFFSET_WAVE_RAM2_L (0x098) // R/W
#define ADVGM_OFFSET_WAVE_RAM2_H (0x09A) // R/W
#define ADVGM_OFFSET_WAVE_RAM3 (0x09C)   // R/W
#define ADVGM_OFFSET_WAVE_RAM3_L (0x09C) // R/W
#define ADVGM_OFFSET_WAVE_RAM3_H (0x09E) // R/W

#define ADVGM_REG_SND1SWEEP ADVGM_REG_16(ADVGM_OFFSET_SND1SWEEP) // Sound 1 Sweep control
#define ADVGM_REG_SND1CNT ADVGM_REG_16(ADVGM_OFFSET_SND1CNT)     // Sound 1 Length, wave duty and envelope control
#define ADVGM_REG_SND1FREQ ADVGM_REG_16(ADVGM_OFFSET_SND1FREQ)   // Sound 1 Frequency, reset and loop control
#define ADVGM_REG_SND2CNT ADVGM_REG_16(ADVGM_OFFSET_SND2CNT)     // Sound 2 Length, wave duty and envelope control
#define ADVGM_REG_SND2FREQ ADVGM_REG_16(ADVGM_OFFSET_SND2FREQ)   // Sound 2 Frequency, reset and loop control
#define ADVGM_REG_SND3SEL ADVGM_REG_16(ADVGM_OFFSET_SND3SEL)     // Sound 3 Enable and wave ram bank control
#define ADVGM_REG_SND3CNT ADVGM_REG_16(ADVGM_OFFSET_SND3CNT)     // Sound 3 Sound length and output level control
#define ADVGM_REG_SND3FREQ ADVGM_REG_16(ADVGM_OFFSET_SND3FREQ)   // Sound 3 Frequency, reset and loop control
#define ADVGM_REG_SND4CNT ADVGM_REG_16(ADVGM_OFFSET_SND4CNT)     // Sound 4 Length, output level and envelope control
#define ADVGM_REG_SND4FREQ ADVGM_REG_16(ADVGM_OFFSET_SND4FREQ)   // Sound 4 Noise parameters, reset and loop control
#define ADVGM_REG_SNDDMGCNT ADVGM_REG_16(ADVGM_OFFSET_SNDDMGCNT) // Sound 1-4 Output level and Stereo control
#define ADVGM_REG_SNDDSCNT ADVGM_REG_16(ADVGM_OFFSET_SNDDSCNT)   // Direct Sound control and Sound 1-4 output ratio
#define ADVGM_REG_SNDSTAT ADVGM_REG_16(ADVGM_OFFSET_SNDSTAT)     // Master sound enable and Sound 1-4 play status
#define ADVGM_REG_SNDBIAS ADVGM_REG_16(ADVGM_OFFSET_SNDBIAS)     // Sound bias and Amplitude resolution control

#define ADVGM_REG_WAVE_RAM ADVGM_PTR_REG_32(ADVGM_OFFSET_WAVE_RAM)   // Sound 3 samples pointer
#define ADVGM_REG_WAVE_RAM0 ADVGM_REG_32(ADVGM_OFFSET_WAVE_RAM0)     // Sound 3 samples 0-7
#define ADVGM_REG_WAVE_RAM0_L ADVGM_REG_16(ADVGM_OFFSET_WAVE_RAM0_L) // Sound 3 samples 0-3
#define ADVGM_REG_WAVE_RAM0_H ADVGM_REG_16(ADVGM_OFFSET_WAVE_RAM0_H) // Sound 3 samples 4-7
#define ADVGM_REG_WAVE_RAM1 ADVGM_REG_32(ADVGM_OFFSET_WAVE_RAM1)     // Sound 3 samples 8-15
#define ADVGM_REG_WAVE_RAM1_L ADVGM_REG_16(ADVGM_OFFSET_WAVE_RAM1_L) // Sound 3 samples 8-11
#define ADVGM_REG_WAVE_RAM1_H ADVGM_REG_16(ADVGM_OFFSET_WAVE_RAM1_H) // Sound 3 samples 12-15
#define ADVGM_REG_WAVE_RAM2 ADVGM_REG_32(ADVGM_OFFSET_WAVE_RAM2)     // Sound 3 samples 16-23
#define ADVGM_REG_WAVE_RAM2_L ADVGM_REG_16(ADVGM_OFFSET_WAVE_RAM2_L) // Sound 3 samples 16-19
#define ADVGM_REG_WAVE_RAM2_H ADVGM_REG_16(ADVGM_OFFSET_WAVE_RAM2_H) // Sound 3 samples 20-23
#define ADVGM_REG_WAVE_RAM3 ADVGM_REG_32(ADVGM_OFFSET_WAVE_RAM3)     // Sound 3 samples 24-31
#define ADVGM_REG_WAVE_RAM3_L ADVGM_REG_16(ADVGM_OFFSET_WAVE_RAM3_L) // Sound 3 samples 24-27
#define ADVGM_REG_WAVE_RAM3_H ADVGM_REG_16(ADVGM_OFFSET_WAVE_RAM3_H) // Sound 3 samples 28-31

// Register values
// ---------------

// SND1SWEEP (NR10) (SOUND1CNT_L)

#define ADVGM_SND1SWEEP_SWEEP_SHIFT_SET(n) ((n) & 0x7) // R/W
#define ADVGM_SND1SWEEP_SWEEP_SHIFT_GET(n) ((n) & 0x7)

#define ADVGM_SND1SWEEP_SWEEP_DIR_INC (0 << 3) // R/W
#define ADVGM_SND1SWEEP_SWEEP_DIR_DEC (1 << 3)

#define ADVGM_SND1SWEEP_SWEEP_TIME_SET(n) (((n) & 0x7) << 4) // R/W
#define ADVGM_SND1SWEEP_SWEEP_TIME_GET(n) (((n) >> 4) & 0x7)

// SND1CNT (NR11, NR12) (SOUND1CNT_H)

#define ADVGM_SND1CNT_LENGTH_SET(n) ((n) & 0x3F) // WO
#define ADVGM_SND1CNT_LENGTH_GET(n) ((n) & 0x3F) // Internal use

#define ADVGM_SND1CNT_WAVE_DUTY_GET(n) (((n) >> 6) & 0x3) // R/W
#define ADVGM_SND1CNT_WAVE_DUTY_12 (0 << 6)
#define ADVGM_SND1CNT_WAVE_DUTY_25 (1 << 6)
#define ADVGM_SND1CNT_WAVE_DUTY_50 (2 << 6)
#define ADVGM_SND1CNT_WAVE_DUTY_75 (3 << 6)

#define ADVGM_SND1CNT_ENV_STEP_TIME_SET(n) (((n) & 0x7) << 8) // R/W
#define ADVGM_SND1CNT_ENV_STEP_TIME_GET(n) (((n) >> 8) & 0x7)

#define ADVGM_SND1CNT_ENV_DIR_DEC (0 << 11) // R/W
#define ADVGM_SND1CNT_ENV_DIR_INC (1 << 11)

#define ADVGM_SND1CNT_ENV_VOLUME_SET(n) (((n) & 0xF) << 12) // R/W
#define ADVGM_SND1CNT_ENV_VOLUME_GET(n) (((n) >> 12) & 0xF)

// SND1FREQ (NR13, NR14) (SOUND1CNT_X)

#define ADVGM_SND1FREQ_PERIOD_SET(n) ((n) & 0x7FF) // WO
#define ADVGM_SND1FREQ_PERIOD_GET(n) ((n) & 0x7FF) // Internal use

#define ADVGM_SND1FREQ_LENGTH_ENABLE (1 << 14) // R/W

#define ADVGM_SND1FREQ_RETRIG (1 << 15) // WO

// SND2CNT (NR21, NR22) (SOUND2CNT_L)

#define ADVGM_SND2CNT_LENGTH_SET(n) ((n) & 0x3F) // WO
#define ADVGM_SND2CNT_LENGTH_GET(n) ((n) & 0x3F) // Internal use

#define ADVGM_SND2CNT_WAVE_DUTY_GET(n) (((n) >> 6) & 0x3) // R/W
#define ADVGM_SND2CNT_WAVE_DUTY_12 (0 << 6)
#define ADVGM_SND2CNT_WAVE_DUTY_25 (1 << 6)
#define ADVGM_SND2CNT_WAVE_DUTY_50 (2 << 6)
#define ADVGM_SND2CNT_WAVE_DUTY_75 (3 << 6)

#define ADVGM_SND2CNT_ENV_STEP_TIME_SET(n) (((n) & 0x7) << 8) // R/W
#define ADVGM_SND2CNT_ENV_STEP_TIME_GET(n) (((n) >> 8) & 0x7)

#define ADVGM_SND2CNT_ENV_DIR_DEC (0 << 11) // R/W
#define ADVGM_SND2CNT_ENV_DIR_INC (1 << 11)

#define ADVGM_SND2CNT_ENV_VOLUME_SET(n) (((n) & 0xF) << 12) // R/W
#define ADVGM_SND2CNT_ENV_VOLUME_GET(n) (((n) >> 12) & 0xF)

// SND2FREQ (NR23, NR24) (SOUND2CNT_H)

#define ADVGM_SND2FREQ_PERIOD_SET(n) ((n) & 0x7FF) // WO
#define ADVGM_SND2FREQ_PERIOD_GET(n) ((n) & 0x7FF) // Internal use

#define ADVGM_SND2FREQ_LENGTH_ENABLE (1 << 14) // R/W

#define ADVGM_SND2FREQ_RETRIG (1 << 15) // WO

// SND3SEL (NR30) (SOUND3CNT_L)

#define ADVGM_SND3SEL_SIZE_32 (0 << 5)
#define ADVGM_SND3SEL_SIZE_64 (1 << 5)

#define ADVGM_SND3SEL_BANK_SET(n) (((n) & 1) << 6)
#define ADVGM_SND3SEL_BANK_GET(n) (((n) >> 6) & 1)

#define ADVGM_SND3SEL_ENABLE (1 << 7)
#define ADVGM_SND3SEL_DISABLE (0 << 7)

// SND3CNT (NR31, NR32) (SOUND3CNT_H)

#define ADVGM_SND3CNT_LENGTH_SET(n) ((n) & 0xFF) // R/W
#define ADVGM_SND3CNT_LENGTH_GET(n) ((n) & 0xFF)

#define ADVGM_SND3CNT_VOLUME_0 (0x0 << 13) // R/W
#define ADVGM_SND3CNT_VOLUME_25 (0x3 << 13)
#define ADVGM_SND3CNT_VOLUME_50 (0x2 << 13)
#define ADVGM_SND3CNT_VOLUME_75 (0x4 << 13)
#define ADVGM_SND3CNT_VOLUME_100 (0x1 << 13)
#define ADVGM_SND3CNT_VOLUME_MASK (0x7 << 13)

// SND3FREQ (NR33, NR34) (SOUND3CNT_X)

#define ADVGM_SND3FREQ_PERIOD_SET(n) ((n) & 0x7FF) // WO
#define ADVGM_SND3FREQ_PERIOD_GET(n) ((n) & 0x7FF) // Internal use

#define ADVGM_SND3FREQ_LENGTH_ENABLE (1 << 14) // R/W

#define ADVGM_SND3FREQ_RETRIG (1 << 15) // WO

// SND4CNT (NR41, NR42) (SOUND4CNT_L)

#define ADVGM_SND4CNT_LENGTH_SET(n) ((n) & 0x3F) // WO
#define ADVGM_SND4CNT_LENGTH_GET(n) ((n) & 0x3F) // Internal use

#define ADVGM_SND4CNT_ENV_STEP_TIME_SET(n) (((n) & 0x7) << 8) // R/W
#define ADVGM_SND4CNT_ENV_STEP_TIME_GET(n) (((n) >> 8) & 0x7)

#define ADVGM_SND4CNT_ENV_DIR_DEC (0 << 11) // R/W
#define ADVGM_SND4CNT_ENV_DIR_INC (1 << 11)

#define ADVGM_SND4CNT_ENV_VOLUME_SET(n) (((n) & 0xF) << 12) // R/W
#define ADVGM_SND4CNT_ENV_VOLUME_GET(n) (((n) >> 12) & 0xF)

// SND4FREQ (NR43, NR44) (SOUND4CNT_H)

#define ADVGM_SND4FREQ_DIV_RATIO_SET(n) ((n) & 0x7) // R/W
#define ADVGM_SND4FREQ_DIV_RATIO_GET(n) ((n) & 0x7)

#define ADVGM_SND4FREQ_WIDTH_15_BITS (0 << 3) // R/W
#define ADVGM_SND4FREQ_WIDTH_7_BITS (1 << 3)

#define ADVGM_SND4FREQ_PRE_STEP_RATIO_SET(n) (((n) & 0xF) << 4) // R/W
#define ADVGM_SND4FREQ_PRE_STEP_RATIO_GET(n) (((n) >> 4) & 0xF)

#define ADVGM_SND4FREQ_LENGTH_ENABLE (1 << 14) // R/W

#define ADVGM_SND4FREQ_RETRIG (1 << 15) // WO

// SNDDMGCNT (SOUNDCNT_L)

#define ADVGM_SNDDMGCNT_PSG_VOL_RIGHT_SET(v) (((v) & 0x7) << 0)
#define ADVGM_SNDDMGCNT_PSG_VOL_RIGHT_GET(v) (((v) >> 0) & 0x7)
#define ADVGM_SNDDMGCNT_PSG_VOL_LEFT_SET(v) (((v) & 0x7) << 4)
#define ADVGM_SNDDMGCNT_PSG_VOL_LEFT_GET(v) (((v) >> 4) & 0x7)

#define ADVGM_SNDDMGCNT_PSG_1_ENABLE_RIGHT (1 << 8)
#define ADVGM_SNDDMGCNT_PSG_2_ENABLE_RIGHT (1 << 9)
#define ADVGM_SNDDMGCNT_PSG_3_ENABLE_RIGHT (1 << 10)
#define ADVGM_SNDDMGCNT_PSG_4_ENABLE_RIGHT (1 << 11)

#define ADVGM_SNDDMGCNT_PSG_1_ENABLE_LEFT (1 << 12)
#define ADVGM_SNDDMGCNT_PSG_2_ENABLE_LEFT (1 << 13)
#define ADVGM_SNDDMGCNT_PSG_3_ENABLE_LEFT (1 << 14)
#define ADVGM_SNDDMGCNT_PSG_4_ENABLE_LEFT (1 << 15)

// SNDDSCNT (SOUNDCNT_H)

#define ADVGM_SNDDSCNT_PSG_VOLUME_25 (0 << 0)
#define ADVGM_SNDDSCNT_PSG_VOLUME_50 (1 << 0)
#define ADVGM_SNDDSCNT_PSG_VOLUME_100 (2 << 0)
#define ADVGM_SNDDSCNT_PSG_VOLUME_MASK (3 << 0)

#define ADVGM_SNDDSCNT_DMA_A_VOLUME_50 (0 << 2)
#define ADVGM_SNDDSCNT_DMA_A_VOLUME_100 (1 << 2)

#define ADVGM_SNDDSCNT_DMA_B_VOLUME_50 (0 << 3)
#define ADVGM_SNDDSCNT_DMA_B_VOLUME_100 (1 << 3)

#define ADVGM_SNDDSCNT_DMA_A_ENABLE_RIGHT (1 << 8)
#define ADVGM_SNDDSCNT_DMA_A_ENABLE_LEFT (1 << 9)

#define ADVGM_SNDDSCNT_DMA_A_TIMER0 (0 << 10)
#define ADVGM_SNDDSCNT_DMA_A_TIMER1 (1 << 10)

#define ADVGM_SNDDSCNT_DMA_A_RESET (1 << 11)

#define ADVGM_SNDDSCNT_DMA_B_ENABLE_RIGHT (1 << 12)
#define ADVGM_SNDDSCNT_DMA_B_ENABLE_LEFT (1 << 13)

#define ADVGM_SNDDSCNT_DMA_B_TIMER0 (0 << 14)
#define ADVGM_SNDDSCNT_DMA_B_TIMER1 (1 << 14)

#define ADVGM_SNDDSCNT_DMA_B_RESET (1 << 15)

// SNDSTAT (SOUNDCNT_X)

#define ADVGM_SNDSTAT_PSG_1_IS_ON (1 << 0) // Read-only flags
#define ADVGM_SNDSTAT_PSG_2_IS_ON (1 << 1)
#define ADVGM_SNDSTAT_PSG_3_IS_ON (1 << 2)
#define ADVGM_SNDSTAT_PSG_4_IS_ON (1 << 3)

#define ADVGM_SNDSTAT_MASTER_ENABLE (1 << 7)
#define ADVGM_SNDSTAT_MASTER_DISABLE (0 << 7)

// SNDBIAS (SOUNDBIAS)

#define ADVGM_SNDBIAS_BIAS_LEVEL_SET(v) (((v) & 0x1FF) << 1)
#define ADVGM_SNDBIAS_BIAS_LEVEL_GET(v) (((v) >> 1) & 0x1FF)
#define ADVGM_SNDBIAS_BIAS_MASK (0x1FF << 1)

#define ADVGM_SNDBIAS_SAMPLE_RATE_32KHZ (0 << 14) // Best for DMA channels A,B
#define ADVGM_SNDBIAS_SAMPLE_RATE_65KHZ (1 << 14)
#define ADVGM_SNDBIAS_SAMPLE_RATE_131KHZ (2 << 14)
#define ADVGM_SNDBIAS_SAMPLE_RATE_262KHZ (3 << 14) // Best for PSG channels 1-4

#endif // ADVGM_HARDWARE_H
