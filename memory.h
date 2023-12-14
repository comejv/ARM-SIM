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
#ifndef __MEMORY_H__
#define __MEMORY_H__
#include <stdint.h>
#include <sys/types.h>

/*
A heap memory type
- size: the size of memory
- data: uint8_t pointer to the beginning of memory
*/
typedef struct memory_data *memory;

/*
Create a memory pointer with a given size. Uses Little Endian by default.
Data: - size: the desired memory size
Return: the allocated and created memory
Side Effect: memory allocation
*/
memory memory_create(size_t size);

/*
Return the size of a given memory_data
Data: - mem: the memory whose size is to be retrieved
Return: the size of mem memory_data
Side Effect: none
*/
size_t memory_get_size(memory mem);

/*
Destroy a given memory object
Data: - mem: the memory to be destroyed
Return: none
Side Effect: memory deallocation
*/
void memory_destroy(memory mem);

/* All these functions perform a read/write access to a byte/half/word data at
 * address a in mem. The result is respectively taken from or stored to the
 * parameter value. The access is made using the given endianness (be == 1 for a
 * big endian access and be == 0 for a little endian access).
 * The return value indicates a succes (0) or a failure (-1).
 */

/*
Read a byte to mem at a given address
Data: - mem: the memory to be read from
	  - address: the address to be read from
	  - value: the value to be set
*/
int memory_read_byte(memory mem, uint32_t address, uint8_t * value);

/*
Read a half word to mem at a given address using a given endianness
Data: - mem: the memory to be read from
	  - address: the address to be read from
	  - value: the value to be set
	  - be: the endianess to be used
*/
int memory_read_half(memory mem, uint32_t address, uint16_t * value, uint8_t be);

/*
Read a word to mem at a given address using a given endianness
Data: - mem: the memory to be read from
	  - address: the address to be read from
	  - value: the value to be set
	  - be: the endianess to be used
*/
int memory_read_word(memory mem, uint32_t address, uint32_t * value, uint8_t be);

/*
Write a byte to mem at a given address
Data: - mem: the memory to be written to
	  - address: the address to be written to
	  - value: the value to be written
*/
int memory_write_byte(memory mem, uint32_t address, uint8_t value);

/*
Write a half word to mem at a given address using a given endianness
Data: - mem: the memory to be written to
	  - address: the address to be written to
	  - value: the value to be written
	  - be: the endianess to be used
*/
int memory_write_half(memory mem, uint32_t address, uint16_t value, uint8_t be);

/*
Write a word to mem at a given address using a given endianness
Data: - mem: the memory to be written to
	  - address: the address to be written to
	  - value: the value to be written
	  - be: the endianess to be used
*/
int memory_write_word(memory mem, uint32_t address, uint32_t value, uint8_t be);

#endif
