.global main
.text

main:
    mov r0, #0x1234
    mov r1, #0x5678
    mov r2, #0x90ab
    mov r3, #0xcdef
    mov r4, #0x2000
    mov r5, #0x3000
    mov r6, #0x4000
    mov r7, #0x5000

    // Store Multiple Decrement Before (Full Descending)
    stmda r4!, {r0-r3}
    // Load Multiple Decrement Before (Full Descending)
    ldmda r4, {r0-r3}
    cmp r0, #0x1234
    bne fail
    cmp r1, #0x5678
    bne fail
    cmp r2, #0x90ab
    bne fail
    cmp r3, #0xcdef
    bne fail

    // Store Multiple Increment Before (Full Ascending)
    stmib r5!, {r0-r3}
    // Load Multiple Increment Before (Full Ascending)
    ldmib r5, {r0-r3}
    cmp r0, #0x1234
    bne fail
    cmp r1, #0x5678
    bne fail
    cmp r2, #0x90ab
    bne fail
    cmp r3, #0xcdef
    bne fail

    // Store Multiple Decrement After (Empty Ascending)
    stmdb r6!, {r0-r3}
    // Load Multiple Decrement After (Empty Ascending)
    ldmdb r6, {r0-r3}
    cmp r0, #0x1234
    bne fail
    cmp r1, #0x5678
    bne fail
    cmp r2, #0x90ab
    bne fail
    cmp r3, #0xcdef
    bne fail

    // Store Multiple Increment After (Empty Descending)
    stmia r7!, {r0-r3}
    // Load Multiple Increment After (Empty Descending)
    ldmia r7, {r0-r3}
    cmp r0, #0x1234
    bne fail
    cmp r1, #0x5678
    bne fail
    cmp r2, #0x90ab
    bne fail
    cmp r3, #0xcdef
    bne fail

    // All tests passed
    
end:
    mov r0, #0
    swi 0x123456

fail:
    // Some test failed
    mov r0, #1
    swi 0x123456