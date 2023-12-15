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
#include "util.h"

static int arm_fetch_code_inst(arm_core p, uint32_t inst)
{
    uint8_t instcode = (inst & INSTRUCTION_MASK) >> INSTRUCTION_SHIFT;
    uint8_t b24 = (inst & (B24_MASK) >> (B24_SHIFT));
    uint8_t opcode = (inst & OPCODE_MASK) >> OPCODE_SHIFT;
    uint8_t S = (inst & S_MASK) >> S_SHIFT;
    uint8_t b4 = (inst & B4_MASK) >> B4_SHIFT;
    uint8_t b20 = (inst & L_MASK) >> L_SHIFT;

    if (instcode == 0 && (opcode >> 2) == 2 && S == 0)
    {
        // Miscellaneous
    }
    else if (instcode == 8)
    {
        if (b24 == 1)
        {
            // Software interrupt
        }
        else
        {
            // Coprocessor
        }
    }
    else if (instcode == 5)
    {
        // Branch and branch with link
    }
    else if (instcode == 4)
    {
        // Load/Store multiple
    }
    else if (instcode == 3)
    {
        if (b4 == 0)
        {
            // Load/store register offset
        }
        else
        {
            // Media instructions / Undefined architecture
        }
    }
    else if (instcode == 2)
    {
        // Load/store immediate offset
    }
    else if (instcode == 1)
    {
        
    }
}

static int arm_execute_instruction(arm_core p)
{
    uint32_t inst;
    int result = arm_fetch(p, &inst);
    if (result == 0)
    {
        uint8_t cond_inst = (inst & CONDITION_MASK) >> CONDITION_SHIFT;
        uint32_t cpsr = arm_read_cpsr(p);
        int FLAG_COND = 0;
        uint8_t cpsr_Z = (cpsr >> Z) & 0x1;
        uint8_t cpsr_N = (cpsr >> N) & 0x1;
        uint8_t cpsr_C = (cpsr >> C) & 0x1;
        uint8_t cpsr_V = (cpsr >> V) & 0x1;
        if (cond_inst == EQ && Z == 1)
            FLAG_COND = 1;
        else if (cond_inst == NE && Z == 0)
            FLAG_COND = 1;
        else if (cond_inst == CS && C == 1)
            FLAG_COND = 1;
        else if (cond_inst == CC && C == 0)
            FLAG_COND = 1;
        else if (cond_inst == MI && N == 1)
            FLAG_COND = 1;
        else if (cond_inst == PL && N == 0)
            FLAG_COND = 1;
        else if (cond_inst == VS && V == 1)
            FLAG_COND = 1;
        else if (cond_inst == VC && V == 0)
            FLAG_COND = 1;
        else if (cond_inst == HI && C == 1 && Z == 0)
            FLAG_COND = 1;
        else if (cond_inst == LS && C == 1 && Z == 1)
            FLAG_COND = 1;
        else if (cond_inst == GE && N == V)
            FLAG_COND = 1;
        else if (cond_inst == LT && N != V)
            FLAG_COND = 1;
        else if (cond_inst == GT && Z == 0 && N == V)
            FLAG_COND = 1;
        else if (cond_inst == LE && (Z == 1 || N != V))
            FLAG_COND = 1;
        else if (cond_inst == AL)
            FLAG_COND = 1;

        if (FLAG_COND)
        {
            return arm_fetch_code_inst(p, inst);
        }
        else
        {
            return 1;
        }
    }
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
