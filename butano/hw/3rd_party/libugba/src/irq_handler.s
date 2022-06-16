// SPDX-License-Identifier: MIT
//
// Copyright (c) 2020-2022 Antonio Niño Díaz

    .section .iwram, "ax", %progbits
    .code 32

    .set MEM_IO_ADDR, 0x04000000
    .set OFFSET_IE, 0x200
    .set OFFSET_IF, 0x202
    .set OFFSET_IME, 0x208

    .global IRQ_GlobalInterruptHandler

IRQ_GlobalInterruptHandler:
    // Get the pending interrupts that the user actually cares about. If
    // something isn't set in IE, ignore it.

    mov     r0, #MEM_IO_ADDR        // r0 = MEM_IO_ADDR
    ldr     r1, [r0, #OFFSET_IE]    // r1 = REG_IE | (REG_IF << 16)
    and     r1, r1, r1, lsr #16     // r1 = REG_IE & REG_IF

    // Iterate from BIT(0) to BIT(13)

    .extern IRQ_VectorTable

    // Notes on the default priority of interrupts:
    //
    // - HBLANK is first because it's very short, so saving a few cycles is
    //   important, specially because it is called every scanline.
    // - VCOUNT is second because it's similar to HBLANK, but it is triggered
    //   less often. However, it needs higher priority than VBL because they
    //   are both triggered at the same time when VBL starts, and VBL is much
    //   longer.

    ldr     r3, =IRQ_VectorTable + 4

    mov     r2, #(1 << 1) // HBLANK
    tst     r1, r2
    bne     interrupt_found

    # add     r3, r3, #4
    # mov     r2, #(1 << 2) // VCOUNT
    # tst     r1, r2
    # bne     interrupt_found

    # sub     r3, r3, #8
    sub     r3, r3, #4
    mov     r2, #(1 << 0) // VBLANK
    tst     r1, r2
    bne     interrupt_found

    add     r3, r3, #12
    mov     r2, #(1 << 3) // TIMER0
    tst     r1, r2
    bne     interrupt_found

    add     r3, r3, #4
    mov     r2, #(1 << 4) // TIMER1
    tst     r1, r2
    bne     interrupt_found

    # add     r3, r3, #4
    # mov     r2, #(1 << 5) // TIMER2
    # tst     r1, r2
    # bne     interrupt_found

    # add     r3, r3, #4
    # mov     r2, #(1 << 6) // TIMER3
    # tst     r1, r2
    # bne     interrupt_found

    # add     r3, r3, #4
    add     r3, r3, #12
    mov     r2, #(1 << 7) // SERIAL
    tst     r1, r2
    bne     interrupt_found

    # add     r3, r3, #4
    # mov     r2, #(1 << 8) // DMA0
    # tst     r1, r2
    # bne     interrupt_found

    # add     r3, r3, #4
    add     r3, r3, #8
    mov     r2, #(1 << 9) // DMA1
    tst     r1, r2
    bne     interrupt_found

    add     r3, r3, #4
    mov     r2, #(1 << 10) // DMA2
    tst     r1, r2
    bne     interrupt_found

    # add     r3, r3, #4
    # mov     r2, #(1 << 11) // DMA3
    # tst     r1, r2
    # bne     interrupt_found

    # add     r3, r3, #4
    add     r3, r3, #8
    mov     r2, #(1 << 12) // KEYPAD
    tst     r1, r2
    bne     interrupt_found

    # add     r3, r3, #4
    # mov     r2, #(1 << 13) // GAMEPAK
    # tst     r1, r2
    # bne     interrupt_found

    // If no interrupt flag is set, fall to the next section of code.

    // If no interrupt handlers have to be called, clear all bits in the IF and
    // BIOS flags register.

    add     r3, r0, #(OFFSET_IF & 0xFF00)
    orr     r3, r3, #(OFFSET_IF & 0xFF)
    ldrh    r1, [r3]
    strh    r1, [r3]

    ldrh    r2, [r0, #-8] // The BIOS register is mirrored at 0x03FFFFF8
    orr     r2, r2, r1
    strh    r2, [r0, #-8]

    bx      lr

    // This point is reached if there is at least one bit set in IF & IE
interrupt_found:
    // r0 = REG_BASE
    // r2 = IRQ bit of the current vector
    // r3 = Pointer to vector to jump to

    // Write bit to IF and the BIOS register to acknowledge this interrupt, but
    // leave the others alone.
    add     r1, r0, #(OFFSET_IF & 0xFF00)
    orr     r1, r1, #(OFFSET_IF & 0xFF)
    strh    r2, [r1]

    // The BIOS register (BIOS_INTR_FLAGS) is mirrored at 0x03FFFFF8
    ldrh    r1, [r0, #-8]
    orr     r1, r1, r2
    strh    r1, [r0, #-8]

    // If the interrupt handler is null, exit handler
    ldr     r3, [r3]
    cmp     r3, #0
    bxeq    lr

    // If this point is reached, there is a valid interrupt handler

    // r0 = REG_BASE
    // r3 = Vector to jump to

    // Clear IME so that we don't get any nested interrupt during the handler of
    // the current one. At the same time, the old value is preserved so that it
    // can be restored after the end of the interrupt handler. Note that it is
    // safe to access IME in 32-bit accesses.
    add     r2, r0, #(OFFSET_IME & 0xFF00)
    orr     r2, r2, #(OFFSET_IME & 0xFF)
    mov     r1, #0
    swp     r1, r1, [r2]

    // Get current spsr
    mrs     r2, spsr

    // Push old IME, spsr and lr
    stmfd   sp!, {r1-r2, lr}

    .equ    MODE_IRQ, 0x12
    .equ    MODE_SYSTEM, 0x1F
    .equ    MODE_MASK, 0x1F

    .equ    FLAG_IRQ_DISABLE, 1 << 7

    // Set CPU mode to system (like user, but privileged, so we can go back to
    // mode IRQ later). Re-enable the master IRQ bit in CPSR so that the
    // interrupt handler can re-enable interrupts by setting IME to 1.
    mrs     r2, cpsr
    //bic   r2, r2, #MODE_MASK // Not needed for MODE_SYSTEM
    bic     r2, r2, #FLAG_IRQ_DISABLE
    orr     r2, r2, #MODE_SYSTEM
    msr     cpsr, r2

    // Call interrupt handler
    push    {lr}

    mov     lr, pc
    bx      r3

    pop     {lr}

    // Disable interrupts while switching modes
    mov     r0, #MEM_IO_ADDR
    str     r0, [r0, #OFFSET_IME]

    // Set CPU mode to IRQ. Disable interrupts so that setting IME to 1
    // afterwards doesn't let the CPU jump to the interrupt handler.
    mrs     r2, cpsr
    bic     r2, r2, #MODE_MASK
    orr     r2, r2, #(MODE_IRQ | FLAG_IRQ_DISABLE)
    msr     cpsr, r2

    // Pop old IME, spsr and lr
    ldmfd   sp!, {r1-r2, lr}

    // Restore spsr
    msr     spsr, r2

    // Restore old IME
    str     r1, [r0, #OFFSET_IME]

    bx      lr

    .end
