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

/**
 * A memory object.
 * It contains a size and a pointer to the data.
 * The data is stored as an array of bytes.
 */
typedef struct memory_data *memory;

/**
 * @brief Creates a memory object of a given size. 
 * It allocates the memory for the data.
 *
 * @param size The size of the memory object to create.
 * @return The created memory object.
 */
memory memory_create(size_t size);

/**
 * @brief Returns the size of a memory object. No side effects.
 *
 * @param mem The memory object to get the size of.
 * @return The size of the memory object.
 */
size_t memory_get_size(memory mem);

/**
 * @brief Destroys a memory object. It frees the memory allocated for the data.
 *
 * @param mem The memory object to destroy.
 * @return void
 */
void memory_destroy(memory mem);

/* All the following functions perform a read/write access to a byte/half/word data at
 * address a in mem. The result is respectively taken from or stored to the
 * parameter value. The access is made using the given endianness (be == 1 for a
 * big endian access and be == 0 for a little endian access).
 * The return value indicates a succes (0) or a failure (-1).
 */

/**
 * @brief Reads a byte from a memory object at a given address. A NULL address
 * reads the mem[0] byte. No side effects.
 *
 * @param mem The memory object to read from.
 * @param address The address to read from.
 * @param value Pointer to a byte where the read value will be stored.
 * @return 0 if successful, -1 if the memory object is NULL or the address is out of bounds.
 */
int memory_read_byte(memory mem, uint32_t address, uint8_t *value);

/**
 * @brief Reads a 16-bit value from a memory object at a given address.
 * The value is read in either big-endian (BE) or little-endian (LE) format,
 * depending on the `be` flag. A NULL address reads the mem[0] byte. No side effects.
 *
 * @param mem The memory object to read from.
 * @param address The address to read from.
 * @param value Pointer to a 16-bit integer where the read value will be stored.
 * @param be Flag indicating whether to read in big-endian format.
 * @return 0 if successful, -1 if the memory object is NULL or the address is out of bounds.
 */
int memory_read_half(memory mem, uint32_t address, uint16_t *value, uint8_t be);

/**
 * @brief Reads a 32-bit value from a memory object at a given address.
 * The value is read in either big-endian (BE) or little-endian (LE) format,
 * depending on the `be` flag. A NULL address reads the mem[0] byte. sNo side effects.
 *
 * @param mem The memory object to read from.
 * @param address The address to read from.
 * @param value Pointer to a 32-bit integer where the read value will be stored.
 * @param be Flag indicating whether to read in big-endian format.
 * @return 0 if successful, -1 if the memory object is NULL or the address is out of bounds.
 */
int memory_read_word(memory mem, uint32_t address, uint32_t *value, uint8_t be);

/**
 * @brief Writes a byte to a memory object at a given address. A NULL address 
 * writes the mem[0] byte. Modificates the memory at a specified address.
 *
 * @param mem The memory object to write to.
 * @param address The address to write to.
 * @param value The value to write.
 * @return 0 if successful, -1 if the memory object is NULL or the address is out of bounds.
 */
int memory_write_byte(memory mem, uint32_t address, uint8_t value);

/**
 * @brief Writes a 16-bit value to a memory object at a given address.
 * The value is written in either big-endian (BE) or little-endian (LE) format,
 * depending on the `be` flag. A NULL address writes the mem[0] byte. Modificates
 * the memory at a specified address.
 *
 * @param mem The memory object to write to.
 * @param address The address to write to.
 * @param value The value to write.
 * @param be Flag indicating whether to write in big-endian format.
 * @return 0 if successful, -1 if the memory object is NULL or the address is out of bounds.
 */
int memory_write_half(memory mem, uint32_t address, uint16_t value, uint8_t be);

/**
 * @brief Writes a 32-bit value to a memory object at a given address.
 * The value is written in either big-endian (BE) or little-endian (LE) format,
 * depending on the `be` flag. A NULL address writes the mem[0] byte. Modificates
 * the memory at a specified address.
 *
 * @param mem The memory object to write to.
 * @param address The address to write to.
 * @param value The value to write.
 * @param be Flag indicating whether to write in big-endian format.
 * @return 0 if successful, -1 if the memory object is NULL or the address is out of bounds.
 */
int memory_write_word(memory mem, uint32_t address, uint32_t value, uint8_t be);

#endif