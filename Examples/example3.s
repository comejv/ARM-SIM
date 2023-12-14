.global main
.text
main:
    ldr r0, =limite
    ldrb r1, [r0]
    add r0, r0, #3
    ldrb r2, [r0]
    swi 0x123456
.data
limite:
    .word 0x12345678
