#include "my_syscalls.h"

void clock(int n)
{
    for (int i = 0; i < n; i++)
    {
    }
}
int longueur(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        i++;
    }
    return i;
}

int main()
{
    char *text = "PROCHAIN ARRET : PONTCHARRA SUR BREDA.";
    int len = 39; // longueur(text);
    int len_affiche = 20;
    int i, j,x;
    char temp[len_affiche];
    my_printstr(text);
    my_putchar('\n');
    for (int g = 0; g < 2; g++)
    {
        for (i = 0; i < len; i++)
        {
            for (j = 0; j < len_affiche; j++)
            {
                x = (i + j);
                while (x >= len)
                {
                    x = x - len;
                }
                temp[j] = text[x];
            }
            my_printstr(temp);
            clock(50000);
            my_putchar('\n'); // Retour au début de la ligne
            
        }
    }
    my_exit();
}
