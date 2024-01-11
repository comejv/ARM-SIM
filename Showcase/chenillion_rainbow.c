#include "my_syscalls.h"
#include "my_utils.h"

int main()
{
    char *text = "SE DEMANDE SI ELEVER DES CHEVRES EN ARDECHE EST UNE BONNE SITUATION. "; //
    int len = my_strlen(text);
    int len_affiche = 30;
    int i, j, x;
    char temp[len_affiche];
    while(1)
    {
        for (i = 0; i < len; i++)
        {
            for (j = 0; j < len_affiche; j++)
            {
                x = my_modulo(i + j, len-1);
                temp[j] = text[x];
            }
            my_printstr("\x1b[");
            my_printint( my_modulo(i, 7) == 0 ? 93 : my_modulo(i, 7)+ 90);
            my_printstr("m");
            my_printstr(temp);
            my_putchar('\r'); // Retour au début de la ligne
            my_clock(50000);
        }
    }
    my_exit();
}
