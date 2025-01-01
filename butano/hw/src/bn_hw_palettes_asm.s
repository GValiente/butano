/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

/*
    unsigned bn_hw_palettes_different_words(
                unsigned four_words_count, const unsigned* color_words, const unsigned* stored_color_words)
    {
        while(four_words_count)
        {
            for(int index = 0; index < 4; ++index)
            {
                if(*color_words != *stored_color_words) [[unlikely]]
                {
                    return four_words_count;
                }

                ++color_words;
                ++stored_color_words;
            }

            --four_words_count;
        }

        return four_words_count;
    }
*/
    .section .iwram, "ax", %progbits
    .align 2
    .arm
    .global bn_hw_palettes_different_words
    .type bn_hw_palettes_different_words, STT_FUNC
bn_hw_palettes_different_words:
    push    {r4-r9}

.loop:
    ldmia   r1!, {r3-r6}
    ldmia   r2!, {r7-r9, r12}
    cmp     r3, r7
    cmpeq   r4, r8
    cmpeq   r5, r9
    cmpeq   r6, r12
    bne     .exit

    subs    r0, #1
    bne     .loop

.exit:
    pop     {r4-r9}
    bx      lr
