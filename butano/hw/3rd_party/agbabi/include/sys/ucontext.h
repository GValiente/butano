/*
===============================================================================

 Context switching definitions

 Copyright (C) 2021-2022 agbabi contributors
 For conditions of distribution and use, see copyright notice in LICENSE.md

===============================================================================
*/

#ifndef _SYS_UCONTEXT_H_
#define _SYS_UCONTEXT_H_
#if defined( __cplusplus )
extern "C" {
#endif

#ifndef __ASSEMBLER__

#include <stddef.h>

#if defined(__DEVKITARM__)
#   include <sys/signal.h>
#else
typedef struct stack_t {
    void* ss_sp;
    size_t ss_size;
} stack_t;
#endif

typedef struct mcontext_t {
    unsigned int arm_r0;
    unsigned int arm_r1;
    unsigned int arm_r2;
    unsigned int arm_r3;
    unsigned int arm_r4;
    unsigned int arm_r5;
    unsigned int arm_r6;
    unsigned int arm_r7;
    unsigned int arm_r8;
    unsigned int arm_r9;
    unsigned int arm_r10;
    unsigned int arm_r11;
    unsigned int arm_r12;
    unsigned int arm_sp;
    unsigned int arm_lr;
    unsigned int arm_pc;
    unsigned int arm_cpsr;
} mcontext_t;

typedef struct ucontext_t {
    struct ucontext_t* uc_link;
    stack_t uc_stack;
    mcontext_t uc_mcontext;
} ucontext_t;

#ifndef NO_POSIX

/// Alias of __agbabi_getcontext
/// \param ucp Pointer to context structure
/// \return 0
int getcontext(ucontext_t* ucp);

/// Alias of __agbabi_setcontext
/// \param ucp Pointer to context structure
/// \return Does not return
int setcontext(const ucontext_t* ucp) __attribute__((noreturn));

/// Alias of __agbabi_swapcontext
/// \param oucp Output address for current context
/// \param ucp Context to swap to
/// \return Although technically this does not return, it will appear to return 0 when switching to oucp
int swapcontext(ucontext_t* __restrict__ oucp, const ucontext_t* __restrict__ ucp);

/// Alias of __agbabi_makecontext
/// \param ucp Pointer to context structure
/// \param func Function to invoke with __agbabi_setcontext
/// \param argc Number of arguments passed to func
/// \param ... List of arguments to be passed to func
void makecontext(ucontext_t* ucp, void(*func)(), int argc, ...);

#endif

#endif // ifndef __ASSEMBLER__

#define STACK_OFFSETOF_SS_SP 0
#if defined(__DEVKITARM__)
#   define STACK_OFFSETOF_SS_SIZE 8
#else
#   define STACK_OFFSETOF_SS_SIZE 4
#endif

#define MCONTEXT_OFFSETOF_ARM_R0 0
#define MCONTEXT_OFFSETOF_ARM_R1 4
#define MCONTEXT_OFFSETOF_ARM_R2 8
#define MCONTEXT_OFFSETOF_ARM_R3 12
#define MCONTEXT_OFFSETOF_ARM_R4 16
#define MCONTEXT_OFFSETOF_ARM_R5 20
#define MCONTEXT_OFFSETOF_ARM_R6 24
#define MCONTEXT_OFFSETOF_ARM_R7 28
#define MCONTEXT_OFFSETOF_ARM_R8 32
#define MCONTEXT_OFFSETOF_ARM_R9 36
#define MCONTEXT_OFFSETOF_ARM_R10 40
#define MCONTEXT_OFFSETOF_ARM_R11 44
#define MCONTEXT_OFFSETOF_ARM_R12 48
#define MCONTEXT_OFFSETOF_ARM_SP 52
#define MCONTEXT_OFFSETOF_ARM_LR 56
#define MCONTEXT_OFFSETOF_ARM_PC 60
#define MCONTEXT_OFFSETOF_ARM_CPSR 64

#define UCONTEXT_OFFSETOF_UC_LINK 0
#define UCONTEXT_OFFSETOF_UC_STACK 4
#if defined(__DEVKITARM__)
#   define UCONTEXT_OFFSETOF_UC_MCONTEXT 16
#else
#   define UCONTEXT_OFFSETOF_UC_MCONTEXT 12
#endif

#if defined( __cplusplus )
} // extern "C"
#endif
#endif // define _SYS_UCONTEXT_H_
