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
#ifndef __ARM_CONSTANTS_H__
#define __ARM_CONSTANTS_H__
#include <stdint.h>

/* ARM Modes */
#define USR 0x10
#define FIQ 0x11
#define IRQ 0x12
#define SVC 0x13
#define ABT 0x17
#define UND 0x1b
#define SYS 0x1f

/* ARM Exceptions (by priority) */
#define RESET 1
#define DATA_ABORT 2
#define FAST_INTERRUPT 3
#define INTERRUPT 4
#define IMPRECISE_ABORT 5 // Unsupported, ARMV6
#define PREFETCH_ABORT 6
#define UNDEFINED_INSTRUCTION 7
#define SOFTWARE_INTERRUPT 8
/* The last one is not realy an exception, but, as we handle software interrupts
 * within the simulator and we decide there to end the simulation, this is a way
 * to tell it to the outside world
 */
#define END_SIMULATION 9

/* Some CPSR bits */
#define N 31
#define Z 30
#define C 29
#define V 28

/* shift operations */
#define LSL 0
#define LSR 1
#define ASR 2
#define ROR 3

/* ARM conditions constants */
#define EQ 0x0
#define NE 0x1
#define CS 0x2
#define CC 0x3
#define MI 0x4
#define PL 0x5
#define VS 0x6
#define VC 0x7
#define HI 0x8
#define LS 0x9
#define GE 0xA
#define LT 0xB
#define GT 0xC
#define LE 0xD
#define AL 0xE

/* Bit mask constants for msr */
/* We simulate architecture v5T */
#define UnallocMask 0x0FFFFF00
#define UserMask 0xF0000000
#define PrivMask 0x0000000F
#define StateMask 0x00000020

/* Fetch masks constants */
#define CONDITION_MASK 0xF0000000
#define INSTRUCTION_MASK 0x0E000000
#define OPCODE_MASK 0x01E00000
#define B24_MASK 0x01000000 
#define P_MASK 0x01000000
#define U_MASK 0x00800000
#define B_MASK 0x00400000
#define W_MASK 0x00200000
#define L_MASK 0x00100000
#define S_MASK 0x00001000 
#define B4_MASK 0x00000010

#define B4_SHIFT 4
#define S_SHIFT 20
#define L_SHIFT 20
#define W_SHIFT 21
#define B_SHIFT 22
#define U_MASK 23
#define P_SHIFT 24
#define B24_SHIFT 24
#define OPCODE_SHIFT 21
#define INSTRUCTION_SHIFT 25
#define CONDITION_SHIFT 28

char *arm_get_exception_name(unsigned char exception);
char *arm_get_mode_name(uint8_t mode);
int8_t arm_get_mode_number(char *name);
char *arm_get_register_name(uint8_t reg);

#endif
