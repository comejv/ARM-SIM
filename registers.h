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
#define SCPSR 17

typedef struct
{
    uint32_t **data;
} register_data;

// R0 - R14 = reg[0] - reg[14];
// PC = reg[15];
// CPSR = reg[16];
// SPSR = reg[17];
struct registers_data
{
    uint8_t mode;
    register_data *reg;
};

typedef struct registers_data *registers;

registers registers_create();
void registers_destroy(registers r);

uint8_t registers_get_mode(registers r);
int registers_current_mode_has_spsr(registers r);
int registers_in_a_privileged_mode(registers r);

uint32_t registers_read(registers r, uint8_t reg, uint8_t mode);
uint32_t registers_read_cpsr(registers r);
uint32_t registers_read_spsr(registers r, uint8_t mode);
void registers_write(registers r, uint8_t reg, uint8_t mode, uint32_t value);
void registers_write_cpsr(registers r, uint32_t value);
void registers_write_spsr(registers r, uint8_t mode, uint32_t value);

#endif
