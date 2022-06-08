# agbabi

GBA optimized library functions for common operations.

Includes implementations for various [aeabi functions](https://developer.arm.com/documentation/ihi0043/e/?lang=en#the-standard-compiler-helper-function-library).

## C standard library

### memcpy
Calls `__aeabi_memcpy` and returns dest.
```c
void* memcpy(void *restrict dest, const void *restrict src, size_t count);
```
Compilers sometimes replace memory-copying loops with a call to `memcpy`.

### memset
Calls `__aeabi_memset` and returns dest.
```c
void* memset(void *dest, int ch, size_t count);
```
Compilers sometimes replace memory-clearing loops with a call to `memset`.

### memmove
Calls either `__agbabi_rmemcpy` or `__aeabi_memcpy` and returns dest.
```c
void* memmove(void *restrict dest, const void *restrict src, size_t count);
```
Compilers sometimes replace overlapping memory-copying loops with a call to `memmove`.

## aeabi helper function library

### Integer (32/32 → 32) division functions
The 32-bit integer division functions return the quotient in r0 or both quotient and remainder in {r0, r1}.    
```c
int __aeabi_idiv(int numerator, int denominator);
unsigned int __aeabi_uidiv(unsigned int numerator, unsigned int denominator);

int __attribute__((__vector_size__(8))) __aeabi_idivmod(int numerator, int denominator);
unsigned int __attribute__((__vector_size__(8))) __aeabi_uidivmod(unsigned int numerator, unsigned int denominator);
```
The division functions take the numerator and denominator in that order.

### Integer (64/64 → 64) division functions
The 64-bit integer division functions return the quotient across {r0, r1} or both quotient and remainder in {r0, r1} and {r2, r3}.    
```c
long long __aeabi_ldiv(long long numerator, long long denominator);
unsigned long long __aeabi_uldiv(unsigned long long numerator, unsigned long long denominator);

long long __attribute__((__vector_size__(16))) __aeabi_ldivmod(long long numerator, long long denominator);
unsigned long long __attribute__((__vector_size__(16)))  __aeabi_uldivmod(unsigned long long numerator, unsigned long long denominator);
```
The division functions take the numerator and denominator in that order.

### Integer (64×64 → 64) multiplication function
Long long implementation of integer multiplication.    
The 64-bit multiplication function returns the result across {r0, r1}.
```c
long long __aeabi_lmul(long long a, long long b);
```

### Integer 64 bit-shift functions
Long long implementations of arithmetic shift right, logical shift left and logical shift right.    
The 64-bit shift functions return the result across {r0, r1}.
```c
long long __aeabi_lasr(long long x, int n);
long long __aeabi_llsl(long long x, int n);
long long __aeabi_llsr(long long x, int n);
```

### Memory copying, clearing, and setting
#### Memory copying
Memcpy-like helper functions are needed to implement structure assignment.
```c
void __aeabi_memcpy8(void *restrict dest, const void *restrict src, size_t n);
void __aeabi_memcpy4(void *restrict dest, const void *restrict src, size_t n);
void __aeabi_memcpy(void *restrict dest, const void *restrict src, size_t n);
void __aeabi_memmove8(void *restrict dest, const void *restrict src, size_t n);
void __aeabi_memmove4(void *restrict dest, const void *restrict src, size_t n);
void __aeabi_memmove(void *restrict dest, const void *restrict src, size_t n);
```
These functions work like the ANSI C memcpy and memmove functions. However, `__aeabi_memcpy4` and `__aeabi_memcpy8` both assume that both of its arguments are 4-byte aligned.

Compilers can replace calls to `memcpy` with calls to one of these functions if they can deduce that the constraints are satisfied. For example, any `memcpy` whose return value is ignored can be replaced with `__aeabi_memcpy`. If the copy is between 4-byte-aligned pointers it can be replaced with `__aeabi_memcpy4`, and so on.

The `size_t` argument does not need to be a multiple of 4 for the 4/8-byte aligned versions, which allows copies with a non-constant size to be specialized according to source and destination alignment.

#### Memory clearing and setting
In similar deference to run-time efficiency we define reduced forms of `memset` and `memclr`.
```c
void __aeabi_memset8(void* dest, size_t n, int c);
void __aeabi_memset4(void* dest, size_t n, int c);
void __aeabi_memset(void* dest, size_t n, int c);
void __aeabi_memclr8(void* dest, size_t n);
void __aeabi_memclr4(void* dest, size_t n);
void __aeabi_memclr(void* dest, size_t n);
```
The `memclr` functions simplify a very common special case of `memset`, namely the one in which `c = 0` and the memory is being cleared to all zeroes.

The `size_t` argument does not need to be a multiple of 4 for the 4/8-byte aligned versions, which allows clears and sets with a non-constant size to be specialized according to the destination alignment.

## agbabi library

### Unsafe integer (32/32 → 32) division function
An unsafe implementation of `__aeabi_uidivmod` that skips the divide by zero check.
```c
unsigned int __attribute__((__vector_size__(8))) __agbabi_unsafe_uidiv(unsigned int numerator, unsigned int denominator);
```
`__aeabi_idiv0` is not called in this situation. The quotient and remainder of a divide by zero is not defined.

### Unsafe integer (64/64 → 64) division function
An unsafe implementation of `__aeabi_uldivmod` that skips the divide by zero check.    
The denominator must have a non-zero hi-word (greater than `0xffffffff`).
```c
unsigned int __attribute__((__vector_size__(16))) __agbabi_unsafe_uldiv(unsigned long long numerator, unsigned long long denominator);
```
`__aeabi_idiv0` is not called in this situation. The quotient and remainder of a divide by zero is not defined.

### Integer (64/32 → 64) division functions
The 64-bit/32-bit integer division functions return the quotient in {r0, r1} or both quotient and remainder in {r0, r1} and {r2}. {r3} is cleared to zero.
```c
unsigned long long __agbabi_uluidiv(unsigned long long numerator, unsigned int denominator);

unsigned long long __attribute__((__vector_size__(16))) __agbabi_uluidivmod(unsigned long long numerator, unsigned int denominator);
```
The division functions take the numerator and denominator in that order.

### Unsafe integer (64/32 → 64) division
An unsafe implementation of `__agbabi_uluidiv` that skips the divide by zero check.
```c
unsigned long long __attribute__((__vector_size__(16))) __agbabi_unsafe_uluidiv(unsigned long long numerator, unsigned int denominator);
```
`__aeabi_idiv0` is not called in this situation. The quotient and remainder of a divide by zero is not defined.

### Fixed-point sine approximation
An implementation of sine that does not use look-up-tables.
```c
int __agbabi_sin(int x);
```
Argument `x` is treated as a signed 16-bit binary angle measurement between -32,768 and +32,767 (-360 and +360 in degrees, -2π and +2π in radians).

The return value is a 32-bit signed fixed-point integer between with 29 fractional bits. In fixed-point, it ranges between -1 and +1 (-536870912 and +536870911 as a 32-bit signed integer).

### Memory copying, clearing, and setting
#### 16-bit memcpy
Used by `__aeabi_memcpy` for cases where either `dest` or `src` are half-aligned.
```c
void __agbabi_memcpy2(void *restrict dest, const void *restrict src, size_t n);
```
`__agbabi_memcpy2` assumes that both of its arguments are 2-byte aligned. This is ideal for VRAM

#### 16-bit memmove
Used by `__aeabi_memmove` for cases where either `dest` or `src` are half-aligned.
```c
void __agbabi_memmove2(void *restrict dest, const void *restrict src, size_t n);
```
`__agbabi_memmove2` assumes that both of its arguments are 2-byte aligned. This is ideal for VRAM

#### Reverse memory copying
Used by `__aeabi_memmove` for reverse-copying in cases where `dest > src`.
```c
void __agbabi_rmemcpy4(void *restrict dest, const void *restrict src, size_t n);
void __agbabi_rmemcpy2(void *restrict dest, const void *restrict src, size_t n);
void __agbabi_rmemcpy(void *restrict dest, const void *restrict src, size_t n);
```
`__agbabi_rmemcpy4` assumes that both of its arguments are 4-byte aligned.    
`__agbabi_rmemcpy2` assumes that both of its arguments are 2-byte aligned.

The `size_t` argument does not need to be a multiple of 4 or 2 for the 4-byte or 2-byte aligned versions, which allows copies with a non-constant size to be specialized according to source and destination alignment.

#### Memory setting
The full 32-bits of `c` are copied into `dest`, which is assumed to be 4-byte aligned.
```c
void __agbabi_wordset4(void* dest, size_t n, int c);
```
If the `size_t` argument is not a multiple of 4, the low bytes of `c` will be copied into the remaining space.

### Context switching
User-level context switching based on the POSIX context control C library.
```c
#include <sys/ucontext.h>
```
`ucontext_t` type is a structure that has the following fields:
```c
typedef struct ucontext_t {
    struct ucontext_t* uc_link;
    stack_t uc_stack;
    mcontext_t uc_mcontext;
} ucontext_t;
```
#### Context get/set
```c
int __agbabi_getcontext(ucontext_t* ucp);
int __agbabi_setcontext(const ucontext_t* ucp) __attribute__((noreturn));
```
`__agbabi_getcontext` initializes the structure pointed to by `ucp` to the currently active context. A successful call returns 0.

`__agbabi_setcontext` restores the user context pointed to by `ucp`. A successful call does not return. The context should have been obtained by a call of `__agbabi_getcontext`, or `__agbabi_makecontext`.

If the context was obtained by a call of `__agbabi_getcontext`, program execution continues as if this call just returned.

If the context was obtained by a call of `__agbabi_makecontext`, program execution continues by a call to the function `func` specified as the second argument of that call to `__agbabi_makecontext`. When the function `func` returns, we continue with the `uc_link` member of the structure `ucp` specified as the first argument of that call to `__agbabi_makecontext`. When this member is `NULL` the program exits by a call to `_exit`.

#### Context make/swap
```c
void __agbabi_makecontext(ucontext_t* ucp, void(*func)(), int argc, ...);
int __agbabi_swapcontext(ucontext_t *restrict oucp, const ucontext_t *restrict ucp);
```
The `__agbabi_makecontext` function modifies the context pointed to by `ucp` (which was obtained from a call to `__agbabi_getcontext`). Before invoking `__agbabi_makecontext`, the caller must allocate a new stack for this context and assign its address to `ucp->uc_stack`, and define a successor context and assign its address to `ucp->uc_link`.

When this context is later activated (using `__agbabi_setcontext` or `__agbabi_swapcontext`) the function `func` is called, and passed the series of integer (`int`) arguments that follow `argc`; the caller must specify the number of these arguments in `argc`. When this function returns, the successor context is activated. If the successor context pointer is `NULL`, the program exits by a call to `_exit`.

The `__agbabi_swapcontext` function saves the current context in the structure pointed to by `oucp`, and then activates the context pointed to by `ucp`.

When successful, `__agbabi_swapcontext` does not return. (But we may return later, in case `oucp` is activated, in which case it looks like `__agbabi_swapcontext` returns `0`.)

### Interrupt handlers
Implements IRQ handling without requiring a "switch-board" handling table.

To use these functions their address must be written to `0x3007FFC`.
```c
void __agbabi_irq_empty();
void __agbabi_irq_user();
```
Certain GBA BIOS functions and hardware features depend on IRQs being raised and then acknowledged by a handler. The function `__agbabi_irq_empty` will simply acknowledge raised IRQs and return.

The handler `__agbabi_irq_user` acknowledges raised IRQs, but will also jump to the function pointer at symbol `__agbabi_irq_uproc` with a branch-exchange, allowing more complex IRQ handling to be implemented.

Nested IRQ handling can be enabled by writing `1` to the lowest bit of `REG_IME` from the `__agbabi_irq_uproc` user procedure.

```c
void(*__agbabi_irq_uproc)(short irqFlags);
```
The argument `flags` of the `__agbabi_irq_uproc` procedure will contain a mask of the raised IRQs.

### Real Time Clock

Requires RTC chip on cartridge.

**Important:**
* `REG_GPIOCNT` (`0x80000c8`) must be set to `1` (read/write enabled) for RTC chip access.
* Interrupts should be disabled during RTC chip access.

#### Initialise RTC

Initializes the RTC clock, returning a non-zero error code if initialization fails.

```c
int __agbabi_rtc_init();
```

`__agbabi_rtc_init` returns 0 on success.

#### Querying time

Retrieve the raw chip time and date.

```c
int __agbabi_rtc_time();
long long __agbabi_rtc_ldatetime();
int __attribute__((__vector_size__(8))) __agbabi_rtc_datetime();
```

These return big-endian BCD encoded values.

`ldatetime` and `datetime` stores date in the upper 32-bits, time in the lower 32-bits.

#### RTC status

Returns the RTC chip status.

```c
int __agbabi_rtc_status();
```

Resets the RTC status.

```c
void __agbabi_rtc_reset();
```

#### Modifying RTC

Writes 8 bits to RTC chip.

```c
void __agbabi_rtc_write8(int n);
```

Set the raw chip time and date.

```c
void __agbabi_rtc_settime(int time);
void __agbabi_rtc_setldatetime(long long datetime);
void __agbabi_rtc_setdatetime(int __attribute__((__vector_size__(8))) datetime);
```

These take big-endian BCD encoded values.

### Coroutines
Basic implementation of stackful coroutines.

The coroutine object type is declared as:
```c
typedef struct agbabi_coro_t {
    unsigned int arm_sp : 31;
    unsigned int joined : 1;
} agbabi_coro_t;
```
The `joined` bit can be used for testing if a coroutine has returned from execution.

#### make

Initializes the coroutine object pointed to by `coro`. `sp_top` is the **top** of the user-allocated stack space for the coroutine.    
**At least 48 bytes must be allocated for the coroutine stack.**
```c
void __agbabi_coro_make(agbabi_coro_t *restrict coro, void *restrict sp_top, int(*coproc)(agbabi_coro_t*));
```

#### resume

Starts the given coroutine if it has not yet started.    
Resumes the given coroutine if it has yielded.    
Restarts the given coroutine if it has joined.
```c
int __agbabi_coro_resume(agbabi_coro_t* coro);
```
The return value is the value yielded or returned by the coroutine.

#### yield

From coroutine `coro`, yields the given `value` back to the caller.
```c
void __agbabi_coro_yield(agbabi_coro_t* coro, int value);
```
When the caller next resumes coroutine `coro`, it will behave as if this function has just returned.

### Fixed-point printing

#### Priting fixed-point numbers

Converts a given fixed point value to ASCII string representation.
```c
char* __agbabi_ufixed_tostr(unsigned int value, char* str, unsigned int precision);
char* __agbabi_fixed_tostr(int value, char* str, unsigned int precision);
```
The precision is the exponent, or fractional places, of the number.    
Returns a pointer to the last written null terminator.

#### Converting to decimal

Converts the fractional portion of a fixed-point number to a 128-bit decimal representation.
```c
unsigned int __attribute__((__vector_size__(16))) __agbabi_frac10(unsigned int frac, unsigned int precision);
```
The upper (integer) portion of the fixed point value must be cleared.    
Up to 32-bits of precision is supported, for a maximum of 32 digits of decimal precision.

Multiplies x by a power of two with logical shift to the left. The `mul128_2` variant implements a multiply-by-two with logical shift left by 1.
```c
unsigned int __attribute__((__vector_size__(16))) __agbabi_lsl128(unsigned int __attribute__((__vector_size__(16))) x, unsigned int shift);
unsigned int __attribute__((__vector_size__(16))) __agbabi_mul128_2(unsigned int __attribute__((__vector_size__(16))) x);
```

#### Converting to BCD

Converts a given 128-bit integer to BCD.
```c
unsigned int __attribute__((__vector_size__(16))) __agbabi_bcd128(unsigned int __attribute__((__vector_size__(16))) x, unsigned int precision);
```
Precision is used to limit how many digits are processed for BCD.

#### Printing BCD encoded fixed point fraction

Writes a given 128-bit BCD encoded fraction into ASCII string (null-terminated).
```c
char* __agbabi_frac_bcd128_tostr(char* str, unsigned int precision, unsigned int __attribute__((__vector_size__(16))) frac);
```
The precision is how many BCD digits to read and convert, not including the null byte.    
Returns a pointer to the end of the written digits, which is a null byte.

## POSIX library

### getcontext
Alias of `__aeabi_getcontext`.
```c
int getcontext(ucontext_t* ucp);
```

### setcontext
Alias of `__aeabi_setcontext`.
```c
int setcontext(const ucontext_t* ucp) __attribute__((noreturn));
```

### makecontext
Alias of `__aeabi_makecontext`.
```c
void makecontext(ucontext_t* ucp, void (*func)(), int argc, ...);
```

### swapcontext
Alias of `__aeabi_swapcontext`.
```c
int swapcontext(ucontext_t *restrict oucp, const ucontext_t *restrict ucp);
```

### _gettimeofday
Writes the current date to `tv`.

`tz` is unused.

Returns 0 on success.
```c
int _gettimeofday(struct timeval *restrict tv, struct timezone *restrict tz);
```
`REG_GPIOCNT` must be set to `1`.    
`REG_IME` is temporarily set to `0` during the RTC access.

This is used internally by the C time APIs.

### settimeofday
Sets the current date to value pointed to by `tv`.

`tz` is unused.

Returns 0 on success.
```c
int settimeofday(const struct timeval *restrict tv, const struct timezone *restrict tz);
```

`REG_GPIOCNT` must be set to `1`.    
`REG_IME` is temporarily set to `0` during the RTC access.
