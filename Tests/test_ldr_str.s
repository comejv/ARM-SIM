.global main

.data
test_word:
.word 0x12345678
test_addr:
.word 0x2000

.text

main:
    ldr r0, LD_test_addr
    ldr r2, [r0]
    ldr r3, LD_test_word
    ldr r0, [r3]
    mov r5, #0
    mov r6, #-0xf
    mov r7, #0xa

    // r0 = #0x12345678
    // r2 = #0x2000
    // r5 = LD_test_addr
    // r6 = -15 (0xfff1)
    // r7 = #0xa
    // r8 = #0x0
    /* *** Immediate Offset *** */
    // Offset = 0 PASSED
    str r0, [r2] // Mem[0x2000] = #0x12345678
    ldr r3, [r2] // r3 = #0x12345678
    cmp r3, r0
    movne r0, #1
    bne fail

    strb r0, [r2] // Mem[0x2000] = #0x78345678
    ldrb r3, [r2] // r3 = #0x00000078
    cmp r3, #0x78
    movne r0, #2
    bne fail
    
    str r8, [r2]
    //  Offset != 0 PASSED
    str r0, [r2, #0x2] // Mem[0x2002] = #0x12345678
    ldr r3, [r2, #0x2] // r3 = #0x12345678
    cmp r3, r0
    movne r0, #3
    bne fail

    str r0, [r2, #-0x2] // Mem[0x1ffe] = #0x12345678
    ldr r3, [r2, #-0x2] // r3 = #0x12345678
    cmp r3, r0
    movne r0, #3
    bne fail

    str r8, [r2]
    strb r0, [r2, #0x2] // Mem[0x2000] = #0x00007800
    ldrb r3, [r2, #0x2] // r3 = #0x00000078
    cmp r3, #0x78
    movne r0, #4
    bne fail

    strb r0, [r2, #-0x2] // Mem[0x1ffe] = #0x78340000
    ldrb r3, [r2, #-0x2] // r3 = #0x00000078
    cmp r3, #0x78
    movne r0, #5
    bne fail


    // Pre-indexing and Post-indexing of LDR STR PASSED

    str r8, [r2] // Mem[0x2000] = 0
    // STR Pre Positive and LDR Post Negative
    str r0, [r2, #0x4]! // r2 = #0x2004 & Mem[0x2004] = #0x12345678
    ldr r5, LD_test_addr
    ldr r4, [r5]
    add r4, r4, #0x4
    cmp r2, r4
    movne r0, #6
    bne fail
    ldr r3, [r2], #-0x4 // r2 = #0x2000 & r3 = #0x12345678
    ldr r4, [r5]
    cmp r2, r4
    movne r0, #7
    bne fail
    cmp r3, r0
    movne r0, #8
    bne fail

    str r8, [r2] // Mem[0x2000] = 0
    // STR Pre Negative and LDR Post Positive
    str r0, [r2, #-0x3]! // r2 = #0x1ffd & Mem[0x1ffd] = #0x12345678 
    ldr r4, [r5]
    sub r4, r4, #0x3
    cmp r2, r4
    movne r0, #9
    bne fail
    ldr r3, [r2], #0x3 // r2 = #0x2000 & r3 = #0x12345678 
    ldr r4, [r5]
    cmp r2, r4
    movne r0, #10
    bne fail
    cmp r3, r0
    movne r0, #11
    bne fail

    str r8, [r2] // Mem[0x2000] = 0
    // STR Post Positive and LDR Pre Negative
    str r0, [r2], #0x2 // r2 = #0x2002
    ldr r4, [r5]
    add r4, r4, #0x2
    cmp r2, r4
    movne r0, #12
    bne fail
    ldr r3, [r2, #-0x2]! // r2 = #0x2000 & r3 = #0x12345678
    
    ldr r4, [r5]
    cmp r2, r4
    movne r0, #13
    bne fail
    cmp r3, r0
    movne r0, #14
    bne fail

    str r8, [r2] // Mem[0x2000] = 0
    // STR Post Negative and LDR Pre Positive
    str r0, [r2], #-0x4 // r2 = #0x1ffc
    ldr r4, [r5]
    sub r4, r4, #0x4
    cmp r2, r4
    movne r0, #15
    bne fail
    ldr r3, [r2, #0x4]!
    ldr r4, [r5]
    cmp r2, r4
    movne r0, #16
    bne fail
    cmp r3, r0
    movne r0, #17
    bne fail


    // Pre-indexing and Post-indexing of STRB LDRB PASSED

    str r8, [r2] // Mem[0x2000] = 0
    // STRB Pre Positive and LDRB Post Negative
    strb r0, [r2, #0x4]! // r2 = #0x2004 & Mem[0x2004] = #0x78345678
    ldr r4, [r5]
    add r4, r4, #0x4
    cmp r2, r4
    movne r0, #18
    bne fail
    ldrb r3, [r2], #-0x4 // r2 = #0x2000 & r3 = #0x78
    ldr r4, [r5]
    cmp r2, r4
    movne r0, #19
    bne fail
    cmp r3, #0x78
    movne r0, #20
    bne fail

    str r8, [r2] // Mem[0x2000] = 0
    // STRB Pre Negative and LDRB Post Positive
    strb r0, [r2, #-0x3]! // r2 = #0x1ffd & Mem[0x1ffd] = #0x78345600 
    ldr r4, [r5]
    sub r4, r4, #0x3
    cmp r2, r4
    movne r0, #21
    bne fail
    ldrb r3, [r2], #0x3 // r2 = #0x2000 & r3 = #0x78 
    cmp r3, #0x78
    movne r0, #22
    bne fail

    str r8, [r2] // Mem[0x2000] = 0
    // STRB Post Positive and LDRB Pre Negative
    strb r0, [r2], #0x2 // r2 = #0x2002 & Mem[0x2000] = #0x78000000
    ldr r4, [r5]
    add r4, r4, #0x2
    cmp r2, r4
    movne r0, #23
    bne fail
    ldrb r3, [r2, #-0x2]! // r2 = #0x2000 & r3 = #0x78
    ldr r4, [r5]
    cmp r2, r4
    movne r0, #24
    bne fail
    cmp r3, #0x78
    movne r0, #25
    bne fail

    str r5, [r2] // Mem[0x2000] = 0
    // STRB Post Negative and LDRB Pre Positive
    strb r0, [r2], #-0x4 // r2 = #0x1ffc & Mem[0x2000] = #0x78000000
    ldr r4, [r5]
    sub r4, r4, #0x4
    cmp r2, r4
    movne r0, #26
    bne fail
    ldrb r3, [r2, #0x4]!
    ldr r4, [r5]
    cmp r2, r4
    movne r0, #27
    bne fail
    cmp r3, #0x78
    movne r0, #28
    bne fail



    /* *** Register Offset *** */  // PASSED
    str r8, [r2] // Mem[0x2000] = 0 
    // Offset
    str r0, [r2, r7] // Mem[0x200a] = #0x12345678
    ldr r3, [r2, r7] // r3 = #0x12345678
    cmp r3, r0
    movne r0, #29
    bne fail

    str r0, [r2, -r7] // Mem[0x1ff6] = #0x12345678
    ldr r3, [r2, -r7] // r3 = #0x12345678
    cmp r3, r0
    movne r0, #30
    bne fail

    strb r0, [r2, r7] // Mem[0x200a] = #0x78345678
    ldrb r3, [r2, r7] // r3 = #0x78
    cmp r3, #0x78
    movne r0, #31
    bne fail

    strb r0, [r2, -r7] // Mem[0x1ff6] = #0x78345678
    ldrb r3, [r2, -r7] // r3 = #0x78
    cmp r3, #0x78
    movne r0, #32
    bne fail


    // Pre-indexing and Post-indexing of LDR STR PASSED

    // STR Pre Positive and LDR Post Negative
    str r0, [r2, r7]! // r2 = #0x200a & Mem[0x200a] = #0x12345678
    ldr r4, [r5]
    add r4, r4, #0xa
    cmp r2, r4
    movne r0, #33
    bne fail
    ldr r3, [r2], -r7 // r2 = #0x2000 & r3 = #0x12345678
    ldr r4, [r5]
    cmp r2, r4
    movne r0, #34
    bne fail
    cmp r3, r0
    movne r0, #35
    bne fail

    // STR Pre Negative and LDR Post Positive
    str r0, [r2, -r7]! // r2 = #0x1ff6 & Mem[0x1ff6] = #0x12345678 
    ldr r4, [r5]
    sub r4, r4, #0xa
    cmp r2, r4
    movne r0, #36
    bne fail
    ldr r3, [r2], r7 // r2 = #0x2000 & r3 = #0x12345678 
    cmp r3, r0
    movne r0, #37
    bne fail

    str r5, [r2] // Mem[0x2000] = 0
    // STR Post Positive and LDR Pre Negative
    str r0, [r2], r7 // r2 = #0x200a
    ldr r4, [r5]
    add r4, r4, #0xa
    cmp r2, r4
    movne r0, #38
    bne fail
    ldr r3, [r2, -r7]! // r2 = #0x2000 & r3 = #0x12345678
    ldr r4, [r5]
    cmp r2, r4
    movne r0, #39
    bne fail
    cmp r3, r0
    movne r0, #40
    bne fail

    str r5, [r2] // Mem[0x2000] = 0
    // STR Post Negative and LDR Pre Positive
    str r0, [r2], -r7 // r2 = #0x1ff6
    ldr r4, [r5]
    sub r4, r4, #0xa
    cmp r2, r4
    movne r0, #41
    bne fail
    ldr r3, [r2, r7]!
    ldr r4, [r5]
    cmp r2, r4
    movne r0, #42
    bne fail
    cmp r3, r0
    movne r0, #43
    bne fail


    // Pre-indexing and Post-indexing of LDRB STRB PASSED

    str r5, [r2] // Mem[0x2000] = 0
    // STR Pre Positive and LDR Post Negative
    strb r0, [r2, r7]! // r2 = #0x200a & Mem[0x200a] = #0x78345678
    ldr r4, [r5]
    add r4, r4, #0xa
    cmp r2, r4
    movne r0, #44
    bne fail
    ldrb r3, [r2], -r7 // r2 = #0x2000 & r3 = #0x78
    ldr r4, [r5]
    cmp r2, r4
    movne r0, #45
    bne fail
    cmp r3, #0x78
    movne r0, #46
    bne fail

    str r5, [r2] // Mem[0x2000] = 0
    // STR Pre Negative and LDR Post Positive
    strb r0, [r2, -r7]! // r2 = #0x1ff6 & Mem[0x1ff6] = #0x78345678 
    ldr r4, [r5]
    sub r4, r4, #0xa
    cmp r2, r4
    movne r0, #47
    bne fail
    ldrb r3, [r2], r7 // r2 = #0x2000 & r3 = #0x78 
    cmp r3, #0x78
    movne r0, #48
    bne fail

    str r5, [r2] // Mem[0x2000] = 0
    // STR Post Positive and LDR Pre Negative
    strb r0, [r2], r7 // r2 = #0x200a
    ldr r4, [r5]
    add r4, r4, #0xa
    cmp r2, r4
    movne r0, #49
    bne fail
    ldrb r3, [r2, -r7]! // r2 = #0x2000 & r3 = #0x78345678
    ldr r4, [r5]
    cmp r2, r4
    movne r0, #50
    bne fail
    cmp r3, #0x78
    movne r0, #51
    bne fail

    str r5, [r2] // Mem[0x2000] = 0
    // STR Post Negative and LDR Pre Positive
    strb r0, [r2], -r7 // r2 = #0x1ff6
    ldr r4, [r5]
    sub r4, r4, #0xa
    cmp r2, r4
    movne r0, #52
    bne fail
    ldrb r3, [r2, r7]!
    ldr r4, [r5]
    cmp r2, r4
    movne r0, #53
    bne fail
    cmp r3, #0x78
    movne r0, #54
    bne fail

    /* *** Reset all register *** */
    ldr r2, LD_test_word
    ldr r0, [r2]
    ldr r4, LD_test_addr
    ldr r2, [r4]
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
    ldr r8, =#0x2014
    ldr r4, [r8]
    cmp r4, r3
    movne r0, #55
    bne fail
    cmp r3, r0
    movne r0, #55
    bne fail

    // STR Post Positive LDR Pre Negative With LSR
    str r2, [r5]
    strb r0, [r2], r7, lsr #2 // Mem[0x2000] = #0x78000000, r2 = #0x2001
    ldr r4, [r5]
    add r4, r4, #0x1
    cmp r2, r4
    movne r0, #56
    bne fail
    ldrb r3, [r2, -r7, lsr #2]! // r3 = #0x78
    ldr r4, [r5]
    cmp r2, r4
    movne r0, #57
    bne fail
    cmp r3, #0x78
    movne r0, #58
    bne fail

    // STRB Pre Negative LDRB Post Negative With ASR
    strb r0, [r2, r6, asr #2]! // Mem[0x2000 + (-0xf >> 2)] = Mem[0x1ffc] = #0x78000000
    ldr r4, [r5]
    sub r4, r4, #0x4
    cmp r2, r4
    movne r0, #59
    bne fail
    ldrb r3, [r2], -r6, asr #2  
    ldr r4, [r5]
    cmp r2, r4
    movne r0, #60
    bne fail
    cmp r3, #0x78
    movne r0, #61
    bne fail
    mov r0, #0
    
    /* *********** */
    // All tests passed
end:
    swi #0x123456

fail:
    // Some test failed
    swi #0x123456

LD_test_word: .word test_word
LD_test_addr: .word test_addr
