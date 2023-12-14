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
#include <stdlib.h>
#include <string.h>
#include "arm_constants.h"

static char *arm_mode_names[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, "USR", "FIQ", "IRQ", "SVC", 0, 0, 0,
    "ABT", 0, 0, 0, "UND", 0, 0, 0, "SYS"
};

static char *arm_register_names[] = { "R00", "R01", "R02", "R03", "R04", "R05",
    "R06", "R07", "R08", "R09", "R10", "R11",
    "R12", "SP", "LR", "PC", "CPSR", "SPSR"
};

static char *arm_exception_names[] = { NULL,
    "reset",
    "data abort",
    "fast interrupt",
    "interrupt",
    "imprecise abort",
    "prefetch abort",
    "undefined instruction",
    "software interrupt",
};

char *arm_get_exception_name(unsigned char exception) {
    if (exception < 8)
        return arm_exception_names[exception];
    else
        return NULL;
}

char *arm_get_mode_name(uint8_t mode) {
    return arm_mode_names[mode];
}

int8_t arm_get_mode_number(char *name) {
    for (int i = 0; i < 32; i++)
        if (arm_mode_names[i] && strcmp(name, arm_mode_names[i]) == 0)
            return i;
    return -1;
}

char *arm_get_register_name(uint8_t reg) {
    return arm_register_names[reg];
}
