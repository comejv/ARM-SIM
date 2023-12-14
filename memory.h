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
#ifndef __MEMORY_H__
#define __MEMORY_H__
#include <stdint.h>
#include <sys/types.h>

typedef struct memory_data *memory;

memory memory_create(size_t size);
size_t memory_get_size(memory mem);
void memory_destroy(memory mem);

/* All these functions perform a read/write access to a byte/half/word data at
 * address a in mem. The result is respectively taken from or stored to the
 * parameter value. The access is made using the given endianess (be == 1 for a
 * big endian access and be == 0 for a little endian access).
 * The return value indicates a succes (0) or a failure (-1).
 */
int memory_read_byte(memory mem, uint32_t address, uint8_t * value);
int memory_read_half(memory mem, uint32_t address, uint16_t * value, uint8_t be);
int memory_read_word(memory mem, uint32_t address, uint32_t * value, uint8_t be);
int memory_write_byte(memory mem, uint32_t address, uint8_t value);
int memory_write_half(memory mem, uint32_t address, uint16_t value, uint8_t be);
int memory_write_word(memory mem, uint32_t address, uint32_t value, uint8_t be);

#endif
