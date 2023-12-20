.global main
.text
incr:
    add r0, r0, #2
    cmp r0, r1, LSL #3
    mov pc, lr
main:
    mov r0, #5
    mov r1, #1 
loop:
    bl incr
    blt loop
end:
    swi 0x123456
.data