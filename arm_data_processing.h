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
#ifndef __ARM_DATA_PROCESSING_H__
#define __ARM_DATA_PROCESSING_H__
#include <stdint.h>
#include "arm_core.h"

/* ARM data processing instruction sections */
#define RM_0 0
#define RM_1 1
#define RM_2 2
#define RM_3 3

#define IMM_0 0
#define IMM_1 1
#define IMM_2 2
#define IMM_3 3
#define IMM_4 4
#define IMM_5 5
#define IMM_6 6
#define IMM_7 7

#define RS_0 8
#define RS_1 9
#define RS_2 10
#define RS_3 11

#define ROTATE_0 8
#define ROTATE_1 9
#define ROTATE_2 10
#define ROTATE_3 11

#define RD_0 12
#define RD_1 13
#define RD_2 14
#define RD_3 15

#define RN_0 16
#define RN_1 17
#define RN_2 18
#define RN_3 19

#define S_BIT 20

#define OPCODE_0 21
#define OPCODE_1 22
#define OPCODE_2 23
#define OPCODE_3 24

/* ARM data processing operators */
#define AND 0b0000
#define EOR 0b0001
#define SUB 0b0010
#define RSB 0b0011
#define ADD 0b0100
#define ADC 0b0101
#define SBC 0b0110
#define RSC 0b0111
#define TST 0b1000
#define TEQ 0b1001
#define CMP 0b1010
#define CMN 0b1011
#define ORR 0b1100
#define MOV 0b1101
#define BIC 0b1110
#define MVN 0b1111

int arm_data_processing_immediate_shift(arm_core p, uint32_t ins);
int arm_data_processing_register_shift(arm_core p, uint32_t ins);
int arm_data_processing_immediate(arm_core p, uint32_t ins);

#endif