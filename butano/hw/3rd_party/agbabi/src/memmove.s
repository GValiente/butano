@===============================================================================
@
@ ABI:
@    __aeabi_memmove, __aeabi_memmove4, __aeabi_memmove8
@ Standard:
@    memmove
@
@ Copyright (C) 2021-2023 agbabi contributors
@ For conditions of distribution and use, see copyright notice in LICENSE.md
@
@===============================================================================

.syntax unified

    .arm
    .align 2

    .section .iwram.__aeabi_memmove, "ax", %progbits
    .global __aeabi_memmove
    .type __aeabi_memmove, %function
__aeabi_memmove:
    cmp     r0, r1
    .extern __agbabi_rmemcpy
    bgt     __agbabi_rmemcpy
    .extern __aeabi_memcpy
    b       __aeabi_memcpy

    .section .iwram.__aeabi_memmove8, "ax", %progbits
    .global __aeabi_memmove8
    .type __aeabi_memmove8, %function
__aeabi_memmove8:
    .global __aeabi_memmove4
    .type __aeabi_memmove4, %function
__aeabi_memmove4:
    cmp     r0, r1
    .extern __agbabi_rmemcpy
    bgt     __agbabi_rmemcpy
    .extern __aeabi_memcpy4
    b       __aeabi_memcpy4

    .section .iwram.__agbabi_memmove1, "ax", %progbits
    .global __agbabi_memmove1
    .type __agbabi_memmove1, %function
__agbabi_memmove1:
    cmp     r0, r1
    .extern __agbabi_rmemcpy1
    bgt     __agbabi_rmemcpy1
    .extern __agbabi_memcpy1
    b       __agbabi_memcpy1

    .section .iwram.memmove, "ax", %progbits
    .global memmove
    .type memmove, %function
memmove:
    push    {r0, lr}
    bl      __aeabi_memmove
    pop     {r0, lr}
    bx      lr
