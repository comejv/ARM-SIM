#include "my_syscalls.h"

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
    return i+1;
}
int division_reste(int a, int b)
{
    while (a > b-1)
    {
        a = a - b;
    }
    return a;
}

int main()
{
    char *text = "PROCHAIN ARRET : PONTCHARRA SUR BREDA. "; //
    int len = longueur(text);
    my_printint(len);
    int len_affiche = 20;
    int i, j, x;
    //char temp[len_affiche];
    while(1)
    {
        for (i = 0; i < len; i++)
        {
            for (j = 0; j < len_affiche; j++)
            {
                x = division_reste(i + j, len-1);
                //temp[j] = text[x];
                // my_printint(i + j);
                // my_printint(x);
                //my_putchar('\n');
                my_printint(i);
                my_putchar('+');
                my_printint(j);
                my_putchar('/');
                my_printint(len - 1);
                my_putchar('=');
                my_printint(x);
                my_putchar(' ');
                
                my_putchar('\n');
            }
            //temp[len_affiche] = '\0';
    
            // my_printstr(temp);
            clock(50000);
            my_putchar('\n'); // Retour au début de la ligne
        }
    }
    my_exit();
}
