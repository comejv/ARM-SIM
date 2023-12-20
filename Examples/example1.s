.global main
.text
decr:
    subs r0, r0, #1
    add r0, r0, #2
    add r0, r0, r0
    mov pc, lr

main:
    mov r0, #0
    mov r0, #5
loop:
    bl decr
    mov r1, #1
    mov r2, #2
    mov r0, r1, LSL #2 
    mov r0, r0, LSR #1
    bne loop
end:
    swi 0x123456
.data
