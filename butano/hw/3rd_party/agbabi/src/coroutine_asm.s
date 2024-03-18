@===============================================================================
@
@ Support:
@    __agbabi_coro_resume, __agbabi_coro_yield, __agbabi_coro_pop
@
@ Copyright (C) 2021-2023 agbabi contributors
@ For conditions of distribution and use, see copyright notice in LICENSE.md
@
@===============================================================================

.syntax unified

    .arm
    .align 2

    .section .iwram.__agbabi_coro_resume, "ax", %progbits
    .global __agbabi_coro_resume
    .type __agbabi_coro_resume, %function
__agbabi_coro_resume:
    push    {r4-r11, lr}
    mov     r1, sp

    ldr     r2, [r0]
    bic     r2, r2, #0x80000000
    mov     sp, r2
    pop     {r4-r11, lr}
    str     r1, [r0]

    bx      lr

    .section .iwram.__agbabi_coro_yield, "ax", %progbits
    .global __agbabi_coro_yield
    .type __agbabi_coro_yield, %function
__agbabi_coro_yield:
    push    {r4-r11, lr}
    mov     r2, sp

    ldr     sp, [r0]
    pop     {r4-r11, lr}
    str     r2, [r0]

    @ Move yield value into r0 and return
    mov     r0, r1
    bx      lr

    .section .iwram.__agbabi_coro_pop, "ax", %progbits
    .global __agbabi_coro_pop
    .type __agbabi_coro_pop, %function
__agbabi_coro_pop:
    ldr     r1, [sp, #4]

    mov     lr, pc
    bx      r1
    ldr     r1, [sp]
    @ r0 contains return value
    @ r1 points to agbabi_coro_t*

    @ Allocate space for storing r4-r12, lr
    sub     r2, sp, #40
    ldr     r3, =__agbabi_coro_pop
    str     r3, [r2, #36] @ Next resume will call __agbabi_coro_pop

    @ Load suspend context
    ldr     sp, [r1]
    pop     {r4-r11, lr}

    @ Set "joined" flag
    orr     r2, r2, #0x80000000
    str     r2, [r1]

    bx      lr
