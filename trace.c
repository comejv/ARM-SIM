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
#include <string.h>
#include "trace.h"
#include "arm_constants.h"

static FILE *output;
/* "Randomly" chosen last address, if the first memory access is 4 bytes after
 * this address, the access will be misinterpreted as sequential. But as the
 * first instruction at reset fetches from 0x0, no problem.
 */
static uint32_t last_address = 0x12345678;
static int enabled = 1;
static char *location_file_stack[128];
static int location_line_stack[128];
static int location_stack_top = -1;
static int trace_flags = 0;
static uint8_t states[32] =
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0
};

#ifdef ARM_TRACE_FORMAT
static char *trace_memory_seq[] = { "N", "S" };
static char *trace_memory_cause[] = { "_", "O" };
static char *trace_memory_type[] = { "W", "R" };
static char *trace_register_type[] = { "W", "R" };
#else
static char *trace_memory_seq[] = { "", "" };
static char *trace_memory_cause[] = { "", ", fetch" };
static char *trace_memory_type[] = { "write", "read" };
static char *trace_register_type[] = { "write", "read" };
#endif

void set_trace_file(FILE *f) {
    output = f;
}

void trace_start_location(char *file, int line) {
    if (enabled) {
        location_stack_top++;
        location_file_stack[location_stack_top] = file;
        location_line_stack[location_stack_top] = line;
    }
}

uint8_t trace_end_location() {
    if (enabled) {
        location_stack_top--;
    }
    return 0;
}

#ifndef ARM_TRACE_FORMAT
static void trace_print_location() {
    if (enabled && (trace_flags & POSITION)) {
        if (location_stack_top >= 0) {
            fprintf(output, "%s, %d: ", location_file_stack[location_stack_top],
                    location_line_stack[location_stack_top]);
        }
    }
}
#endif

void trace_memory(uint32_t cycle, uint8_t type, uint8_t size,
                  uint8_t cause, uint32_t address, uint32_t value) {
    if (enabled && (trace_flags & MEMORY)) {
        uint8_t seq;

        seq = (address == last_address + 4) ? 1 : 0;
        last_address = address;
#ifdef ARM_TRACE_FORMAT
        fprintf(output, "M%s%s%d%s__ %08X %08X\n", trace_memory_seq[seq],
                trace_memory_type[type], size, trace_memory_cause[cause], address, value);
#else
        trace_print_location();
        fprintf(output,
                "Cycle %d, Mem %s%s (%d bytes%s) addr: %08X, val: %08X\n",
                cycle, trace_memory_seq[seq], trace_memory_type[type], size,
                trace_memory_cause[cause], address, value);
#endif
    }
}

void trace_register(uint32_t cycle, uint8_t type, uint8_t reg, uint8_t mode, uint32_t value) {
    if (enabled && (trace_flags & REGISTERS)) {
        char mode_name[5] = "";
        if (arm_get_mode_name(mode)) {
            strcpy(mode_name, "_");
            strcat(mode_name, arm_get_mode_name(mode));
        }
#ifdef ARM_TRACE_FORMAT
        fprintf(output, "R%s %s%s %08X\n",
                trace_register_type[type], arm_get_register_name(reg), mode_name, value);
#else
        trace_print_location();
        fprintf(output, "Cycle %d, Register %s, %s%s, val: %08X\n",
                cycle, trace_register_type[type], arm_get_register_name(reg), mode_name, value);
#endif
    }
}

void trace_arm_state(registers r) {
    int mode, reg, count;

    if (enabled && (trace_flags & STATE)) {
        for (mode = 0; mode < 32; mode++) {
            if (arm_get_mode_name(mode) && states[mode]) {
                fprintf(output, "%s:", arm_get_mode_name(mode));
                count = 0;
                for (reg = 0; reg < 16; reg++) {
                    if ((count > 0) && (count % 5 == 0))
                        fprintf(output, "\n    ");
                    count++;
                    fprintf(output, "   %3s=%08X", arm_get_register_name(reg),
                            registers_read(r, reg, mode));
                }
                fprintf(output, "   CPSR=%08X", registers_read_cpsr(r));
                fprintf(output, "   SPSR=%08X", registers_read_spsr(r, mode));
                fprintf(output, "\n");
            }
        }
    }
}

void trace_disable() {
    enabled = 0;
}

void trace_enable() {
    enabled = 1;
}

void trace_add(int flags) {
    if (flags < 32) {
        if (flags >= 0) {
            trace_flags |= STATE;
            states[flags] = 1;
        }
    } else {
        trace_flags |= flags;
    }
}
