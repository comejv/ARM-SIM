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
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"

#define LDR_STR_IMMEDIATE 0x2
#define LDR_STR_REGISTER 0x3
#define LDR_STR_MULTIPLE 0x4

// #define LDR_INST

// Shift
#define RD_SHIFT 12
#define RN_SHIFT 16
#define L_SHIFT 20
#define W_SHIFT 21
#define B_SHIFT 22
#define U_SHIFT 23
#define P_SHIFT 24
// #define LDR_STR

// Mask
#define RN_RD_MASK 0xF
#define L_MASK 1 << L_SHIFT
#define W_MASK 1 << W_SHIFT
#define B_MASK 1 << B_SHIFT
#define U_MASK 1 << U_SHIFT
#define P_MASK 1 << P_SHIFT 

#define IS_LDR(inst) return (L_MASK & inst) ? 1 : 0
#define IS_STR(inst) return (!IS_LDR(inst))
#define IS_BYTE(isnt) return (B_MASK & inst) ? 1 : 0
#define IS_WORD(inst) return (!IS_BYTE(inst))

int arm_load_word(arm_core p, uint32_t ins) {
    // Bit L == 1 -> IS_LOAD(inst)
    // Bit B == 0 -> IS_WORD(inst)

    /* A COMPLETER
    * Detecter Bit I (Immediat)
    * Fonction a page: 194 - Partie A4.1.23
    * Verifier Rd == PC
    *
    */
}

int arm_load_store(arm_core p, uint32_t ins)
{
    return UNDEFINED_INSTRUCTION;
}

int arm_load_store_multiple(arm_core p, uint32_t ins)
{
    return UNDEFINED_INSTRUCTION;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins)
{
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}

int arm_handle_load_store(arm_core p, uint32_t ins)
{
    int ret;

    uint8_t ins_type = ins & INSTRUCTION_MASK >> INSTRUCTION_SHIFT;

    switch (ins_type)
    {
    case LDR_STR_IMMEDIATE:
    case LDR_STR_REGISTER:
        ret = arm_load_store(p, ins);
        break;
    case LDR_STR_MULTIPLE:
        ret = arm_load_store_multiple(p, ins);
        break;
    default:
        ret = UNDEFINED_INSTRUCTION;
        break;
    }

    return ret;
}
