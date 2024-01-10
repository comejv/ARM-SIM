#include "my_syscalls.h"

void clock(int n){
  for (int i = 0; i < n; i++){
    
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
    char *text = "SE DEMANDE SI ELEVER DES CHEVRES EN ARDECHES EST UNE BONNE SITUATION. "; //
    int len = longueur(text);
    int len_affiche = 30;
    int i, j, x;
    char temp[len_affiche];
    while(1)
    {
        for (i = 0; i < len; i++)
        {
            for (j = 0; j < len_affiche; j++)
            {
                x = division_reste(i + j, len-1);
                temp[j] = text[x];
            }
    
            my_printstr(temp);
            my_putchar('\r'); // Retour au début de la ligne
            clock(50000);
        }
    }
    my_exit();
}
