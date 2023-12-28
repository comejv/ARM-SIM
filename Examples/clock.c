#include "my_syscalls.h"

void reception_IRQ(){
    my_putchar('I');
    my_putchar('R');
    my_putchar('Q');
    my_putchar('\n');
}

__attribute__((section(".irq_vector"))) void irq_handler(void)
{
    reception_IRQ();
} 



int main()
{
    while (1)
    {
    }
    return 0;
}