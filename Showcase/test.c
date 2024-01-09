#include "stdio.h"

void clock(int n)
{
    for (int i = 0; i < n; i++)
    {
    }
}
int longueur(char *text)
{
    int i = 0;
    while (text[i] != '\0')
    {
        i++;
    }
    return i + 1;
}
int division_reste(int a, int b)
{
    while (a >= b)
    {
        a = a - b;
    }
    return a;
}

int main()
{
    char *text = "PROCHAIN ARRET : PONTCHARRA SUR BREDA. "; //
    int len = longueur(text);
    int len_affiche = 20;
    int i, j, x;
    char temp[len_affiche];
    while (1)
    {
        for (i = 0; i < len; i++)
        {
            for (j = 0; j < len_affiche; j++)
            {
                x = division_reste(i + j, len - 1);
                temp[j] = text[x];
                // my_printint(i + j);
                // my_printint(x);
                // my_putchar('\n');
                printf("%d %d %d\n", i, j, x);
            }
            
            temp[len_affiche] = '\0';

            //printf("%s\n",temp);
            clock(50000);

        }
    }
    return 0;
}
