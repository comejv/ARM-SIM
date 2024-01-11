.global main
.text

main:
    mov r8, #0x12
    mov r1, #0x56
    mov r2, #0x90
    mov r3, #0xcd
    mov r4, #0x20
    mov r5, #0x30
    mov r6, #0x40
    mov r7, #0x50
    mov r0, #1

    // Store Multiple Decrement Before (Full Descending)
    stmda r7!, {r8, r1, r2, r3}
    mov r8, #0
    mov r1, #0
    mov r2, #0
    mov r3, #0
    // Load Multiple Decrement Before (Full Descending)
    ldmib r7!, {r8, r1, r2, r3}
    cmp r8, #0x12
    bne fail
    cmp r1, #0x56
    bne fail
    cmp r2, #0x90
    bne fail
    cmp r3, #0xcd
    bne fail

    // Store Multiple Increment Before (Full Ascending)
    stmdb r5!, {r8, r1, r2, r3}
    mov r8, #0
    mov r1, #0
    mov r2, #0
    mov r3, #0
    // Load Multiple Increment Before (Full Ascending)
    ldmia r5!, {r8, r1, r2, r3}
    cmp r8, #0x12
    bne fail
    cmp r1, #0x56
    bne fail
    cmp r2, #0x90
    bne fail
    cmp r3, #0xcd
    bne fail

    // Store Multiple Decrement After (Empty Ascending)
    push {r8, r1, r2, r3}
    mov r8, #0
    mov r1, #0
    mov r2, #0
    mov r3, #0
    // Load Multiple Decrement After (Empty Ascending)
    pop {r8, r1, r2, r3}
    cmp r8, #0x12
    bne fail
    cmp r1, #0x56
    bne fail
    cmp r2, #0x90
    bne fail
    cmp r3, #0xcd
    bne fail
    mov r0, #0

    mov r0, #0x12
    mov r1, #0x23
    mov r2, #0x2000
    strd r0, [r2]
    ldrd r4, [r2]
    cmp r0, r4
    movne r0, #1
    bne fail
    cmp r1, r5
    movne r0, #2
    bne fail
    mov r0, #0
    // All tests passed
end:
    swi 0x123456

fail:
    // Some test failed
    swi 0x123456
