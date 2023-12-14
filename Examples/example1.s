.global main
.text
decr:
    subs r0, r0, #1
    mov pc, lr

main:
    mov r0, #5
loop:
    bl decr
    bne loop
end:
    swi 0x123456
.data
