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
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"
#include <debug.h>
#include <stdlib.h>
#define bL 24

int arm_branch(arm_core p, uint32_t ins)
{
    uint32_t PCvalue = arm_read_register(p, PC);
    if (get_bit(ins, bL))
    {
        arm_write_register(p, LR, PCvalue - 4); // PC value already updated by fetch
    }
    uint32_t bits_23_0 = get_bits(ins, 23, 0);
    uint32_t extended;
    if (get_bit(bits_23_0, 23))
    {
        extended = bits_23_0 | 0xFF000000;
    }
    else
    {
        extended = bits_23_0;
    }
    extended = extended << 2;
    arm_write_register(p, PC, PCvalue + extended);
    return 0;
}

int arm_coprocessor_others_swi(arm_core p, uint32_t ins)
{
    if (get_bit(ins, 24))
    {
        return SOFTWARE_INTERRUPT;
    }
    return UNDEFINED_INSTRUCTION;
}

int arm_miscellaneous(arm_core p, uint32_t ins)
{
    printf("BX LR atteri donc ici ?\n");
    uint32_t Rd = get_bits(ins, 15, 12);
    if (get_bit(ins, 22))
    {
        if (!arm_current_mode_has_spsr(p))
        {
            return DATA_ABORT;
        }
        uint32_t SPSR_value = arm_read_spsr(p);
        arm_write_register(p, Rd, SPSR_value);
    }
    else
    {
        uint32_t CPSR_value = arm_read_cpsr(p);
        arm_write_register(p, Rd, CPSR_value);
    }
    return 0;
}

int arm_miscellaneous_2(arm_core p, uint32_t ins, uint32_t cpsr)
{ // BX
    uint8_t Rm = get_bits(ins, 3, 0);
    uint32_t Rm_v = arm_read_register(p, Rm);
    uint32_t PC_v = Rm_v & 0xFFFFFFFE;
    arm_write_register(p, PC, PC_v);
    return 0;
}

int arm_multiply(arm_core p, uint32_t ins, uint32_t cpsr)
{
    uint8_t Rd = get_bits(ins, 19, 16);
    uint8_t Rs = get_bits(ins, 11, 8);
    uint8_t Rm = get_bits(ins, 3, 0);
    uint8_t S = get_bit(ins, 4);

    uint32_t Rs_v = arm_read_register(p, Rs);
    uint32_t Rm_v = arm_read_register(p, Rm);
    uint32_t Rd_v = Rs_v * Rm_v;
    arm_write_register(p, Rd, Rd_v);
    if (S == 1)
    {
        if (get_bit(Rd_v, 31) == 0)
        {
            cpsr = clr_bit(cpsr, C);
        }
        else
        {
            cpsr = set_bit(cpsr, C);
        }
        if (Rd_v == 0)
        {
            cpsr = set_bit(cpsr, Z);
        }
        else
        {
            cpsr = clr_bit(cpsr, Z);
        }
        arm_write_cpsr(p, cpsr);
    }
    return 0;
}