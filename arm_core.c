/*
Armator - simulateur de jeu d'instruction ARMv5T à but pédagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.

Contact: Guillaume.Huard@imag.fr
	 Bâtiment IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'Hères
*/
#include "arm_core.h"
#include "registers.h"
#include "no_trace_location.h"
#include "arm_constants.h"
#include "arm_exception.h"
#include "util.h"
#include "trace.h"
#include <stdlib.h>

/* In ARM prior to ARMv6 the endianess is not controlled by the processor but depends on
   the interconnect from the processor to the memory. In this simulator, we decided to
   set the endianess at compile time according to the symbol BIG_ENDIAN_SIMULATOR.
   From ARMv6, data access endianess should comply with bit 9 of cpsr (E),
   see ARM manual A4-129
*/
#ifdef BIG_ENDIAN_SIMULATOR
#define ENDIANESS 1
#else
#define ENDIANESS 0
#endif

struct arm_core_data {
    uint32_t cycle_count;
    registers reg;
    memory mem;
};

arm_core arm_create(registers reg, memory mem) {
    arm_core p;

    p = malloc(sizeof(struct arm_core_data));
    if (p) {
        p->mem = mem;
        p->reg = reg;
        p->cycle_count = 0;
        // We reset the CPU upon creation
        arm_exception(p, RESET);
        // Because we don't have any OS, we initialize sp here
        // This is especially useful for code relying on the stack
        // such as examples written in C
        arm_write_register(p, 13, memory_get_size(mem));
    }
    return p;
}

void arm_destroy(arm_core p) {
    free(p);
}

int arm_current_mode_has_spsr(arm_core p) {
    return registers_current_mode_has_spsr(p->reg);
}

int arm_in_a_privileged_mode(arm_core p) {
    return registers_in_a_privileged_mode(p->reg);
}

uint32_t arm_get_cycle_count(arm_core p) {
    return p->cycle_count;
}

static uint32_t arm_read_register_internal(arm_core p, uint8_t reg, uint8_t mode) {
    uint32_t value = registers_read(p->reg, reg, mode);
    /* In this implementation, the program counter is incremented during the fetch.
     * Thus, to meet the specification (see manual A2-9), we add 4 whenever the
     * value of the pc is read, so that instructions read their own address + 8 when
     * reading the pc.
     */
    if (reg == 15) {
        value += 4;
        value &= 0xFFFFFFFD;
    }
    trace_register(p->cycle_count, READ, reg, mode, value);
    return value;
}

uint32_t arm_read_register(arm_core p, uint8_t reg) {
    return arm_read_register_internal(p, reg, registers_get_mode(p->reg));
}

uint32_t arm_read_usr_register(arm_core p, uint8_t reg) {
    return arm_read_register_internal(p, reg, USR);
}

uint32_t arm_read_cpsr(arm_core p) {
    uint32_t value = registers_read_cpsr(p->reg);
    trace_register(p->cycle_count, READ, CPSR, 0, value);
    return value;
}

uint32_t arm_read_spsr(arm_core p) {
    uint8_t mode = registers_get_mode(p->reg);
    uint32_t value = registers_read_spsr(p->reg, mode);
    trace_register(p->cycle_count, READ, SPSR, mode, value);
    return value;
}

static void arm_write_register_internal(arm_core p, uint8_t reg, uint8_t mode, uint32_t value) {
    registers_write(p->reg, reg, mode, value);
    trace_register(p->cycle_count, WRITE, reg, mode, value);
}

void arm_write_register(arm_core p, uint8_t reg, uint32_t value) {
    arm_write_register_internal(p, reg, registers_get_mode(p->reg), value);
}

void arm_write_usr_register(arm_core p, uint8_t reg, uint32_t value) {
    arm_write_register_internal(p, reg, USR, value);
}

void arm_write_cpsr(arm_core p, uint32_t value) {
    registers_write_cpsr(p->reg, value);
    trace_register(p->cycle_count, WRITE, CPSR, 0, value);
}

void arm_write_spsr(arm_core p, uint32_t value) {
    registers_write_spsr(p->reg, registers_get_mode(p->reg), value);
    trace_register(p->cycle_count, WRITE, SPSR, registers_get_mode(p->reg), value);
}

int arm_read_byte(arm_core p, uint32_t address, uint8_t *value) {
    int result;

    result = memory_read_byte(p->mem, address, value);
    trace_memory(p->cycle_count, READ, 1, OTHER_ACCESS, address, *value);
    return result;
}

int arm_read_half(arm_core p, uint32_t address, uint16_t *value) {
    int result;

    result = memory_read_half(p->mem, address, value, ENDIANESS);
    trace_memory(p->cycle_count, READ, 2, OTHER_ACCESS, address, *value);
    return result;
}

int arm_read_word(arm_core p, uint32_t address, uint32_t *value) {
    int result;

    result = memory_read_word(p->mem, address, value, ENDIANESS);
    trace_memory(p->cycle_count, READ, 4, OTHER_ACCESS, address, *value);
    return result;
}

int arm_fetch(arm_core p, uint32_t *value) {
    int result = -1;
    uint32_t address = 0;

    /* According to the comment in arm_read_register_internal, the fetch
     * increments the PC (this makes easier the implementation any instruction
     * that changes the PC later on).
     */
    uint8_t mode = registers_get_mode(p->reg);
    address = registers_read(p->reg, 15, mode);
    registers_write(p->reg, 15, mode, address + 4);
    result = memory_read_word(p->mem, address, value, ENDIANESS);
    p->cycle_count++;
    trace_memory(p->cycle_count, READ, 4, OPCODE_FETCH, address, *value);
    return result;
}

int arm_write_byte(arm_core p, uint32_t address, uint8_t value) {
    int result;

    result = memory_write_byte(p->mem, address, value);
    trace_memory(p->cycle_count, WRITE, 1, OTHER_ACCESS, address, value);
    return result;
}

int arm_write_half(arm_core p, uint32_t address, uint16_t value) {
    int result;

    result = memory_write_half(p->mem, address, value, ENDIANESS);
    trace_memory(p->cycle_count, WRITE, 2, OTHER_ACCESS, address, value);
    return result;
}

int arm_write_word(arm_core p, uint32_t address, uint32_t value) {
    int result;

    result = memory_write_word(p->mem, address, value, ENDIANESS);
    trace_memory(p->cycle_count, WRITE, 4, OTHER_ACCESS, address, value);
    return result;
}
