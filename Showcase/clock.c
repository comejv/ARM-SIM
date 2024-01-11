#include "my_syscalls.h"
#include "my_utils.h"

void irq_handler(void)
{
    int lr;
    asm("mov %0, lr" : "=r"(lr));
    /* Here your irq code */

    asm("sub r8, r8, #1");

    /**********************/
    asm("mov lr, %0" : "=r"(lr));
    asm("b acquitter_irq");
}

void __attribute__((section(".irq_vector"), naked)) irq_vector(void)
{
    asm volatile(
        "b irq_handler");
}

void main()
{
    int n_min = 5;
    asm("mov r8, %0" : "=r"(n_min));

    while (1)
    {
        asm("mov %0, r8" : "=r"(n_min));
        my_printstr("\x1b[2K\x1b[");
        my_printint(my_modulo(n_min, 7) == 0 ? 93 : my_modulo(n_min, 7) + 90);
        my_printstr("m");

        switch (n_min)
        {
        case -1:
            my_printstr("Et oh j'ai dit que c'était fini !\n");
            my_exit();
        case 0:
            my_printstr("La présentation est finie !\r");
            break;
        case 1:
            my_printstr("Il ne reste plus que ");
            my_printint(n_min);
            my_printstr(" minute de présentation...\r");
            break;
        default:
            my_printstr("Il ne reste plus que ");
            my_printint(n_min);
            my_printstr(" minutes de présentation...\r");
            break;
        }
    }
    my_exit();
}
