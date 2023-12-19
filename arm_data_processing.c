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
#include "arm_branch_other.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"

/**
 * @brief set the Z and N flags depending on the value contained in rd
 * @param p  the arm core
 * @param rd_value  the value contained in the register rd
 * @return void
 */
void set_zn(arm_core p, uint32_t rd_value)
{
	uint32_t zn = 0;
	// set the N bit
	if (get_bit(rd_value, N))
	{
		set_bit(zn, N);
	}
	// set the Z bit
	if (rd_value == 0)
	{
		set_bit(zn, Z);
	}
	uint32_t cpsr = arm_read_cpsr(p);
	set_bits(cpsr, N, Z, get_bits(zn, N, Z));
	arm_write_cpsr(p, cpsr);
	return;
}

uint8_t carryFrom(uint32_t rn_val, uint32_t shifter_op_val, uint8_t carry) {
	if (rn_val + shifter_op_val + carry >= 1 << 32) {
		return 1;
	}
	else {
		return 0;
	}
}

/**
 * @brief DYLAN
 * @param rn_val  the value contained in the register rn
 * @param shifter_op_val DYLAN
 * @param carry DYLAN
 * @param opcode the 4-bits code encoding the current operation
 * @return uint8_t  DYLAN
 */
uint8_t overflowFrom(uint32_t rn_val, uint32_t shifter_op_val, uint8_t carry, uint32_t opcode) {
	uint8_t rn_sign = get_bit(rn_val, 31);
	uint8_t shifter_op_sign = get_bit(shifter_op_val, 31);
	uint8_t result_sign;

	switch (opcode)
	{
	case ADD:
	case ADC:
		uint8_t result_sign = get_bit(rn_val + shifter_op_val + carry, 31);
		if ((rn_sign == shifter_op_sign) && (result_sign != rn_sign)) {
			return 1;
		}
		else {
			return 0;
		}
	case SUB:
		uint8_t result_sign = get_bit(rn_val - shifter_op_val, 31);
		if ((rn_sign != shifter_op_sign) && (result_sign != rn_sign)) {
			return 1;
		}
		else {
			return 0;
		}
	case SBC:
		uint8_t result_sign = get_bit(rn_val - shifter_op_val - ~carry, 31);
		if ((rn_sign != ~shifter_op_sign) && (result_sign))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	case RSB:
		uint8_t result_sign = get_bit(shifter_op_val - rn_val, 31);
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
		uint8_t result_sign = get_bit(shifter_op_val - rn_val - ~carry, 31);
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

uint8_t borrowFrom(uint32_t op1, uint32_t op2, uint8_t carry) {
	if (op1 - op2 - ~carry < 0) {
		return 1;
	}
	else {
		return 0;
	}
}

uint8_t get_shifter_carry_out(uint32_t rotate, uint32_t shifter_operand, uint8_t carry) {
	if (!rotate) {
		return carry;
	}
	else {
		return get_bit(shifter_operand, 31);
	}
}

/**
 * @brief Calculate the result of rm_value logical shifted to the left by shift and stores it in shifter_operand. Stores in shifter_carry_out the update value of the C flag.
 * @param p  the arm core
 * @param shifted_operand  a pointer on the value which will later be stored in rd 
 * @param shifter_carry_out  a pointer on the value which will later be used to update the C flag
 * @param shift  the number size of the shift 
 * @param rm_value  the value to shift
 * @return void
 */
void calcul_LSL(arm_core p, uint32_t *shifter_operand, uint32_t *shifter_carry_out, uint32_t shift, uint32_t rm_value)
{
	if (!shift)
	{
		*shifter_operand = rm_value;
		*shifter_carry_out = get_bit(arm_read_cpsr(p), C);
	}
	else if (shift < 32)
	{
		*shifter_operand = (rm_value << shift);
		*shifter_carry_out = get_bit(rm_value, 32 - shift);
	}
	else if (shift == 32)
	{
		*shifter_operand = 0;
		*shifter_carry_out = get_bit(rm_value, 0);
	}
	else
	{
		*shifter_operand = 0;
		*shifter_carry_out = 0;
	}
	return;
}

/**
 * @brief Calculate the result of rm_value logical shifted to the right by shift and stores it in shifter_operand. stores in shifter_carry_out the update value of the C flag.
 * @param p  the arm core
 * @param shifted_operand  a pointer on the value which will later be stored in rd 
 * @param shifter_carry_out  a pointer on the value which will later be used to update the C flag
 * @param shift  the number size of the shift 
 * @param rm_value  the value to shift
 * @return void
 */
void calcul_LSR(arm_core p, uint32_t *shifter_operand, uint32_t *shifter_carry_out, uint32_t shift, uint32_t rm_value)
{
	if (!shift)
	{
		*shifter_operand = rm_value;
		*shifter_carry_out = get_bit(arm_read_cpsr(p), C);
	}
	else if (shift < 32)
	{
		*shifter_operand = (rm_value >> shift);
		*shifter_carry_out = get_bit(rm_value, 32 - shift);
	}
	else if (shift == 32)
	{
		*shifter_operand = 0;
		*shifter_carry_out = get_bit(rm_value, 0);
	}
	else
	{
		*shifter_operand = 0;
		*shifter_carry_out = 0;
	}
	return;
}

/**
 * @brief Calculate the result of rm_value arithmetically shifted to the right by shift and stores it in shifter_operand. Stores in shifter_carry_out the update value of the C flag.
 * @param p  the arm core
 * @param shifted_operand  a pointer on the value which will later be stored in rd 
 * @param shifter_carry_out  a pointer on the value which will later be used to update the C flag
 * @param shift  the number size of the shift 
 * @param rm_value  the value to shift
 * @return void
 */
void calcul_ASR(arm_core p, uint32_t *shifter_operand, uint32_t *shifter_carry_out, uint32_t shift, uint32_t rm_value, uint8_t is_immediate)
{
	if (!shift && is_immediate) //shift = 0 et is_immediate = 1 ie valeur directe
	{
		if (!get_bit(rm_value, 31))
		{
			*shifter_operand = 0;
			*shifter_carry_out = get_bit(rm_value, 31);
		}
		else
		{
			*shifter_operand = 0xFFFFFFFF;
			*shifter_carry_out = get_bit(rm_value, 31);
		}
	}
	else if (!shift && !is_immediate) //shift = 0 et is_immediate = 0 ie valeur par registre
 	{
		*shifter_operand = rm_value;
		*shifter_carry_out = get_bit(arm_read_cpsr(p), C);
	}
	else if (shift < 32) // shift < 32 et type de valeur indifférent
	{
		*shifter_operand = (asr(rm_value, shift));
		*shifter_carry_out = get_bit(rm_value, shift - 1);
	}
	else //shift >= 32 (possible qu'avec valeur par registre)
	{
		if (!get_bit(rm_value, 31)) 
		{
			*shifter_operand = 0;
			*shifter_carry_out = get_bit(rm_value, 31);
		}
		else
		{
			*shifter_operand = 0xFFFFFFFF;
			*shifter_carry_out = get_bit(rm_value, 31);
		}
	}
	return;
}

void calcul_ROR(arm_core p, uint32_t *shifter_operand, uint32_t *shifter_carry_out, uint32_t shift, uint32_t rm_value, uint8_t is_immediate)
{
	if (!shift && is_immediate) // Si shift = 0 et immediate
	{
		*shifter_operand = get_bit(arm_read_cpsr(p), C) << 31 | rm_value >> 1;
		*shifter_carry_out = get_bit(rm_value, 0);
	}
	else if ((shift != 0) && is_immediate) // Si shift = 0 et is_immediate = 0 (ie valeur par registre) OU shift != 0 et is_immediate = 1 (ie valeur directe)
	{
		*shifter_operand = rm_value;
		*shifter_carry_out = get_bit(arm_read_cpsr(p), C);
	}
	else if (!get_bits(shift, 4, 0) && !is_immediate) // Si shift[0 - 4] = 0 et register
	{
		*shifter_operand = rm_value;
		*shifter_carry_out = get_bit(rm_value, 31);
	}
	else // Si shift[0 - 4] != 0 et register
	{
		*shifter_operand = ror(rm_value, get_bits(shift, 4, 0));
		*shifter_carry_out = get_bit(rm_value, get_bits(shift, 4, 0));
	}
	return;
}

void get_shifter_operand(arm_core p, uint32_t ins, register rm, uint32_t rm_value, uint8_t is_immediate, uint32_t shifter_operand, uint32_t shifter_carry_out)
{
	uint32_t shift;
	uint8_t shift_operator = get_bits(ins, 6, 5);
	uint32_t rs, rs_value;
	uint32_t res[2];

	if (is_immediate == 1) // immediate
	{
		shift = get_bits(ins, 11, 7);
	}
	else // register
	{
		rs = get_bits(ins, RS_3, RS_0);
		rs_value = arm_read_register(p, rs);
		shift = rs_value;
	}

	switch (shift_operator)
	{
	case LSL:
		calcul_LSL(p, &shifter_operand, &shifter_carry_out, shift, rm_value);
		break;
	case LSR:
		calcul_LSR(p, &shifter_operand, &shifter_carry_out, shift, rm_value);
		break;
	case ASR:
		calcul_ASR(p, &shifter_operand, &shifter_carry_out, shift, rm_value, is_immediate);
		break;
	case ROR:
		calcul_ROR(p, &shifter_operand, &shifter_carry_out, shift, rm_value, is_immediate);
		break;
	default:
		return UNDEFINED_INSTRUCTION;
	}
	return;
}

/* Decoding functions for different classes of instructions */
int arm_data_processing_immediate_shift(arm_core p, uint32_t ins)
{
	uint32_t opcode = get_bit(ins, OPCODE_2);
	uint8_t s_bit = get_bit(ins, S_BIT);
	register rd = get_bits(ins, RD_3, RD_0);
	uint32_t rd_value = arm_read_register(p, rd);
	uint32_t shift;
	register rm = get_bits(ins, RM_3, RM_0);
	uint32_t rm_value = arm_read_register(p, rm);
	uint32_t shifter_operand;
	uint32_t shifter_carry_out;

	switch (opcode)
	{
	case MOV:
		get_shifter_operand(p, ins, rm, rm_value, 1, shifter_operand, shifter_carry_out);
		rd_value = shifter_operand;
		if (s_bit & (rd == 15)) // rd = PC register
		{
			if (arm_current_mode_has_spsr(p))
			{
				arm_write_register(p, CPSR, registers_read_spsr(p, registers_get_mode(p)));
			}
			else
			{
				return UNDEFINED_INSTRUCTION;
			}
		}
		else if (s_bit)
		{
			// need to update CPSR
			register cpsr = arm_read_cpsr(p);
			set_zn(p, rd_value);
			if (shifter_carry_out)
			{
				set_bit(cpsr, C);
			}
			else
			{
				clear_bit(cpsr, C);
			}
		}
		break;
	case MVN:
		get_shifter_operand(p, ins, rm, rm_value, 1, shifter_operand, shifter_carry_out);
		rd_value = ~shifter_operand;
		if (s_bit & (rd == 15)) // rd = PC register
		{
			if (arm_current_mode_has_spsr(p))
			{
				arm_write_register(p, CPSR, registers_read_spsr(p, registers_get_mode(p)));
			}
			else
			{
				return UNDEFINED_INSTRUCTION;
			}
		}
		else if (s_bit)
		{
			//need to update CPSR
			register cpsr = arm_read_cpsr(p);
			set_zn(p, rd_value);
			if (shifter_carry_out)
			{
				set_bit(cpsr, C);
			}
			else
			{
				clear_bit(cpsr, C);
			}
		}
		break;
	default:
		return UNDEFINED_INSTRUCTION;
	}
	return 0;
}

int arm_data_processing_register_shift(arm_core p, uint32_t ins)
{
	uint32_t opcode = get_bit(ins, OPCODE_2);
	uint8_t s_bit = get_bit(ins, S_BIT);
	register rd = get_bits(ins, RD_3, RD_0);
	uint32_t rd_value = arm_read_register(p, rd);
	uint32_t shift;
	register rm = get_bits(ins, RM_3, RM_0);
	uint32_t rm_value = arm_read_register(p, rm);
	uint32_t shifter_operand;
	uint32_t shifter_carry_out;

	switch (opcode)
	{
	case MOV:
		get_shifter_operand(p, ins, rm, rm_value, 0, shifter_operand, shifter_carry_out);
		rd_value = shifter_operand;
		if (s_bit & (rd == 15)) // rd = PC register
		{
			if (arm_current_mode_has_spsr(p))
			{
				arm_write_register(p, CPSR, registers_read_spsr(p, registers_get_mode(p)));
			}
			else
			{
				return UNDEFINED_INSTRUCTION;
			}
		}
		else if (s_bit)
		{
			// need to update CPSR
			register cpsr = arm_read_cpsr(p);
			set_zn(p, rd_value);
			if (shifter_carry_out)
			{
				set_bit(cpsr, C);
			}
			else
			{
				clear_bit(cpsr, C);
			}
		}
		break;
	case MVN:
		get_shifter_operand(p, ins, rm, rm_value, 0, shifter_operand, shifter_carry_out);
		rd_value = ~shifter_operand;
		if (s_bit & (rd == 15)) // rd = PC register
		{
			if (arm_current_mode_has_spsr(p))
			{
				arm_write_register(p, CPSR, registers_read_spsr(p, registers_get_mode(p)));
			}
			else
			{
				return UNDEFINED_INSTRUCTION;
			}
		}
		else if (s_bit)
		{
			// need to update CPSR
			register cpsr = arm_read_cpsr(p);
			set_zn(p, rd_value);
			if (shifter_carry_out)
			{
				set_bit(cpsr, C);
			}
			else
			{
				clear_bit(cpsr, C);
			}
		}
		break;
	default:
		return UNDEFINED_INSTRUCTION;
	}
	return 0;
}

int arm_data_processing_immediate(arm_core p, uint32_t ins)
{
	uint32_t opcode = get_bits(ins, OPCODE_3, OPCODE_0);
	uint32_t s_bit = get_bit(ins, S_BIT);
	uint32_t rn = get_bits(ins, RN_3, RN_0);
	uint32_t rd = get_bits(ins, RD_3, RD_0);
	uint32_t immediate = get_bits(ins, IMM_7, IMM_0);
	uint32_t rotate = get_bits(ins, ROTATE_3, ROTATE_0);
	uint32_t carry = get_bit(arm_read_cpsr(p), C);

	uint32_t rn_value = arm_read_register(p, rn);
	uint32_t rd_value = arm_read_register(p, rd);
	uint32_t shifter_operand = ror(immediate, rotate * 2);
	uint8_t shifter_carry_out;

	uint8_t v_flag = 2;
	uint8_t c_flag;

	switch (opcode)
	{
	case ADD:
		arm_write_register(p, rd, rn_value + shifter_operand);
		v_flag = overflowFrom(rn_value, shifter_operand, carry, opcode);
		c_flag = carryFrom(rn_value, shifter_operand, carry);
		break;
	case ADC:
		arm_write_register(p, rd, rn_value + shifter_operand + carry);
		v_flag = overflowFrom(rn_value, shifter_operand, carry, opcode);
		c_flag = carryFrom(rn_value, shifter_operand, carry);
		break;
	case SUB:
		arm_write_register(p, rd, rn_value - shifter_operand);
		c_flag = ~borrowFrom(rn_value, shifter_operand, carry);
		v_flag = overflowFrom(rn_value, shifter_operand, carry, opcode);
		break;
	case SBC:
		arm_write_register(p, rd, rn_value - shifter_operand - ~carry);
		c_flag = ~borrowFrom(rn_value, shifter_operand, carry);
		v_flag = overflowFrom(rn_value, shifter_operand, carry, opcode);
		break;
	case AND:
		arm_write_register(p, rd, rn_value & shifter_operand);
		c_flag = get_shifter_carry_out(rotate, shifter_operand, carry);
		break;
	case EOR:
		arm_write_register(p, rd, rn_value ^ shifter_operand);
		c_flag = get_shifter_carry_out(rotate, shifter_operand, carry);
		break;
	case RSB:
		arm_write_register(p, rn, rd_value - shifter_operand);
		c_flag = ~borrowFrom(shifter_operand, rn_value, carry);
		v_flag = overflowFrom(rn_value, shifter_operand, carry, opcode);
		break;
	case RSC:
		arm_write_register(p, rd, shifter_operand - rn_value + c_flag);
		c_flag = ~borrowFrom(shifter_operand, rn_value, carry);
		v_flag = overflowFrom(rn_value, shifter_operand, carry, opcode);
		break;
	case ORR:
		arm_write_register(p, rd, rn_value | shifter_operand);
		c_flag = get_shifter_carry_out(rotate, shifter_operand, carry);
		break;
	case BIC:
		arm_write_register(p, rd, rn_value & ~shifter_operand);
		c_flag = get_shifter_carry_out(rotate, shifter_operand, carry);
		break;
	default:
		return UNDEFINED_INSTRUCTION;
	}

	if (s_bit & (rd == 15)) // rd = PC register
	{
		if (arm_current_mode_has_spsr(p))
		{
			arm_write_register(p, CPSR, registers_read_spsr(p, registers_get_mode(p)));
		}
		else
		{
			return UNDEFINED_INSTRUCTION;
		}
	}
	else if (s_bit)
	{
		// need to update CPSR
		set_zn(p, rd_value);
		if (v_flag == 1) {
			set_bit(arm_read_cpsr(p), V);
		}
		else if (v_flag == 0) {
			clr_bit(arm_read_cpsr(p), V);
		}
		if (c_flag == 1) {
			set_bit(arm_read_cpsr(p), C);
		}
		else if (c_flag == 0) {
			clr_bit(arm_read_cpsr(p), C);
		} else {
			return UNDEFINED_INSTRUCTION;
		}
	}
	return 0;
}
