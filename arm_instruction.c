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
#include "arm_instruction.h"
#include "arm_exception.h"
#include "arm_data_processing.h"
#include "arm_load_store.h"
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "debug.h"
#include "util.h"

static int arm_fetch_code_inst(arm_core p, uint32_t inst, uint32_t cpsr)
{
    int err = UNDEFINED_INSTRUCTION;
    uint8_t instcode = get_bits(inst, 27, 25);
    uint8_t b4 = get_bit(inst, 4);
    uint8_t b7 = get_bit(inst, 7);
    uint8_t opcode2b = get_bits(inst, 24, 23);
    uint8_t S = get_bit(inst, 20);
    uint8_t b5 = get_bit(inst, 5);
    uint8_t b6 = get_bit(inst, 6);
    uint8_t b21 = get_bit(inst, 21);
    uint8_t b24_20 = get_bits(inst, 24, 20);
    uint8_t b7_4 = get_bits(inst, 7, 4);
    uint8_t b24 = get_bit(inst, 24);

    debug("inst: %x\n", inst);
    switch (instcode)
    {
    case 0x0:
        if (b4 == 0)
        {
            if (opcode2b == 2 && S == 0)
            {
                // Misceallaneous instruction (1)
                err = arm_miscellaneous(p, inst);
            }
            else
            {
                // Data processing immediate shift
                err = arm_data_processing_immediate_shift(p, inst, cpsr);
            }
        }
        else
        {
            if (b7 == 0)
            {
                if (opcode2b == 2 && S == 0)
                {
                    err = arm_miscellaneous_2(p, inst, cpsr);
                }
                else
                {
                    // Data processing register shift
                    err = arm_data_processing_register_shift(p, inst, cpsr);
                }
            }
            else
            {
                // Multiplies; Extra load/stores
                if (b5 == 0 && b6 == 0)
                {
                    err = arm_multiply(p, inst, cpsr);
                }
                else
                {
                    // Extra load/stores
                    err = arm_load_store_miscellaneous(p, inst);
                }
            }
        }
        break;
    case 0x1:
        if (opcode2b == 2 && S == 0)
        {
            if (b21 == 0)
            {
                err = UNDEFINED_INSTRUCTION;
            }
            else
            {
                // Move immediate to status register
            }
        }
        else
        {
            // Data processing immediate
            debug("Data processing immediate, ins: %x\n", inst);
            err = arm_data_processing_immediate(p, inst, cpsr);
        }
        break;
    case 0x2:
        // Load/Store immediate offset
        err = arm_load_store_immediate_offset(p, inst);
        break;
    case 0x3:
        if (b4 == 0)
        {
            // Load/Store register offset
            err = arm_load_store_register_offset(p, inst);
        }
        else
        {
            if (b24_20 == 0x1F && b7_4 == 0xF)
            {
                // Architecturraly undefined
                return UNDEFINED_INSTRUCTION;
            }
            else
            {
                // Media instructions
            }
        }
        break;
    case 0x4:
        // Load/Store multiple
        err = arm_load_store_multiple(p, inst);
        break;
    case 0x5:
        // Branch and branch with link
        err = arm_branch(p, inst);
        break;
    case 0x6:
        // Coprocessor load/store and register transfers
        err = arm_coprocessor_load_store(p, inst);
        break;
    case 0x7:
        if (b24 == 0)
        {
            if (b4 == 0)
            {
                // Coprocessor data processing
            }
            else
            {
                // Coprocessor register transfers
            }
        }
        else
        {
            // Software interrupt
            err = arm_coprocessor_others_swi(p, inst);
        }
        break;
    default:
        // Should not happen
        return UNDEFINED_INSTRUCTION;
    }
    return err;
}

static int arm_execute_instruction(arm_core p)
{
    uint32_t inst;
    int result = arm_fetch(p, &inst);
    if (result == 0)
    {
        uint8_t cond_inst = get_bits(inst, 31, 28);
        uint32_t cpsr = arm_read_cpsr(p);
        int flag_cond = 0;
        uint8_t cpsr_Z = get_bit(cpsr, Z);
        uint8_t cpsr_N = get_bit(cpsr, N);
        uint8_t cpsr_C = get_bit(cpsr, C);
        uint8_t cpsr_V = get_bit(cpsr, V);
        debug("ZNCV : %d %d %d %d\n", cpsr_Z, cpsr_N, cpsr_C, cpsr_V);
        if (cond_inst == EQ && cpsr_Z == 1)
            flag_cond = 1;
        else if (cond_inst == NE && cpsr_Z == 0)
            flag_cond = 1;
        else if (cond_inst == CS && cpsr_C == 1)
            flag_cond = 1;
        else if (cond_inst == CC && cpsr_C == 0)
            flag_cond = 1;
        else if (cond_inst == MI && cpsr_N == 1)
            flag_cond = 1;
        else if (cond_inst == PL && cpsr_N == 0)
            flag_cond = 1;
        else if (cond_inst == VS && cpsr_V == 1)
            flag_cond = 1;
        else if (cond_inst == VC && cpsr_V == 0)
            flag_cond = 1;
        else if (cond_inst == HI && cpsr_C == 1 && cpsr_Z == 0)
            flag_cond = 1;
        else if (cond_inst == LS && cpsr_C == 1 && cpsr_Z == 1)
            flag_cond = 1;
        else if (cond_inst == GE && cpsr_N == cpsr_V)
            flag_cond = 1;
        else if (cond_inst == LT && cpsr_N != cpsr_V)
            flag_cond = 1;
        else if (cond_inst == GT && cpsr_Z == 0 && cpsr_N == cpsr_V)
            flag_cond = 1;
        else if (cond_inst == LE && (cpsr_Z == 1 || cpsr_N != cpsr_V))
            flag_cond = 1;
        else if (cond_inst == AL)
            flag_cond = 1;
        if (flag_cond)
        {
            return arm_fetch_code_inst(p, inst, cpsr);
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

int arm_step(arm_core p)
{
    int result;

    result = arm_execute_instruction(p);
    if (result)
    {
        return arm_exception(p, result);
    }
    return result;
}
