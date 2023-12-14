/*
Armator - simulateur de jeu d'instruction ARMv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).

Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�cifique. Reportez-vous � la
Licence Publique G�n�rale GNU pour plus de d�tails.

Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me
temps que ce programme ; si ce n'est pas le cas, �crivez � la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
�tats-Unis.

Contact: Guillaume.Huard@imag.fr
	 B�timent IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'H�res
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "registers.h"
#include "arm_constants.h"
#include "util.h"

void print_test(int result) {
    if (result)
        printf("Test succeded\n");
    else
        printf("TEST FAILED !!\n");
}

int main() {
    registers r;
    uint32_t word_value[15], word_read;
    int i;

    r = registers_create();
    if (r == NULL) {
        fprintf(stderr, "Error when creating simulated registers\n");
        exit(1);
    }
    printf("Registers created\n");
    srandom(getpid());
    for (i = 0; i < 15; i++) {
        word_value[i] = random();
    }

    // Arm reset
    printf("Reseting registers...\n");
    registers_write_cpsr(r, 0x1d3);
    uint8_t mode = registers_get_mode(r);
    registers_write(r, 7, mode, word_value[0]);
    printf("PC value: %d %d\n",*(r->reg[7].data[mode]), word_value[0]);
    printf("Testing read and write to general registers:\n");
    for (i = 0; i < 15; i++) {
        registers_write(r, i, mode, word_value[i]);
    }
    for (i = 0; i < 15; i++) {
        word_read = registers_read(r, mode, i);
        printf("- register %d : ", i);
        print_test(word_read == word_value[i]);
    }

    printf("Current mode : ");
    print_test(registers_get_mode(r) == SVC);
    printf("Mode is priviledged : ");
    print_test(registers_in_a_privileged_mode(r));
    printf("Mode has spsr : ");
    print_test(registers_current_mode_has_spsr(r));

    registers_destroy(r);

    return 0;
}
