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
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include "gdb_protocol.h"
#include "debug.h"
#include "csapp.h"
#include "util.h"
#include "arm_core.h"
#include "arm_constants.h"
#include "trace.h"

/* This file contains an implementation of the GDB RSP protocol that will be used to let GDB communicate
 * with our simulator. It is documented here for instance :
 * https://ftp.gnu.org/old-gnu/Manuals/gdb/html_node/gdb_129.html#SEC134
 * For those interested, GDB has a debug mode that provide you with the details of the communications :
 * set debug remote
 */
#define MAX_PACKET_SIZE 4096
#define MAX_BREAKPOINTS_NUMBER 1024

struct gdb_protocol_data {
    arm_core arm;
    registers reg;
    memory mem;
    int target_exception;
    int fd;
    pthread_mutex_t *lock;
    char packet[MAX_PACKET_SIZE];
    uint32_t breakpoints[MAX_BREAKPOINTS_NUMBER];
    int breakpoints_number;
    int len;
    char *buffer;
};

typedef void (*gdb_handler_t)(gdb_protocol_data_t, char *);
static gdb_handler_t handler[256];

static int gdb_find_breakpoint(gdb_protocol_data_t gdb, uint32_t address) {
    int begin = 0, end, middle;
    end = gdb->breakpoints_number;
    while (begin < end) {
        middle = (begin + end) >> 1;
        if (gdb->breakpoints[middle] > address) {
            end = middle;
        } else {
            if (gdb->breakpoints[middle] < address) {
                begin = middle + 1;
            } else {
                return middle;
            }
        }
    }
    return -1;
}

static int gdb_add_breakpoint(gdb_protocol_data_t gdb, uint32_t address) {
    if (gdb->breakpoints_number < MAX_BREAKPOINTS_NUMBER) {
        if (gdb_find_breakpoint(gdb, address) != -1)
            return 1;
        int j = gdb->breakpoints_number++;
        while ((j > 0) && (gdb->breakpoints[j - 1] > address)) {
            gdb->breakpoints[j] = gdb->breakpoints[j - 1];
            j--;
        }
        gdb->breakpoints[j] = address;
        return 1;
    } else {
        return 0;
    }
}

static int gdb_remove_breakpoint(gdb_protocol_data_t gdb, uint32_t address) {
    int position;
    position = gdb_find_breakpoint(gdb, address);
    if (position == -1) {
        return 0;
    } else {
        int last = --gdb->breakpoints_number;
        for (int j = position; j < last; j++) {
            gdb->breakpoints[j] = gdb->breakpoints[j + 1];
        }
        return 1;
    }
}

static void gdb_send_ack(gdb_protocol_data_t gdb) {
    Rio_writen(gdb->fd, "+", 1);
}

static void gdb_send_buffer(gdb_protocol_data_t gdb) {
    unsigned char check = 0;
    int i;

    gdb->packet[0] = '$';
    i = 1;
    while (gdb->packet[i] != '\0') {
        check += gdb->packet[i];
        i++;
    }
    gdb->packet[i++] = '#';
    snprintf(gdb->packet + i, 3, "%02x", check);
    i += 2;
    gdb->packet[i] = '\0';
    gdb->len = i;
    gdb_transmit_packet(gdb);
}

static void gdb_send_data(gdb_protocol_data_t gdb, char *data) {
    strcpy(gdb->buffer, data);
    gdb_send_buffer(gdb);
}

/* Read and write to/from a string of bytes (in hexadecimal) in big
 * endian only : the byte order matches the textual representation
*/
static uint32_t read_uint32(char *data) {
    uint32_t value;
    sscanf(data, "%08x", &value);
    return value;
}

static void write_uint32(char *data, uint32_t value) {
    snprintf(data, 9, "%08x", value);
}

/* Handling of exception raised in target
 * To povide gdb with some bit of information, we map ARM exceptions to
 * existing signals, but this is a rough interpretation of what's happening
 */
void gdb_send_stop_reason(gdb_protocol_data_t gdb) {
    char *action;
    switch (gdb->target_exception) {
    case UNDEFINED_INSTRUCTION:
        action = "S04";
        break;
    case PREFETCH_ABORT:
        action = "S0A";
        break;
    case DATA_ABORT:
        action = "S0B";
        break;
    case END_SIMULATION:
        action = "X05";
        break;
    default:
        action = "S05";
    }
    gdb_send_data(gdb, action);
}

/* GDB Protocol commands handlers */
static void single_step(gdb_protocol_data_t gdb) {
    gdb->target_exception = arm_step(gdb->arm);
    trace_arm_state(gdb->reg);
}

static void single_step_with_signal(gdb_protocol_data_t gdb) {
    // Nothing special to do, the exception has been handled into the simulator
    single_step(gdb);
}


static void cont(gdb_protocol_data_t gdb, char *data) {
    int stop;
    uint32_t PC;
    do {
        single_step(gdb);
        PC = registers_read(gdb->reg, 15, registers_get_mode(gdb->reg));
        stop = (gdb_find_breakpoint(gdb, PC) != -1);
        if (stop) {
            debug("Cont stopped by a breakpoint at address %x\n", PC);
        } else {
            stop = gdb->target_exception;
            if (stop)
                debug("Cont stopped by an exception (%d)\n", gdb->target_exception);
        }
    } while (!stop);
    gdb_send_stop_reason(gdb);
}

static void cont_with_signal(gdb_protocol_data_t gdb, char *data) {
    cont(gdb, data);
}

static void kill_request(gdb_protocol_data_t gdb, char *data) {
    shutdown(gdb->fd, SHUT_WR);
}

static void query(gdb_protocol_data_t gdb, char *data) {
    if (strcmp(data, "Offsets") == 0)
        gdb_send_data(gdb, "Text=0;Data=0;Bss=0");
    else if (strncmp(data, "Supported", 9) == 0)
        gdb_send_data(gdb, "PacketSize=400");
    else if (strcmp(data, "TStatus") == 0)
        gdb_send_data(gdb, "T0;tnotrun:0");
    else if (strcmp(data, "Symbol::") == 0)
        gdb_send_data(gdb, "");
    else {
        debug("Unsupported query : [%s], giving empty answer\n", data);
        /* Unsupported query, giving an empty answer */
        gdb_send_data(gdb, "");
    }
}

static void read_register(gdb_protocol_data_t gdb, char *buffer, uint8_t reg) {
    assert(reg < 16);
    write_uint32(buffer, registers_read(gdb->reg, reg, registers_get_mode(gdb->reg)));
}

static void read_general_registers(gdb_protocol_data_t gdb, char *data) {
    char *position;
    int i, j;

    position = gdb->buffer;
    /* General register r0..r14 */
    for (i = 0; i < 16; i++) {
        read_register(gdb, position, i);
        position += 8;
    }
    /* Floating point register f0..f7 */
    /* Not implemented */
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 3; j++) {
            snprintf(position, 9, "xxxxxxxx");
            position += 8;
        }
    }
    /* Status registers */
    /* fps not implemented */
    snprintf(position, 9, "xxxxxxxx");
    position += 8;
    write_uint32(position, registers_read_cpsr(gdb->reg));
    gdb_send_buffer(gdb);
}

static void read_memory(gdb_protocol_data_t gdb, char *data) {
    unsigned int address, size;
    char *position;
    uint8_t value;

    sscanf(data, "%x,%x", &address, &size);
    position = gdb->buffer;
    if (address < memory_get_size(gdb->mem)) {
        position[0] = '\0';
        while (size-- && (memory_read_byte(gdb->mem, address++, &value) != -1)) {
            snprintf(position, 3, "%02x", value);
            position += 2;
        }
    } else {
        snprintf(position, 4, "E%02X", EFAULT);
    }
    gdb_send_buffer(gdb);
}

static void read_single_register(gdb_protocol_data_t gdb, char *data) {
    unsigned int reg;
    reg = atoi(data);
    read_register(gdb, gdb->buffer, reg);
    gdb_send_buffer(gdb);
}

static void reason(gdb_protocol_data_t gdb, char *data) {
    gdb_send_stop_reason(gdb);
}

static void set_thread(gdb_protocol_data_t gdb, char *data) {
    char type = *data;
    int value = atoi(data + 1);

    // No threads, so support standard selection for any or all threads
    if (((type == 'c') || (type == 'g')) && (value < 1) && (value > -2))
        gdb_send_data(gdb, "OK");
    else
        gdb_send_data(gdb, "E01");
}

static void step(gdb_protocol_data_t gdb, char *data) {
    single_step(gdb);
    gdb_send_stop_reason(gdb);
}

static void step_with_signal(gdb_protocol_data_t gdb, char *data) {
    single_step_with_signal(gdb);
    gdb_send_stop_reason(gdb);
}

static void write_register(gdb_protocol_data_t gdb, unsigned int reg, char *data) {
    uint32_t value;

    value = read_uint32(data);
    assert(reg < 16);
    if ((reg == 15) && (value > memory_get_size(gdb->mem))) {
        warning
            ("PC value is outside the address space, did you set the right endianess in gdb ?\n");
    }
    registers_write(gdb->reg, reg, registers_get_mode(gdb->reg), value);
    debug("Writing %d to register %d\n", value, reg);
}

static void write_general_registers(gdb_protocol_data_t gdb, char *data) {
    uint32_t value;
    char *position;
    int i, j;

    position = data;
    /* General register r0..r15 */
    for (i = 0; i < 16; i++) {
        write_register(gdb, i, position);
        position += 8;
    }
    /* Floating point register f0..f7 */
    /* Not implemented */
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 3; j++) {
            //value = read_uint32(position);
            position += 8;
        }
    }
    /* Status registers */
    /* fps not implemented */
    // value = read_uint32(position);
    position += 8;
    value = read_uint32(position);
    registers_write_cpsr(gdb->reg, value);
    debug("cpsr = %08x\n", value);

    gdb_send_data(gdb, "OK");
}

static void write_single_register(gdb_protocol_data_t gdb, char *data) {
    unsigned int reg;

    sscanf(data, "%x", &reg);
    data = index(data, '=') + 1;
    write_register(gdb, reg, data);
    gdb_send_data(gdb, "OK");
}

static void write_memory_binary(gdb_protocol_data_t gdb, char *data) {
    unsigned int address, size, i, write_ok;
    char *content;
    uint8_t value;

    sscanf(data, "%x,%x", &address, &size);
    content = index(data, ':') + 1;
    debug("Writing %d bytes at address %08x : ", size, address);
    write_ok = address < memory_get_size(gdb->mem);
    for (i = 0; (i < size) && write_ok; i++) {
        if (*content == 0x7d) {
            content++;
            value = *content ^ (char) 0x20;
        } else {
            value = *content;
        }
        write_ok = memory_write_byte(gdb->mem, address++, value) == 0;
        if (i < 32)
            debug_raw("%02x", value);
        content++;
    }
    debug_raw("...\n");
    if (write_ok)
        gdb_send_data(gdb, "OK");
    else
        gdb_send_data(gdb, "E02");
}

static void set_breakpoint(gdb_protocol_data_t gdb, char *data) {
    uint8_t type;
    uint32_t address, length;

    sscanf(data, "%hhx,%x,%x", &type, &address, &length);
    if (type != 0)
        gdb_send_data(gdb, "E03");
    else {
        if (gdb_add_breakpoint(gdb, address)) {
            debug("Added breakpoint at address %x\n", address);
            gdb_send_data(gdb, "OK");
        } else {
            gdb_send_data(gdb, "E04");
        }
    }
}

static void remove_breakpoint(gdb_protocol_data_t gdb, char *data) {
    uint8_t type;
    uint32_t address, length;

    sscanf(data, "%hhx,%x,%x", &type, &address, &length);
    if (type != 0)
        gdb_send_data(gdb, "E03");
    else {
        if (gdb_remove_breakpoint(gdb, address)) {
            debug("Removed breakpoint from address %x\n", address);
            gdb_send_data(gdb, "OK");
        } else {
            gdb_send_data(gdb, "E05");
        }
    }
}

/* End of GDB Protocol commands handlers */

gdb_protocol_data_t gdb_init_data(arm_core arm, registers reg, memory mem, int fd,
                                  pthread_mutex_t *lock) {
    gdb_protocol_data_t gdb;

    gdb = malloc(sizeof(struct gdb_protocol_data));
    if (gdb) {
        gdb->arm = arm;
        gdb->reg = reg;
        gdb->mem = mem;
        gdb->target_exception = 0;
        gdb->fd = fd;
        gdb->lock = lock;
        gdb->len = 0;
        gdb->buffer = gdb->packet + 1;
        gdb->breakpoints_number = 0;
    }
    return gdb;
}

void gdb_release_data(gdb_protocol_data_t gdb) {
    free(gdb);
}

void gdb_init() {
    int i;

    debug("gdb protocol handlers initialization\n");
    for (i = 0; i < 256; i++)
        handler[i] = NULL;
    handler['c'] = cont;
    handler['C'] = cont_with_signal;
    handler['k'] = kill_request;
    handler['q'] = query;
    handler['g'] = read_general_registers;
    handler['m'] = read_memory;
    handler['p'] = read_single_register;
    handler['?'] = reason;
    handler['H'] = set_thread;
    handler['s'] = step;
    handler['S'] = step_with_signal;
    handler['G'] = write_general_registers;
    handler['X'] = write_memory_binary;
    handler['P'] = write_single_register;
    handler['z'] = remove_breakpoint;
    handler['Z'] = set_breakpoint;
}

void gdb_require_retransmission(gdb_protocol_data_t gdb) {
    Rio_writen(gdb->fd, "-", 1);
}

void gdb_packet_analysis(gdb_protocol_data_t gdb, char *packet, int length) {
    int i;
    unsigned char check = 0;
    unsigned int given;
    unsigned char index;

    for (i = 1; i < length - 3; i++)
        check += packet[i];
    sscanf(packet + i + 1, "%x", &given);
    debug("Received packet : ");
    debug_raw_binary(packet, min(16, strlen(packet)));
    if (check == given) {
        debug_raw(", checksum ok\n");
        gdb_send_ack(gdb);
    } else {
        debug_raw(", checksum failed, expected %02x got %02x\n", given, check);
        debug("Requiring retransmission\n");
        gdb_require_retransmission(gdb);
        return;
    }
    packet[i] = '\0';
    index = packet[1];
    if (handler[index]) {
        pthread_mutex_lock(gdb->lock);
        handler[index] (gdb, packet + 2);
        pthread_mutex_unlock(gdb->lock);
    } else {
        debug("Unsupported request, sending empty answer\n");
        gdb_send_data(gdb, "");
    }
}

void gdb_transmit_packet(gdb_protocol_data_t gdb) {
    debug("Transmitting packet: %s\n", gdb->packet);
    Rio_writen(gdb->fd, gdb->packet, gdb->len);
}
