@ Tests instruction simple
@
@ eq	    Z==1
@ ne	    Z==0
@ cs or hs	C==1
@ cc or lo	C==0
@ mi	    N==1
@ pl	    N==0
@ vs	    V==1
@ vc	    V==0
@ hi	    (C==1) && (Z==0)
@ ls	    (C==0) || (Z==1)
@ ge	    N==V
@ lt	    N!=V
@ gt	    (Z==0) && (N==V)
@ le	    (Z==1) || (N!=V)
@
@ Résultats attendus par registre à la fin de ce test:
@ R0 = 0
@ Si R0 n'est pas égal à 0, cela signifie qu'il y a eu une erreur:
@ R0 = 1 : MOV
@ R0 = 2 : MVN
@ R0 = 3 : AND
@ R0 = 4 : EOR
@ R0 = 5 : SUB
@ R0 = 6 : RSB
@ R0 = 7 : ADD
@ R0 = 8 : ADC
@ R0 = 9 : SBC
@ R0 = 10: RSC
@ R0 = 11: ORR
@ R0 = 12: BIC
@
@ R0 = 20: MOV LSL imm
@ R0 = 21: MOV LSR imm
@ R0 = 22: MOV ASR imm
@ R0 = 23: MOV ROR imm
@ R0 = 24: MOV LSL reg
@ R0 = 25: MOV LSR reg
@ R0 = 26: MOV ASR reg
@ R0 = 27: MOV ROR reg
@
@ R0 = 30: ADDS set C
@ R0 = 31: ADDS set V
@ R0 = 32: ANDS set C
@ R0 = 33: EORS set C
@ R0 = 34: SUBS set C
@ R0 = 35: SUBS set V
@ R0 = 36: RSBS set C
@ R0 = 37: RSBS set V

.global main

.text

main:

    LDR R4, =#0xA
    LDR R5, =#0xFFFFFFFF
    LDR R6, =#0x80000002
    LDR R7, =#0xFFFFFFFB

    MOV R0, #5
    MOV R1, R0
    CMP R1, #5
    MOV R0, #1
    BNE fail

    MVN R0, #0
    CMP R0, R5
    MOV R0, #2
    BNE fail

    AND R0, R1, #15
    CMP R0, #5
    MOV R0, #3
    BNE fail

    EOR R0, R1, #15
    CMP R0, #10
    MOV R0, #4
    BNE fail

    SUB R0, R4, #5
    CMP R0, #5
    MOV R0, #51
    BNE fail
    SUB R0, R4, #15
    CMP R0, R7
    MOV R0, #52
    BNE fail

    MOV R4, #5 
    RSB R8, R4, #10
    CMP R8, #5
    MOV R0, #6
    BNE fail

    MOV R4, #2
    ADD R0, R4, #3
    CMP R0, #5
    MOV R0, #7
    BNE fail

    MOV R4, #2
    ADDS R1, R4, #1
    ADC R0, R4, #3
    CMP R0, #5
    MOV R0, #8
    BNE fail

    MOV R4, #10
    ADDS R1, R4, #1
    @Carry = 0 => ~Carry = 1
    SBC R8, R4, #5
    CMP R8, #4
    MOV R0, #9
    BNE fail

    MOV R4, #4
    ADDS R1, R4, #1
    @Carry = 0 => ~Carry = 1
    RSC R0, R4, #10
    CMP R0, #5
    MOV R0, #10
    BNE fail

    MOV R4, #0
    ORR R0, R4, #5
    CMP R0, #5
    MOV R0, #11
    BNE fail

    MOV R4, #5
    BIC R0, R4, #10
    CMP R0, #5
    MOV R0, #12
    BNE fail

    @ Tests shift par valeur immédiate, par registre, LSL, LSR, ASR, ROR
    @ Résultats attendus par registre à chaque test:
    @ R0 = 5 * 2 = 10
    @ R1 = 10 // 2 = 5
    @ R0 = 2  |   0000...0101 -> 0000...0010
    @ R3 = 2^32 + 2    |   0000...0101 -> 1000...0010

    @ Tests par valeur immédiate

    MOV R4, #5
    MOV R0, R4, LSL #1
    CMP R0, #10
    MOV R0, #20
    BNE fail

    MOV R0, R4, LSR #1
    CMP R0, #2
    MOV R0, #21
    BNE fail

    MOV R0, R4, ASR #1 
    CMP R0, #2
    MOV R0, #22
    BNE fail

    MOV R0, R4, ROR #1
    CMP R0, R6
    MOV R0, #23
    BNE fail

    @ Tests par registre
    MOV R1, #1

    MOV R0, R4, LSL R1
    CMP R0, #10
    MOV R0, #24
    BNE fail

    MOV R0, R4, LSR R1
    CMP R0, #2
    MOV R0, #25
    BNE fail

    MOV R0, R4, ASR R1
    CMP R0, #2
    MOV R0, #26
    BNE fail

    MOV R0, R4, ROR R1
    CMP R0, R6
    MOV R0, #27
    BNE fail

    @ Tests instruction avec S

    @Case 0

    MOV R4, #3
    ADDS R0, R4, #2 
    MOV R0, #30
    BCS fail            @c = 1
    MOV R0, #31
    BVS fail            @v = 1

    @Case 1
    MOV R1, R5
    MOV R2, #0

    ANDS R0, R1, R2, LSL #2
    MOV R0, #32
    BCS fail

    EORS R0, R1, R2
    MOV R0, #33
    BCS fail

    @Case 2

    SUBS R0, R1, R2     @   1111 1111 1111 1111        1111 1111 1111 1111
    MOV R0, #34         @ - 0000 0000 0000 0000  <=> +10000 0000 0000 0000  => C = 1 and V = 1  
    BCC fail            
    MOV R0, #35
    BVS fail

    RSBS R0, R1, R2
    MOV R0, #36
    BCS fail
    MOV R0, #37
    BVS fail

end:
    mov r0, #0
    swi #0x123456

fail:
    mov r1, #0
    SWI #0x123456
    
