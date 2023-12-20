.global main
.text

main:
b fail

fail:
mov r0, #69
swi #123456

.data
