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
#include "arm_core.h"
#include "util.h"
#include "debug.h"

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

void u_bit_handle(uint8_t u, uint32_t *addr, uint32_t offset) {
    if (u) {
        *addr += offset;
    } else {
        *addr -= offset;
    }
}

void b_bit_handle(arm_core p, uint8_t l,uint8_t b, uint32_t *addr, uint8_t rd) {
    uint32_t success, value_word;
    uint8_t value_byte;
    if (l) {
        if (b) {
            if (arm_read_byte(p, *addr, &value_byte) == 0) {
                arm_write_register(p, rd, value_byte);
            }
        } else {
            if (arm_read_word(p, *addr, &value_word) == 0) {
                arm_write_register(p, rd, value_word);
            }
        }
    } else {
        if (b == 1) {
            value_byte = (uint8_t)arm_read_register(p, rd);
            success = arm_write_byte(p, *addr, value_byte);
        } else {
            value_word = arm_read_register(p, rd);
            success = arm_write_word(p, *addr, value_word);
        }

        if (success != 0) {
            // Segmentation fault ?
        }
    }
}

void arm_load_immediate_offset(arm_core p, uint8_t l, uint8_t u, uint8_t b, uint8_t rd, uint32_t *addr, uint32_t offset) {
    u_bit_handle(u, addr, offset);

    uint32_t success, value;

    b_bit_handle(p, l, b, addr, rd);
}

void arm_load_immediate_preindexing(arm_core p, uint8_t l, uint8_t u, uint8_t b, uint8_t rn, uint8_t rd, uint32_t *addr, uint32_t offset) {
    arm_load_immediate_offset(p, l, u, b, rd, addr, offset);
    arm_write_register(p, rn, *addr);
}

void arm_load_immediate_postindexing(arm_core p, uint8_t l, uint8_t u, uint8_t b, uint8_t rn, uint8_t rd, uint32_t *addr, uint32_t offset) {
    b_bit_handle(p, l, b, addr, rd);

    u_bit_handle(u, addr, offset);
    arm_write_register(p, rn, *addr);
}

void arm_store_immediate_offset(arm_core p, uint8_t l, uint8_t u, uint8_t b, uint8_t rd, uint32_t *addr, uint32_t offset) {
    u_bit_handle(u, addr, offset);

    uint8_t value_byte;
    uint32_t value_word, success;


}
void arm_store_immediate_preindexing(arm_core p, uint8_t l, uint8_t u, uint8_t b, uint8_t rn, uint8_t rd, uint32_t *addr, uint32_t offset) {
    arm_store_immediate_offset(p, l, u, b, rd, addr, offset);
    arm_write_register(p, rn, *addr);
}

void arm_store_immediate_postindexing(arm_core p, uint8_t l, uint8_t u, uint8_t b, uint8_t rn, uint8_t rd, uint32_t *addr, uint32_t offset) {
    b_bit_handle(p, l, b, addr, rd);

    u_bit_handle(u, addr, offset);
    arm_write_register(p, rn, *addr);
}

int arm_load_store_immediate_offset(arm_core p, uint32_t ins) {
    uint8_t p_bit = get_bit(ins, P_SHIFT);
    uint8_t u_bit = get_bit(ins, U_SHIFT);
    uint8_t b_bit = get_bit(ins, B_SHIFT);
    uint8_t w_bit = get_bit(ins, W_SHIFT);
    uint8_t l_bit = get_bit(ins, L_SHIFT);

    uint8_t register_n = get_bits(ins, 19, 16);
    uint8_t register_d = get_bits(ins, 15, 12);
    uint32_t offset = get_bits(ins, 11, 0);
    uint32_t address_base = arm_read_register(p, register_n);

    if (l_bit == 1) { // LDR{B}
        if (p_bit == 1) { // Pre-indexing or Offset
            if (w_bit == 1) { // Pre-indexing   
                arm_load_immediate_preindexing(p, l_bit, u_bit, b_bit, register_n, register_d, &address_base, offset);
            } else { // Offset
                arm_load_immediate_offset(p, l_bit, u_bit, b_bit, register_d, &address_base, offset);
            }
        } else { // (p_bit == 0) 
            if (w_bit == 1) {
                return UNDEFINED_INSTRUCTION;
            } else { 
                arm_load_immediate_postindexing(p, l_bit, u_bit, b_bit, register_n, register_d, &address_base, offset);
            }
        }
    } else { // STR{B}
        if (p_bit == 1) { // Pre-indexing or Offset
            if (w_bit == 1) { // Pre-indexing   
                arm_store_immediate_preindexing(p, l_bit, u_bit, b_bit, register_n, register_d, &address_base, offset);
            } else { // Offset
                arm_store_immediate_offset(p, l_bit, u_bit, b_bit, register_d, &address_base, offset);
            }
        } else { // (p_bit == 0) 
            if (w_bit == 1) {
                return UNDEFINED_INSTRUCTION;
            } else {
                arm_store_immediate_postindexing(p, l_bit, u_bit, b_bit, register_n, register_d, &address_base, offset);
            }
        }
    }
    return UNDEFINED_INSTRUCTION;
}

int arm_load_store_register_offset(arm_core p, uint32_t ins)
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