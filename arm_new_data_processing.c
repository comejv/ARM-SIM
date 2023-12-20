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
#include "arm_data_processing.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"

void set_zn(arm_core p, uint32_t rd_value)
{
    uint32_t cpsr = arm_read_cpsr(p);
    // set the N bit
    if (get_bit(rd_value, N))
    {
        cpsr = set_bit(cpsr, N);
    }
    // set the Z bit
    if (rd_value == 0)
    {
        cpsr = set_bit(cpsr, Z);
    }
    arm_write_cpsr(p, cpsr);
    return;
}

uint8_t carryFrom(uint32_t rn_val, uint32_t shifter_op_val, uint8_t carry)
{
    uint64_t true_result = rn_val + shifter_op_val + carry;
    if (true_result >= 0x100000000)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t overflowFrom(uint32_t rn_val, uint32_t shifter_op_val, uint8_t carry, uint32_t opcode)
{
    uint8_t rn_sign = get_bit(rn_val, 31);
    uint8_t shifter_op_sign = get_bit(shifter_op_val, 31);
    uint8_t result_sign;

    switch (opcode)
    {
    case ADD:
    case ADC:
        result_sign = get_bit(rn_val + shifter_op_val + carry, 31);
        if ((rn_sign == shifter_op_sign) && (result_sign != rn_sign))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    case SUB:
        result_sign = get_bit(rn_val - shifter_op_val, 31);
        if ((rn_sign != shifter_op_sign) && (result_sign != rn_sign))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    case SBC:
        result_sign = get_bit(rn_val - shifter_op_val - ~carry, 31);
        if ((rn_sign != ~shifter_op_sign) && (result_sign))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    case RSB:
        result_sign = get_bit(shifter_op_val - rn_val, 31);
        if ((rn_sign != ~shifter_op_sign) && (result_sign))
        {
            return 1;
        }
        else
        {
            return 0;
        }
        break;
    case RSC:
        result_sign = get_bit(shifter_op_val - rn_val - ~carry, 31);
        if ((rn_sign != ~shifter_op_sign) && (result_sign))
        {
            return 1;
        }
        else
        {
            return 0;
        }
        break;
    default:
        return 3;
    }
}

uint8_t borrowFrom(uint32_t op1, uint32_t op2, uint8_t carry)
{
    if (op1 - op2 - ~carry < 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int apply_operator(arm_core p, uint8_t opcode, uint8_t Rd, uint32_t Rn_value, uint8_t S, uint32_t shifter_operand, uint32_t shifter_carry_out)
{
    uint8_t update_Rd = 1;
    uint32_t Rd_value;
    switch (opcode)
    {
    case AND:
        Rd_value = Rn_value & shifter_operand;
        break;
    case EOR:
        Rd_value = Rn_value ^ shifter_carry_out;
        break;
    case SUB:
        Rd_value = Rn_value - shifter_operand;
        break;
    case RSB:
        Rd_value = shifter_operand - Rn_value;
        break;
    case ADD:
        Rd_value = Rn_value + shifter_operand;
    case ADC:
        Rd_value = Rn_value + shifter_operand + get_bit(arm_read_cpsr(p), C);
        break;
    case SBC:
        Rd_value = Rn_value - shifter_operand - (~get_bit(arm_read_cpsr(p), C));
        break;
    case RSC:
        Rd_value = shifter_operand - Rn_value - (~get_bit(arm_read_cpsr(p), C));
        break;
    case TST:
        Rd_value =  Rn_value & shifter_operand;
        update_Rd = 0; // Do not erase Rd content
        S = 1; // Force flags update 
        break;
    case TEQ:
        Rd_value =  Rn_value ^ shifter_operand;
        update_Rd = 0; // Do not erase Rd content
        S = 1; // Force flags update         break;   
    case CMP:
        Rd_value = Rn_value - shifter_operand;
        update_Rd = 0; // Do not erase Rd content
        S = 1; // Force flags update 
        break;
    case CMN:
        Rd_value = Rn_value + shifter_operand;
        update_Rd = 0; // Do not erase Rd content
        S = 1; // Force flags update 
        break;
    case ORR:
        Rd_value = Rn_value | shifter_operand;
        break;
    case MOV:
        Rd_value = shifter_operand;
        break;
    case BIC:
        Rd_value = Rn_value & ~shifter_operand;
        break;
    case MVN:
        Rd_value = ~shifter_operand;
        break;
    default:
        return UNDEFINED_INSTRUCTION; // Should not append 
        break;
    }
    if (update_Rd)
    {
        arm_write_register(p, Rd, Rd_value);
    }
    if (S){
        /* TO DO */
    }
    return 0;
}

void get_shifter_operand_immediate_shift(arm_core p, uint8_t shift_imm, uint8_t shift_code, uint32_t Rm_value, uint32_t *shifter_operand, uint32_t *shift_carry_out)
{
    switch (shift_code)
    {
    case 0b000: // Logical shift left by immediate (LSL)
        if (shift_imm == 0)
        {
            *shifter_operand = Rm_value;
            *shift_carry_out = get_bit(arm_read_cpsr(p), C);
        }
        else
        {
            *shifter_operand = Rm_value << shift_imm;
            *shift_carry_out = get_bit(Rm_value, 32 - shift_imm);
        }
        break;
    case 0b010: // Logical shift right by immediate (LSR)
        if (shift_imm == 0)
        {
            *shifter_operand = Rm_value;
            *shift_carry_out = get_bit(arm_read_cpsr(p), 31);
        }
        else
        {
            *shifter_operand = Rm_value >> shift_imm;
            *shift_carry_out = get_bit(Rm_value, shift_imm - 1);
        }
        break;
    case 0b100: // Arithmetic shift right by immediate (ASR)
        if (shift_imm == 0)
        {
            if (get_bit(Rm_value, 31) == 0)
            {
                *shifter_operand = 0;
            }
            else
            {
                *shifter_operand = 0xFFFFFFFF;
            }
            *shift_carry_out = get_bit(Rm_value, 31);
        }
        else
        {
            *shifter_operand = asr(Rm_value, shift_imm);
            *shift_carry_out = get_bit(Rm_value, shift_imm - 1);
        }
        break;

    case 0b110: // Rotate right by immediate (ROR)
        if (shift_imm == 0)
        {
            *shifter_operand = (get_bit(arm_read_cpsr(p), C) << 31) | (Rm_value >> 1);
            *shift_carry_out = get_bit(Rm_value, 0);
        }
        else
        {
            *shifter_operand = ror(Rm_value, shift_imm);
            *shift_carry_out = get_bit(Rm_value, shift_imm - 1);
        }
    default:
        break; // Should not append
    }
}

void get_shifter_operand_register_shift(arm_core p, uint8_t shift_code, uint32_t Rm_value, uint32_t Rs_value, uint32_t *shifter_operand, uint32_t *shift_carry_out)
{
    uint8_t Rs_significant_bits = get_bits(Rs_value, 7, 0);
    switch (shift_code)
    {
    case 0b000: // Logical shift left by immediate (LSL)
        if (Rs_significant_bits == 0)
        {
            *shifter_operand = Rm_value;
            *shift_carry_out = get_bit(arm_read_cpsr(p), C);
        }
        else if (Rs_significant_bits < 32)
        {
            *shifter_operand = Rm_value << Rs_significant_bits;
            *shift_carry_out = get_bit(Rm_value, 32 - Rs_significant_bits);
        }
        else if (Rs_significant_bits == 32)
        {
            *shifter_operand = 0;
            *shift_carry_out = get_bit(Rm_value, 0);
        }
        else
        {
            *shifter_operand = 0;
            *shift_carry_out = 0;
        }
        break;
    case 0b010: // Logical shift right by immediate (LSR)
        if (Rs_significant_bits == 0)
        {
            *shifter_operand = Rm_value;
            *shift_carry_out = get_bit(arm_read_cpsr(p), C);
        }
        else if (Rs_significant_bits < 32)
        {
            *shifter_operand = Rm_value >> Rs_significant_bits;
            *shift_carry_out = get_bit(Rm_value, Rs_significant_bits - 1);
        }
        else if (Rs_significant_bits == 32)
        {
            *shifter_operand = 0;
            *shift_carry_out = get_bit(Rm_value, 31);
        }
        else
        {
            *shifter_operand = 0;
            *shift_carry_out = 0;
        }
        break;
    case 0b100: // Arithmetic shift right by immediate (ASR)
        if (Rs_significant_bits == 0)
        {
            *shifter_operand = Rm_value;
            *shift_carry_out = get_bit(arm_read_cpsr(p), C);
        }
        else if (Rs_significant_bits < 32)
        {
            *shifter_operand = asr(Rm_value, Rs_significant_bits);
            *shift_carry_out = get_bit(Rm_value, Rs_significant_bits - 1);
        }
        else if (Rs_significant_bits >= 32)
        {
            if (get_bit(Rm_value, 31) == 0)
            {
                *shifter_operand = 0;
            }
            else
            {
                *shifter_operand = 0xFFFFFFFF;
            }
            *shift_carry_out = get_bit(Rm_value, 31);
        }
        break;
    case 0b110: // Rotate right by immediate (ROR)
        if (Rs_significant_bits == 0)
        {
            *shifter_operand = Rm_value;
            *shift_carry_out = get_bit(arm_read_cpsr(p), C);
        }
        else if (get_bits(Rs_value, 4, 0) == 0)
        {
            *shifter_operand = Rm_value;
            *shift_carry_out = get_bit(Rm_value, 31);
        }
        else
        {
            *shifter_operand = ror(Rm_value, get_bits(Rs_value, 4, 0));
            *shift_carry_out = get_bit(Rm_value, get_bits(Rs_value, 4, 0) - 1);
        }
    default:
        break; // Should not append
    }
}

/* Decoding functions for different classes of instructions */
int arm_data_processing_immediate_shift(arm_core p, uint32_t ins)
{
    uint8_t shift_imm = get_bits(ins, 11, 7);
    uint8_t shift_code = get_bits(ins, 6, 4);
    uint8_t Rm = get_bits(ins, 3, 0);
    uint32_t Rm_value = arm_read_register(p, Rm);
    uint32_t shifter_carry_out;
    uint32_t shifter_operand;

    get_shifter_operand_immediate_shift(p, shift_imm, shift_code, Rm_value, &shifter_operand, &shifter_carry_out);
    return 0;
}

int arm_data_processing_register_shift(arm_core p, uint32_t ins)
{
    uint8_t shift_code = get_bits(ins, 6, 4);
    uint8_t Rm = get_bits(ins, 3, 0);
    uint32_t Rm_value = arm_read_register(p, Rm);
    uint8_t Rs = get_bits(ins, 11, 8);
    uint32_t Rs_value = arm_read_register(p, Rs);
    uint32_t shifter_operand;
    uint32_t shifter_carry_out;

    get_shifter_operand_register_shift(p, shift_code, Rm_value, Rs_value, &shifter_operand, &shifter_carry_out);
    return 0;
}

int arm_data_processing_immediate(arm_core p, uint32_t ins)
{
    uint32_t immed_8 = get_bits(ins, 7, 0);
    uint8_t rotate_imm = get_bits(ins, 11, 8);
    uint32_t shifter_operand = ror(immed_8, rotate_imm);
    uint32_t shifter_carry_out;
    if (rotate_imm == 0)
    {
        shifter_carry_out = get_bit(arm_read_cpsr(p), C);
    }
    else
    {
        shifter_carry_out = get_bit(shifter_operand, 31);
    }
}