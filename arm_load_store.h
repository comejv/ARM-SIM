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
#ifndef __ARM_LOAD_STORE_H__
#define __ARM_LOAD_STORE_H__
#include <stdint.h>
#include "arm_core.h"


/**
 * @brief      Load/Store word or unsigned byte
 * @details    This function implements the load/store word or unsigned byte instruction
 * @param      p     The processor
 * @param      ins   The instruction
 * 
 * @return     0 if the instruction is executed correctly or ARM exception
*/
int arm_load_store_miscellaneous(arm_core p, uint32_t ins);

/**
 * @brief      Load/Store halfword
 * @details    This function implements the load/store halfword instruction
 * @param      p     The processor
 * @param      ins   The instruction
 * 
 * @return     0 if the instruction is executed correctly or ARM exception
*/
int arm_load_store_immediate_offset(arm_core p, uint32_t ins);

/**
 * @brief      Load/Store register offset
 * @details    This function implements the load/store register offset instruction
 * @param      p     The processor
 * @param      ins   The instruction
 * 
 * @return     0 if the instruction is executed correctly or ARM exception
*/
int arm_load_store_register_offset(arm_core p, uint32_t ins);

/**
 * @brief      Load/Store multiple
 * @details    This function implements the load/store multiple instruction
 * @param      p     The processor
 * @param      ins   The instruction
 * 
 * @return     0 if the instruction is executed correctly or ARM exception
*/
int arm_load_store_multiple(arm_core p, uint32_t ins);

/**
 * @brief      Load/Store coprocessor
 * @details    This function implements the load/store coprocessor instruction
 * @param      p     The processor
 * @param      ins   The instruction
 * 
 * @return     0 if the instruction is executed correctly or ARM exception
*/
int arm_coprocessor_load_store(arm_core p, uint32_t ins);

#endif
