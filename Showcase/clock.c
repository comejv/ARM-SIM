#include "my_syscalls.h"

void message()
{
    my_putchar('I');
    my_putchar('\n');
    my_putchar('R');
    my_putchar('\n');
    my_putchar('Q');
    my_putchar('\n');
}

void irq_handler(void)
{
    int lr;
    asm("mov %0, lr" : "=r"(lr));
    /* Here your irq code */

    message();

    /**********************/
    asm("mov lr, %0" : "=r"(lr));
    asm("b acquitter_irq");
}

void __attribute__((section(".irq_vector"), naked)) irq_vector(void)
{
    asm volatile(
        "b irq_handler");
}

int main()
{
    while (1)
    {
    }
    return 0;
}
