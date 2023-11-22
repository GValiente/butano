@ Thanks Ketsuban!
@
@ reset_flags:
@
@ #define RESET_EWRAM      0x01
@ #define RESET_IWRAM      0x02
@ #define RESET_PALETTE    0x04
@ #define RESET_VRAM       0x08
@ #define RESET_OAM        0x10
@ #define RESET_SIO_REGS   0x20
@ #define RESET_SOUND_REGS 0x40
@ #define RESET_REGS       0x80
@ #define RESET_ALL        0xFF

@ void bn_hw_soft_reset(u32 reset_flags)
    .align 2
    .thumb_func
    .global bn_hw_soft_reset
    .type bn_hw_soft_reset STT_FUNC
bn_hw_soft_reset:
    ldr r3, =0x4000208
    mov r2, #0
    strb r2, [r3, #0]
    ldr r1, =0x3007f00
    mov sp, r1
    swi 1
    swi 0
    .pool

@ void bn_hw_hard_reset()
    .align 2
    .thumb_func
    .global bn_hw_hard_reset
    .type bn_hw_hard_reset STT_FUNC
bn_hw_hard_reset:
    swi 38
    .pool
