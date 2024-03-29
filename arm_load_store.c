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
#include "arm_constants.h"
#include "arm_core.h"
#include "arm_exception.h"
#include "debug.h"
#include "util.h"

#define LDR_STR_IMMEDIATE 0x2
#define LDR_STR_REGISTER 0x3
#define LDR_STR_MULTIPLE 0x4

// Shift
#define RD_SHIFT 12
#define RN_SHIFT 16
#define L_SHIFT 20
#define W_SHIFT 21
#define B_SHIFT 22
#define U_SHIFT 23
#define P_SHIFT 24

void u_bit_handle(uint8_t u, uint32_t *addr, uint32_t offset);
int b_bit_handle(arm_core p, uint8_t l, uint8_t b, uint32_t *addr, uint8_t rd);
int arm_load_immediate_offset(arm_core p, uint8_t l, uint8_t u, uint8_t b, uint8_t rd, uint32_t *addr, uint32_t offset);
int arm_load_immediate_preindexing(arm_core p, uint8_t l, uint8_t u, uint8_t b, uint8_t rn, uint8_t rd, uint32_t *addr, uint32_t offset);
int arm_load_immediate_postindexing(arm_core p, uint8_t l, uint8_t u, uint8_t b, uint8_t rn, uint8_t rd, uint32_t *addr, uint32_t offset);
int arm_store_immediate_offset(arm_core p, uint8_t l, uint8_t u, uint8_t b, uint8_t rd, uint32_t *addr, uint32_t offset);
int arm_store_immediate_preindexing(arm_core p, uint8_t l, uint8_t u, uint8_t b, uint8_t rn, uint8_t rd, uint32_t *addr, uint32_t offset);
int arm_store_immediate_postindexing(arm_core p, uint8_t l, uint8_t u, uint8_t b, uint8_t rn, uint8_t rd, uint32_t *addr, uint32_t offset);

void u_bit_handle(uint8_t u, uint32_t *addr, uint32_t offset)
{
    if (u)
    {
        *addr += (int32_t)offset;
    }
    else
    {
        *addr -= (int32_t)offset;
    }
}

int b_bit_handle(arm_core p, uint8_t l, uint8_t b, uint32_t *addr, uint8_t rd)
{
    uint32_t value_word = 0;
    uint8_t value_byte = 0;
    int err = 0;

    if (l)
    {
        if (b)
        {
            err = arm_read_byte(p, *addr, &value_byte);
            if (err)
            {
                return err;
            }
            arm_write_register(p, rd, value_byte);
        }
        else
        {
            err = arm_read_word(p, *addr, &value_word);
            if (err)
            {
                return err;
            }

            arm_write_register(p, rd, value_word);
        }
    }
    else
    {
        if (b == 1)
        {
            value_byte = (uint8_t)arm_read_register(p, rd);
            err = arm_write_byte(p, *addr, value_byte);
        }
        else
        {
            value_word = arm_read_register(p, rd);
            err = arm_write_word(p, *addr, value_word);
        }
    }
    return err;
}

int arm_load_immediate_offset(arm_core p, uint8_t l, uint8_t u, uint8_t b,
                              uint8_t rd, uint32_t *addr, uint32_t offset)
{
    u_bit_handle(u, addr, offset);

    return b_bit_handle(p, l, b, addr, rd);
}

int arm_load_immediate_preindexing(arm_core p, uint8_t l, uint8_t u, uint8_t b,
                                   uint8_t rn, uint8_t rd, uint32_t *addr,
                                   uint32_t offset)
{
    int err = arm_load_immediate_offset(p, l, u, b, rd, addr, offset);
    if (err)
    {
        return err;
    }
    err = arm_write_register(p, rn, *addr);

    return err;
}

int arm_load_immediate_postindexing(arm_core p, uint8_t l, uint8_t u, uint8_t b,
                                    uint8_t rn, uint8_t rd, uint32_t *addr,
                                    uint32_t offset)
{
    int err = b_bit_handle(p, l, b, addr, rd);
    if (!err)
    {
        u_bit_handle(u, addr, offset);
        arm_write_register(p, rn, *addr);
    }
    return err;
}

int arm_store_immediate_offset(arm_core p, uint8_t l, uint8_t u, uint8_t b,
                               uint8_t rd, uint32_t *addr, uint32_t offset)
{
    u_bit_handle(u, addr, offset);
    return b_bit_handle(p, l, b, addr, rd);
}

int arm_store_immediate_preindexing(arm_core p, uint8_t l, uint8_t u, uint8_t b,
                                    uint8_t rn, uint8_t rd, uint32_t *addr,
                                    uint32_t offset)
{
    int err = arm_store_immediate_offset(p, l, u, b, rd, addr, offset);
    if (err)
    {
        return err;
    }
    arm_write_register(p, rn, *addr);
    return err;
}

int arm_store_immediate_postindexing(arm_core p, uint8_t l, uint8_t u, uint8_t b,
                                     uint8_t rn, uint8_t rd, uint32_t *addr,
                                     uint32_t offset)
{
    int err = b_bit_handle(p, l, b, addr, rd);
    if (err)
    {
        return err;
    }

    u_bit_handle(u, addr, offset);

    arm_write_register(p, rn, *addr);
    return err;
}

int arm_load_store_immediate_offset(arm_core p, uint32_t ins)
{
    uint8_t p_bit = get_bit(ins, P_SHIFT);
    uint8_t u_bit = get_bit(ins, U_SHIFT);
    uint8_t b_bit = get_bit(ins, B_SHIFT);
    uint8_t w_bit = get_bit(ins, W_SHIFT);
    uint8_t l_bit = get_bit(ins, L_SHIFT);

    uint8_t register_n = get_bits(ins, 19, 16);
    uint8_t register_d = get_bits(ins, 15, 12);
    uint32_t offset = get_bits(ins, 11, 0);
    uint32_t address_base = arm_read_register(p, register_n);
    int err;
    debug("arm_load_store_immediate_offset with ins %x\n", ins);
    if (l_bit == 1)
    { // LDR{B}
        if (p_bit == 1)
        { // Pre-indexing or Offset
            if (w_bit == 1)
            { // Pre-indexing
                err = arm_load_immediate_preindexing(p, l_bit, u_bit, b_bit,
                                                     register_n, register_d,
                                                     &address_base, offset);
            }
            else
            { // Offset
                err = arm_load_immediate_offset(
                    p, l_bit, u_bit, b_bit, register_d, &address_base, offset);
            }
        }
        else
        { // (p_bit == 0)
            if (w_bit == 1)
            {
                err = UNDEFINED_INSTRUCTION;
            }
            else
            {
                err = arm_load_immediate_postindexing(p, l_bit, u_bit, b_bit,
                                                      register_n, register_d,
                                                      &address_base, offset);
            }
        }
    }
    else
    { // STR{B}
        if (p_bit == 1)
        { // Pre-indexing or Offset
            if (w_bit == 1)
            { // Pre-indexing
                err = arm_store_immediate_preindexing(p, l_bit, u_bit, b_bit,
                                                      register_n, register_d,
                                                      &address_base, offset);
            }
            else
            { // Offset
                err = arm_store_immediate_offset(
                    p, l_bit, u_bit, b_bit, register_d, &address_base, offset);
            }
        }
        else
        { // (p_bit == 0)
            if (w_bit == 1)
            {
                err = UNDEFINED_INSTRUCTION;
            }
            else
            {
                err = arm_store_immediate_postindexing(p, l_bit, u_bit, b_bit,
                                                       register_n, register_d,
                                                       &address_base, offset);
            }
        }
    }
    return err;
}

int arm_load_store_register_offset(arm_core p, uint32_t ins)
{
    uint8_t p_bit = get_bit(ins, P_SHIFT);
    uint8_t u_bit = get_bit(ins, U_SHIFT);
    uint8_t b_bit = get_bit(ins, B_SHIFT);
    uint8_t w_bit = get_bit(ins, W_SHIFT);
    uint8_t l_bit = get_bit(ins, L_SHIFT);
    uint8_t lpw_bits = l_bit << 2 | p_bit << 1 | w_bit;

    uint8_t register_n = get_bits(ins, 19, 16);
    uint8_t register_d = get_bits(ins, 15, 12);
    uint8_t shift_imm = get_bits(ins, 11, 7);
    uint8_t shift = get_bits(ins, 6, 5);
    uint8_t register_m = get_bits(ins, 3, 0);
    uint32_t offset = arm_read_register(p, register_m);
    uint32_t address_base = arm_read_register(p, register_n);

    debug("shift imm : %x\nshift = %x\n", shift_imm, shift);

    uint8_t scaled = (uint8_t)shift_imm | (uint8_t)shift;
    int err = 0;

    debug("arm_load_store_register_offset with ins %x\n", ins);
    if (scaled != 0)
    {
        switch (shift)
        {
        case LSL:
            offset <<= shift_imm;
            break;

        case LSR:
            if (shift_imm == 0)
                offset = 0;
            else
                offset >>= shift_imm;
            break;

        case ASR:
            if (shift_imm == 0)
            {
                if (get_bit(arm_read_register(p, register_m), 31) == 1)
                    offset = 0xFFFFFFFF;
                else
                    offset = 0;
            }
            else
            {
                offset = (int)offset >> shift_imm;
            }
            break;

        case ROR:
            if (shift_imm == 0)
                offset = (get_bit(arm_read_cpsr(p), C) << 31) | register_m >> 1;
            else
                offset = ror(offset, shift_imm);
            break;

        default:
            // It should not exist
            err = UNDEFINED_INSTRUCTION;
        }
    }

    switch (lpw_bits)
    {
    case 0b000: // L 0 -> Store // P 0 -> Postindexing // Wdoit etre a 0
        err =
            arm_store_immediate_postindexing(p, l_bit, u_bit, b_bit, register_n,
                                             register_d, &address_base, offset);
        break;
    case 0b001: // W - 1
        err = UNDEFINED_INSTRUCTION;
        break;
    case 0b010: // P - 1 W - 0 => Register Offset
        err = arm_store_immediate_offset(p, l_bit, u_bit, b_bit, register_d,
                                         &address_base, offset);
        break;
    case 0b011: // P - 1 W - 1 => Register Pre-indexing
        err =
            arm_store_immediate_preindexing(p, l_bit, u_bit, b_bit, register_n,
                                            register_d, &address_base, offset);
        break;
    case 0b100: // L 1 -> Load // P 0 -> Post indexing
        err =
            arm_load_immediate_postindexing(p, l_bit, u_bit, b_bit, register_n,
                                            register_d, &address_base, offset);
        break;
    case 0b101:
        err = UNDEFINED_INSTRUCTION;
        break;
    case 0b110:
        err = arm_load_immediate_offset(p, l_bit, u_bit, b_bit, register_d,
                                        &address_base, offset);
        break;
    case 0b111:
        err = arm_load_immediate_preindexing(p, l_bit, u_bit, b_bit, register_n,
                                             register_d, &address_base, offset);
        break;
    default:
        err = UNDEFINED_INSTRUCTION;
        break;
    }

    return err;
}

int arm_load_store_miscellaneous(arm_core p, uint32_t ins)
{
    debug("arm_load_store_miscellaneous with ins %x\n", ins);
    // Get parameters
    uint8_t p_bit = get_bit(ins, P_SHIFT);
    uint8_t u_bit = get_bit(ins, U_SHIFT);
    uint8_t w_bit = get_bit(ins, W_SHIFT);
    uint8_t b_bits = get_bit(ins, B_SHIFT);
    uint8_t lsh_bits =
        get_bit(ins, L_SHIFT) << 2 | get_bit(ins, 6) << 1 | get_bit(ins, 5);

    // uint8_t rn = get_bits(ins, RN_SHIFT + 3, RN_SHIFT);
    // uint8_t rd = get_bits(ins, RD_SHIFT + 3, RN_SHIFT);
    uint8_t rn = get_bits(ins, 19, 16);
    uint8_t rd = get_bits(ins, 15, 12);

    uint8_t op1 = get_bits(ins, 8 + 3, 8);
    uint8_t op0 = get_bits(ins, 0 + 3, 0);

    uint32_t address = 0;
    uint8_t offset = 0;

    uint32_t signed_mask = 0;
    uint32_t value_word = 0;
    uint16_t value_half = 0;
    uint8_t value_byte = 0;
    int err = 0;

    // Check instruction type
    if (p_bit && b_bits && !w_bit) // Immediate offset
    {
        offset = op1 << 4 | op0;
        if (u_bit)
        {
            address = arm_read_register(p, rn) + offset;
        }
        else
        {
            address = arm_read_register(p, rn) - offset;
        }
    }
    else if (p_bit && !b_bits && !w_bit) // Register offset
    {
        if (u_bit)
        {
            address = arm_read_register(p, rn) + arm_read_register(p, op0);
        }
        else
        {
            address = arm_read_register(p, rn) - arm_read_register(p, op0);
        }
    }
    else if (p_bit && b_bits && w_bit) // Immediate pre indexed
    {
        offset = op1 << 4 | op0;
        if (u_bit)
        {
            address = arm_read_register(p, rn) + offset;
        }
        else
        {
            address = arm_read_register(p, rn) - offset;
        }
        arm_write_register(p, rn, address);
    }
    else if (p_bit && !b_bits && w_bit) // Register pre indexed
    {
        if (u_bit)
        {
            address = arm_read_register(p, rn) + arm_read_register(p, op0);
        }
        else
        {
            address = arm_read_register(p, rn) - arm_read_register(p, op0);
        }
        arm_write_register(p, rn, address);
    }
    else if (!p_bit && b_bits && !w_bit) // Immediate post indexed
    {
        address = arm_read_register(p, rn);
        offset = op1 << 4 | op0;
        if (u_bit)
        {
            arm_write_register(p, rn, arm_read_register(p, rn) + offset);
        }
        else
        {
            arm_write_register(p, rn, arm_read_register(p, rn) - offset);
        }
    }
    else if (!p_bit && !b_bits && !w_bit) // Register post indexed
    {
        address = arm_read_register(p, rn);
        if (u_bit)
        {
            arm_write_register(
                p, rn, arm_read_register(p, rn) + arm_read_register(p, op0));
        }
        else
        {
            arm_write_register(
                p, rn, arm_read_register(p, rn) - arm_read_register(p, op0));
        }
    }
    else
    {
        return UNDEFINED_INSTRUCTION;
    }

    switch (lsh_bits) // NB: If lsh_bits == 5 or lsh_bits == 0 then addressing
                      // mode 2
    {
    case 1: // Store half word, L = 0, S = 0, H = 1
        value_half = (uint16_t)arm_read_register(p, rd);
        err = arm_write_half(p, address, value_half);
        break;
    case 2: // Load double word, L = 0, S = 1, H = 0
        if (rd % 2 == 0 && rd != 14 && get_bits(address, 1, 0) == 0b00 && get_bit(address, 2) == 0)
        {
            err = arm_read_word(p, address, &value_word);
            arm_write_register(p, rd, (uint32_t)value_word);
            debug("Reading word 0x%x from address 0x%x and storing in register %d\n", value_word, address, rd);
            if (err)
                break;
            err = arm_read_word(p, address + 4, &value_word);
            arm_write_register(p, rd + 1, (uint32_t)value_word);
            debug("Reading word 0x%x from address 0x%x and storing in register %d\n", value_word, address + 4, rd + 1);
            debug("Err : %d\n", err);
        }
        else
        {
            err = UNDEFINED_INSTRUCTION;
        }
        break;
    case 3: // Store double word, L = 0, S = 1, H = 1
        if (rd % 2 == 0 && rd != 14 && get_bits(address, 1, 0) == 0b00 && get_bit(address, 2) == 0)
        {
            value_word = (uint32_t)arm_read_register(p, rd);
            err = arm_write_word(p, address, value_word);
            debug("Writing register %d, value 0x%x to address 0x%x\n", rd, value_word, address);
            if (err)
                break;
            value_word = (uint32_t)arm_read_register(p, rd + 1);
            err = arm_write_word(p, address + 4, value_word);
            debug("Writing register %d, value 0x%x to address 0x%x\n", rd + 1, value_word, address + 4);
            debug("Err : %d\n", err);
        }
        else
        {
            err = UNDEFINED_INSTRUCTION;
        }
        break;
    case 5: // Load unsigned half word, L = 1, S = 0, H = 1
        err = arm_read_half(p, address, &value_half);
        arm_write_register(p, rd, (uint32_t)value_half);
        break;
    case 6: // Load signed byte, L = 1, S = 1, H = 0
        err = arm_read_byte(p, address, &value_byte);
        if (get_bit(value_byte, 7))
            signed_mask = ~signed_mask;
        signed_mask = set_bits(signed_mask, 7, 0, value_byte);
        arm_write_register(p, rd, signed_mask);
        break;
    case 7: // Load signed half word, L = 1, S = 1, H = 1
        err = arm_read_half(p, address, &value_half);
        if (get_bit(value_half, 15))
            signed_mask = ~signed_mask;
        signed_mask = set_bits(signed_mask, 15, 0, value_half);
        arm_write_register(p, rd, signed_mask);
        break;
    default:
        // It should'nt happen
        break;
    }

    return err;
}

int arm_load_store_multiple(arm_core p, uint32_t ins)
{
    uint8_t P = get_bit(ins, 24);
    uint8_t U = get_bit(ins, 23);
    uint8_t S = get_bit(ins, 22);
    uint8_t W = get_bit(ins, 21);
    uint8_t L = get_bit(ins, 20);
    uint8_t Rn = get_bits(ins, 19, 16);
    uint16_t register_list = get_bits(ins, 15, 0);
    uint32_t Rn_v = arm_read_register(p, Rn);
    uint32_t number_of_set_bits = 0;
    uint32_t address, end_address, data, W_adress;
    for (int i = 0; i < 16; i++)
    {
        number_of_set_bits +=
            get_bit(register_list, i); // Fix the function call
    }
    if (!P && U)
    {
        debug("Increment After\n");
        address = Rn_v;
        end_address = Rn_v + (number_of_set_bits * 4) - 4;
        W_adress = Rn_v + (number_of_set_bits * 4);
    }
    else if (P && U)
    {
        debug("Increment Before\n");
        address = Rn_v + 4;
        end_address = Rn_v + (number_of_set_bits * 4);
        W_adress = Rn_v + (number_of_set_bits * 4);
    }
    else if (!P && !U)
    {
        debug("Decrement After\n");
        address = Rn_v - (number_of_set_bits * 4) + 4;
        end_address = Rn_v;
        W_adress = Rn_v - (number_of_set_bits * 4);
    }
    else // P && !U
    {
        debug("Decrement Before\n");
        address = Rn_v - (number_of_set_bits * 4);
        end_address = Rn_v - 4;
        W_adress = Rn_v - (number_of_set_bits * 4);
    }
    debug("Operation from %x to %x\n", address, end_address);

    for (int i = 0; i < 16; i++)
    {
        if (get_bit(register_list, i))
        {
            if (L)
            {
                arm_read_word(p, address, &data);
                if (i == 15)
                {
                    data = data & 0xFFFFFFFC;
                }
                debug("LOAD %x from %x in R%d with P = %d\n", data, address, i,
                      P);
                arm_write_register(p, i, data);
            }
            else
            {
                uint32_t data = arm_read_register(p, i);
                debug("STORE %x at %x in R%d with P = %d\n", data, address, i,
                      P);
                arm_write_word(p, address, data);
            }
            address += 4;
        }
    }
    if (L && S && get_bit(ins, 15)) // Check if PC has been changed
    {
        arm_write_cpsr(p, arm_read_spsr(p));
    }
    if (W == 1)
    {
        debug("New R%d value : %x\n", Rn, W_adress);
        arm_write_register(p, Rn, W_adress);
    }
    return 0;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins)
{
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}
