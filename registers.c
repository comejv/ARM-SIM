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
#include "registers.h"
#include "arm_constants.h"
#include <stdlib.h>

registers registers_create()
{
    registers r = NULL;
    r = (registers)malloc(sizeof(struct registers_data));
    r->mode = USR;
    r->reg = (register_data *)malloc(sizeof(register_data) * N_REGISTRES);
    for (int i = 0; i < N_REGISTRES; i++)
    {
        r->reg[i].data = (uint32_t **)malloc(sizeof(uint32_t *) * N_MODES);
        uint32_t v_USER, v_SUPERVISOR, v_ABORT, v_UNDEFINED, v_INTERRUPT, v_FAST_INTERRUPT; 
        switch (i)
        {
        case 8:
        case 9:
        case 10:
        case 11:
        case 12: // There reg has the same value for mode 0 (USR) and mode 1 (FIQ)
            for (int j = 0; j < N_MODES - 1; j++)
            {
                r->reg[i].data[j] = &v_USER;
            }
            r->reg[i].data[6] = &v_FAST_INTERRUPT;
            break;
        case 13:
        case 14:
            r->reg[i].data[0] = &v_USER;
            r->reg[i].data[1] = &v_USER;
            r->reg[i].data[2] = &v_SUPERVISOR;
            r->reg[i].data[3] = &v_ABORT;
            r->reg[i].data[4] = &v_UNDEFINED;
            r->reg[i].data[5] = &v_INTERRUPT;
            r->reg[i].data[6] = &v_FAST_INTERRUPT;
            break;
        case 17:
            r->reg[i].data[0] = NULL;
            r->reg[i].data[1] = NULL;
            r->reg[i].data[2] = &v_SUPERVISOR;
            r->reg[i].data[3] = &v_ABORT;
            r->reg[i].data[4] = &v_UNDEFINED;
            r->reg[i].data[5] = &v_INTERRUPT;
            r->reg[i].data[6] = &v_FAST_INTERRUPT;
            break;
        default:
            for (int j = 0; j < N_MODES; j++)
            {
                r->reg[i].data[j] = &v_USER;
            }
            break;
        }
    }
    return r;
}

void registers_destroy(registers r)
{
    if (r != NULL)
    {
        for (int i = 0; i < N_REGISTRES; i++)
        {
            free(r->reg[i].data);
        }
        free(r->reg);
    }
    free(r);
    return;
}

uint8_t registers_get_mode(registers r)
{
    uint8_t m;
    switch (r->mode)
    {
    case USR:
        m = 0;
        break;
    case SYS:
        m = 1;
        break;
    case SVC:
        m = 2;
        break;
    case ABT:
        m = 3;
        break;
    case UND:
        m = 4;
        break;
    case IRQ:
        m = 5;
        break;
    case FIQ:
        m = 6;
        break;
    default:
        break;
    }
    return m;
}

static int registers_mode_has_spsr(registers r, uint8_t mode)
{
    return mode >= 2;
}

int registers_current_mode_has_spsr(registers r)
{
    return registers_mode_has_spsr(r, registers_get_mode(r));
}

int registers_in_a_privileged_mode(registers r)
{
    return r->mode != USR;
}

uint32_t registers_read(registers r, uint8_t reg, uint8_t mode)
{
    return *(r->reg[reg].data[mode]);
}

uint32_t registers_read_cpsr(registers r)
{
    return *(r->reg[CPSR].data[0]);
}

uint32_t registers_read_spsr(registers r, uint8_t mode)
{
    return *(r->reg[SCPSR].data[mode]);
}

void registers_write(registers r, uint8_t reg, uint8_t mode, uint32_t value)
{
    *r->reg[reg].data[mode] = value;
    return;
}

void registers_write_cpsr(registers r, uint32_t value)
{
    *r->reg[CPSR].data[0] = value;
    return;
}

void registers_write_spsr(registers r, uint8_t mode, uint32_t value)
{
    if (registers_mode_has_spsr(r, mode))
    {
        *r->reg[SCPSR].data[mode] = value;
    }
    return;
}
