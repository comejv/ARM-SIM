.global main
.text
main:
    mvns r0, #1
    bmi end 
    b main
end:
    swi 0x123456
.data