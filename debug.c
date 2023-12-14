/*
Armator - simulateur de jeu d'instruction ARMv5T à but pédagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.

Contact: Guillaume.Huard@imag.fr
	 Bâtiment IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'Hères
*/
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_FILES_NUMBER 64

char *debugged_files[MAX_FILES_NUMBER];
int nb_debugged_files = 0;

void add_debug_to(char *name) {
    int i = 0, j;

    while ((i < nb_debugged_files) && (i < MAX_FILES_NUMBER) &&
           (strcmp(debugged_files[i], name) < 0)) {
        i++;
    }
    if (i >= MAX_FILES_NUMBER)
        return;
    for (j = nb_debugged_files; j > i; j--)
        debugged_files[j] = debugged_files[j - 1];
    debugged_files[i] = name;
    nb_debugged_files++;
}

int __is_debugged(char *name) {
    int from, to, middle, result;

    from = 0;
    to = nb_debugged_files - 1;
    while (from < to) {
        middle = (from + to) / 2;
        result = strcmp(debugged_files[middle], name);
        if (result < 0)
            from = middle + 1;
        else if (result > 0)
            to = middle - 1;
        else
            return 1;
    }
    if ((from == to) && (strcmp(debugged_files[from], name) == 0))
        return 1;
    return 0;
}

int __debug_raw_binary(char *data, int len) {
    while (len--) {
        if (isprint(*data))
            fprintf(stderr, "%c", *data);
        else
            fprintf(stderr, ".");
        data++;
    }
    return 0;
}
