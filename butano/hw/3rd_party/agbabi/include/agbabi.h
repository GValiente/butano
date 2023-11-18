/*
===============================================================================

 C header file for agbabi

 Copyright (C) 2021-2023 agbabi contributors
 For conditions of distribution and use, see copyright notice in LICENSE.md

===============================================================================
*/

#ifndef AGBABI_H
#define AGBABI_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/**
 * Signed 64-bit division
 * @param numerator
 * @param denominator
 * @return quotient
 */
long long __agbabi_ldiv(long long numerator, long long denominator) __attribute__((const));

/**
 * Unsigned 64-bit division
 * @param numerator
 * @param denominator
 * @return quotient
 */
unsigned long long __agbabi_uldiv(unsigned long long numerator, unsigned long long denominator) __attribute__((const));

/**
 * Unsigned 64-bit division and modulo
 * Check for divide by zero is not performed
 * @param numerator
 * @param denominator
 * @return [quotient, remainder]
 */
unsigned long long __attribute__((vector_size(sizeof(unsigned long long) * 2))) __agbabi_unsafe_uldivmod(unsigned long long numerator, unsigned long long denominator) __attribute__((const));

/**
 * Unsigned 64-bit / 32-bit -> 64-bit division
 * @param numerator
 * @param denominator
 * @return quotient
 */
unsigned long long __agbabi_uluidiv(unsigned long long numerator, unsigned int denominator) __attribute__((const));

/**
 * Unsigned 64-bit / 32-bit -> 64-bit division and modulo
 * @param numerator
 * @param denominator
 * @return [quotient, remainder]
 */
unsigned long long __attribute__((vector_size(sizeof(unsigned long long) * 2))) __agbabi_uluidivmod(unsigned long long numerator, unsigned int denominator) __attribute__((const));

/**
 * Unsigned 64-bit / 32-bit -> 64-bit division and modulo
 * Check for divide by zero is not performed
 * @param numerator
 * @param denominator
 * @return [quotient, remainder]
 */
unsigned long long __attribute__((vector_size(sizeof(unsigned long long) * 2))) __agbabi_unsafe_uluidivmod(unsigned long long numerator, unsigned int denominator) __attribute__((const));

/**
 * Unsigned 32-bit division and modulo
 * Check for divide by zero is not performed
 * @param numerator
 * @param denominator
 * @return [quotient, remainder]
 */
unsigned int __attribute__((vector_size(sizeof(unsigned int) * 2))) __agbabi_unsafe_uidivmod(unsigned int numerator, unsigned int denominator) __attribute__((const));

/**
 * Copies n bytes from src to dest (forward)
 * Assumes dest and src are 2-byte aligned
 * @param dest Destination address
 * @param src Source address
 * @param n Number of bytes to copy
 */
void __agbabi_memcpy2(void* __restrict__ dest, const void* __restrict__ src, size_t n) __attribute__((nonnull(1, 2)));

/**
 * Copies n bytes from src to dest (forward)
 * This is a slow, unaligned, byte-by-byte copy: ideal for SRAM
 * @param dest Destination address
 * @param src Source address
 * @param n Number of bytes to copy
 */
void __agbabi_memcpy1(void* __restrict__ dest, const void* __restrict__ src, size_t n) __attribute__((nonnull(1, 2)));

/**
 * Copies n bytes from src to dest (backwards)
 * This is a slow, unaligned, byte-by-byte copy: ideal for SRAM
 * @param dest Destination address
 * @param src Source address
 * @param n Number of bytes to copy
 */
void __agbabi_rmemcpy1(void* __restrict__ dest, const void* __restrict__ src, size_t n) __attribute__((nonnull(1, 2)));

/**
 * Copies n bytes from src to dest (backwards)
 * @param dest Destination address
 * @param src Source address
 * @param n Number of bytes to copy
 */
void __agbabi_rmemcpy(void* __restrict__ dest, const void* __restrict__ src, size_t n) __attribute__((nonnull(1, 2)));

/**
 * Copies n bytes in multiples of 16 bytes from src to dest (forward) using FIQ mode
 * Assumes dest and src are 4-byte aligned
 * @param dest Destination address
 * @param src Source address
 * @param n Number of bytes to copy, must be a multiple of 16
 */
void __agbabi_fiq_memcpy4x4(void* __restrict__ dest, const void* __restrict__ src, size_t n) __attribute__((nonnull(1, 2)));

/**
 * Copies n bytes from src to dest (forward) using FIQ mode
 * Assumes dest and src are 4-byte aligned
 * @param dest Destination address
 * @param src Source address
 * @param n Number of bytes to copy
 */
void __agbabi_fiq_memcpy4(void* __restrict__ dest, const void* __restrict__ src, size_t n) __attribute__((nonnull(1, 2)));

/**
 * Fills dest with n bytes of c
 * Assumes dest is 4-byte aligned
 * Trailing copy uses the low word of c, and the low byte of c
 * @param dest Destination address
 * @param n Number of bytes to set
 * @param c Value to set
 */
void __agbabi_lwordset4(void* dest, size_t n, long long c) __attribute__((nonnull(1)));

/**
 * Fills dest with n bytes of c
 * Assumes dest is 4-byte aligned
 * Trailing copy uses the low byte of c
 * @param dest Destination address
 * @param n Number of bytes to set
 * @param c Value to set
 */
void __agbabi_wordset4(void* dest, size_t n, int c) __attribute__((nonnull(1)));

/**
 * Fixed-point sine approximation
 * @param x 15-bit binary angle measurement
 * @return Q29 signed fixed point between -1 and +1
 */
int __agbabi_sin(int x) __attribute__((const));

/**
 * Calculates the arc tangent of x, y
 * @param x Q12 signed fixed point coord around circle
 * @param y Q12 signed fixed point coord around circle
 * @return 15-bit binary angle measurement
 */
unsigned int __agbabi_atan2(int x, int y) __attribute__((const));

/**
 * Calculates the integer square root of x
 * @param x
 * @return Square root of x
 */
int __agbabi_sqrt(unsigned int x) __attribute__((const));

/**
 * Empty IRQ handler that acknowledges raised IRQs
 */
void __agbabi_irq_empty(void) __attribute__((error("IRQ handler should not be directly called")));

/**
 * Nested IRQ handler that calls __agbabi_irq_user_fn with the raised IRQ flags
 */
void __agbabi_irq_user(void) __attribute__((error("IRQ handler should not be directly called")));

/**
 * Handler called by __agbabi_irq_user
 * @param irqFlags 16-bit mask of the raised flags
 */
extern void(*__agbabi_irq_user_fn)(int irqFlags);

/**
 * Coroutine state
 * @param arm_sp Pointer to coroutine stack
 * @param joined Flag if the coroutine has joined
 */
typedef struct {
    unsigned int arm_sp : 31;
    unsigned int joined : 1;
} __agbabi_coro_t;

/**
 * Initializes a coro struct to call a given coroutine
 * @param coro pointer to coro struct to initialize
 * @param sp_top the TOP of the stack for this coroutine (stack grows down!)
 * @param coproc procedure to call as a coroutine
 */
void __agbabi_coro_make(__agbabi_coro_t* __restrict__ coro, void* __restrict__ sp_top, int(*coproc)(__agbabi_coro_t*)) __attribute__((nonnull(1, 2, 3)));

/**
 * Starts/resumes a given coroutine
 * @param coro coroutine to start/resume
 * @return integer value from coroutine
 */
int __agbabi_coro_resume(__agbabi_coro_t* coro) __attribute__((nonnull(1)));

/**
 * Yields a given value of a coroutine back to the caller
 * @param coro coroutine that is yielding
 * @param value returned to caller
 */
void __agbabi_coro_yield(__agbabi_coro_t* coro, int value) __attribute__((nonnull(1)));

/**
 * Initialize GPIO pins for RTC
 * @return 0 for success
 */
int __agbabi_rtc_init(void);

/**
 * Get the current, raw time from the RTC
 * @return Raw time in BCD
 */
unsigned int __agbabi_rtc_time(void);

/**
 * Set the Hour, Minute, Second
 * @param time raw BCD (second, minute, hour)
 */
void __agbabi_rtc_settime(unsigned int time);

/**
 * [raw time in BCD, raw date in BCD]
 */
typedef unsigned int __attribute__((vector_size(sizeof(unsigned int) * 2))) __agbabi_datetime_t;

/**
 * Get the current, raw date & time from the RTC
 * @return [raw time in BCD, raw date in BCD]
 */
__agbabi_datetime_t __agbabi_rtc_datetime(void);

/**
 * Set the time and date
 * @param datetime [raw BCD time, raw BCD date]
 */
void __agbabi_rtc_setdatetime(__agbabi_datetime_t datetime);

/**
 * Multiboot parameters
 * All callbacks must return 0 to continue, or non-zero to cancel
 * @param header Pointer to 192 bytes of header data
 * @param begin Pointer to start of Multiboot ROM data
 * @param end Pointer to end of Multiboot ROM data
 * @param palette 8-bit index of Multiboot animation
 * @param clients_connected Callback with a mask of which clients are connected
 * @param clients_connected Callback with a mask of which clients are connected
 * @param palette_progress Mask of which clients are waiting to receive palette data
 * @param accept Callback to confirm sending Multiboot ROM
 */
typedef struct {
    const void* header;
    const void* begin;
    const void* end;
    int palette;
    int(*clients_connected)(int mask);
    int(*header_progress)(int prog);
    int(*palette_progress)(int mask);
    int(*accept)(void);
} __agbabi_multiboot_t;

/**
 * Send Multiboot data over serial IO
 * IRQs must first be disabled
 * @param param Pointer to __agbabi_multiboot_t
 * @return 0 on success, 1 on failure with errno set to the error code
 */
int __agbabi_multiboot(const __agbabi_multiboot_t* param) __attribute__((nonnull(1)));

/**
 * Check EWRAM speed
 * @return 0 for slow WRAM (OXY, NTR), 1 for fast EWRAM (AGB, AGS)
 */
int __agbabi_poll_ewram(void) __attribute__((const));

#ifdef __cplusplus
}
#endif
#endif /* define AGBABI_H */
