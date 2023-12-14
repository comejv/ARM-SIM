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
#ifndef __TRACE_H__
#define __TRACE_H__
#include <stdio.h>
#include <stdint.h>
#include "arm_core.h"

#define CPSR 16
#define SPSR 17

#define OTHER_ACCESS 0
#define OPCODE_FETCH 1

#define WRITE 0
#define READ  1

#define MEMORY    32
#define REGISTERS 64
#define STATE     128
#define POSITION  256

void set_trace_file(FILE * f);
void trace_start_location(char *file, int line);
uint8_t trace_end_location();
void trace_memory(uint32_t cycle, uint8_t type, uint8_t size,
									uint8_t cause, uint32_t address, uint32_t value);
void trace_register(uint32_t cycle, uint8_t type, uint8_t reg, uint8_t mode, uint32_t value);
void trace_arm_state(registers r);
void trace_disable();
void trace_enable();
void trace_add(int flags);

#endif
