.global main
.text

main:
    mov r0, #5
    swi 0x000001
    mov r0, #6
    swi 0x000001 
    mov r0, #7
    swi 0x000001 
    mov r0, #8
    swi 0x000001 
    mov r0, #9
    swi 0x000001 
    mov r0, #10
    swi 0x000001
end:
    swi 0x123456

.data
