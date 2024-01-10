.global main

.text

main:
    ldr r3, =#0x0
    ldr r4, =#0xA
    ldr r5, =#0xFFFFFFFF
    ldr r6, =#0x80000002
    ldr r7, =#0xFFFFFFFB

    @ r1: for the instructions
    @ r2: result for testing

    // --- DATA PROCESSING IMMEDIATE --- //
    @ mov/mvn
    mov r2, #10
    cmp r2, r4
    movne r0, #1
    bne fail

    mvn r2, #0
    cmp r2, r5
    movne r0, #2 
    bne fail

    @ and/ands
    mov r1, #0b0011
    and r2, r1, #0b0101
    cmp r2, #1
    movne r0, #3
    bne fail

    ands r2, r1, #0b0101
    movne r0, #4
    beq fail // Test Z != 0
    movne r0, #5
    blt fail // Test N != 1
    adc r3, r2, #0
    cmp r3, r2 // Test C != 1
    movne r0, #6
    bne fail
    cmp r2, #1 // Test Result
    movne r0, #7
    bne fail

    @ eor/eors - CV unaffected
    mov r1, #0b0011
    eor r2, r1, #0b0101
    cmp r2, #0b0110
    movne r0, #8
    bne fail

    eors r2, r1, #0b1010
    movne r0, #9
    beq fail //  Test Z != 0
    movne r0, #10
    blt fail // Test N != 1
    cmp r2, #0b1001
    movne r0, #11 
    bne fail
    
    @ sub
    mov r1, #10
    sub r2, r1, #3
    cmp r2, #7
    movne r0, #12
    bne fail

    subs r2, r1, #11
    movne r0, #13
    beq fail // Test Z != 0
    blt ok // Test N
    movne r0, #14
    b fail
ok: // Test V ?1
    adc r2, r2, #0
    cmp r2, #0
    movne r0, #15
    bne fail // Test C

    @ rsb
    rsb r2, r4, #5
    cmp r2, r7
    movne r0, #016
    bne fail

    @ add
    add r2, r4, #2
    cmp r2, #12
    movne r0, #17
    bne fail 

    adds r2, r4, #-0xA
    movne r0, #18
    bne fail
    movne r0, #19
    blt fail     
    cmp r2, #0
    bne fail

    @ sbc
    adds r7, r4, #1
    sbc r2, r4, #5
    cmp r2, #4
    movne r0, #20
    bne fail

    @ rsc // NOT COMPLETED
    mov r7, #4
    adds r1, r7, #1
    @Carry = 0 => ~Carry = 1
    RSC r0, r4, #10
    cmp r0, r5
    movne r0, #20
    bne fail

    @ ORR
    mov r1, #0b0011
    orr r2, r1, #0b0101
    cmp r2, #0b0111
    movne r0, #21
    bne fail

    @ BIC
    mov r4, #10
    bic r2, r4, #3
    cmp r2, #8
    movne r0, #22
    bne fail

    // RESET REGISTER
    mov r2, #0
    mov r3, #0
    mov r1, #0
    ldr r4, =#0xA
    ldr r5, =#0xFFFFFFFF
    ldr r6, =#0x80000002
    ldr r7, =#0xFFFFFFFB
    // --- DATA PROCESSING IMMEDIATE SHIFT --- //

    mov r4, #5
    mov r0, r4, LSL #1
    cmp r0, #10
    movne r0, #23
    bne fail

    mov r0, r4, LSR #1
    cmp r0, #2
    movne r0, #21
    bne fail

    mov r0, r4, ASR #1 
    cmp r0, #2
    movne r0, #22
    bne fail

    mov r0, r4, ROR #1
    cmp r0, r6
    movne r0, #23
    bne fail

    // RESET REGISTER
    mov r2, #0
    mov r3, #0
    mov r1, #0
    ldr r4, =#0xA
    ldr r5, =#0xFFFFFFFF
    ldr r6, =#0x80000002
    ldr r7, =#0xFFFFFFFB
    // --- DATA PROCESSING REGISTER SHIFT --- //
    @ Tests par registre
    mov r1, #1

    mov r0, r4, LSL r1
    cmp r0, #20
    movne r0, #24
    bne fail

    mov r0, r4, LSR r1
    cmp r0, #5
    movne r0, #25
    bne fail

    mov r0, r4, ASR r1
    cmp r0, #5
    movne r0, #26
    bne fail

    mov r0, r4, ROR r1
    cmp r0, #5
    movne r0, #27
    bne fail
    mov r0, #0

end:
    swi #0x123456

fail:
    swi #0x123456