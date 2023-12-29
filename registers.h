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
#ifndef __REGISTERS_H__
#define __REGISTERS_H__
#include <stdint.h>
#include <stdio.h>

#define N_REGISTRES 18
#define N_MODES 7
// Mode 0 = USER; 1 = SYSTEM; 2 = SUPERVISOR;
// Mode 3 = ABORT; 4 = UNDEFINED; 5 = INTERRUPT; 6 = FAST INTERRUPT
 
#define CPSR 16
#define SPSR 17

/**
 * Data structure that represent a register 
 * ptrs gives access to data depending on registers mode
 */
typedef struct
{
    uint32_t *data;
    uint32_t **ptrs;
} register_data;

/**
 * Data structure that represent a list of registers 
 */
struct registers_data
{
    register_data *reg;
};

typedef struct registers_data *registers;

/**
 * @brief A constructor function that init a registers list
 * 
 * @return registers, the list of 37 registers 
 */
registers registers_create();

/**
 * @brief Free the registers list
 * 
 * @param r the registers list
 */
void registers_destroy(registers r);

/**
 * @brief Get the int corresponding to the pointer index to read ro write a register
 * @param mode the current registers mode (using arm_constant.h)
 * 
 * @return uint8_t 
 */
uint8_t get_read_write_mode(uint8_t mode);

/**
 * @brief Get the int corresponding to the current mode of registers 
 * 
 * @param r the register list
 * @return uint8_t a number between 0 and 6
 */
uint8_t registers_get_mode(registers r);

/**
 * @brief Is true if the current mode has a spsr
 * @param r the register list 
 * @return int a boolean
 */
int registers_current_mode_has_spsr(registers r);

/**
 * @brief Is true if the current mode is privileged (mode > 0)
 * @param r the register list 
 * @return int a boolean
*/
int registers_in_a_privileged_mode(registers r);

/**
 * @brief Get the value of the register reg in the mode mode
 * @param r  the register list
 * @param reg  the number of the register
 * @param mode  the mode of the register
 * @return uint32_t  the value of the register in the sepcified mode
 */
uint32_t registers_read(registers r, uint8_t reg, uint8_t mode);

/**
* @brief Get the value of the cpsr register
* @param r  the register list
* @return uint32_t  the value of the cpsr register
*/
uint32_t registers_read_cpsr(registers r);

/**
 * @brief Get the value of the spsr register in the mode mode
 * @param r  the register list
 * @param mode  the mode of the register
 * @return uint32_t  the value of the spsr register in the sepcified mode
 */
uint32_t registers_read_spsr(registers r, uint8_t mode);

/**
 * @brief Write the value of the register reg in the mode mode
 * @param r  the register list
 * @param reg  the number of the register
 * @param mode  the mode of the register
 * @param value  the value to write in the register
 * @return void
 * @sideeffect  the register value in the sepcified mode is modified
 */
void registers_write(registers r, uint8_t reg, uint8_t mode, uint32_t value);

/**
 * @brief Write the value of the cpsr register
 * @param r  the register list
 * @param value  the value to write in the register
 * @return void
 * @sideeffect  the CPSR register value is modified
 */
void registers_write_cpsr(registers r, uint32_t value);

/**
 * @brief Write the value of the spsr register in the mode mode
 * @param r  the register list
 * @param mode  the mode of the register
 * @param value  the value to write in the register
 * @return void
 * @sideeffect  the SPSR register value in the sepcified mode is modified
 */
void registers_write_spsr(registers r, uint8_t mode, uint32_t value);

#endif
