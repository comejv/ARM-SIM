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

uint32_t set_zn(arm_core p, uint32_t rd_value, uint32_t cpsr)
{
    // set the N bit
    if (get_bit(rd_value, N))
    {
        cpsr = set_bit(cpsr, N);
    }
    else
    {
        cpsr = clr_bit(cpsr, N);
    }
    // set the Z bit
    if (rd_value == 0)
    {
        cpsr = set_bit(cpsr, Z);
    }
    else
    {
        cpsr = clr_bit(cpsr, Z);
    }

    return cpsr;
}

uint32_t carryFrom(uint64_t true_result, int32_t cpsr)
{
    if (true_result >= 0x100000000)
    {
        cpsr = set_bit(cpsr, C);
    }
    else
    {
        cpsr = clr_bit(cpsr, C);
    }
    return cpsr;
}

uint32_t overflowFrom(uint32_t rn_val, uint32_t shifter_op_val, uint8_t opcode, uint32_t cpsr)
{
    uint8_t rn_sign = get_bit(rn_val, 31);                 // Get the sign of the first operand
    uint8_t shifter_op_sign = get_bit(shifter_op_val, 31); // Get the sign of the second operand
    int64_t true_result = 0;
    uint8_t result_sign;
    switch (opcode)
    {
    case ADC:
        true_result += get_bit(cpsr, C);
    case ADD:
    case CMN:
        true_result += rn_val + shifter_op_val;
        result_sign = get_bit(rn_val + shifter_op_val, 31);
        if ((rn_sign == shifter_op_sign) && (result_sign != rn_sign))
        {
            cpsr = set_bit(cpsr, V);
        }
        else
        {
            cpsr = clr_bit(cpsr, V);
        }
        break;
    case SBC:
    case RSC:
        true_result = !get_bit(cpsr, C);
    case SUB:
    case CMP:
        true_result += rn_val - shifter_op_val;
        result_sign = get_bit(true_result, 31);
        if ((rn_sign != shifter_op_sign) && (result_sign != rn_sign))
        {
            cpsr = set_bit(cpsr, V);
        }
        else
        {
            cpsr = clr_bit(cpsr, V);
        }
        break;

    default:
        break;
    }
    return cpsr;
}

uint32_t borrowFrom(uint64_t true_result, uint32_t cpsr)
{
    if (get_bit(true_result, 31))
    {
        cpsr = set_bit(cpsr, C);
    }
    else
    {
        cpsr = clr_bit(cpsr, C);
    }
    return cpsr;
}

int apply_operator(arm_core p, uint32_t ins, uint32_t shifter_operand, uint8_t shifter_carry_out, uint8_t S, uint32_t cpsr)
{
    uint8_t opcode = get_bits(ins, 24, 21);
    uint8_t Rd = get_bits(ins, 15, 12);
    uint8_t Rn = get_bits(ins, 19, 16);
    uint32_t Rn_value = arm_read_register(p, Rn);
    uint8_t update_Rd = 1;
    uint8_t ZNCVupdatecase;
    uint64_t Rd_value; // true result, recast in 32b when writed
    switch (opcode)
    {
    case AND:
        Rd_value = Rn_value & shifter_operand;
        ZNCVupdatecase = 1;
        break;
    case EOR:
        Rd_value = Rn_value ^ shifter_operand;
        ZNCVupdatecase = 1;
        break;
    case SUB:
        Rd_value = Rn_value - shifter_operand;
        ZNCVupdatecase = 2;
        break;
    case RSB:
        Rd_value = shifter_operand - Rn_value;
        ZNCVupdatecase = 2;
        break;
    case ADD:
        Rd_value = Rn_value + shifter_operand;
        ZNCVupdatecase = 0;
        break;
    case ADC:
        Rd_value = Rn_value + shifter_operand + get_bit(cpsr, C);
        ZNCVupdatecase = 0;
        break;
    case SBC:
        Rd_value = Rn_value - shifter_operand - (!get_bit(cpsr, C));
        ZNCVupdatecase = 2;
        break;
    case RSC:
        Rd_value = shifter_operand - Rn_value - (!get_bit(cpsr, C));
        ZNCVupdatecase = 2;
        break;
    case TST:
        Rd_value = Rn_value & shifter_operand;
        update_Rd = 0; // Do not erase Rd content
        S = 1;         // Force flags update
        ZNCVupdatecase = 1;
        break;
    case TEQ:
        Rd_value = Rn_value ^ shifter_operand;
        update_Rd = 0; // Do not erase Rd content
        S = 1;         // Force flags update
        ZNCVupdatecase = 1;
        break;
    case CMP:
        Rd_value = Rn_value - shifter_operand;
        update_Rd = 0; // Do not erase Rd content
        S = 1;         // Force flags update
        ZNCVupdatecase = 2;
        break;
    case CMN:
        Rd_value = Rn_value + shifter_operand;
        update_Rd = 0; // Do not erase Rd content
        S = 1;         // Force flags update
        ZNCVupdatecase = 0;
        break;
    case ORR:
        Rd_value = Rn_value | shifter_operand;
        ZNCVupdatecase = 1;
        break;
    case MOV:
        Rd_value = shifter_operand;
        ZNCVupdatecase = 1;
        break;
    case BIC:
        Rd_value = Rn_value & ~shifter_operand;
        ZNCVupdatecase = 1;
        break;
    case MVN:
        Rd_value = ~shifter_operand;
        ZNCVupdatecase = 1;
        break;
    default:
        return UNDEFINED_INSTRUCTION; // Should not append
        break;
    }
    if (update_Rd)
    {
        if (S && Rd == 15)
        {
            if (arm_current_mode_has_spsr(p))
            {
                cpsr = arm_read_spsr(p);
            }
        }
        arm_write_register(p, Rd, Rd_value);
    }
    if (S)
    {
        cpsr = set_zn(p, Rd_value, cpsr); // Update Z and N flags
        switch (ZNCVupdatecase)
        {
        case 0:
            /* code */
            cpsr = carryFrom(Rd_value, cpsr); // Update C flag
            cpsr = overflowFrom(Rn_value, shifter_operand, opcode, cpsr); // Update V flag
            break;
        case 1:
            if (get_bit(shifter_carry_out, 0))
            {
                cpsr = set_bit(cpsr, C); // Update C flag
            }
            else
            {
                cpsr = clr_bit(cpsr, C); // Update C flag
            }
            // No update on V flag
            break;
        case 2:
            cpsr = borrowFrom(Rd_value, cpsr);                            // Update C flag
            cpsr = overflowFrom(Rn_value, shifter_operand, opcode, cpsr); // Update V flag
            break;
        default:
            break;
        }
        debug("Updating CPSR value to: %x\n", cpsr);
        arm_write_cpsr(p, cpsr);
    }
    return 0;
}

void get_shifter_operand_immediate_shift(arm_core p, uint8_t shift_imm, uint8_t shift_code, uint32_t Rm_value, uint8_t S, uint32_t *shifter_operand, uint8_t *shift_carry_out, uint32_t cpsr)
{
    switch (shift_code)
    {
    case LSL: // Logical shift left by immediate (LSL)
        if (shift_imm == 0)
        {
            *shifter_operand = Rm_value;
            *shift_carry_out = get_bit(cpsr, C);
        }
        else
        {
            *shifter_operand = Rm_value << shift_imm;
            *shift_carry_out = get_bit(Rm_value, 32 - shift_imm);
        }
        break;
    case LSR: // Logical shift right by immediate (LSR)
        if (shift_imm == 0)
        {
            *shifter_operand = Rm_value;
            *shift_carry_out = get_bit(cpsr, 31);
        }
        else
        {
            *shifter_operand = Rm_value >> shift_imm;
            *shift_carry_out = get_bit(Rm_value, shift_imm - 1);
        }
        break;
    case ASR: // Arithmetic shift right by immediate (ASR)
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

    case ROR: // Rotate right by immediate (ROR)
        if (shift_imm == 0)
        {
            *shifter_operand = (cpsr << 31) | (Rm_value >> 1);
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

void get_shifter_operand_register_shift(arm_core p, uint8_t shift_code, uint32_t Rm_value, uint32_t Rs_value, uint8_t S, uint32_t *shifter_operand, uint8_t *shift_carry_out, uint32_t cpsr)
{
    uint8_t Rs_significant_bits = get_bits(Rs_value, 7, 0);
    switch (shift_code)
    {
    case LSL: // Logical shift left by immediate (LSL)

        if (Rs_significant_bits == 0)
        {
            *shifter_operand = Rm_value;
            *shift_carry_out = get_bit(cpsr, C);
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
    case LSR: // Logical shift right by immediate (LSR)
        if (Rs_significant_bits == 0)
        {
            *shifter_operand = Rm_value;
            *shift_carry_out = get_bit(cpsr, C);
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
    case ASR: // Arithmetic shift right by immediate (ASR)
        if (Rs_significant_bits == 0)
        {
            *shifter_operand = Rm_value;
            *shift_carry_out = get_bit(cpsr, C);
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
    case ROR: // Rotate right by immediate (ROR)
        if (Rs_significant_bits == 0)
        {
            *shifter_operand = Rm_value;
            *shift_carry_out = get_bit(cpsr, C);
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
int arm_data_processing_immediate_shift(arm_core p, uint32_t ins, uint32_t cpsr)
{
    uint8_t S = get_bit(ins, 20);
    uint8_t shift_imm = get_bits(ins, 11, 7);
    uint8_t shift_code = get_bits(ins, 6, 5);
    uint8_t Rm = get_bits(ins, 3, 0);
    uint32_t Rm_value = arm_read_register(p, Rm);
    uint8_t shifter_carry_out = 0;
    uint32_t shifter_operand = 0;
    get_shifter_operand_immediate_shift(p, shift_imm, shift_code, Rm_value, S, &shifter_operand, &shifter_carry_out, cpsr);
    return apply_operator(p, ins, shifter_operand, shifter_carry_out, S, cpsr);
}

int arm_data_processing_register_shift(arm_core p, uint32_t ins, uint32_t cpsr)
{
    uint8_t S = get_bit(ins, 20);
    uint8_t shift_code = get_bits(ins, 6, 5);
    uint8_t Rm = get_bits(ins, 3, 0);
    uint32_t Rm_value = arm_read_register(p, Rm);
    uint8_t Rs = get_bits(ins, 11, 8);
    uint32_t Rs_value = arm_read_register(p, Rs);
    uint32_t shifter_operand = 0;
    uint8_t shifter_carry_out = 0;

    get_shifter_operand_register_shift(p, shift_code, Rm_value, Rs_value, S, &shifter_operand, &shifter_carry_out, cpsr);
    return apply_operator(p, ins, shifter_operand, shifter_carry_out, S, cpsr);
}

int arm_data_processing_immediate(arm_core p, uint32_t ins, uint32_t cpsr)
{
    uint8_t S = get_bit(ins, 20);
    uint32_t immed_8 = get_bits(ins, 7, 0);
    uint8_t rotate_imm = get_bits(ins, 11, 8);
    uint32_t shifter_operand = ror(immed_8, rotate_imm * 2);
    uint8_t shifter_carry_out = 0;
    if (rotate_imm == 0)
    {
        shifter_carry_out = get_bit(cpsr, C);
    }
    else
    {
        shifter_carry_out = get_bit(shifter_operand, 31);
    }
    return apply_operator(p, ins, shifter_operand, shifter_carry_out, S, cpsr);
}
