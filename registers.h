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
#ifndef __REGISTERS_H__
#define __REGISTERS_H__
#include <stdint.h>
#include <stdio.h>

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
