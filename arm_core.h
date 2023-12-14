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
#ifndef __ARM_CORE_H__
#define __ARM_CORE_H__
#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>
#include "registers.h"
#include "memory.h"

typedef struct arm_core_data *arm_core;

void arm_init();
arm_core arm_create(registers reg, memory mem);
void arm_destroy(arm_core p);

int arm_current_mode_has_spsr(arm_core p);
int arm_in_a_privileged_mode(arm_core p);
uint32_t arm_get_cycle_count(arm_core p);

uint32_t arm_read_register(arm_core p, uint8_t reg);
uint32_t arm_read_usr_register(arm_core p, uint8_t reg);
uint32_t arm_read_cpsr(arm_core p);
uint32_t arm_read_spsr(arm_core p);
void arm_write_register(arm_core p, uint8_t reg, uint32_t value);
void arm_write_usr_register(arm_core p, uint8_t reg, uint32_t value);
void arm_write_cpsr(arm_core p, uint32_t value);
void arm_write_spsr(arm_core p, uint32_t value);

int arm_fetch(arm_core p, uint32_t * value);
int arm_read_byte(arm_core p, uint32_t address, uint8_t * value);
int arm_read_half(arm_core p, uint32_t address, uint16_t * value);
int arm_read_word(arm_core p, uint32_t address, uint32_t * value);
int arm_write_byte(arm_core p, uint32_t address, uint8_t value);
int arm_write_half(arm_core p, uint32_t address, uint16_t value);
int arm_write_word(arm_core p, uint32_t address, uint32_t value);

#include "trace_location.h"
#endif
