
    .section .iwram, "ax", %progbits
    .align 2
    .arm
    .global swi_RLUnCompReadNormalWrite8bit
    .type swi_RLUnCompReadNormalWrite8bit STT_FUNC
swi_RLUnCompReadNormalWrite8bit:
    stmfd sp!, { r2-r4 }
    
    ldr r2, [r0], #4
    lsr r2, #8                  @ decomp_len
    
    @ check for invalid decompression parameters
    @ check_invalid_decomp r2, r3, r0, .rl_uncomp_read_normal_write_8bit_return
    
    .rl_uncomp_read_normal_write_8bit_check_skip:       @ used in boot_screen
    
    .rl_uncomp_read_normal_write_8bit_loop:
        ldrb r3, [r0], #1
        lsls r3, #0x19          @ carry flag = uncomp/comp flag
        lsr r3, #0x19
        
        bcc .rl_uncomp_read_normal_write_8bit_uncompressed
        
        .rl_uncomp_read_normal_write_8bit_compressed:
            ldrb r4, [r0], #1   @ data
            add r3, #3          @ expand_length += 3
            sub r2, r3          @ decomp_len -= expand_length
            
            .rl_uncomp_read_normal_write_8bit_compressed_loop:
                strb r4, [r1], #1
                subs r3, #1
                bgt .rl_uncomp_read_normal_write_8bit_compressed_loop
            
            b .rl_uncomp_read_normal_write_8bit_loop_end
            
        .rl_uncomp_read_normal_write_8bit_uncompressed:
            add r3, #1          @ expand_length += 1
            sub r2, r3          @ decomp_len -= expand_length
            
            .rl_uncomp_read_normal_write_8bit_uncompressed_loop:
                ldrb r4, [r0], #1
                strb r4, [r1], #1
                subs r3, #1
                bgt .rl_uncomp_read_normal_write_8bit_uncompressed_loop
                
        .rl_uncomp_read_normal_write_8bit_loop_end:
            cmp r2, #0
            bgt .rl_uncomp_read_normal_write_8bit_loop
    
    .rl_uncomp_read_normal_write_8bit_return:
        ldmfd sp!, { r2-r4 }
        bx lr
 

    .section .iwram, "ax", %progbits
    .align 2
    .arm
    .global swi_RLUnCompReadNormalWrite16bit
    .type swi_RLUnCompReadNormalWrite16bit STT_FUNC
swi_RLUnCompReadNormalWrite16bit:
    @ basically the same thing as above, except we buffer the bytes and write them 2 at a time
    @ in the original BIOS, any leftover byte (if the uncompressed length is not divisible by 2) is NOT written
    
    stmfd sp!, { r2-r6 }
    
    ldr r2, [r0], #4
    lsr r2, #8                  @ decomp_len
    
    @ check for invalid decompression parameters
    @ check_invalid_decomp r2, r3, r0, .rl_uncomp_read_normal_write_16bit_return
    
    mov r5, #0                  @ keep track of upper/lower byte
    mov r6, #0                  @ write buffer
    
    .rl_uncomp_read_normal_write_16bit_loop:
        ldrb r3, [r0], #1
        lsls r3, #0x19          @ carry flag = uncomp/comp flag
        lsr r3, #0x19
        
        bcc .rl_uncomp_read_normal_write_16bit_uncompressed
        
        .rl_uncomp_read_normal_write_16bit_compressed:
            ldrb r4, [r0], #1   @ data
            add r3, #3          @ expand_length += 3
            sub r2, r3          @ decomp_len -= expand_length
            
            .rl_uncomp_read_normal_write_16bit_compressed_loop:
                subs r3, #1
                blt .rl_uncomp_read_normal_write_16bit_loop_end
                
                orr r6, r4, lsl r5
                eors r5, #8
                @ store only if it's an even byte we are checking
                bne .rl_uncomp_read_normal_write_16bit_compressed_loop
                
                strh r6, [r1], #2
                mov r6, #0              @ clear buffer
                b .rl_uncomp_read_normal_write_16bit_compressed_loop
            
        .rl_uncomp_read_normal_write_16bit_uncompressed:
            add r3, #1          @ expand_length += 1
            sub r2, r3          @ decomp_len -= expand_length
            
            .rl_uncomp_read_normal_write_16bit_uncompressed_loop:
                subs r3, #1
                blt .rl_uncomp_read_normal_write_16bit_loop_end
                
                ldrb r4, [r0], #1
                orr r6, r4, lsl r5
                eors r5, #8
                @ store only if it's an even byte we are checking
                bne .rl_uncomp_read_normal_write_16bit_uncompressed_loop
                
                strh r6, [r1], #2     @ last byte is not stored for misaligned decomp_len
                mov r6, #0            @ clear buffer
                
                b .rl_uncomp_read_normal_write_16bit_uncompressed_loop
                
        .rl_uncomp_read_normal_write_16bit_loop_end:
            cmp r2, #0
            bgt .rl_uncomp_read_normal_write_16bit_loop
    
    .rl_uncomp_read_normal_write_16bit_return:
        ldmfd sp!, { r2-r6 }
        bx lr
 
