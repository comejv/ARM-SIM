.global main
.text

main:
    mov r0, #0x1234
    mov r1, #0x5678
    mov r0, r1, lsl #16
    mov r1, #0x90ab
    mov r2, #0xcdef
    mov r1, r2, lsl #16
    mov r2, #0x2000
    mov r5, #0
    mov r6, #-0xf
    mov r7, #0xa

    // r0 = #0x12345678
    // r2 = #0x2000
    // r5 = #0x0
    // r6 = -15 (0xfff1)
    // r7 = #0xa
    /* *** Immediate Offset *** */
    // Offset = 0 PASSED
    str r0, [r2] // Mem[0x2000] = #0x12345678
    ldr r3, [r2] // r3 = #0x12345678
    cmp r3, r0
    bne fail

    strb r0, [r2] // Mem[0x2000] = #0x78345678
    ldrb r3, [r2] // r3 = #0x00000078
    cmp r3, #0x78
    bne fail
    
    str r5, [r2]
    //  Offset != 0 PASSED
    str r0, [r2, #0x2] // Mem[0x2002] = #0x12345678
    ldr r3, [r2, #0x2] // r3 = #0x12345678
    cmp r3, r0
    bne fail

    str r0, [r2, #-0x2] // Mem[0x1ffe] = #0x12345678
    ldr r3, [r2, #-0x2] // r3 = #0x12345678
    cmp r3, r0
    bne fail

    str r5, [r2]
    strb r0, [r2, #0x2] // Mem[0x2000] = #0x00007800
    ldrb r3, [r2, #0x2] // r3 = #0x00000078
    cmp r3, #0x78
    bne fail

    strb r0, [r2, #-0x2] // Mem[0x1ffe] = #0x78340000
    ldrb r3, [r2, #-0x2] // r3 = #0x00000078
    cmp r3, #0x78
    bne fail


    // Pre-indexing and Post-indexing of LDR STR PASSED

    str r5, [r2] // Mem[0x2000] = 0
    // STR Pre Positive and LDR Post Negative
    str r0, [r2, #0x4]! // r2 = #0x2004 & Mem[0x2004] = #0x12345678
    mov r4, #0x2004
    cmp r2, r4
    bne fail
    ldr r3, [r2], #-0x4 // r2 = #0x2000 & r3 = #0x12345678
    mov r4, #0x2000
    cmp r2, r4
    bne fail
    cmp r3, r0
    bne fail

    str r5, [r2] // Mem[0x2000] = 0
    // STR Pre Negative and LDR Post Positive
    str r0, [r2, #-0x3]! // r2 = #0x1ffd & Mem[0x1ffd] = #0x12345678 
    mov r4, #0x1ffd
    cmp r2, r4
    bne fail
    ldr r3, [r2], #0x3 // r2 = #0x2000 & r3 = #0x12345678 
    mov r4, #0x2000
    cmp r2, r4
    bne fail
    cmp r3, r0
    bne fail

    str r5, [r2] // Mem[0x2000] = 0
    // STR Post Positive and LDR Pre Negative
    str r0, [r2], #0x2 // r2 = #0x2002
    mov r4, #0x2002
    cmp r2, r4
    bne fail
    ldr r3, [r2, #-0x2]! // r2 = #0x2000 & r3 = #0x12345678
    
    mov r4, #0x2000
    cmp r2, r4
    bne fail
    cmp r3, r0
    bne fail

    str r5, [r2] // Mem[0x2000] = 0
    // STR Post Negative and LDR Pre Positive
    str r0, [r2], #-0x4 // r2 = #0x1ffc
    mov r4, #0x1ffc
    cmp r2, r4
    bne fail
    ldr r3, [r2, #0x4]!
    mov r4, #0x2000
    cmp r2, r4
    bne fail
    cmp r3, r0
    bne fail


    // Pre-indexing and Post-indexing of STRB LDRB PASSED

    str r5, [r2] // Mem[0x2000] = 0
    // STRB Pre Positive and LDRB Post Negative
    strb r0, [r2, #0x4]! // r2 = #0x2004 & Mem[0x2004] = #0x78345678
    mov r4, #0x2004
    cmp r2, r4
    bne fail
    ldrb r3, [r2], #-0x4 // r2 = #0x2000 & r3 = #0x78
    mov r4, #0x2000
    cmp r2, r4
    bne fail
    cmp r3, #0x78
    bne fail

    str r5, [r2] // Mem[0x2000] = 0
    // STRB Pre Negative and LDRB Post Positive
    strb r0, [r2, #-0x3]! // r2 = #0x1ffd & Mem[0x1ffd] = #0x78345600 
    mov r2, #0x1ffd
    cmp r2, r4
    bne fail
    ldrb r3, [r2], #0x3 // r2 = #0x2000 & r3 = #0x78 
    cmp r3, #0x78
    bne fail

    str r5, [r2] // Mem[0x2000] = 0
    // STRB Post Positive and LDRB Pre Negative
    strb r0, [r2], #0x2 // r2 = #0x2002 & Mem[0x2000] = #0x78000000
    mov r4, #0x2002
    cmp r2, r4
    bne fail
    ldrb r3, [r2, #-0x2]! // r2 = #0x2000 & r3 = #0x78
    mov r4, #0x2000
    cmp r2, r4
    bne fail
    cmp r3, #0x78
    bne fail

    str r5, [r2] // Mem[0x2000] = 0
    // STRB Post Negative and LDRB Pre Positive
    strb r0, [r2], #-0x4 // r2 = #0x1ffc & Mem[0x2000] = #0x78000000
    mov r4, #0x1ffc
    cmp r2, r4
    bne fail
    ldrb r3, [r2, #0x4]!
    mov r4, #0x2000
    cmp r2, r4
    bne fail
    cmp r3, #0x78
    bne fail



    /* *** Register Offset *** */  // PASSED
    str r5, [r2] // Mem[0x2000] = 0 
    // Offset
    str r0, [r2, r7] // Mem[0x200a] = #0x12345678
    ldr r3, [r2, r7] // r3 = #0x12345678
    cmp r3, r0
    bne fail

    str r0, [r2, -r7] // Mem[0x1ff6] = #0x12345678
    ldr r3, [r2, -r7] // r3 = #0x12345678
    cmp r3, r0
    bne fail

    strb r0, [r2, r7] // Mem[0x200a] = #0x78345678
    ldrb r3, [r2, r7] // r3 = #0x78
    cmp r3, #0x78
    bne fail

    strb r0, [r2, -r7] // Mem[0x1ff6] = #0x78345678
    ldrb r3, [r2, -r7] // r3 = #0x78
    cmp r3, r0
    bne fail


    // Pre-indexing and Post-indexing of LDR STR PASSED

    // STR Pre Positive and LDR Post Negative
    str r0, [r2, r7]! // r2 = #0x200a & Mem[0x200a] = #0x12345678
    mov r4, #0x200a
    cmp r2, r4
    bne fail
    ldr r3, [r2], -r7 // r2 = #0x2000 & r3 = #0x12345678
    mov r4, #0x2000
    cmp r2, r4
    bne fail
    cmp r3, r0
    bne fail

    // STR Pre Negative and LDR Post Positive
    str r0, [r2, -r7]! // r2 = #0x1ff6 & Mem[0x1ff6] = #0x12345678 
    mov r4, #0x1ff6
    cmp r2, r4
    bne fail
    ldr r3, [r2], r7 // r2 = #0x2000 & r3 = #0x12345678 
    cmp r3, r0
    bne fail

    str r5, [r2] // Mem[0x2000] = 0
    // STR Post Positive and LDR Pre Negative
    str r0, [r2], r7 // r2 = #0x200a
    mov r4, #0x200a
    cmp r2, r4
    bne fail
    ldr r3, [r2, -r7]! // r2 = #0x2000 & r3 = #0x12345678
    mov r4, #0x2000
    cmp r2, r4
    bne fail
    cmp r3, r0
    bne fail

    str r5, [r2] // Mem[0x2000] = 0
    // STR Post Negative and LDR Pre Positive
    str r0, [r2], -r7 // r2 = #0x1ff6
    mov r3, #0x1ff6
    cmp r2, r3
    bne fail
    ldr r3, [r2, r7]!
    mov r4, #0x2000
    cmp r2, r4
    bne fail
    cmp r3, r0
    bne fail


    // Pre-indexing and Post-indexing of LDRB STRB PASSED

    str r5, [r2] // Mem[0x2000] = 0
    // STR Pre Positive and LDR Post Negative
    strb r0, [r2, r7]! // r2 = #0x200a & Mem[0x200a] = #0x78345678
    mov r4, #0x20a
    cmp r2, r4
    bne fail
    ldrb r3, [r2], -r7 // r2 = #0x2000 & r3 = #0x78
    mov r4, #0x2000
    cmp r2, r4
    bne fail
    cmp r3, #0x78
    bne fail

    str r5, [r2] // Mem[0x2000] = 0
    // STR Pre Negative and LDR Post Positive
    strb r0, [r2, -r7]! // r2 = #0x1ff6 & Mem[0x1ff6] = #0x78345678 
    mov r4, #0x1ff6
    cmp r2, r4
    bne fail
    ldrb r3, [r2], r7 // r2 = #0x2000 & r3 = #0x78 
    cmp r3, r0
    bne fail

    str r5, [r2] // Mem[0x2000] = 0
    // STR Post Positive and LDR Pre Negative
    strb r0, [r2], r7 // r2 = #0x200a
    mov r4, #0x200a
    cmp r2, r4
    bne fail
    ldrb r3, [r2, -r7]! // r2 = #0x2000 & r3 = #0x78345678
    mov r4, #0x2000
    cmp r2, r4
    bne fail
    cmp r3, #0x78
    bne fail

    str r5, [r2] // Mem[0x2000] = 0
    // STR Post Negative and LDR Pre Positive
    strb r0, [r2], -r7 // r2 = #0x1ff6
    mov r4, #0x1ff6
    cmp r2, r4
    bne fail
    ldrb r3, [r2, r7]!
    mov r4, #0x2000
    cmp r2, r4
    bne fail
    cmp r3, #0x78
    bne fail

    /* *** Reset all register *** */
    mov r0, #0x1234
    mov r1, #0x5678
    mov r0, r1, lsl #16
    mov r1, #0x90ab
    mov r2, #0xcdef
    mov r1, r2, lsl #16
    mov r2, #0x2000
    mov r5, #0
    mov r6, #-0xf
    mov r7, #0x5

    // r0 = #0x12345678
    // r2 = #0x2000
    // r5 = #0x0
    // r6 = -15 (0xfff1)
    // r7 = #0x5
    /* *** Scaled Register Offset *** */
    // Offset test with LSL
    str r5, [r2]
    str r0, [r2, r7, lsl #2] // Mem[0x2000 + #0x5 << 4] = Mem[0x2014] = #0x12345678
    ldr r3, [r2, r7, lsl #2] // r3 = #0x12345678
    cmp r3, r0
    bne fail

    // STR Post Positive LDR Pre Negative With LSR
    strb r0, [r2], r7, lsr #2 // Mem[0x2000] = #0x78000000, r2 = #0x2001
    mov r3, #0x2001
    cmp r2, r3
    bne fail
    ldrb r3, [r2, -r7, lsr #2]! // r3 = #0x78
    mov r4, #0x2000
    cmp r2, r4
    bne fail
    cmp r3, #0x78
    bne fail

    // STRB Pre Negative LDRB Post Negative With ASR
    strb r0, [r2, r6, asr #2]! // Mem[0x2000 + (-0xf >> 2)] = Mem[0x1ffc] = #0x78000000
    mov r4, #0x1ffc
    cmp r2, r4
    bne fail
    ldrb r3, [r2], -r6, asr #2  
    mov r4, #0x2000
    cmp r2, r4
    bne fail
    cmp r3, #0x78
    bne fail

    
    /* *********** */
    // All tests passed
    bx lr

fail:
    // Some test failed
    swi #0x123456
    bx lr
