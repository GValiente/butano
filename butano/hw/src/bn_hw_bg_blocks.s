/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

/*
    void bn_hw_bg_blocks_commit_half_words(
            const uint16_t* source_data_ptr, unsigned half_words, uint16_t offset, uint16_t* destination_vram_ptr)
    {
        for(unsigned index = 0; index < half_words; ++index)
        {
            destination_vram_ptr[index] = source_data_ptr[index] + offset;
        }
    }
*/
    .section .iwram, "ax", %progbits
    .align 2
    .arm
    .global bn_hw_bg_blocks_commit_half_words
    .type bn_hw_bg_blocks_commit_half_words, STT_FUNC
bn_hw_bg_blocks_commit_half_words:

.half_words_loop:
    ldrh    r12, [r0], #2
    add     r12, r2, r12
    strh    r12, [r3], #2
    subs    r1, #1
    bne     .half_words_loop

    bx      lr


/*
    void bn_hw_bg_blocks_commit_words(
            const unsigned* source_data_ptr, unsigned words, unsigned word_offset, unsigned* destination_vram_ptr)
    {
        for(unsigned index = 0; index < words; ++index)
        {
            destination_vram_ptr[index] = source_data_ptr[index] + word_offset;
        }
    }
*/
    .section .iwram, "ax", %progbits
    .align 2
    .arm
    .global bn_hw_bg_blocks_commit_words
    .type bn_hw_bg_blocks_commit_words, STT_FUNC
bn_hw_bg_blocks_commit_words:

.words_loop:
    ldr     r12, [r0], #4
    add     r12, r2, r12
    str     r12, [r3], #4
    subs    r1, #1
    bne     .words_loop

    bx      lr


/*
    void bn_hw_bg_blocks_commit_blocks(
            const unsigned* source_data_ptr, unsigned blocks, unsigned word_offset, unsigned* destination_vram_ptr)
    {
        while(blocks)
        {
            for(int index = 0; index < 8; ++index)
            {
                *destination_vram_ptr = *source_data_ptr + word_offset;
                ++source_data_ptr;
                ++destination_vram_ptr;
            }

            --blocks;
        }
    }
*/
    .section .iwram, "ax", %progbits
    .align 2
    .arm
    .global bn_hw_bg_blocks_commit_blocks
    .type bn_hw_bg_blocks_commit_blocks, STT_FUNC
bn_hw_bg_blocks_commit_blocks:
    push    {r4-r10}

.blocks_loop:
    ldmia   r0!, {r4-r10, r12}
    add     r4, r4, r2
    add     r5, r5, r2
    add     r6, r6, r2
    add     r7, r7, r2
    add     r8, r8, r2
    add     r9, r9, r2
    add     r10, r10, r2
    add     r12, r12, r2
    stmia   r3!, {r4-r10, r12}

    subs    r1, #1
    bne     .blocks_loop

    pop     {r4-r10}
    bx      lr
