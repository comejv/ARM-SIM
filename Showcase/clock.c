#include "my_syscalls.h"

void message()
{
    my_putchar('I');
    my_putchar('\n');
}

void irq_handler(void)
{
    message();
    acquitter_irq();
}

void __attribute__((section(".irq_vector"), naked)) irq_vector(void)
{
    asm volatile(
        "b irq_handler"
    );
}

int main()
{
    while (1)
    {
    }
    return 0;
}
