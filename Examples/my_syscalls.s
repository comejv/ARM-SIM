.global my_exit
.global my_putchar
.global my_printint 
.global acquitter_irq 
my_exit:
    swi 0x123456
my_putchar:
    swi 0x000001
    mov pc, lr
my_printint:
    swi 0x000002
    mov pc, lr    
acquitter_irq:
	swi 0xFFFFFF
	mov pc, lr 