/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_COMMON_H
#define BN_HW_COMMON_H

/**
 * @brief Store data in EWRAM.
 */
#define BN_DATA_EWRAM __attribute__((section(".ewram")))

#ifndef BN_EWRAM_BSS_SECTION
    /**
     * @brief Default EWRAM BSS section.
     */
    #define BN_EWRAM_BSS_SECTION ".sbss"
#endif

/**
 * @brief Store uninitialized data in EWRAM.
 */
#define BN_DATA_EWRAM_BSS __attribute__((section(BN_EWRAM_BSS_SECTION)))

/**
 * @brief Store code in IWRAM.
 */
#define BN_CODE_IWRAM __attribute__((section(".iwram")))

/**
 * @brief Store code in EWRAM.
 */
#define BN_CODE_EWRAM __attribute__((section(".ewram")))

/**
 * @brief Creates a compiler level memory barrier forcing optimizer to not re-order memory accesses across the barrier.
 *
 * https://stackoverflow.com/questions/14950614/working-of-asm-volatile-memory
 */
#define BN_BARRIER asm volatile("" ::: "memory")

#endif
