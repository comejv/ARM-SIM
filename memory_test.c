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
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "memory.h"
#include "util.h"

void print_test(int result) {
    if (result)
        printf("Test succeded\n");
    else
        printf("TEST FAILED !!\n");
}

int compare(void *a, void *b, size_t size, int reverse) {
    int i, j, j_step;

    if (reverse) {
        j = size - 1;
        j_step = -1;
    } else {
        j = 0;
        j_step = 1;
    }
    for (i = 0; i < size; i++, j += j_step)
        if (*((uint8_t *) a + i) != *((uint8_t *) b + j))
            return 0;
    return 1;
}

int compare_with_sim(void *a, memory m, size_t size, int reverse) {
    int i, j, j_step;
    uint8_t value;

    if (reverse) {
        j = size - 1;
        j_step = -1;
    } else {
        j = 0;
        j_step = 1;
    }
    for (i = 0; i < size; i++, j += j_step) {
        memory_read_byte(m, j, &value);
        if (*((uint8_t *) a + i) != value)
            return 0;
    }
    return 1;
}

int main() {
    char *endianess[] = { "little", "big" };
    memory m;
    uint32_t word_value = 0x11223344, word_read;
    uint16_t half_value = 0x5566, half_read;
    uint8_t *position;
    int i, my_endianess;

    m = memory_create(4);
    if (m == NULL) {
        fprintf(stderr, "Error when creating simulated memory\n");
        exit(1);
    }
    assert(memory_get_size(m) == 4);

    my_endianess = is_big_endian();
    printf("I'm a %s endian host\n", endianess[my_endianess]);

    printf("Writing 4 bytes at address 0, then reading the word and half, "
           "the result should depend on endianess of the access to the simulated memory :\n");
    position = (uint8_t *) & word_value;
    for (i = 0; i < 4; i++) {
        memory_write_byte(m, i, *(position + i));
    }
    printf("- word read with the same endianess as me, ");
    memory_read_word(m, 0, &word_read, my_endianess);
    print_test(compare(&word_value, &word_read, 4, 0));
    printf("- half read with the same endianess as me, ");
    memory_read_half(m, 0, &half_read, my_endianess);
    print_test(compare(&word_value, &half_read, 2, 0));
    printf("- word read with a different endianess than me, ");
    memory_read_word(m, 0, &word_read, 1 - my_endianess);
    print_test(compare(&word_value, &word_read, 4, 1));
    printf("- half read with a different endianess than me, ");
    memory_read_half(m, 0, &half_read, 1 - my_endianess);
    print_test(compare(&word_value, &half_read, 2, 1));

    printf("Writing word and half at address 0, then reading the bytes, "
           "the result should depend on simulated memory endianess :\n");
    printf("- word write with the same endianess as me, ");
    memory_write_word(m, 0, word_value, my_endianess);
    print_test(compare_with_sim(&word_value, m, 4, 0));
    printf("- half write with the same endianess as me, ");
    memory_write_half(m, 0, half_value, my_endianess);
    print_test(compare_with_sim(&half_value, m, 2, 0));
    printf("- word write with a different endianess than me, ");
    memory_write_word(m, 0, word_value, 1 - my_endianess);
    print_test(compare_with_sim(&word_value, m, 4, 1));
    printf("- half write with a different endianess than me, ");
    memory_write_half(m, 0, half_value, 1 - my_endianess);
    print_test(compare_with_sim(&half_value, m, 2, 1));

    memory_destroy(m);

    return 0;
}
