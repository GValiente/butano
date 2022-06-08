/*
===============================================================================

 ABI:
    __aeabi_memmove, __aeabi_memmove4, __aeabi_memmove8
 Standard:
    memmove

 Copyright (C) 2021-2022 agbabi contributors
 For conditions of distribution and use, see copyright notice in LICENSE.md

===============================================================================
*/

    .arm
    .align 2

    .section .iwram.__aeabi_memmove, "ax", %progbits
    .global __aeabi_memmove
__aeabi_memmove:
    cmp     r0, r1
    .extern __agbabi_rmemcpy
    bgt     __agbabi_rmemcpy
    .extern __aeabi_memcpy
    b       __aeabi_memcpy

    .global __aeabi_memmove8
__aeabi_memmove8:
    .global __aeabi_memmove4
__aeabi_memmove4:
    cmp     r0, r1
    .extern __agbabi_rmemcpy4
    bgt     __agbabi_rmemcpy4
    .extern __aeabi_memcpy4
    b       __aeabi_memcpy4

    .global __agbabi_memmove2
__agbabi_memmove2:
    cmp     r0, r1
    .extern __agbabi_rmemcpy2
    bgt     __agbabi_rmemcpy2
    .extern __agbabi_memcpy2
    b       __agbabi_memcpy2

    .section .iwram.memmove, "ax", %progbits
    .global memmove
memmove:
    push    {r0, lr}
    bl      __aeabi_memmove
    pop     {r0, lr}
    bx      lr
