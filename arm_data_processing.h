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

/**
 * @brief Get the cpsr pointer to the cpsr whithout reading it twice in the same instruction execution
 * 
 * @param p thet machine state that contain registers 
 * @param cpsr_ptr a pointer used to check if cpsr_ptr has been already readed. 
 * @return uint32_t* pointer to cpsr value
 */
uint32_t *get_cpsr(arm_core p, uint32_t *cpsr_ptr);


/**
 * @brief A function that apply the operation using Rn and shifter_operand and store the result (if necesserary) in Rd.
 * Update CPSR zncv flags if S using cpsr_ptr and shifter_carry_out
 * 
 * @param p arm_core, all the simulator state 
 * @param ins the instruction
 * @param shifter_operand used as 2nd operand for the data processing operations 
 * @param shifter_carry_out can be used for zncv flags
 * @param S 
 * @param cpsr_ptr a pointer that allow access to cpsr whithout reading it twice (using get_cpsr(cpsr_ptr))
 * @return int the error code of the instruction readed (0 if no error)
 */
int apply_operator(arm_core p, uint32_t ins, uint32_t shifter_operand, uint8_t shifter_carry_out, uint8_t S, uint32_t *cpsr_ptr);

/**
 * @brief Get the shifter operand for immediate shift instruction format 
 * 
 * @param p arm_core, all the simulator state 
 * @param shift_code the shift code that said which operation we should apply (LSL, LSR....)
 * @param Rm_value the value of the Rm registry 
 * @param Rs_value the value of the Rs registry 
 * @param S the value of the S bit. If 1, we calculate the shift_carry_out. 
 * @param shifter_operand A pointer to modify the shifting operand value 
 * @param shift_carry_out A pointer to modify the shifting carry out value 
 * @param cpsr_ptr A pointer 
 * side effect : modify the value of shifter_operand, shift_carry_out and cpsr_ptr
 */
void get_shifter_operand_immediate_shift(arm_core p, uint8_t shift_imm, uint8_t shift_code, uint32_t Rm_value, uint8_t S, uint32_t *shifter_operand, uint8_t *shift_carry_out, uint32_t *cpsr_ptr);

/**
 * @brief Get the shifter operand for register shift instruction format 
 * 
 * @param p arm_core, all the simulator state 
 * @param shift_code the shift code that said which operation we should apply (LSL, LSR....)
 * @param Rm_value the value of the Rm registry 
 * @param Rs_value the value of the Rs registry 
 * @param S the value of the S bit. If 1, we calculate the shift_carry_out. 
 * @param shifter_operand A pointer to modify the shifting operand value 
 * @param shift_carry_out A pointer to modify the shifting carry out value 
 * @param cpsr_ptr A pointer 
 * side effect : modify the value of shifter_operand, shift_carry_out and cpsr_ptr
 */
void get_shifter_operand_register_shift(arm_core p, uint8_t shift_code, uint32_t Rm_value, uint32_t Rs_value, uint8_t S, uint32_t *shifter_operand, uint8_t *shift_carry_out, uint32_t *cpsr_ptr);


/**
 * @brief A handler for data processing instruction using immediate with shift 
 * 
 * @param p arm_core, all the simulator state 
 * @param ins the instruction readed
 * @return int, the error code of the instruction reading (0 if no error)
 */
int arm_data_processing_immediate_shift(arm_core p, uint32_t ins);

/**
 * @brief A handler for data processing instruction using registers with shift 
 * 
 * @param p arm_core, all the simulator state 
 * @param ins the instruction readed
 * @return int, the error code of the instruction readed (0 if no error)
 */
int arm_data_processing_register_shift(arm_core p, uint32_t ins);

/**
 * @brief A handler for data processing instruction using immediate 
 * 
 * @param p arm_core, all the simulator state 
 * @param ins the instruction readed
 * @return int the error code of the instruction readed (0 if no error)
 */
int arm_data_processing_immediate(arm_core p, uint32_t ins);

#endif