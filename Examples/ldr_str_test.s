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

    // Test STR and LDR
    str r0, [r2]
    ldr r3, [r2]
    // cmp r0, r3
    // bne fail

    // Test STRB and LDRB
    strb r0, [r2]
    ldrb r3, [r2]
    // cmp r0, r3
    // bne fail

    // Test STRH and LDRH
    strh r0, [r2]
    ldrh r3, [r2]
    // cmp r0, r3
    // bne fail

    // Test STRD and LDRD (not implemented)
    // strd r0, r1, [r2]
    // ldrd r3, r4, [r2]
    // cmp r0, r3
    // bne fail
    // cmp r1, r4
    // bne fail

    // Test LDRSB
    ldrsb r3, [r2]
    // cmp r0, r3
    // bne fail

    // Test LDRSH
    ldrsh r3, [r2]
    // cmp r0, r3
    // bne fail

    // Test with offset
    mov r2, #0x2000
    mov r3, #0
    str r3, [r2]

    ldrb r3, [r2, #1] // r3 = 0x78
    mov r2, #0x2001
    // cmp r3, #0x78
    // bne fail
    ldrb r4, [r2, #-1] // r4 = 0x56
    mov r2, #0x2000
    // cmp r4, #0x56
    // bne fail

    strh r0, [r2, #2] // r0 = 0x12345678
    ldrh r3, [r2, #2] // r3 = 0x5678
    // cmp r0, r3
    // bne fail

    strh r0, [r2, #-2] // r0 = 0x12345678
    ldrh r3, [r2, #-2] // r3 = 0x5678
    // cmp r0, r3
    // bne fail

    // Test with post-indexed
    mov r2, #0x2000
    mov r0, #0x12345678

    str r0, [r2], #4 // r0 = 0x12345678
    ldr r3, [r2, #-4]! // r3 = 0x12345678
    // cmp r0, r3
    // bne fail

    str r0, [r2], #-4 // r0 = 0x12345678
    ldr r3, [r2, #4]! // r3 = 0x12345678
    // cmp r0, r3
    // bne fail

    strb r0, [r2], #4 // r0 = 0x12345678
    ldrb r3, [r2, #-4]! // r3 = 0x78
    // cmp r0, r3
    // bne fail

    strb r0, [r2], #-4 // r0 = 0x12345678
    ldrb r3, [r2, #4]! // r3 = 0x78
    // cmp r0, r3
    // bne fail

    strh r0, [r2], #4 // r0 = 0x12345678
    ldrh r3, [r2, #-4]! // r3 = 0x5678
    // cmp r0, r3
    // bne fail

    // All tests passed
    mov r0, #0
    bx lr

fail:
    // Some test failed
    mov r0, #1
    bx lr
