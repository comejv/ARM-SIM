.global my_exit
.global my_putchar
my_exit:
    swi 0x123456
my_putchar:
	swi 0x000001
	mov pc, lr
