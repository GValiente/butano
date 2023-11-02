// GBT Player v4.4.1
//
// SPDX-License-Identifier: MIT
//
// Copyright (c) 2020-2022 Antonio Niño Díaz <antonio_nd@outlook.com>

#ifndef GBT_HARDWARE_H__
#define GBT_HARDWARE_H__

#include <stdio.h>
#include "../../../include/bn_hw_common.h"

#define BIT(n)              (1 << (n))

#define EWRAM_BSS BN_DATA_EWRAM_BSS

// Definitions taken from libugba

#define MEM_IO_ADDR         (0x04000000)

#define REG_16(r)           *((volatile uint16_t *)(MEM_IO_ADDR + (r)))
#define REG_32(r)           *((volatile uint32_t *)(MEM_IO_ADDR + (r)))

#define PTR_REG_8(r)        ((volatile uint8_t *)(MEM_IO_ADDR + (r)))
#define PTR_REG_16(r)       ((volatile uint16_t *)(MEM_IO_ADDR + (r)))

#define OFFSET_SOUND1CNT_L  (0x060) // R/W
#define OFFSET_SOUND1CNT_H  (0x062) // R/W
#define OFFSET_SOUND1CNT_X  (0x064) // R/W
#define OFFSET_SOUND2CNT_L  (0x068) // R/W
#define OFFSET_SOUND2CNT_H  (0x06C) // R/W
#define OFFSET_SOUND3CNT_L  (0x070) // R/W
#define OFFSET_SOUND3CNT_H  (0x072) // R/W
#define OFFSET_SOUND3CNT_X  (0x074) // R/W
#define OFFSET_SOUND4CNT_L  (0x078) // R/W
#define OFFSET_SOUND4CNT_H  (0x07C) // R/W
#define OFFSET_SOUNDCNT_L   (0x080) // R/W
#define OFFSET_SOUNDCNT_H   (0x082) // R/W
#define OFFSET_SOUNDCNT_X   (0x084) // R/W
#define OFFSET_WAVE_RAM     (0x090) // R/W -- 2x10h bytes

#define REG_SOUND1CNT_L     REG_16(OFFSET_SOUND1CNT_L)
#define REG_SOUND1CNT_H     REG_16(OFFSET_SOUND1CNT_H)
#define REG_SOUND1CNT_X     REG_16(OFFSET_SOUND1CNT_X)
#define REG_SOUND2CNT_L     REG_16(OFFSET_SOUND2CNT_L)
#define REG_SOUND2CNT_H     REG_16(OFFSET_SOUND2CNT_H)
#define REG_SOUND3CNT_L     REG_16(OFFSET_SOUND3CNT_L)
#define REG_SOUND3CNT_H     REG_16(OFFSET_SOUND3CNT_H)
#define REG_SOUND3CNT_X     REG_16(OFFSET_SOUND3CNT_X)
#define REG_SOUND4CNT_L     REG_16(OFFSET_SOUND4CNT_L)
#define REG_SOUND4CNT_H     REG_16(OFFSET_SOUND4CNT_H)
#define REG_SOUNDCNT_L      REG_16(OFFSET_SOUNDCNT_L)
#define REG_SOUNDCNT_H      REG_16(OFFSET_SOUNDCNT_H)
#define REG_SOUNDCNT_X      REG_16(OFFSET_SOUNDCNT_X)
#define REG_WAVE_RAM        PTR_REG_16(OFFSET_WAVE_RAM)

// Sound Registers
// ---------------

// SOUND1CNT_L (NR10)

#define SOUND1CNT_L_SWEEP_SHIFT_SET(n)  ((n) & 0x7) // R/W
#define SOUND1CNT_L_SWEEP_SHIFT_GET(n)  ((n) & 0x7)

#define SOUND1CNT_L_SWEEP_DIR_INC       (0 << 3) // R/W
#define SOUND1CNT_L_SWEEP_DIR_DEC       (1 << 3)

#define SOUND1CNT_L_SWEEP_TIME_SET(n)   (((n) & 0x7) << 4) // R/W
#define SOUND1CNT_L_SWEEP_TIME_GET(n)   (((n) >> 4) & 0x7)

// SOUND1CNT_H (NR11, NR12)

#define SOUND1CNT_H_LENGTH_SET(n)       ((n) & 0x3F) // WO
#define SOUND1CNT_H_LENGTH_GET(n)       ((n) & 0x3F) // Internal use

#define SOUND1CNT_H_WAVE_DUTY_GET(n)    (((n) >> 6) & 0x3) // R/W
#define SOUND1CNT_H_WAVE_DUTY_12        (0 << 6)
#define SOUND1CNT_H_WAVE_DUTY_25        (1 << 6)
#define SOUND1CNT_H_WAVE_DUTY_50        (2 << 6)
#define SOUND1CNT_H_WAVE_DUTY_75        (3 << 6)

#define SOUND1CNT_H_ENV_STEP_TIME_SET(n) (((n) & 0x7) << 8) // R/W
#define SOUND1CNT_H_ENV_STEP_TIME_GET(n) (((n) >> 8) & 0x7)

#define SOUND1CNT_H_ENV_DIR_DEC         (0 << 11) // R/W
#define SOUND1CNT_H_ENV_DIR_INC         (1 << 11)

#define SOUND1CNT_H_ENV_VOLUME_SET(n)   (((n) & 0xF) << 12) // R/W
#define SOUND1CNT_H_ENV_VOLUME_GET(n)   (((n) >> 12) & 0xF)

// SOUND1CNT_X (NR13, NR14)

#define SOUND1CNT_X_FREQUENCY_SET(n)    ((n) & 0x7FF) // WO
#define SOUND1CNT_X_FREQUENCY_GET(n)    ((n) & 0x7FF) // Internal use

#define SOUND1CNT_X_ONE_SHOT            (1 << 14) // R/W

#define SOUND1CNT_X_RESTART             (1 << 15) // WO

// SOUND2CNT_L (NR21, NR22)

#define SOUND2CNT_L_LENGTH_SET(n)       ((n) & 0x3F) // WO
#define SOUND2CNT_L_LENGTH_GET(n)       ((n) & 0x3F) // Internal use

#define SOUND2CNT_L_WAVE_DUTY_GET(n)    (((n) >> 6) & 0x3) // R/W
#define SOUND2CNT_L_WAVE_DUTY_12        (0 << 6)
#define SOUND2CNT_L_WAVE_DUTY_25        (1 << 6)
#define SOUND2CNT_L_WAVE_DUTY_50        (2 << 6)
#define SOUND2CNT_L_WAVE_DUTY_75        (3 << 6)

#define SOUND2CNT_L_ENV_STEP_TIME_SET(n) (((n) & 0x7) << 8) // R/W
#define SOUND2CNT_L_ENV_STEP_TIME_GET(n) (((n) >> 8) & 0x7)

#define SOUND2CNT_L_ENV_DIR_DEC         (0 << 11) // R/W
#define SOUND2CNT_L_ENV_DIR_INC         (1 << 11)

#define SOUND2CNT_L_ENV_VOLUME_SET(n)   (((n) & 0xF) << 12) // R/W
#define SOUND2CNT_L_ENV_VOLUME_GET(n)   (((n) >> 12) & 0xF)

// SOUND2CNT_H (NR23, NR24)

#define SOUND2CNT_H_FREQUENCY_SET(n)    ((n) & 0x7FF) // WO
#define SOUND2CNT_H_FREQUENCY_GET(n)    ((n) & 0x7FF) // Internal use

#define SOUND2CNT_H_ONE_SHOT            (1 << 14) // R/W

#define SOUND2CNT_H_RESTART             (1 << 15) // WO

// SOUND3CNT_L (NR30)

#define SOUND3CNT_L_SIZE_32             (0 << 5)
#define SOUND3CNT_L_SIZE_64             (1 << 5)

#define SOUND3CNT_L_BANK_SET(n)         (((n) & 1) << 6)
#define SOUND3CNT_L_BANK_GET(n)         (((n) >> 6) & 1)

#define SOUND3CNT_L_ENABLE              (1 << 7)
#define SOUND3CNT_L_DISABLE             (0 << 7)

// SOUND3CNT_H (NR31, NR32)

#define SOUND3CNT_H_LENGTH_SET(n)       ((n) & 0xFF) // R/W
#define SOUND3CNT_H_LENGTH_GET(n)       ((n) & 0xFF)

#define SOUND3CNT_H_VOLUME_0            (0x0 << 13) // R/W
#define SOUND3CNT_H_VOLUME_25           (0x3 << 13)
#define SOUND3CNT_H_VOLUME_50           (0x2 << 13)
#define SOUND3CNT_H_VOLUME_75           (0x4 << 13)
#define SOUND3CNT_H_VOLUME_100          (0x1 << 13)
#define SOUND3CNT_H_VOLUME_MASK         (0x7 << 13)

// SOUND3CNT_X (NR33, NR34)

#define SOUND3CNT_X_SAMPLE_RATE_SET(n)  ((n) & 0x7FF) // WO
#define SOUND3CNT_X_SAMPLE_RATE_GET(n)  ((n) & 0x7FF) // Internal use

#define SOUND3CNT_X_ONE_SHOT            (1 << 14) // R/W

#define SOUND3CNT_X_RESTART             (1 << 15) // WO

// SOUND4CNT_L (NR41, NR42)

#define SOUND4CNT_L_LENGTH_SET(n)       ((n) & 0x3F) // WO
#define SOUND4CNT_L_LENGTH_GET(n)       ((n) & 0x3F) // Internal use

#define SOUND4CNT_L_ENV_STEP_TIME_SET(n) (((n) & 0x7) << 8) // R/W
#define SOUND4CNT_L_ENV_STEP_TIME_GET(n) (((n) >> 8) & 0x7)

#define SOUND4CNT_L_ENV_DIR_DEC         (0 << 11) // R/W
#define SOUND4CNT_L_ENV_DIR_INC         (1 << 11)

#define SOUND4CNT_L_ENV_VOLUME_SET(n)   (((n) & 0xF) << 12) // R/W
#define SOUND4CNT_L_ENV_VOLUME_GET(n)   (((n) >> 12) & 0xF)

// SOUND4CNT_H (NR43, NR44)

#define SOUND4CNT_H_DIV_RATIO_SET(n)    ((n) & 0x7) // R/W
#define SOUND4CNT_H_DIV_RATIO_GET(n)    ((n) & 0x7)

#define SOUND4CNT_H_WIDTH_15_BITS       (0 << 3) // R/W
#define SOUND4CNT_H_WIDTH_7_BITS        (1 << 3)

#define SOUND4CNT_H_FREQUENCY_SET(n)    (((n) & 0xF) << 4) // R/W
#define SOUND4CNT_H_FREQUENCY_GET(n)    (((n) >> 4) & 0xF)

#define SOUND4CNT_H_ONE_SHOT            (1 << 14) // R/W

#define SOUND4CNT_H_RESTART             (1 << 15) // WO

// SOUNDCNT_L

#define SOUNDCNT_L_PSG_VOL_RIGHT_SET(v) (((v) & 0x7) << 0)
#define SOUNDCNT_L_PSG_VOL_RIGHT_GET(v) (((v) >> 0) & 0x7)
#define SOUNDCNT_L_PSG_VOL_LEFT_SET(v)  (((v) & 0x7) << 4)
#define SOUNDCNT_L_PSG_VOL_LEFT_GET(v)  (((v) >> 4) & 0x7)

#define SOUNDCNT_L_PSG_1_ENABLE_RIGHT   (1 << 8)
#define SOUNDCNT_L_PSG_2_ENABLE_RIGHT   (1 << 9)
#define SOUNDCNT_L_PSG_3_ENABLE_RIGHT   (1 << 10)
#define SOUNDCNT_L_PSG_4_ENABLE_RIGHT   (1 << 11)

#define SOUNDCNT_L_PSG_1_ENABLE_LEFT    (1 << 12)
#define SOUNDCNT_L_PSG_2_ENABLE_LEFT    (1 << 13)
#define SOUNDCNT_L_PSG_3_ENABLE_LEFT    (1 << 14)
#define SOUNDCNT_L_PSG_4_ENABLE_LEFT    (1 << 15)

// SOUNDCNT_H

#define SOUNDCNT_H_PSG_VOLUME_25        (0 << 0)
#define SOUNDCNT_H_PSG_VOLUME_50        (1 << 0)
#define SOUNDCNT_H_PSG_VOLUME_100       (2 << 0)
#define SOUNDCNT_H_PSG_VOLUME_MASK      (3 << 0)

#define SOUNDCNT_H_DMA_A_VOLUME_50      (0 << 2)
#define SOUNDCNT_H_DMA_A_VOLUME_100     (1 << 2)

#define SOUNDCNT_H_DMA_B_VOLUME_50      (0 << 3)
#define SOUNDCNT_H_DMA_B_VOLUME_100     (1 << 3)

#define SOUNDCNT_H_DMA_A_ENABLE_RIGHT   (1 << 8)
#define SOUNDCNT_H_DMA_A_ENABLE_LEFT    (1 << 9)

#define SOUNDCNT_H_DMA_A_TIMER0         (0 << 10)
#define SOUNDCNT_H_DMA_A_TIMER1         (1 << 10)

#define SOUNDCNT_H_DMA_A_RESET          (1 << 11)

#define SOUNDCNT_H_DMA_B_ENABLE_RIGHT   (1 << 12)
#define SOUNDCNT_H_DMA_B_ENABLE_LEFT    (1 << 13)

#define SOUNDCNT_H_DMA_B_TIMER0         (0 << 14)
#define SOUNDCNT_H_DMA_B_TIMER1         (1 << 14)

#define SOUNDCNT_H_DMA_B_RESET          (1 << 15)

// SOUNDCNT_X

#define SOUNDCNT_X_PSG_1_IS_ON          (1 << 0) // Read-only flags
#define SOUNDCNT_X_PSG_2_IS_ON          (1 << 1)
#define SOUNDCNT_X_PSG_3_IS_ON          (1 << 2)
#define SOUNDCNT_X_PSG_4_IS_ON          (1 << 3)

#define SOUNDCNT_X_MASTER_ENABLE        (1 << 7)
#define SOUNDCNT_X_MASTER_DISABLE       (0 << 7)

#endif // GBT_HARDWARE_H__
