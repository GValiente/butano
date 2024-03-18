/*
===============================================================================

 Support:
    __agbabi_coro_make

 Copyright (C) 2021-2023 agbabi contributors
 For conditions of distribution and use, see copyright notice in LICENSE.md

===============================================================================
*/

#include "../include/agbabi.h"

void __agbabi_coro_make(__agbabi_coro_t* __restrict__ coro, void* __restrict__ sp_top, int(*coproc)(__agbabi_coro_t*)) {
    void __agbabi_coro_pop(void);

    /* AAPCS wants stack to be aligned to 8 bytes */
    unsigned int alignedTop = ((unsigned int) sp_top) & ~0x7u;
    unsigned int* stack = (unsigned int*) alignedTop;

    /* Allocate space for storing r4-r11, lr, self, and entry procedure (11 words) */
    stack -= 11;
    stack[8] = (unsigned int) __agbabi_coro_pop; /* lr */
    stack[9] = (unsigned int) coro;
    stack[10] = (unsigned int) coproc;

    coro->arm_sp = ((unsigned int) stack) & 0x7fffffff;
    coro->joined = 0; /* Clear joined flag (ready to start) */
}
