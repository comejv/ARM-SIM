#include "my_syscalls.h"

void clock(int n)
{
    for (int i = 0; i < n; i++)
    {
    }
}

int main()
{
    char text[] = "Chenillion";
    int len = sizeof(text) - 1;

    while (1)
    {
        for (int i = 0; i < len; i++)
        {
            my_putchar(text[i]);
            // Forcer l'affichage
            clock(100000);
        }
        for (int i = len - 1; i >= 0; i--)
        {
            my_putchar('\b');
            my_putchar('\b'); // Effacer le dernier caractère
        }
    }

    return 0;
}
