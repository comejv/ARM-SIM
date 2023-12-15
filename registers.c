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
        r->reg[i].ptrs = (uint32_t **)malloc(sizeof(uint32_t *) * N_MODES);
        switch (i)
        {
        case 8:
        case 9:
        case 10:
        case 11:
        case 12: // There reg has the same value for mode 0 (USR) and mode 1 (FIQ)
            r->reg[i].data = (uint32_t *)malloc(sizeof(uint32_t) * 2);
            for (int j = 0; j < N_MODES - 1; j++)
            {
                r->reg[i].ptrs[j] = &r->reg[i].data[0];
            }
            r->reg[i].ptrs[6] = &r->reg[i].data[1];
            break;
        case 13:
        case 14:
            r->reg[i].data = (uint32_t *)malloc(sizeof(uint32_t) * 6);
            for (int j = 2; j < N_MODES; j++)
            {
                r->reg[i].ptrs[j] = &r->reg[i].data[j];
            }
            r->reg[i].ptrs[0] = &r->reg[i].data[0];
            r->reg[i].ptrs[1] = &r->reg[i].data[0];
            break;
        case 17:
            r->reg[i].ptrs[0] = NULL;
            r->reg[i].ptrs[1] = NULL;
            r->reg[i].data = (uint32_t *)malloc(sizeof(uint32_t) * 5);
            for (int j = 2; j < N_MODES; j++)
            {
                r->reg[i].ptrs[j] = &r->reg[i].data[j];
            }
            break;
        default:
            r->reg[i].data = (uint32_t *)malloc(sizeof(uint32_t) * 1);
            for (int j = 0; j < N_MODES; j++)
            {
                r->reg[i].ptrs[j] = &r->reg[i].data[0];
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
            free(r->reg[i].ptrs);
        }
        free(r->reg);
    }
    free(r);
    return;
}

uint8_t get_read_write_mode(uint8_t mode){
    uint8_t m;
    switch (mode)
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

uint8_t registers_get_mode(registers r)
{
    return r->mode;
}

static int registers_mode_has_spsr(registers r, uint8_t mode)
{
    return get_read_write_mode(mode) >= 2;
}

int registers_current_mode_has_spsr(registers r)
{
    return registers_mode_has_spsr(r, registers_get_mode(r));
}

int registers_in_a_privileged_mode(registers r)
{
    return registers_get_mode(r) != USR;
}

uint32_t registers_read(registers r, uint8_t reg, uint8_t mode)
{
    uint8_t m = get_read_write_mode(mode);
    return *(r->reg[reg].ptrs[m]);
}

uint32_t registers_read_cpsr(registers r)
{
    return *(r->reg[CPSR].ptrs[0]);
}

uint32_t registers_read_spsr(registers r, uint8_t mode)
{
    uint8_t m = get_read_write_mode(mode);
    return *(r->reg[SCPSR].ptrs[m]);
}

void registers_write(registers r, uint8_t reg, uint8_t mode, uint32_t value)
{
    uint8_t m = get_read_write_mode(mode);
    *(r->reg[reg].ptrs[m]) = value;
    return;
}

void registers_write_cpsr(registers r, uint32_t value)
{
    *(r->reg[CPSR].ptrs[0]) = value;
    return;
}

void registers_write_spsr(registers r, uint8_t mode, uint32_t value)
{
    if (registers_mode_has_spsr(r, mode))
    {
        uint8_t m = get_read_write_mode(mode);
        *(r->reg[SCPSR].ptrs[m]) = value;
    }
    return;
}