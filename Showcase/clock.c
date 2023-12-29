#include "my_syscalls.h"

void message()
{
    my_putchar('I');
    my_putchar('\n');
}

__attribute__((section(".irq_vector"))) void irq_handler(void)
{
    message();
    acquitter_irq();
}

int main()
{
    for (int i = 0; i < 100000; i++)
    {
    }
    return 0;
}