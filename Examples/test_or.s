.global main
.text
main:
    mov r0, #0xF0
    orr r0, r0, #0x0F
end:
    swi 0x123456
.data