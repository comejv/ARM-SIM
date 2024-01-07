#include "stdio.h"
#include "unistd.h"

// void clock(int n)
// {
//     for (int i = 0; i < n; i++)
//     {
//     }
// }
int longueur(char *text)
{
    int i = 0;
    while (text[i] != '\0')
    {
        i++;
    }
    return i;
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
    char *text = "PROCHAIN ARRET : PONTCHARRA SUR BREDA ";
    int len = longueur(text) + 1;
    int len_affiche = 20;
    int i, j, x;
    char temp[len_affiche];
    for (int g = 0; g < 2; g++)
    {
        for (i = 0; i < len; i++)
        {
            for (j = 0; j <= len_affiche; j++)
            {
                x = division_reste(i + j, len - 1);
                temp[j] = text[x];
            }
            printf("%s", temp);
            usleep(50000);
            printf("\n"); // Retour au début de la ligne
        }
    }
    return 0;
}
